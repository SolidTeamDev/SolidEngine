#include <string>
#include "editor.hpp"
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

    void Editor::UpdateEditorCamera()
    {
        editorCamera.UpdateCamera(sceneFramebuffer.size);

        Transform& editorCameraT = editorCamera.transform;

        //== Mouse

        Vec2d mousePos{};
        editorInputManager->GetCursorPos(mousePos.x,mousePos.y);
        Vec2d deltaPos {mousePos.x - sceneFramebuffer.size.x/2,
                          mousePos.y - sceneFramebuffer.size.y/2};

        float mouseSensitivity = 0.1f;

        deltaPos.x *= mouseSensitivity * Time::DeltaTime();
        deltaPos.y *= mouseSensitivity * Time::DeltaTime();

        Vec3 rot = editorCameraT.GetRotation().ToEuler();

        /*rot.x += deltaPos.x;
        rot.y += deltaPos.y;

        if(rot.x >= S_PI_2)
            rot.x = S_PI_2;
        else if (rot.x <= -S_PI_2)
            rot.x = -S_PI_2;

        editorCameraT.SetRotation(Quat(rot));*/

        //== Movement
        float updateCamSpeed = (float)(camSpeed * Time::DeltaTime());
        float forwardVelocity = 0;

        if(editorInputManager->IsPressed(EInputList::FORWARD))
            forwardVelocity = updateCamSpeed;
        if(editorInputManager->IsPressed(EInputList::BACK))
            forwardVelocity = -updateCamSpeed;

        float strafeVelocity = 0;

        if(editorInputManager->IsPressed(EInputList::LEFT))
            strafeVelocity = updateCamSpeed;
        if(editorInputManager->IsPressed(EInputList::RIGHT))
            strafeVelocity = -updateCamSpeed;

        if(editorInputManager->IsPressed(EInputList::UP))
            editorCameraT.Translate(Vec3(0,-updateCamSpeed,0));
        if(editorInputManager->IsPressed(EInputList::DOWN))
            editorCameraT.Translate(Vec3(0,updateCamSpeed,0));


        /*editorCameraT.Translate(Vec3(forwardVelocity,
                                     -forwardVelocity,
                                     forwardVelocity));

        editorCameraT.Translate(Vec3(strafeVelocity,
                                     0,
                                     strafeVelocity));*/

        editorCameraT.Translate(Vec3(sin(-rot.y) * cos(-rot.x) * forwardVelocity,
                                     -(sin(rot.x) * forwardVelocity),
                                     cos(rot.y) * cos(rot.x) * forwardVelocity));

        editorCameraT.Translate(Vec3(cos(rot.y)  * strafeVelocity,
                                     0,
                                     sin(rot.y)  * strafeVelocity));
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

		Compiler->srcPath = CodePath;
		Compiler->IncludePath = EngineIncPath;
		Compiler->ProjectName = ProjectName;
		Compiler->CreateCmake();
	}
} //!namespace
