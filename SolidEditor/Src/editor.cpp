#include <string>

#include "editor.hpp"
#undef ERROR
#include "UI/solidUI.hpp"
#include "UI/editorInterface.hpp"
#include "Inputs/editorInputs.hpp"
#include "Resources/ressources.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"
#include "ECS/Components/scriptList.hpp"
namespace Solid
{
    InputManager<int>* Editor::editorInputManager = nullptr;
	bool Editor::play = false;
	bool Editor::paused = false;
	std::vector<CompDataSave> Editor::compsSave;
	std::vector<PrimaryCompSave<Transform>> Editor::transSave;
	std::vector<PrimaryCompSave<Camera>> Editor::camSave;
	std::vector<PrimaryCompSave<AudioSource>> Editor::audioSave;
	std::vector<PrimaryCompSave<MeshRenderer>> Editor::meshSave;
	std::vector<PrimaryCompSave<RigidBody>> Editor::rbSave;
	std::vector<PrimaryCompSave<BoxCollider>> Editor::boxSave;
	std::vector<PrimaryCompSave<SphereCollider>> Editor::sphereSave;
	std::vector<PrimaryCompSave<CapsuleCollider>> Editor::capsuleSave;
    Editor::Editor()
    {
        WindowParams windowParams
        {
            .title = "Solid Editor",
            .windowSize = {1280,720}
        };

        RendererParams rendererParams
        {
            .rendererType = ERendererType::OpenGl45
        };
		engine = Engine::GetInstance();
        engine->InitEngineContext(windowParams, rendererParams);

        if(!engine->IsEngineContextInitialized())
            throw ThrowError("Engine not correctly initialized !",ESolidErrorCode::S_INIT_ERROR);

        const Window* window = engine->window;

        editorInputManager = new InputManager<int>(window->GetHandle());
        InitEditorInputList(editorInputManager);




    }

    Editor::~Editor()
    {
    	delete editorInputManager;
        //delete engine;
    }

    void Editor::Run()
    {
        Window* window = engine->window;
        Renderer* renderer = engine->renderer;
        EditorInterface editorInterface(window, renderer);

        glfwSwapInterval(0);

	    while (!glfwWindowShouldClose(window->GetHandle()))
        {
            glfwPollEvents();
            editorInputManager->Update();

            //TODO: Update engine task in engine
			engine->ForceUpdate();
            if(play && !paused)
            {
	            engine->Update();
	            engine->FixedUpdate();
	            engine->LateUpdate();
            }

            renderer->ClearColor({0,0,0,1});
            renderer->Clear(window->GetWindowSize());

            editorInterface.Update();

            Time::Update();

            window->SwapBuffers();
        }

    }

    void Editor::LoadResources( fs::path& p)
    {
        engine->EnableMultiThread(true);
		ResourcesLoader::SolidPath = p;
        ResourcesLoader loader;


        loader.SetManager(&(engine->resourceManager));

        auto before = std::chrono::high_resolution_clock::now();
        loader.LoadResourcesFromFolder(p);
        auto after = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::ratio<1,1000>> fp_s = after - before;
        std::cout << "Loading MT has Taken " << fp_s.count() << " milliseconds\n";
        engine->EnableMultiThread(false);
    }


	void Editor::InitFromProject(fs::path _projectPath)
	{
		std::string ProjectName =CurrentProjectJson["Project"]["Name"] ;
		fs::path ProjectPath =_projectPath.parent_path().string() ;
		fs::path AssetPath =ProjectPath.string() + "/" +(std::string(CurrentProjectJson["Project"]["AssetFolder"])) ;
		fs::path CodePath =ProjectPath.string() +"/" +(std::string(CurrentProjectJson["Project"]["SourcesFolder"])) ;
		fs::path EngineIncPath =ProjectPath.string() +"/" +(std::string(CurrentProjectJson["Project"]["EngineIncludeFolder"])) ;

		LoadResources(AssetPath);
		fs::path init = EngineIncPath;
		init.append("INIT");
		bool b = (!fs::is_empty(EngineIncPath) && fs::exists(init));
		if(!b)
		{
			auto editP =fs::current_path();
			auto EngineInc = editP.append("Include");

			if(fs::exists(EngineInc))
			{
				const auto opt = fs::copy_options::recursive | fs::copy_options::update_existing;
				fs::copy(EngineInc, EngineIncPath, opt);
			}


			fs::create_directory(init);


		}
		auto editP =fs::current_path();
		editP.append("Dlls");
		auto t = CodePath;
		t = t.append("Dlls");
		if(!fs::exists(t))
		{
			fs::create_directory(t);
			if(fs::exists(editP))
			{
				const auto opt = fs::copy_options::recursive | fs::copy_options::update_existing;
				fs::copy(editP, t, opt);
			}
		}
		else
		{
			if(fs::exists(editP))
			{
				const auto opt = fs::copy_options::recursive | fs::copy_options::update_existing;
				fs::copy(editP, t, opt);
			}
		}
		GameCompiler* Compiler = Engine::GetInstance()->Compiler;
		Compiler->srcPath = CodePath;
		Compiler->IncludePath = EngineIncPath;
		Compiler->DllPath = CodePath;
		Compiler->DllPath.append("Build").append(ProjectName+".dll");
		Compiler->ProjectName = ProjectName;

		fs::path tmp = fs::current_path();
		tmp.append("Temp").append(ProjectName+"_Tmp.dll");
		if(fs::exists(tmp))
		{
			Compiler->hGetProcIDDLL = LoadLibrary(tmp.string().c_str());
			if(Compiler->hGetProcIDDLL == nullptr)
			{
				Log::Send("LIB LOAD FAILED", Log::ELogSeverity::ERROR);
				Compiler->entryPoint = nullptr;
				Compiler->getClass = nullptr;
				Compiler->getNamespace = nullptr;
			}
			else
			{
				Compiler->entryPoint = (f_Entry)GetProcAddress(Compiler->hGetProcIDDLL, "Entry");
				Compiler->getClass = (f_GetClass)GetProcAddress(Compiler->hGetProcIDDLL, "GetClass");
				Compiler->getNamespace = (f_GetNamespace)GetProcAddress(Compiler->hGetProcIDDLL, "GetNamespace");
			}
		}

		Compiler->CreateCmake();
	}

	void Editor::Play()
	{
    	if(!play)
	    {
    		compsSave.clear();
		    transSave.clear();
		    camSave.clear();
		    audioSave.clear();
		    meshSave.clear();
		    rbSave.clear();
		    boxSave.clear();
		    sphereSave.clear();
		    capsuleSave.clear();
		    auto compArray =Engine::GetInstance()->ecsManager.GetCompArray<ScriptList>();
		    std::array<ScriptList, MAX_ENTITIES>& array = compArray->GetArray();
		    std::unordered_map<Entity, size_t>& idArray = compArray->GetIndexesArray();
		    std::unordered_map<size_t , Entity>& entArray = compArray->GetEntitiesArray();

		    for(auto& elt : idArray)
		    {
			    ScriptList& scriptListToSave =array[elt.second];
			    for(Script* scriptToSave : scriptListToSave.GetAllScripts())
			    {
				    CompDataSave compD;
				    compD.compID = (void*)scriptToSave;
				    for(auto& field : scriptToSave->getArchetype().fields)
				    {
					    FieldData fieldD;
					    fieldD.fName= field.name;
					    void* fData =field.getDataAddress(scriptToSave);
					    uint fSize = field.type.archetype->memorySize;
					    fieldD.fData.resize(fSize);
					    std::memcpy(fieldD.fData.data(), fData, fSize);
					    compD.fields.push_back(std::move(fieldD));
				    }

				    compD.entityCompIndex = elt.second;
				    compsSave.push_back(std::move(compD));
			    }

		    }
		    //primary Comps Save
		    {

			    {
				    auto compA =Engine::GetInstance()->ecsManager.GetCompArray<Transform>();
				    std::array<Transform, MAX_ENTITIES>& array = compA->GetArray();
				    std::unordered_map<Entity, size_t>& idArray = compA->GetIndexesArray();
				    std::unordered_map<size_t , Entity>& entArray = compA->GetEntitiesArray();
				    for(auto& elt : idArray)
				    {
				        PrimaryCompSave<Transform> save {.comp =array[elt.second], .TID =&(array[elt.second]) };
				        transSave.push_back(std::move(save));
				    }
			    }
			    {
				    auto compA =Engine::GetInstance()->ecsManager.GetCompArray<Camera>();
				    std::array<Camera, MAX_ENTITIES>& array = compA->GetArray();
				    std::unordered_map<Entity, size_t>& idArray = compA->GetIndexesArray();
				    std::unordered_map<size_t , Entity>& entArray = compA->GetEntitiesArray();
				    for(auto& elt : idArray)
				    {
				        PrimaryCompSave<Camera> save {.comp =array[elt.second], .TID =&(array[elt.second]) };
				        camSave.push_back(std::move(save));
				    }
			    }
			    {
				    auto compA =Engine::GetInstance()->ecsManager.GetCompArray<AudioSource>();
				    std::array<AudioSource, MAX_ENTITIES>& array = compA->GetArray();
				    std::unordered_map<Entity, size_t>& idArray = compA->GetIndexesArray();
				    std::unordered_map<size_t , Entity>& entArray = compA->GetEntitiesArray();
				    for(auto& elt : idArray)
				    {
				        PrimaryCompSave<AudioSource> save {.comp =array[elt.second], .TID =&(array[elt.second]) };
				        audioSave.push_back(std::move(save));
				    }
			    }
			    {
				    auto compA =Engine::GetInstance()->ecsManager.GetCompArray<MeshRenderer>();
				    std::array<MeshRenderer, MAX_ENTITIES>& array = compA->GetArray();
				    std::unordered_map<Entity, size_t>& idArray = compA->GetIndexesArray();
				    std::unordered_map<size_t , Entity>& entArray = compA->GetEntitiesArray();
				    for(auto& elt : idArray)
				    {
				        PrimaryCompSave<MeshRenderer> save {.comp =array[elt.second], .TID =&(array[elt.second]) };
				        meshSave.push_back(std::move(save));
				    }
			    }
			    {
				    auto compA =Engine::GetInstance()->ecsManager.GetCompArray<RigidBody>();
				    std::array<RigidBody, MAX_ENTITIES>& array = compA->GetArray();
				    std::unordered_map<Entity, size_t>& idArray = compA->GetIndexesArray();
				    std::unordered_map<size_t , Entity>& entArray = compA->GetEntitiesArray();
				    for(auto& elt : idArray)
				    {
				        PrimaryCompSave<RigidBody> save {.comp =array[elt.second], .TID =&(array[elt.second]) };
				        rbSave.push_back(std::move(save));
				    }
			    }
			    {
				    auto compA =Engine::GetInstance()->ecsManager.GetCompArray<BoxCollider>();
				    std::array<BoxCollider, MAX_ENTITIES>& array = compA->GetArray();
				    std::unordered_map<Entity, size_t>& idArray = compA->GetIndexesArray();
				    std::unordered_map<size_t , Entity>& entArray = compA->GetEntitiesArray();
				    for(auto& elt : idArray)
				    {
				        PrimaryCompSave<BoxCollider> save {.comp =array[elt.second], .TID =&(array[elt.second]) };
				        boxSave.push_back(std::move(save));
				    }
			    }
			    {
				    auto compA =Engine::GetInstance()->ecsManager.GetCompArray<CapsuleCollider>();
				    std::array<CapsuleCollider, MAX_ENTITIES>& array = compA->GetArray();
				    std::unordered_map<Entity, size_t>& idArray = compA->GetIndexesArray();
				    std::unordered_map<size_t , Entity>& entArray = compA->GetEntitiesArray();
				    for(auto& elt : idArray)
				    {
				        PrimaryCompSave<CapsuleCollider> save {.comp =array[elt.second], .TID =&(array[elt.second]) };
				        capsuleSave.push_back(std::move(save));
				    }
			    }
			    {
				    auto compA =Engine::GetInstance()->ecsManager.GetCompArray<SphereCollider>();
				    std::array<SphereCollider, MAX_ENTITIES>& array = compA->GetArray();
				    std::unordered_map<Entity, size_t>& idArray = compA->GetIndexesArray();
				    std::unordered_map<size_t , Entity>& entArray = compA->GetEntitiesArray();
				    for(auto& elt : idArray)
				    {
				        PrimaryCompSave<SphereCollider> save {.comp =array[elt.second], .TID =&(array[elt.second]) };
				        sphereSave.push_back(std::move(save));
				    }
			    }

			    Engine::GetInstance()->Init();
		    }

		    play = true;
			//save component State
	    }
	}

	void Editor::Pause()
	{
		if(play)
		{
			paused = !paused;
		}
		else
		{
			paused = false;
		}
	}

	void Editor::Stop()
	{
		if(play)
		{
			if(paused)
				paused = false;
			auto compArray =Engine::GetInstance()->ecsManager.GetCompArray<ScriptList>();
			std::array<ScriptList, MAX_ENTITIES>& array = compArray->GetArray();
			std::unordered_map<Entity, size_t>& idArray = compArray->GetIndexesArray();
			std::unordered_map<size_t , Entity>& entArray = compArray->GetEntitiesArray();
			// reset state
			//std::array<Script*, MAX_ENTITIES> newCompArray {};
			//std::vector<Entity> compsNotCreated;
			for(auto& elt : compsSave)
			{
				for(ScriptList& sl : array)
				{
					for(Script* eltScript : sl.GetAllScripts())
					{
						if((std::size_t)eltScript == (std::size_t)elt.compID)
						{
							for(auto& field : elt.fields)
							{
								const rfk::Field* f= eltScript->getArchetype().getField(field.fName);
								if(f != nullptr)
									f->setData(eltScript, field.fData.data(), field.fData.size());
							}
						}


					}
				}

			}

			for(auto& elt : transSave)
			{
				*((Transform*)elt.TID) = elt.comp;
			}
			for(auto& elt : camSave)
			{
				*((Camera*)elt.TID) = elt.comp;
			}
			for(auto& elt : audioSave)
			{
				*((AudioSource*)elt.TID) = elt.comp;
			}
			for(auto& elt : meshSave)
			{
				*((MeshRenderer*)elt.TID) = elt.comp;
			}
			for(auto& elt : rbSave)
			{
				*((RigidBody*)elt.TID) = elt.comp;
			}
			for(auto& elt : boxSave)
			{
				*((BoxCollider*)elt.TID) = elt.comp;
			}
			for(auto& elt : sphereSave)
			{
				*((SphereCollider*)elt.TID) = elt.comp;
			}
			for(auto& elt : capsuleSave)
			{
				*((CapsuleCollider*)elt.TID) = elt.comp;
			}

			play = false;
			compsSave.clear();
			transSave.clear();
			camSave.clear();
			audioSave.clear();
			meshSave.clear();
			rbSave.clear();
			boxSave.clear();
			sphereSave.clear();
			capsuleSave.clear();

		}
	}
} //!namespace
