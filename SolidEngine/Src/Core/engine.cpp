#include "Core/engine.hpp"

#include <iostream>
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
#include "Refureku/Refureku.h"

namespace Solid
{
	__declspec(dllexport) Engine* Engine::instance = nullptr;
    Engine::Engine() :
            threadPool(&taskManager),
            resourceManager(this),
            audio(),
            ecsManager(),
            graphicsResourceMgr()
    {

        InitEcs();
    }

    Engine::~Engine()
    {

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
        ecsManager.RegisterComponent<Script*>();
        ecsManager.RegisterComponent<AudioSource>();
        ecsManager.RegisterComponent<RigidBody>();
        ecsManager.RegisterComponent<BoxCollider>();
        ecsManager.RegisterComponent<SphereCollider>();
        ecsManager.RegisterComponent<CapsuleCollider>();

        //Register Signature
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

        physicsSystem = ecsManager.RegisterSystem<PhysicsSystem>();
        {
            Signature signature;
            signature.set(ecsManager.GetComponentType<Transform>());
            ecsManager.SetSystemSignature<PhysicsSystem>(signature);
        }

	    scriptSystem = ecsManager.RegisterSystem<ScriptSystem>();
	    {
		    Signature signature;
		    signature.set(ecsManager.GetComponentType<Script*>());
		    ecsManager.SetSystemSignature<ScriptSystem>(signature);
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

        /// TEMPORARY
        UIContext::InitializeSolidUI(window->GetHandle());
        ///

        if(window != nullptr && renderer != nullptr)
            engineContextInit = true;
	    graphicsResourceMgr.Init(&resourceManager, renderer);
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

    void Engine::Update()
    {
		scriptSystem->Update();
    }

    void Engine::FixedUpdate()
    {
        physicsSystem->Update(physics, (float)Time::DeltaTime());
    }

    void Engine::LateUpdate()
    {

    }

	void Engine::LoadScene(const fs::path &p)
	{
		json j;
		std::ifstream file("test.SolidScene",std::ifstream::binary | std::ifstream::ate);



		//SCENE : implement load function here
		if(!file.is_open())
		{
			abort();
		}
		rfk::Namespace const* n = rfk::Database::getNamespace("Solid");
		std::uint64_t readPos = 0;
		std::size_t cmpNameSize = 0;
		std::ifstream::pos_type pos = file.tellg()  ;

		std::vector<char>  buffer(pos);

		file.seekg(0, std::ios::beg);
		file.read(&buffer[0], pos);

		std::size_t jsonSize = 0;
		ResourcesLoader::ReadFromBuffer(buffer.data(), &jsonSize, sizeof(std::size_t),readPos);
		std::string jsonStr;
		jsonStr.resize(jsonSize / sizeof(std::string::value_type));
		ResourcesLoader::ReadFromBuffer(buffer.data(), jsonStr.data(), jsonSize,readPos);
		j = j.parse(jsonStr) ;

		Engine* engine = instance;
		GameObject* world = engine->ecsManager.GetWorld();
		for (auto it = world->childs.begin() ; it != world->childs.end();)
		{
			(*it)->RemoveCurrent();
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
		std::function<void(GameObject*)> AddAllComps = [&](GameObject* elt)
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
					Components *cmp = myClass->makeInstance<Components>();

					if (className == "Transform")
					{
						Transform *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(Transform *) cmp);
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
							ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t), readPos);
							if (isNull == 256)
							{}
							else
							{}
							char *buf = new char[memSize]();
							ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);
							const rfk::Field *f = t->getArchetype().getField(Name);
							f->setData(t, ((void *) buf), memSize);
							delete[] buf;

							if (Name == "rotation")
							{
								t->SetRotation(t->GetRotation());
							}
						}

					}
					else if (className == "AudioSource")
					{
						AudioSource *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(AudioSource *) cmp);

						//Get Comp FieldName
						for (int i = 0; i < FieldNum; ++i)
						{
							short isNull = -1;

							//Get Comp FieldName
							ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
							                                readPos);
							std::string Name;
							Name.resize(cmpNameSize / sizeof(std::string::value_type));
							ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

							//Get IsNull
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
							std::size_t memSize = 0;
							//get Field Data
							const rfk::Field *f = t->getArchetype().getField(Name);
							if (f->type.archetype->name == "String")
							{
								ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
								                                readPos);
								String *str = (String *) f->getDataAddress(t);
								str->resize(memSize / sizeof(std::string::value_type));

								ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), memSize, readPos);
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

						}
						t->Init();
					}
					else if (className == "MeshRenderer")
					{
						MeshRenderer *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(MeshRenderer *) cmp);

						//Get Comp FieldName
						for (int i = 0; i < FieldNum; ++i)
						{
							short isNull = -1;

							//Get Comp FieldName
							ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
							                                readPos);
							std::string Name;
							Name.resize(cmpNameSize / sizeof(std::string::value_type));
							ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

							//Get IsNull
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
							std::size_t memSize = 0;
							//get Field Data
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

						}
						t->Init();
					}
				}
				AddAllComps(go);
			}
		};
		AddAllComps(world);
	}
} //!namespace


