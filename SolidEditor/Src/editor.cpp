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
    InputManager* Editor::editorInputManager = nullptr;
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
	SceneResource* Editor::tempScene = nullptr;
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

        editorInputManager = new InputManager(window->GetHandle());
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
		if(fs::is_empty(EngineIncPath) || !fs::exists(init) || ForceInit)
		{
			auto editP =fs::current_path();
			auto EngineInc = editP.append("Include");

			if(fs::exists(EngineInc))
			{
				const auto opt = fs::copy_options::recursive | fs::copy_options::update_existing;
				fs::copy(EngineInc, EngineIncPath, opt);
			}

			if(!fs::exists(init))
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
		    if(tempScene != nullptr)
		    	delete tempScene;
		    tempScene = Engine::GetInstance()->SaveTempScene();


		    Engine::GetInstance()->InitScript();

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
			std::string selectName;
			if(EditorInterface::selectedGO != nullptr)
				selectName = EditorInterface::selectedGO->name;
			Engine::GetInstance()->LoadTempScene(tempScene);
			if(tempScene != nullptr)
			{
				delete tempScene;
				tempScene = nullptr;
			}
			if(!selectName.empty())
				EditorInterface::selectedGO = Engine::GetInstance()->ecsManager.FindGameObjectByName(selectName);
			play = false;
			Engine::GetInstance()->DestroyScript();
		}
	}
} //!namespace
