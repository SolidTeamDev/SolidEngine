#include "Core/engine.hpp"

#include <iostream>
#include <sstream>
#include "Rendering/OpenGL45/openGl45Renderer.hpp"
#include "UI/solidUI.hpp"
#include "Resources/ressources.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"
#include "ECS/Components/camera.hpp"
#include "ECS/Components/rigidBody.hpp"
#include "ECS/Components/script.hpp"
#include "ECS/Components/audioSource.hpp"
#include "ECS/Components/rigidBody.hpp"
#include "ECS/Components/boxCollider.hpp"
#include "ECS/Components/sphereCollider.hpp"
#include "ECS/Components/capsuleCollider.hpp"
#include "ECS/Components/light.hpp"
#include "ECS/Components/scriptList.hpp"
#include "ECS/Components/particleEffect.hpp"

#include "ECS/Components/animation.hpp"
#include "Refureku/Refureku.h"

namespace Solid
{
	SOLID_API Engine* Engine::instance = nullptr;
    Engine::Engine() :
            threadPool(&taskManager),
            resourceManager(this),
            audio(),
            ecsManager(),
            graphicsResourceMgr()
    {
    	Log::Send("Engine Creation");
	    Compiler = GameCompiler::GetInstance();
        InitEcs();
    }

    Engine::~Engine()
    {

        delete inputManager;
        UIContext::ReleaseSolidUI();
        delete window;
        threadPool.TerminateAllThreads();
    }

    void Engine::InitEcs()
    {
        ecsManager.Init();

        //Components Registration
        ecsManager.RegisterComponent<Transform>();
        ecsManager.RegisterComponent<MeshRenderer>();
        ecsManager.RegisterComponent<Camera>();
	    ecsManager.RegisterComponent<ScriptList>();
        ecsManager.RegisterComponent<AudioSource>();
        ecsManager.RegisterComponent<RigidBody>();
        ecsManager.RegisterComponent<BoxCollider>();
        ecsManager.RegisterComponent<SphereCollider>();
        ecsManager.RegisterComponent<CapsuleCollider>();
	    ecsManager.RegisterComponent<Light>();
	    ecsManager.RegisterComponent<ParticleEffect>();
        ecsManager.RegisterComponent<Animation>();


        //Register Signature
	    transformSystem = ecsManager.RegisterSystem<TransformSystem>();
	    {
		    Signature signature;
		    signature.set(ecsManager.GetComponentType<Transform>());
		    ecsManager.SetSystemSignature<TransformSystem>(signature);
	    }
        rendererSystem = ecsManager.RegisterSystem<RendererSystem>();
        {
            Signature signature;
            signature.set(ecsManager.GetComponentType<Transform>());
            signature.set(ecsManager.GetComponentType<MeshRenderer>());
            ecsManager.SetSystemSignature<RendererSystem>(signature);
        }

        audioSystem = ecsManager.RegisterSystem<AudioSystem>();
        {
            Signature signature;
            signature.set(ecsManager.GetComponentType<Transform>());
            signature.set(ecsManager.GetComponentType<AudioSource>());
            ecsManager.SetSystemSignature<AudioSystem>(signature);
        }

	    cameraSystem = ecsManager.RegisterSystem<CameraSystem>();
	    {
		    Signature signature;
		    signature.set(ecsManager.GetComponentType<Transform>());
		    signature.set(ecsManager.GetComponentType<Camera>());
		    ecsManager.SetSystemSignature<CameraSystem>(signature);
	    }

        physicsSystem = ecsManager.RegisterSystem<PhysicsSystem>();
        {
            Signature signature;
            signature.set(ecsManager.GetComponentType<Transform>());
            ecsManager.SetSystemSignature<PhysicsSystem>(signature);
        }

	    scriptSystem = ecsManager.RegisterSystem<ScriptSystem>();
	    {
		    Signature signature;
		    signature.set(ecsManager.GetComponentType<ScriptList>());
		    ecsManager.SetSystemSignature<ScriptSystem>(signature);
	    }
	    animSystem = ecsManager.RegisterSystem<AnimationSystem>();
	    {
		    Signature signature;
		    signature.set(ecsManager.GetComponentType<Animation>());
		    ecsManager.SetSystemSignature<AnimationSystem>(signature);
	    }
	    particleEffectSystem = ecsManager.RegisterSystem<ParticleEffectSystem>();
	    {
	    	Signature signature;
		    signature.set(ecsManager.GetComponentType<Transform>());
		    signature.set(ecsManager.GetComponentType<ParticleEffect>());
		    ecsManager.SetSystemSignature<ParticleEffectSystem>(signature);
	    }
    }

    void Engine::InitEngineContext(const WindowParams& _windowParams, const RendererParams& _rendererParams)
    {
        window   = new Window(_windowParams);
        switch (_rendererParams.rendererType)
        {
            case ERendererType::OpenGl45:
                renderer = OpenGL45Renderer::InitRenderer();
                break;
        }

        inputManager = new InputManager(window->GetHandle());

        /// TEMPORARY
        UIContext::InitializeSolidUI(window->GetHandle());
        ///

        if(window != nullptr && renderer != nullptr)
            engineContextInit = true;
	    graphicsResourceMgr.Init(&resourceManager, renderer);
	    particleEffectSystem->InitShaderForGL();
	    PlayBuffer =renderer->CreateFramebuffer(window->GetWindowSize());

    }

    bool Engine::IsEngineContextInitialized() const
    {

        return engineContextInit;
    }

	Engine *Engine::GetInstance()
	{
    	if(instance == nullptr)
    		instance = new Engine();

    	return instance;
	}

	void Engine::EnableMultiThread(bool _b)
	{
		mtEnabled = _b;
		if(_b){threadPool.PlayAllThreads();}
		else{threadPool.PauseAllThreads();}
	}

    void Engine::InitScript()
    {
        scriptSystem->Init();
    }

    void Engine::DestroyScript()
    {
        scriptSystem->Destroy();
    }

    void Engine::Update()
    {
        inputManager->Update();

        scriptSystem->Update();
    }

    void Engine::FixedUpdate()
    {
        physicsSystem->Update(physics, (float)Time::DeltaTime());
        scriptSystem->FixedUpdate();
    }

    void Engine::LateUpdate()
    {
        scriptSystem->LateUpdate();
    }

	void Engine::ForceUpdate()
	{
		transformSystem->Update();
		cameraSystem->Update();

	}

	template<class T>
	T* Engine::AddComp(const std::string &className, std::vector<char> &buffer, std::uint64_t &readPos,
	                              GameObject *go, Components *cmp, std::size_t FieldNum, std::size_t cmpNameSize)
	{
		T *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(T *) cmp);
		for (int i = 0; i < FieldNum; ++i)
		{
			short isNull = -1;

			//Get Comp FieldName
			ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
			                                readPos);
			std::string Name;
			Name.resize(cmpNameSize / sizeof(std::string::value_type));
			ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

			//Get is Null
			ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
			std::size_t memSize = 0;
			//Get Field Data
			const rfk::Field *f = t->getArchetype().getField(Name);
			if (f->type.archetype->name == "String")
			{
				ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
				                                readPos);
				String *str = (String *) f->getDataAddress(t);
				str->resize(memSize / sizeof(std::string::value_type));

				ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), memSize, readPos);
			}
			else if (f->type.archetype->name == "vectorStr")
			{
				std::size_t vecSize = 0;
				ResourcesLoader::ReadFromBuffer(buffer.data(), &vecSize, sizeof(std::size_t),
				                                readPos);
				vectorStr *vstr = (vectorStr *) f->getDataAddress(t);

				for (int k = 0; k < vecSize; ++k)
				{
					String str;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
					                                readPos);
					str.resize(memSize);
					ResourcesLoader::ReadFromBuffer(buffer.data(), str.data(), memSize, readPos);

					vstr->push_back(std::move(str));
				}
			}
			else
			{

				ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
				                                readPos);
				char *buf = new char[memSize]();
				ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);

				if (isNull == 256)
				{
					std::string s = std::string(buf, memSize);
					f->setData(t, s);
				}
				else
				{

					f->setData(t, ((void *) buf), memSize);
				}


				delete[] buf;
			}

			if (className == "Transform" && Name == "rotation")
			{

				((Transform*)t)->SetRotation(((Transform *) t)->GetLocalRotation());
			}
		}
		if(className.find("Collider") != std::string::npos)
			t->Release();
		t->Init();
		return t;
	}
	void Engine::AddAllComps(GameObject *elt, std::vector<char> &buffer, uint64_t &readPos)
	{
		std::size_t cmpNameSize = 0;
		rfk::Namespace const* n = rfk::Database::getNamespace("Solid");
		{
			//get num of Childs
			std::size_t childNum = 0;
			ResourcesLoader::ReadFromBuffer(buffer.data(), &childNum, sizeof(std::size_t), readPos);

			for (int games = 0; games < childNum; ++games)
			{
				GameObject* go = elt->childs.at(games);

				//get num of comps
				std::size_t cmpNum = 0;
				ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNum, sizeof(std::size_t), readPos);


				for (int j = 0; j < cmpNum; ++j)
				{
					//get Comp Class / str
					ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t), readPos);
					std::string className;
					className.resize(cmpNameSize / sizeof(std::string::value_type));
					ResourcesLoader::ReadFromBuffer(buffer.data(), className.data(), cmpNameSize, readPos);

					//get Field Num
					std::size_t FieldNum = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &FieldNum, sizeof(std::size_t), readPos);


					rfk::Class const *myClass = n->getClass(className);
					rfk::Namespace const* ns =Compiler->GetNamespace("Solid");
					if(myClass == nullptr && ns != nullptr)
						myClass = ns->getClass(className);
					//COMPCUSTO

					if(myClass != nullptr)
					{
						Components *cmp = myClass->makeInstance<Components>();

						if (className == "Transform")
						{

							AddComp<Transform>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
							delete cmp;
						}
						else if (className == "AudioSource")
						{
							AddComp<AudioSource>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
							delete cmp;
						}
						else if (className == "MeshRenderer")
						{
							AddComp<MeshRenderer>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
							delete cmp;
						}
						else if (className == "Animation")
						{
							AddComp<Animation>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
							delete cmp;
						}
						else if (className == "RigidBody")
						{
							AddComp<RigidBody>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
							delete cmp;
						}
						else if (className == "BoxCollider")
						{
							AddComp<BoxCollider>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
							delete cmp;
						}
						else if (className == "SphereCollider")
						{
							AddComp<SphereCollider>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
							delete cmp;
						}
						else if (className == "CapsuleCollider")
						{
							AddComp<CapsuleCollider>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
							delete cmp;
						}
						else if (className == "Camera")
						{
							Camera* cam = AddComp<Camera>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
							if(cam->IsActive())
								cam->SetActiveCamera();
							delete cmp;
						}
						else if (className == "Light")
						{
							AddComp<Light>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
							delete cmp;
						}
						else if (ns != nullptr && myClass->isSubclassOf(*ns->getClass("Script")))
						{

							if(!ecsManager.GotComponent<ScriptList>(go->GetEntity()))
							{
								ecsManager.AddComponent(go, ScriptList());
							}
							Script* s = ecsManager.GetComponent<ScriptList>(go->GetEntity()).AddScript((Script*)cmp);
							for (int i = 0; i < FieldNum; ++i)
							{
								short isNull = -1;

								//Get Comp FieldName
								ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
								                                readPos);
								std::string Name;
								Name.resize(cmpNameSize / sizeof(std::string::value_type));
								ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

								//Get is Null
								ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
								std::size_t memSize = 0;
								//Get Field Data

								const rfk::Field *f = s->getArchetype().getField(Name, rfk::EFieldFlags::Default,
								                                                 true);

								if (f->type.archetype->name == "String")
								{
									ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
									                                readPos);
									String *str = (String *) f->getDataAddress(s);
									str->resize(memSize / sizeof(std::string::value_type));

									ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), memSize, readPos);
								}
								else if (f->type.archetype->name == "vectorStr")
								{
									std::size_t vecSize = 0;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &vecSize, sizeof(std::size_t),
									                                readPos);
									vectorStr *vstr = (vectorStr *) f->getDataAddress(s);

									for (int k = 0; k < vecSize; ++k)
									{
										String str;
										ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
										                                readPos);
										str.resize(memSize);
										ResourcesLoader::ReadFromBuffer(buffer.data(), str.data(), memSize, readPos);

										vstr->push_back(std::move(str));
									}
								}
								else
								{
									ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t), readPos);
									if (isNull == 256)
									{}
									else
									{}
									char *buf = new char[memSize]();
									ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);

									f->setData(s, ((void *) buf), memSize);
									delete[] buf;

								}





							}
						}
					}

				}
				AddAllComps(go, buffer, readPos);
			}
		};
	}

	void Engine::LoadScene(const char *name)
	{
		json j;
		SceneResource* scene = resourceManager.GetSceneByName(name);

		//SCENE : implement load function here
		if(scene == nullptr)
		{
			return;
		}
		rfk::Namespace const* n = rfk::Database::getNamespace("Solid");
		std::uint64_t readPos = 0;
		//LOADS




		std::size_t jsonSize = 0;
		ResourcesLoader::ReadFromBuffer(scene->rawScene.data(), &jsonSize, sizeof(std::size_t),readPos);
		std::string jsonStr;
		jsonStr.resize(jsonSize / sizeof(std::string::value_type));
		ResourcesLoader::ReadFromBuffer(scene->rawScene.data(), jsonStr.data(), jsonSize,readPos);
		j = j.parse(jsonStr) ;

		Engine* engine = instance;
		GameObject* world = engine->ecsManager.GetWorld();
		for (auto it = world->childs.begin() ; it != world->childs.end();)
		{
			engine->ecsManager.DestroyEntity((*it)->GetEntity());
			//(*it)->RemoveCurrent();
			it = world->childs.begin();
			if(it == world->childs.end())
			{
				break;
			}
		}

		std::function<void(json&,Entity)> Lambda = [&, engine](json& j,Entity e){
			for(auto it = j.begin(); it != j.end(); ++it)
			{
				std::string key = (it).key();
				if(key.find("GameObject") != std::string::npos)
				{
					GameObject* ent = engine->ecsManager.CreateEntity( it.value()["Name"], e);
					Lambda(std::ref(it.value()), ent->GetEntity());
				}
			}
		};
		for(auto it = j["Scene"].begin(); it != j["Scene"].end(); ++it)
		{
			std::string key = (it).key();
			if(key.find("GameObject") != std::string::npos)
			{
				std::string name = it.value()["Name"];
				GameObject* ent = engine->ecsManager.CreateEntity(name);
				Lambda(std::ref(it.value()), ent->GetEntity());
			}
		}
		std::string str = j ["Scene"]["{SkyBoxName}"];
		Engine::GetInstance()->renderer->_map = Engine::GetInstance()->graphicsResourceMgr.GetCubemap(str.c_str());
		AddAllComps(world, scene->rawScene, readPos);

		for(auto& elt : LoadedSceneCallbacks)
		{
		    elt(scene);
		}

	}

	void Engine::SaveScene(const fs::path &p)
	{
		bool isNew = false;
    	SceneResource* scene = resourceManager.GetSceneByName(p.filename().string().c_str());
    	if(scene == nullptr)
	    {
		    isNew = true;
    		scene = new SceneResource();
	    }
		scene->rawScene.clear();
		json j;
		j["Scene"].array();
		j = j.flatten();
		std::string elt = "/Scene";
		GameObject* world = ecsManager.GetWorld();
		std::function<void(json&, GameObject*, std::string&)> Lambda = [&](json& j, GameObject* elt, std::string& path){

			for(GameObject* sub : elt->childs)
			{
				std::string subP = path + "/{GameObject_"+ std::to_string(sub->GetEntity()) + "}" ;
				j[subP + "/Name"] = sub->name;
				Lambda(std::ref(j), sub, std::ref(subP));
			}

		};
		std::string subP = elt + "/{SkyBoxName}" ;
		if(renderer->_map != nullptr)
			j[subP] = renderer->_map->name;
		else
			j[subP] = "NO_SKYBOX";

		Lambda(std::ref(j), world, std::ref(elt));
		j = j.unflatten();

		std::ofstream file(p, std::ifstream::binary | std::ifstream::trunc);
		std::vector<char> buffer;
		std::stringstream sstr;
		sstr << std::setw(4) << j << std::endl;
		std::size_t sstrSize = sstr.str().size() * sizeof(std::string::value_type);
		ResourcesLoader::Append(buffer, &sstrSize , sizeof(std::size_t));

		ResourcesLoader::Append(buffer, sstr.str().data(), sstrSize);
		std::function<void(GameObject*)> LambdaCmp = [&](GameObject* elt){
			//store Num of Childs
			std::size_t ChildNum = elt->childs.size();
			ResourcesLoader::Append(buffer, &ChildNum, sizeof(std::size_t));
			for(GameObject* sub : elt->childs)
			{
				//store num of comps
				std::size_t cmpNum = sub->compsList.size();
				ResourcesLoader::Append(buffer, &cmpNum, sizeof(std::size_t));
				for(Components* cmp : sub->compsList)
				{
					if(cmp->getArchetype().name == "ScriptList")
					{
							for(Script* script : ((ScriptList*)cmp)->GetAllScripts())
							{
								Log::Send(script->getArchetype().name);
								std::size_t offset =0;

								std::size_t scriptNameSize = 0;

								//store comp name / string
								scriptNameSize = script->getArchetype().name.size()*sizeof(std::string::value_type);
								ResourcesLoader::Append(buffer, &scriptNameSize, sizeof(std::size_t));
								ResourcesLoader::Append(buffer, (void*)script->getArchetype().name.data(),  scriptNameSize);

								//store num of fields
								std::size_t numFields = script->getArchetype().fields.size();
								ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
								for(auto& cField : script->getArchetype().fields)//2 cField var WARN
								{
									std::size_t size = 0;
									size = cField.name.size()*sizeof(std::string::value_type);
									//store field name / string
									ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
									ResourcesLoader::Append(buffer, (void*)cField.name.data(),  size);
									short isNull = 128;
									if(cField.type.archetype == nullptr)
									{
										isNull = 256;
										std::string str = cField.getData<std::string>(script);
										std::size_t strS =  str.size()*sizeof(std::string::value_type);
										//store isNull
										ResourcesLoader::Append(buffer, &isNull, sizeof(short));
										//store field data
										ResourcesLoader::Append(buffer, &strS, sizeof(std::size_t));
										ResourcesLoader::Append(buffer, str.data(), strS);
									}
									else
									{
										ResourcesLoader::Append(buffer, &isNull, sizeof(short));
										if(cField.type.archetype->name == "String")
										{
											String* str = (String*)cField.getDataAddress(script);
											size = str->size()*sizeof(std::string::value_type);
											ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
											ResourcesLoader::Append(buffer, str->data(), size);
										}
										else if(cField.type.archetype->name == "vectorStr")
										{
											vectorStr* vstr = (vectorStr*)cField.getDataAddress(script);
											size = vstr->size();
											ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
											for(auto& str : *vstr)
											{
												std::size_t strSize = str.size()*sizeof(std::string::value_type);
												ResourcesLoader::Append(buffer, &strSize, sizeof(std::size_t));
												ResourcesLoader::Append(buffer, str.data(), strSize);
											}
										}
										else
										{
											size = cField.type.archetype->memorySize;
											//store isNull
											//store field data
											ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
											ResourcesLoader::Append(buffer, cField.getDataAddress(script), size);

										}
									}

								}

							}
					}
					else
					{
						Log::Send(cmp->getArchetype().name);
						std::size_t offset =0;

						std::size_t cmpNameSize = 0;

						//store comp name / string
						cmpNameSize = cmp->getArchetype().name.size()*sizeof(std::string::value_type);
						ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
						ResourcesLoader::Append(buffer, (void*)cmp->getArchetype().name.data(),  cmpNameSize);

						//store num of fields
						std::size_t numFields = cmp->getArchetype().fields.size();
						ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
						for(auto& cField : cmp->getArchetype().fields)//2 cField var WARN
						{
							std::size_t size = 0;
							size = cField.name.size()*sizeof(std::string::value_type);
							//store field name / string
							ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
							ResourcesLoader::Append(buffer, (void*)cField.name.data(),  size);
							short isNull = 128;
							if(cField.type.archetype == nullptr)
							{
								isNull = 256;
								std::string str = cField.getData<std::string>(cmp);
								std::size_t strS =  str.size()*sizeof(std::string::value_type);
								//store isNull
								ResourcesLoader::Append(buffer, &isNull, sizeof(short));
								//store field data
								ResourcesLoader::Append(buffer, &strS, sizeof(std::size_t));
								ResourcesLoader::Append(buffer, str.data(), strS);
							}
							else
							{
								ResourcesLoader::Append(buffer, &isNull, sizeof(short));
								if(cField.type.archetype->name == "String")
								{
									String* str = (String*)cField.getDataAddress(cmp);
									size = str->size()*sizeof(std::string::value_type);
									ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
									ResourcesLoader::Append(buffer, str->data(), size);
								}
								else if(cField.type.archetype->name == "vectorStr")
								{
									vectorStr* vstr = (vectorStr*)cField.getDataAddress(cmp);
									size = vstr->size();
									ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
									for(auto& str : *vstr)
									{
										std::size_t strSize = str.size()*sizeof(std::string::value_type);
										ResourcesLoader::Append(buffer, &strSize, sizeof(std::size_t));
										ResourcesLoader::Append(buffer, str.data(), strSize);
									}
								}
								else
								{
									size = cField.type.archetype->memorySize;
									//store isNull
									//store field data
									ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
									ResourcesLoader::Append(buffer, cField.getDataAddress(cmp), size);

								}
							}

						}

					}


				}
				LambdaCmp(sub);
			}

		};
		LambdaCmp(world);
		scene->rawScene = buffer;
		scene->name = p.filename().string();
		ResourcesLoader loader;
		scene->path.clear();
		loader.SetPath(scene->path, p);
		buffer.clear();

		scene->ToDataBuffer(buffer);
		file.write(buffer.data(), buffer.size());
		file.close();
		if(isNew)
			resourceManager.AddResource(scene);
	}

	void Engine::LoadTempScene(SceneResource* name)
	{
		json j;
		SceneResource* scene =name;

		//SCENE : implement load function here
		if(scene == nullptr)
		{
			return;
		}
		rfk::Namespace const* n = rfk::Database::getNamespace("Solid");
		std::uint64_t readPos = 0;
		//LOADS




		std::size_t jsonSize = 0;
		ResourcesLoader::ReadFromBuffer(scene->rawScene.data(), &jsonSize, sizeof(std::size_t),readPos);
		std::string jsonStr;
		jsonStr.resize(jsonSize / sizeof(std::string::value_type));
		ResourcesLoader::ReadFromBuffer(scene->rawScene.data(), jsonStr.data(), jsonSize,readPos);
		j = j.parse(jsonStr) ;

		Engine* engine = instance;
		GameObject* world = engine->ecsManager.GetWorld();
		for (auto it = world->childs.begin() ; it != world->childs.end();)
		{
			engine->ecsManager.DestroyEntity((*it)->GetEntity());
			//(*it)->RemoveCurrent();
			it = world->childs.begin();
			if(it == world->childs.end())
			{
				break;
			}
		}

		std::function<void(json&,Entity)> Lambda = [&, engine](json& j,Entity e){
			for(auto it = j.begin(); it != j.end(); ++it)
			{
				std::string key = (it).key();
				if(key.find("GameObject") != std::string::npos)
				{
					GameObject* ent = engine->ecsManager.CreateEntity( it.value()["Name"], e);
					Lambda(std::ref(it.value()), ent->GetEntity());
				}
			}
		};
		for(auto it = j["Scene"].begin(); it != j["Scene"].end(); ++it)
		{
			std::string key = (it).key();
			if(key.find("GameObject") != std::string::npos)
			{
				std::string name = it.value()["Name"];
				GameObject* ent = engine->ecsManager.CreateEntity(name);
				Lambda(std::ref(it.value()), ent->GetEntity());
			}
		}
		std::string str = j ["Scene"]["{SkyBoxName}"];
		Engine::GetInstance()->renderer->_map = Engine::GetInstance()->graphicsResourceMgr.GetCubemap(str.c_str());
		AddAllComps(world, scene->rawScene, readPos);

		for(auto& elt : LoadedSceneCallbacks)
		{
			elt(scene);
		}

	}

	SceneResource* Engine::SaveTempScene()
	{

		SceneResource* scene = new SceneResource();
		json j;
		j["Scene"].array();
		j = j.flatten();
		std::string elt = "/Scene";
		GameObject* world = ecsManager.GetWorld();
		std::function<void(json&, GameObject*, std::string&)> Lambda = [&](json& j, GameObject* elt, std::string& path){

			for(GameObject* sub : elt->childs)
			{
				std::string subP = path + "/{GameObject_"+ std::to_string(sub->GetEntity()) + "}" ;
				j[subP + "/Name"] = sub->name;
				Lambda(std::ref(j), sub, std::ref(subP));
			}

		};
		std::string subP = elt + "/{SkyBoxName}" ;
		if(renderer->_map != nullptr)
			j[subP] = renderer->_map->name;
		else
			j[subP] = "NO_SKYBOX";

		Lambda(std::ref(j), world, std::ref(elt));
		j = j.unflatten();

		std::vector<char> buffer;
		std::stringstream sstr;
		sstr << std::setw(4) << j << std::endl;
		std::size_t sstrSize = sstr.str().size() * sizeof(std::string::value_type);
		ResourcesLoader::Append(buffer, &sstrSize , sizeof(std::size_t));

		ResourcesLoader::Append(buffer, sstr.str().data(), sstrSize);
		std::function<void(GameObject*)> LambdaCmp = [&](GameObject* elt){
			//store Num of Childs
			std::size_t ChildNum = elt->childs.size();
			ResourcesLoader::Append(buffer, &ChildNum, sizeof(std::size_t));
			for(GameObject* sub : elt->childs)
			{
				//store num of comps
				std::size_t cmpNum = sub->compsList.size();
				ResourcesLoader::Append(buffer, &cmpNum, sizeof(std::size_t));
				for(Components* cmp : sub->compsList)
				{
					if(cmp->getArchetype().name == "ScriptList")
					{
						for(Script* script : ((ScriptList*)cmp)->GetAllScripts())
						{
							Log::Send(script->getArchetype().name);
							std::size_t offset =0;

							std::size_t scriptNameSize = 0;

							//store comp name / string
							scriptNameSize = script->getArchetype().name.size()*sizeof(std::string::value_type);
							ResourcesLoader::Append(buffer, &scriptNameSize, sizeof(std::size_t));
							ResourcesLoader::Append(buffer, (void*)script->getArchetype().name.data(),  scriptNameSize);

							//store num of fields
							std::size_t numFields = script->getArchetype().fields.size();
							ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
							for(auto& cField : script->getArchetype().fields)//2 cField var WARN
							{
								std::size_t size = 0;
								size = cField.name.size()*sizeof(std::string::value_type);
								//store field name / string
								ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
								ResourcesLoader::Append(buffer, (void*)cField.name.data(),  size);
								short isNull = 128;
								if(cField.type.archetype == nullptr)
								{
									isNull = 256;
									std::string str = cField.getData<std::string>(script);
									std::size_t strS =  str.size()*sizeof(std::string::value_type);
									//store isNull
									ResourcesLoader::Append(buffer, &isNull, sizeof(short));
									//store field data
									ResourcesLoader::Append(buffer, &strS, sizeof(std::size_t));
									ResourcesLoader::Append(buffer, str.data(), strS);
								}
								else
								{
									ResourcesLoader::Append(buffer, &isNull, sizeof(short));
									if(cField.type.archetype->name == "String")
									{
										String* str = (String*)cField.getDataAddress(script);
										size = str->size()*sizeof(std::string::value_type);
										ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
										ResourcesLoader::Append(buffer, str->data(), size);
									}
									else if(cField.type.archetype->name == "vectorStr")
									{
										vectorStr* vstr = (vectorStr*)cField.getDataAddress(script);
										size = vstr->size();
										ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
										for(auto& str : *vstr)
										{
											std::size_t strSize = str.size()*sizeof(std::string::value_type);
											ResourcesLoader::Append(buffer, &strSize, sizeof(std::size_t));
											ResourcesLoader::Append(buffer, str.data(), strSize);
										}
									}
									else
									{
										size = cField.type.archetype->memorySize;
										//store isNull
										//store field data
										ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
										ResourcesLoader::Append(buffer, cField.getDataAddress(script), size);

									}
								}

							}

						}
					}
					else
					{
						Log::Send(cmp->getArchetype().name);
						std::size_t offset =0;

						std::size_t cmpNameSize = 0;

						//store comp name / string
						cmpNameSize = cmp->getArchetype().name.size()*sizeof(std::string::value_type);
						ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
						ResourcesLoader::Append(buffer, (void*)cmp->getArchetype().name.data(),  cmpNameSize);

						//store num of fields
						std::size_t numFields = cmp->getArchetype().fields.size();
						ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
						for(auto& cField : cmp->getArchetype().fields)//2 cField var WARN
						{
							std::size_t size = 0;
							size = cField.name.size()*sizeof(std::string::value_type);
							//store field name / string
							ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
							ResourcesLoader::Append(buffer, (void*)cField.name.data(),  size);
							short isNull = 128;
							if(cField.type.archetype == nullptr)
							{
								isNull = 256;
								std::string str = cField.getData<std::string>(cmp);
								std::size_t strS =  str.size()*sizeof(std::string::value_type);
								//store isNull
								ResourcesLoader::Append(buffer, &isNull, sizeof(short));
								//store field data
								ResourcesLoader::Append(buffer, &strS, sizeof(std::size_t));
								ResourcesLoader::Append(buffer, str.data(), strS);
							}
							else
							{
								ResourcesLoader::Append(buffer, &isNull, sizeof(short));
								if(cField.type.archetype->name == "String")
								{
									String* str = (String*)cField.getDataAddress(cmp);
									size = str->size()*sizeof(std::string::value_type);
									ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
									ResourcesLoader::Append(buffer, str->data(), size);
								}
								else if(cField.type.archetype->name == "vectorStr")
								{
									vectorStr* vstr = (vectorStr*)cField.getDataAddress(cmp);
									size = vstr->size();
									ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
									for(auto& str : *vstr)
									{
										std::size_t strSize = str.size()*sizeof(std::string::value_type);
										ResourcesLoader::Append(buffer, &strSize, sizeof(std::size_t));
										ResourcesLoader::Append(buffer, str.data(), strSize);
									}
								}
								else
								{
									size = cField.type.archetype->memorySize;
									//store isNull
									//store field data
									ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
									ResourcesLoader::Append(buffer, cField.getDataAddress(cmp), size);

								}
							}

						}

					}


				}
				LambdaCmp(sub);
			}

		};
		LambdaCmp(world);
		scene->rawScene = buffer;
		scene->name = "TempScene";

		scene->path.clear();

		return scene;
	}

	void Engine::AddLoadedSceneCallback(const std::function<void(Resource*)>& _func)
	{
		LoadedSceneCallbacks.push_back(_func);
	}

	void Engine::SetActiveCamera(Camera *_newCam)
	{
		activeCamera = _newCam;
	}

	void Engine::RenderToBuffer(const Vec2i& _size = {0,0})
	{
		PlayBuffer.size = (_size.x <= 0 && _size.y <= 0) ? window->GetWindowSize() : _size;
        renderer->UpdateFramebuffer(PlayBuffer);

    	if(activeCamera == nullptr)
    		return;

		activeCamera->UpdateCamera(PlayBuffer.size);
		renderer->BeginFramebuffer(PlayBuffer);
		renderer->ClearColor({0.f,0.f,0.f,1});
		renderer->Clear(PlayBuffer.size);
		rendererSystem->Update(renderer, *activeCamera);
		renderer->DrawSkybox(*activeCamera);
		renderer->EndFramebuffer();
		audioSystem->Update(*activeCamera);
	}

} //!namespace


