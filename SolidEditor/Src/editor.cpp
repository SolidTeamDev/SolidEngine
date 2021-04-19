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

    void Editor::LoadResources(bool _solid)
    {
        engine->EnableMultiThread(true);

        ResourcesLoader loader;

        loader.SetManager(&(engine->resourceManager));

        fs::path p = fs::current_path().append("Resources");

        if(_solid)
			p = fs::current_path().append("SolidResources");
        auto before = std::chrono::high_resolution_clock::now();
        loader.LoadResourcesFromFolder(p);
        auto after = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::ratio<1,1000>> fp_s = after - before;
        std::cout << "Loading MT has Taken " << fp_s.count() << " milliseconds\n";
        engine->EnableMultiThread(false);
    }
} //!namespace
