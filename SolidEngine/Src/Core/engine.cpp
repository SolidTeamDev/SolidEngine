#include "Core/engine.hpp"

#include <iostream>
#include <sstream>
#include "Rendering/OpenGL45/openGl45Renderer.hpp"
#include "UI/solidUI.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
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

	    EditorContext = UI::GetCurrentContext();
	    ImGuiIO& editorIO = UI::GetIO();
	    UICompCtx =ImGui::CreateContext(editorIO.Fonts);

	    UI::SetCurrentContext(UICompCtx);
	    ImGui::StyleColorsDark();

	    ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), false);
	    ImGui_ImplOpenGL3_Init("#version 450");

	    UI::SetCurrentContext(EditorContext);

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
        animSystem->Update();
        scriptSystem->Update();
    }

    void Engine::FixedUpdate()
    {
        physicsSystem->Update(physics, (float)Time::DeltaTime());
        scriptSystem->FixedUpdate();
    }

    void Engine::LateUpdate()
    {
	    BeginUIComponents();
	    scriptSystem->LateUpdate();
	    EndUIComponents();
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
			                                readPos, buffer.size());
			std::string Name;
			Name.resize(cmpNameSize / sizeof(std::string::value_type));
			ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos, buffer.size());

			//Get is Null
			ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos, buffer.size());
			std::size_t memSize = 0;
			//Get Field Data
			const rfk::Field *f = t->getArchetype().getField(Name);
			std::size_t fieldSize = 0;
			ResourcesLoader::ReadFromBuffer(buffer.data(), &fieldSize, sizeof(std::size_t),
			                                readPos, buffer.size());
			if(f == nullptr)
			{
				readPos+=fieldSize;
				continue;
			}
			if (f->type.archetype->name == "String")
			{
				String *str = (String *) f->getDataAddress(t);
				str->resize(fieldSize / sizeof(std::string::value_type));

				ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), fieldSize, readPos, buffer.size());
			}
			else if (f->type.archetype->name == "vectorStr")
			{
				std::size_t vecSize = 0;
				vectorStr *vstr = (vectorStr *) f->getDataAddress(t);
				ResourcesLoader::ReadFromBuffer(buffer.data(), &vecSize, sizeof(std::size_t),
				                                readPos, buffer.size());
				for (int k = 0; k < vecSize; ++k)
				{
					String str;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
					                                readPos, buffer.size());
					str.resize(memSize);
					ResourcesLoader::ReadFromBuffer(buffer.data(), str.data(), memSize, readPos, buffer.size());

					vstr->push_back(std::move(str));
				}
			}
			else
			{
				if (isNull == 256)
				{}
				else
				{}
				char *buf = new char[fieldSize]();
				ResourcesLoader::ReadFromBuffer(buffer.data(), buf, fieldSize, readPos, buffer.size());

				f->setData(t, ((void *) buf), fieldSize);
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
			ResourcesLoader::ReadFromBuffer(buffer.data(), &childNum, sizeof(std::size_t), readPos, buffer.size());

			for (int games = 0; games < childNum; ++games)
			{
				GameObject* go = elt->childs.at(games);

				//get num of comps
				std::size_t cmpNum = 0;
				ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNum, sizeof(std::size_t), readPos, buffer.size());


				for (int j = 0; j < cmpNum; ++j)
				{
					//get Comp Class / str
					ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t), readPos, buffer.size());
					std::string className;
					className.resize(cmpNameSize / sizeof(std::string::value_type));
					ResourcesLoader::ReadFromBuffer(buffer.data(), className.data(), cmpNameSize, readPos, buffer.size());

					//get Field Num
					std::size_t FieldNum = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &FieldNum, sizeof(std::size_t), readPos, buffer.size());

					//get SkipSize
					std::size_t SkipSize = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &SkipSize, sizeof(std::size_t), readPos, buffer.size());


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
						else if (className == "ParticleEffect")
						{



							ParticleEffect *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(ParticleEffect *) cmp);
							std::size_t sizep = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &sizep, sizeof(std::size_t),readPos, buffer.size());
							t->particlesSize = sizep;

							std::size_t isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								std::size_t numP = 0;
								ResourcesLoader::ReadFromBuffer(buffer.data(), &numP, sizeof(std::size_t), readPos, buffer.size());
								t->SetCount(numP);
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								float em = 0;
								ResourcesLoader::ReadFromBuffer(buffer.data(), &em, sizeof(float), readPos, buffer.size());
								t->SetEmitRate(em);
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{

								std::size_t nsize = 0;
								ResourcesLoader::ReadFromBuffer(buffer.data(), &nsize, sizeof(std::size_t), readPos, buffer.size());
								std::string TexName;
								TexName.resize(nsize / sizeof(std::string::value_type));
								ResourcesLoader::ReadFromBuffer(buffer.data(), TexName.data(), nsize, readPos, buffer.size());
								t->SetTex(graphicsResourceMgr.GetTexture(TexName.c_str()));
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{

							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->velFromPosGen == nullptr)
									t->velFromPosGen = std::make_shared<VelFromPosGen>();
								ResourcesLoader::ReadFromBuffer(buffer.data(), &t->velFromPosGen->offset, sizeof(Vec4), readPos, buffer.size());
								ResourcesLoader::ReadFromBuffer(buffer.data(), &t->velFromPosGen->minScale, sizeof(float), readPos, buffer.size());
								ResourcesLoader::ReadFromBuffer(buffer.data(), &t->velFromPosGen->maxScale, sizeof(float), readPos, buffer.size());

							}
							else
							{
								t->velFromPosGen = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->colGen == nullptr)
									t->colGen = std::make_shared<BasicColorGen>();

								{
									auto ptr = t->colGen;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minStartCol, sizeof(Vec4), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxStartCol, sizeof(Vec4), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minEndCol, sizeof(Vec4), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxEndCol, sizeof(Vec4), readPos, buffer.size());
								}
							}
							else
							{
								t->colGen = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->sphereVelGen == nullptr)
									t->sphereVelGen = std::make_shared<SphereVelGen>();

								{
									auto ptr = t->sphereVelGen;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minVel, sizeof(float), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxVel, sizeof(float), readPos, buffer.size());

								}
							}
							else
							{
								t->sphereVelGen = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->spherePosGen == nullptr)
									t->spherePosGen = std::make_shared<SpherePosGen>();

								{
									auto ptr = t->spherePosGen;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->center, sizeof(Vec4), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->radX, sizeof(float), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->radY, sizeof(float), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->radZ, sizeof(float), readPos, buffer.size());

								}
							}
							else
							{
								t->spherePosGen = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->timeGen == nullptr)
									t->timeGen = std::make_shared<BasicTimeGen>();

								{
									auto ptr = t->timeGen;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minTime, sizeof(float), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxTime, sizeof(float), readPos, buffer.size());


								}
							}
							else
							{
								t->timeGen = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->velGen == nullptr)
									t->velGen = std::make_shared<BasicVelGen>();

								{
									auto ptr = t->velGen;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minStartVel, sizeof(Vec4), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxStartVel, sizeof(Vec4), readPos, buffer.size());

								}
							}
							else
							{
								t->velGen = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->boxPosGen == nullptr)
									t->boxPosGen = std::make_shared<BoxPosGen>();

								{
									auto ptr = t->boxPosGen;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->pos, sizeof(Vec4), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxStartPosOffset, sizeof(Vec4), readPos, buffer.size());

								}
							}
							else
							{
								t->boxPosGen = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->killerZoneUpdater == nullptr)
									t->killerZoneUpdater = std::make_shared<KillerZoneUpdater>();

								{
									auto ptr = t->killerZoneUpdater;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->pos, sizeof(Vec3), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->offset, sizeof(Vec3), readPos, buffer.size());

								}
							}
							else
							{
								t->killerZoneUpdater = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->attractorUpdater == nullptr)
									t->attractorUpdater = std::make_shared<AttractorUpdater>();

								{
									auto ptr = t->attractorUpdater;
									std::size_t attsize = 0;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &attsize, sizeof(std::size_t), readPos, buffer.size());
									t->attractorUpdater->attractors.resize(attsize);
									ResourcesLoader::ReadFromBuffer(buffer.data(), ptr->attractors.data(), sizeof(Vec4) * attsize, readPos, buffer.size());

								}
							}
							else
							{
								t->attractorUpdater = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->velColUpdater == nullptr)
									t->velColUpdater = std::make_shared<VelColorUpdater>();

								{
									auto ptr = t->velColUpdater;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minVel, sizeof(Vec4), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxVel, sizeof(Vec4), readPos, buffer.size());

								}
							}
							else
							{
								t->velColUpdater = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->posColUpdater == nullptr)
									t->posColUpdater = std::make_shared<PosColorUpdater>();


								{
									auto ptr = t->posColUpdater;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minPos, sizeof(Vec4), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxPos, sizeof(Vec4), readPos, buffer.size());

								}
							}
							else
							{
								t->posColUpdater = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->colorUpdater == nullptr)
									t->colorUpdater = std::make_shared<BasicColorUpdater>();

								{
									auto ptr = t->colorUpdater;

								}
							}
							else
							{
								t->colorUpdater = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->eulerUpdater == nullptr)
									t->eulerUpdater = std::make_shared<EulerUpdater>();

								{
									auto ptr = t->eulerUpdater;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->globalAcceleration, sizeof(Vec4), readPos, buffer.size());

								}
							}
							else
							{
								t->eulerUpdater = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->floorUpdater == nullptr)
									t->floorUpdater = std::make_shared<FloorUpdater>();

								{
									auto ptr = t->floorUpdater;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->bounceFactor, sizeof(float), readPos, buffer.size());
									ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->floorY, sizeof(float), readPos, buffer.size());


								}
							}
							else
							{
								t->floorUpdater = nullptr;
							}
							isNull = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
							if(isNull == 128)
							{
								if(t->timeUpdater == nullptr)
									t->timeUpdater = std::make_shared<BasicTimeUpdater>();

							}
							else
							{
								t->timeUpdater = nullptr;
							}
							t->UpdateSystem();
							t->UpdateEmitter();
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
								                                readPos, buffer.size());
								std::string Name;
								Name.resize(cmpNameSize / sizeof(std::string::value_type));
								ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos, buffer.size());

								//Get is Null
								ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos, buffer.size());
								std::size_t memSize = 0;
								//Get Field Data

								const rfk::Field *f = s->getArchetype().getField(Name, rfk::EFieldFlags::Default,
								                                                 true);

								std::size_t fieldSize = 0;
								ResourcesLoader::ReadFromBuffer(buffer.data(), &fieldSize, sizeof(std::size_t),
								                                                        readPos, buffer.size());

								if(f == nullptr)
								{
									readPos += fieldSize;
									continue;
								}

								if (f->type.archetype->name == "String")
								{
									String *str = (String *) f->getDataAddress(s);
									str->resize(fieldSize / sizeof(std::string::value_type));

									ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), fieldSize, readPos, buffer.size());
								}
								else if (f->type.archetype->name == "vectorStr")
								{
									std::size_t vecSize = 0;
									vectorStr *vstr = (vectorStr *) f->getDataAddress(s);
									ResourcesLoader::ReadFromBuffer(buffer.data(), &vecSize, sizeof(std::size_t),
									                                readPos, buffer.size());
									for (int k = 0; k < vecSize; ++k)
									{
										String str;
										ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
										                                readPos, buffer.size());
										str.resize(memSize);
										ResourcesLoader::ReadFromBuffer(buffer.data(), str.data(), memSize, readPos, buffer.size());

										vstr->push_back(std::move(str));
									}
								}
								else
								{
									if (isNull == 256)
									{}
									else
									{}
									char *buf = new char[fieldSize]();
									ResourcesLoader::ReadFromBuffer(buffer.data(), buf, fieldSize, readPos, buffer.size());

									f->setData(s, ((void *) buf), fieldSize);
									delete[] buf;

								}





							}
						}
					}
					else
					{
						readPos+= SkipSize;
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
		ResourcesLoader::ReadFromBuffer(scene->rawScene.data(), &jsonSize, sizeof(std::size_t), readPos, scene->rawScene.size());
		std::string jsonStr;
		jsonStr.resize(jsonSize / sizeof(std::string::value_type));
		ResourcesLoader::ReadFromBuffer(scene->rawScene.data(), jsonStr.data(), jsonSize, readPos, scene->rawScene.size());
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
					if(it.value()["TAG"].is_string())
						ent->tag = it.value()["TAG"];
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
				if(it.value()["TAG"].is_string())
					ent->tag = it.value()["TAG"];
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

		std::string subP = elt + "/{SkyBoxName}" ;
		if(renderer->_map != nullptr)
			j[subP] = renderer->_map->name;
		else
			j[subP] = "NO_SKYBOX";

		setJsonSave(std::ref(j), world, std::ref(elt));
		j = j.unflatten();

		std::ofstream file(p, std::ifstream::binary | std::ifstream::trunc);
		std::vector<char> buffer;
		std::stringstream sstr;
		sstr << std::setw(4) << j << std::endl;
		std::size_t sstrSize = sstr.str().size() * sizeof(std::string::value_type);
		ResourcesLoader::Append(buffer, &sstrSize , sizeof(std::size_t));

		ResourcesLoader::Append(buffer, sstr.str().data(), sstrSize);

		SaveAllComp(world, buffer);
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
		ResourcesLoader::ReadFromBuffer(scene->rawScene.data(), &jsonSize, sizeof(std::size_t), readPos, scene->rawScene.size());
		std::string jsonStr;
		jsonStr.resize(jsonSize / sizeof(std::string::value_type));
		ResourcesLoader::ReadFromBuffer(scene->rawScene.data(), jsonStr.data(), jsonSize, readPos, scene->rawScene.size());
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
					if(it.value()["TAG"].is_string())
						ent->tag = it.value()["TAG"];
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
				if(it.value()["TAG"].is_string())
					ent->tag = it.value()["TAG"];
				Lambda(std::ref(it.value()), ent->GetEntity());
			}
		}
		std::string str = j ["Scene"]["{SkyBoxName}"];
		Engine::GetInstance()->renderer->_map = Engine::GetInstance()->graphicsResourceMgr.GetCubemap(str.c_str());
		AddAllComps(world, scene->rawScene, readPos);
		

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
				j[subP + "/TAG"] = sub->tag;
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

		SaveAllComp(world, buffer);
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
		particleEffectSystem->Update(*activeCamera);

		UI::SetCurrentContext(UICompCtx);
		if(hasEndedUIRendering)
		{
			ImGui_ImplOpenGL3_RenderDrawData(UI::GetDrawData());

			if (UI::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backupWindow = glfwGetCurrentContext();
				UI::UpdatePlatformWindows();
				UI::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backupWindow);
			}
		}

		UI::SetCurrentContext(EditorContext);

		renderer->EndFramebuffer();
		audioSystem->Update(*activeCamera);

    }

	void Engine::setJsonSave(json &j, GameObject *elt, std::string &path)
	{

    	for(GameObject* sub : elt->childs)
    	{
    		std::string subP = path + "/{GameObject_"+ std::to_string(sub->GetEntity()) + "}" ;
		    j[subP + "/Name"] = sub->name;
		    j[subP + "/TAG"] = sub->tag;
		    setJsonSave(std::ref(j), sub, std::ref(subP));
    	}


	}

	void Engine::SaveAllComp(GameObject *elt, std::vector<char> &buffer)
	{
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

						std::size_t offset =0;

						std::size_t scriptNameSize = 0;

						//store comp name / string
						scriptNameSize = script->getArchetype().name.size()*sizeof(std::string::value_type);
						ResourcesLoader::Append(buffer, &scriptNameSize, sizeof(std::size_t));
						ResourcesLoader::Append(buffer, (void*)script->getArchetype().name.data(),  scriptNameSize);

						//store num of fields
						std::size_t numFields = script->getArchetype().fields.size();
						ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
						//calc skip size
						std::size_t skipSize = 0;
						for(auto& cField : script->getArchetype().fields)//2 cField var WARN
						{
							std::size_t size = 0;
							size = cField.name.size()*sizeof(std::string::value_type);
							//store field name / string
							skipSize += sizeof(std::size_t);
							skipSize +=  size;
							short isNull = 128;
							if(cField.type.archetype == nullptr)
							{
								isNull = 256;
								std::string str = cField.getData<std::string>(script);
								std::size_t strS =  str.size()*sizeof(std::string::value_type);
								//store isNull
								skipSize += sizeof(short);
								//store field data
								skipSize +=  sizeof(std::size_t);
								skipSize +=  strS;
							}
							else
							{
								skipSize +=  sizeof(short);
								if(cField.type.archetype->name == "String")
								{
									String* str = (String*)cField.getDataAddress(script);
									size = str->size()*sizeof(std::string::value_type);
									skipSize +=  sizeof(std::size_t);
									skipSize +=  size;
								}
								else if(cField.type.archetype->name == "vectorStr")
								{
									vectorStr* vstr = (vectorStr*)cField.getDataAddress(cmp);
									size = vstr->size();
									std::size_t fieldsize = sizeof(std::size_t);
									for(auto& str : *vstr)
									{
										fieldsize += str.size()*sizeof(std::string::value_type);
										fieldsize += sizeof(std::size_t);
									}

									skipSize +=  sizeof(std::size_t);

									skipSize +=  sizeof(std::size_t);
									for(auto& str : *vstr)
									{
										std::size_t strSize = str.size()*sizeof(std::string::value_type);
										skipSize += sizeof(std::size_t);
										skipSize +=  strSize;
									}
								}
								else
								{
									size = cField.type.archetype->memorySize;
									//store isNull
									//store field data
									skipSize += sizeof(std::size_t);
									skipSize += size;

								}
							}

						}

						ResourcesLoader::Append(buffer, &skipSize, sizeof(std::size_t));


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
									vectorStr* vstr = (vectorStr*)cField.getDataAddress(cmp);
									size = vstr->size();
									std::size_t fieldsize = sizeof(std::size_t);
									for(auto& str : *vstr)
									{
										fieldsize += str.size()*sizeof(std::string::value_type);
										fieldsize += sizeof(std::size_t);
									}

									ResourcesLoader::Append(buffer, &fieldsize, sizeof(std::size_t));

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
				else if(cmp->getArchetype().name == "ParticleEffect")
				{
					std::size_t offset =0;

					std::size_t cmpNameSize = 0;

					//store comp name / string
					cmpNameSize = cmp->getArchetype().name.size()*sizeof(std::string::value_type);
					ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
					ResourcesLoader::Append(buffer, (void*)cmp->getArchetype().name.data(),  cmpNameSize);
					//store num of fields
					std::size_t numFields = 0;
					ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
					ParticleEffect* effect = (ParticleEffect*)cmp;
					std::size_t skipSize = 0;


					std::size_t isNull = (effect->GetSystem() != nullptr) ? 128 : 256;
					skipSize += sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->GetSystem();
						skipSize +=  sizeof(std::size_t);

					}
					isNull = (effect->GetEmitter() != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->GetEmitter();
						skipSize +=  sizeof(float);

					}
					isNull = (effect->GetParticleTex() != nullptr) ? 128 : 256;
					skipSize += sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->GetParticleTex();
						cmpNameSize = ptr->name.size()*sizeof(std::string::value_type);
						skipSize += sizeof(std::size_t);
						skipSize += ( cmpNameSize);

					}
					isNull = (effect->GetRenderer() != nullptr) ? 128 : 256;
					skipSize += sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->GetRenderer();
					}
					isNull = (effect->velFromPosGen != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->velFromPosGen;
						skipSize +=  sizeof(Vec4);
						skipSize +=  sizeof(float);
						skipSize += sizeof(float);
					}
					isNull = (effect->colGen != nullptr) ? 128 : 256;
					skipSize += sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->colGen;
						skipSize +=  sizeof(Vec4);
						skipSize +=  sizeof(Vec4);
						skipSize +=  sizeof(Vec4);
						skipSize +=  sizeof(Vec4);
					}
					isNull = (effect->sphereVelGen != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->sphereVelGen;
						skipSize += sizeof(float);
						skipSize +=  sizeof(float);

					}
					isNull = (effect->spherePosGen != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->spherePosGen;
						skipSize +=  sizeof(Vec4);
						skipSize +=  sizeof(float);
						skipSize +=  sizeof(float);
						skipSize += sizeof(float);

					}
					isNull = (effect->timeGen != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->timeGen;
						skipSize +=  sizeof(float);
						skipSize +=  sizeof(float);


					}
					isNull = (effect->velGen != nullptr) ? 128 : 256;
					skipSize += sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->velGen;
						skipSize +=  sizeof(Vec4);
						skipSize +=  sizeof(Vec4);

					}
					isNull = (effect->boxPosGen != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->boxPosGen;
						skipSize +=  sizeof(Vec4);
						skipSize +=  sizeof(Vec4);

					}
					isNull = (effect->killerZoneUpdater != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->killerZoneUpdater;
						skipSize += sizeof(Vec3);
						skipSize +=  sizeof(Vec3);

					}
					isNull = (effect->attractorUpdater != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->attractorUpdater;
						cmpNameSize = ptr->attractors.size();
						skipSize +=  sizeof(std::size_t);
						skipSize += ( sizeof(Vec4) * cmpNameSize);

					}
					isNull = (effect->velColUpdater != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->velColUpdater;
						skipSize +=  sizeof(Vec4);
						skipSize +=  sizeof(Vec4);

					}
					isNull = (effect->posColUpdater != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->posColUpdater;
						skipSize +=  sizeof(Vec4);
						skipSize += sizeof(Vec4);

					}
					isNull = (effect->colorUpdater != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->colorUpdater;

					}
					isNull = (effect->eulerUpdater != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->eulerUpdater;
						skipSize +=  sizeof(Vec4);

					}
					isNull = (effect->floorUpdater != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->floorUpdater;
						skipSize +=  sizeof(float);
						skipSize +=  sizeof(float);


					}
					isNull = (effect->timeUpdater != nullptr) ? 128 : 256;
					skipSize +=  sizeof(std::size_t);
					if(isNull == 128)
					{
						auto ptr = effect->timeUpdater;

					}


					ResourcesLoader::Append(buffer, &skipSize, sizeof(std::size_t));




					std::size_t numP = effect->particlesSize;
					ResourcesLoader::Append(buffer, &numP, sizeof(std::size_t));

					isNull = (effect->GetSystem() != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->GetSystem();
						ResourcesLoader::Append(buffer, &ptr->count, sizeof(std::size_t));

					}
					isNull = (effect->GetEmitter() != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->GetEmitter();
						ResourcesLoader::Append(buffer, &ptr->emitRate, sizeof(float));

					}
					isNull = (effect->GetParticleTex() != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->GetParticleTex();
						cmpNameSize = ptr->name.size()*sizeof(std::string::value_type);
						ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
						ResourcesLoader::Append(buffer, (void*)ptr->name.data(),  cmpNameSize);

					}
					isNull = (effect->GetRenderer() != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->GetRenderer();
					}
					isNull = (effect->velFromPosGen != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->velFromPosGen;
						ResourcesLoader::Append(buffer, &ptr->offset, sizeof(Vec4));
						ResourcesLoader::Append(buffer, &ptr->minScale, sizeof(float));
						ResourcesLoader::Append(buffer, &ptr->maxScale, sizeof(float));
					}
					isNull = (effect->colGen != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->colGen;
						ResourcesLoader::Append(buffer, &ptr->minStartCol, sizeof(Vec4));
						ResourcesLoader::Append(buffer, &ptr->maxStartCol, sizeof(Vec4));
						ResourcesLoader::Append(buffer, &ptr->minEndCol, sizeof(Vec4));
						ResourcesLoader::Append(buffer, &ptr->maxEndCol, sizeof(Vec4));
					}
					isNull = (effect->sphereVelGen != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->sphereVelGen;
						ResourcesLoader::Append(buffer, &ptr->minVel, sizeof(float));
						ResourcesLoader::Append(buffer, &ptr->maxVel, sizeof(float));

					}
					isNull = (effect->spherePosGen != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->spherePosGen;
						ResourcesLoader::Append(buffer, &ptr->center, sizeof(Vec4));
						ResourcesLoader::Append(buffer, &ptr->radX, sizeof(float));
						ResourcesLoader::Append(buffer, &ptr->radY, sizeof(float));
						ResourcesLoader::Append(buffer, &ptr->radZ, sizeof(float));

					}
					isNull = (effect->timeGen != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->timeGen;
						ResourcesLoader::Append(buffer, &ptr->minTime, sizeof(float));
						ResourcesLoader::Append(buffer, &ptr->maxTime, sizeof(float));


					}
					isNull = (effect->velGen != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->velGen;
						ResourcesLoader::Append(buffer, &ptr->minStartVel, sizeof(Vec4));
						ResourcesLoader::Append(buffer, &ptr->maxStartVel, sizeof(Vec4));

					}
					isNull = (effect->boxPosGen != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->boxPosGen;
						ResourcesLoader::Append(buffer, &ptr->pos, sizeof(Vec4));
						ResourcesLoader::Append(buffer, &ptr->maxStartPosOffset, sizeof(Vec4));

					}
					isNull = (effect->killerZoneUpdater != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->killerZoneUpdater;
						ResourcesLoader::Append(buffer, &ptr->pos, sizeof(Vec3));
						ResourcesLoader::Append(buffer, &ptr->offset, sizeof(Vec3));

					}
					isNull = (effect->attractorUpdater != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->attractorUpdater;
						cmpNameSize = ptr->attractors.size();
						ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
						ResourcesLoader::Append(buffer, ptr->attractors.data(), sizeof(Vec4) * cmpNameSize);

					}
					isNull = (effect->velColUpdater != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->velColUpdater;
						ResourcesLoader::Append(buffer, &ptr->minVel, sizeof(Vec4));
						ResourcesLoader::Append(buffer, &ptr->maxVel, sizeof(Vec4));

					}
					isNull = (effect->posColUpdater != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->posColUpdater;
						ResourcesLoader::Append(buffer, &ptr->minPos, sizeof(Vec4));
						ResourcesLoader::Append(buffer, &ptr->maxPos, sizeof(Vec4));

					}
					isNull = (effect->colorUpdater != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->colorUpdater;

					}
					isNull = (effect->eulerUpdater != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->eulerUpdater;
						ResourcesLoader::Append(buffer, &ptr->globalAcceleration, sizeof(Vec4));

					}
					isNull = (effect->floorUpdater != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->floorUpdater;
						ResourcesLoader::Append(buffer, &ptr->bounceFactor, sizeof(float));
						ResourcesLoader::Append(buffer, &ptr->floorY, sizeof(float));


					}
					isNull = (effect->timeUpdater != nullptr) ? 128 : 256;
					ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
					if(isNull == 128)
					{
						auto ptr = effect->timeUpdater;

					}

				}
				else
				{

					std::size_t offset =0;

					std::size_t cmpNameSize = 0;

					//store comp name / string
					cmpNameSize = cmp->getArchetype().name.size()*sizeof(std::string::value_type);
					ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
					ResourcesLoader::Append(buffer, (void*)cmp->getArchetype().name.data(),  cmpNameSize);

					//store num of fields
					std::size_t numFields = cmp->getArchetype().fields.size();
					ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
					std::size_t skipSize = 0;
					for(auto& cField : cmp->getArchetype().fields)//2 cField var WARN
					{
						std::size_t size = 0;
						size = cField.name.size()*sizeof(std::string::value_type);
						//store field name / string
						skipSize += sizeof(std::size_t);
						skipSize +=  size;
						short isNull = 128;
						if(cField.type.archetype == nullptr)
						{
							isNull = 256;
							std::string str = cField.getData<std::string>(cmp);
							std::size_t strS =  str.size()*sizeof(std::string::value_type);
							//store isNull
							skipSize += sizeof(short);
							//store field data
							skipSize +=  sizeof(std::size_t);
							skipSize +=  strS;
						}
						else
						{
							skipSize +=  sizeof(short);
							if(cField.type.archetype->name == "String")
							{
								String* str = (String*)cField.getDataAddress(cmp);
								size = str->size()*sizeof(std::string::value_type);
								skipSize +=  sizeof(std::size_t);
								skipSize +=  size;
							}
							else if(cField.type.archetype->name == "vectorStr")
							{
								vectorStr* vstr = (vectorStr*)cField.getDataAddress(cmp);
								size = vstr->size();
								std::size_t fieldsize = sizeof(std::size_t);
								for(auto& str : *vstr)
								{
									fieldsize += str.size()*sizeof(std::string::value_type);
									fieldsize += sizeof(std::size_t);
								}

								skipSize +=  sizeof(std::size_t);

								skipSize +=  sizeof(std::size_t);
								for(auto& str : *vstr)
								{
									std::size_t strSize = str.size()*sizeof(std::string::value_type);
									skipSize += sizeof(std::size_t);
									skipSize +=  strSize;
								}
							}
							else
							{
								size = cField.type.archetype->memorySize;
								//store isNull
								//store field data
								skipSize += sizeof(std::size_t);
								skipSize += size;

							}
						}

					}

					ResourcesLoader::Append(buffer, &skipSize, sizeof(std::size_t));


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
								std::size_t fieldsize = sizeof(std::size_t);
								for(auto& str : *vstr)
								{
								    fieldsize += str.size()*sizeof(std::string::value_type);
								    fieldsize += sizeof(std::size_t);
								}

								ResourcesLoader::Append(buffer, &fieldsize, sizeof(std::size_t));

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
			SaveAllComp(sub, buffer);
		}



	}

	void Engine::BeginUIComponents()
	{
		if(hasBegunUIRendering)
			return;
		hasBegunUIRendering = true;
		hasEndedUIRendering = false;
		ImVec2 vPos = UI::GetMainViewport()->Pos;
		UI::SetCurrentContext(UICompCtx);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		UI::NewFrame();

		UI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.f, 3.f));
		UI::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));

		UI::PushStyleVar(ImGuiStyleVar_TabRounding, 2.f);
		UI::PushStyleVar(ImGuiStyleVar_FrameRounding, 3);
		UI::PushStyleVar(ImGuiStyleVar_WindowRounding, 3);


		std::function<Vec2(ImVec2)> convert = [](ImVec2 v) -> Vec2{ return Vec2(v.x,v.y);};

		Vec2 vS = convert(UI::GetMainViewport()->Size);
		Vec2 ratio = Vec2(  vS.x/(float )PlayBuffer.size.x,  vS.y/(float )PlayBuffer.size.y);
		Vec2 ratio2 = Vec2((float )PlayBuffer.size.x / vS.x,(float )PlayBuffer.size.y / vS.y);
		Vec2 Mouse = convert(UI::GetIO().MousePos);
		Vec2 fbPos = Vec2(PlayBuffer.pos.x,PlayBuffer.pos.y);
		Vec2 loacal = fbPos -convert(vPos);
		Vec2 loPos = Vec2(loacal.x,loacal.y);
		UI::GetIO().DisplayFramebufferScale = ImVec2(ratio2.x, ratio2.y);
		Mouse.x -= loacal.x;
		Mouse.y -= loacal.y;
		UI::GetIO().MousePos.y = Mouse.y;
		UI::GetIO().MousePos.x = Mouse.x;
		UI::GetIO().MousePos.y *= ratio.y;
		UI::GetIO().MousePos.x *= ratio.x;

	}

	void Engine::EndUIComponents()
	{
    	if(!hasBegunUIRendering)
    		return;
		UI::PopStyleVar(5);

		UI::Render();

		UI::SetCurrentContext(EditorContext);
		hasBegunUIRendering = false;
		hasEndedUIRendering = true;
	}

} //!namespace


