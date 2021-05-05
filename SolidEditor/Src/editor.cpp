#include <string>
#include "editor.hpp"
#undef ERROR
#include "UI/solidUI.hpp"
#include "UI/editorInterface.hpp"
#include "Inputs/editorInputs.hpp"
#include "Resources/ressources.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"

namespace Solid
{
    InputManager<int>* Editor::editorInputManager = nullptr;

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

        Compiler = GameCompiler::GetInstance();


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

        String t = "hh";


	    while (!glfwWindowShouldClose(window->GetHandle()))
        {
            glfwPollEvents();
            editorInputManager->Update();

            //TODO: Update engine task in engine
            bool play = true;
            if(play)
                engine->Update();
            engine->FixedUpdate();
            engine->LateUpdate();

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


	void Editor::InitFromProject()
	{
		std::string ProjectName =CurrentProjectJson["Project"]["Name"] ;
		fs::path ProjectPath =std::string(CurrentProjectJson["Project"]["Path"]) ;
		fs::path AssetPath =ProjectPath.string() + "/" +(std::string(CurrentProjectJson["Project"]["AssetFolder"])) ;
		fs::path CodePath =ProjectPath.string() +"/" +(std::string(CurrentProjectJson["Project"]["SourcesFolder"])) ;
		fs::path EngineIncPath =ProjectPath.string() +"/" +(std::string(CurrentProjectJson["Project"]["EngineIncludeFolder"])) ;

		LoadResources(AssetPath);
		bool b = (!fs::is_empty(EngineIncPath) && fs::exists(EngineIncPath.append("INIT")));
		if(!b)
		{
			auto editP =fs::current_path();
			auto EngineInc = editP.append("Include");

			if(fs::exists(EngineInc))
			{
				const auto opt = fs::copy_options::recursive | fs::copy_options::update_existing;
				fs::copy(EngineInc, EngineIncPath, opt);
			}


			fs::create_directory(EngineIncPath.string() + "/INIT");


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
} //!namespace
