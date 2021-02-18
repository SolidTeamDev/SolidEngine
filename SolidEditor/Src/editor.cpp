#include "editor.hpp"
#include "UI/solidUI.hpp"
#include "UI/editorInterface.hpp"
#include "Inputs/editorInputs.hpp"

#include "Ressources/Ressources.hpp"

#include <string>

namespace Solid
{
    Framebuffer Editor::sceneFramebuffer;

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

        engine.InitEngineContext(windowParams, rendererParams);

        if(!engine.IsEngineContextInitialized())
            throw std::runtime_error("Engine not correctly initialized !");

        const Window* window = engine.window;

        editorInputManager = new InputManager<int>(window->GetHandle());
        InitEditorInputList(editorInputManager);

        sceneFramebuffer = engine.renderer->CreateFramebuffer(window->GetWindowSize());
    }

    Editor::~Editor()
    {
        delete editorInputManager;
    }

    void Editor::Run()
    {
        Window* window = engine.window;
        Renderer* renderer = engine.renderer;
        EditorInterface editorInterface(window);

        engine.EnableMultiThread(true);

        ResourcesLoader loader;

        loader.SetManager(engine.RManager);
        fs::path p = fs::current_path().append("SolidResources");

        auto before = std::chrono::high_resolution_clock::now();
        loader.LoadResourcesFromFolder(p);
        auto after = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::ratio<1,1000>> fp_s = after - before;
        std::cout << "Loading has Taken " << fp_s.count() << " milliseconds\n";

        glfwSwapInterval(0);

        while (!glfwWindowShouldClose(window->GetHandle()) && !editorInputManager->IsPressed(EInputList::ESCAPE))
        {
            glfwPollEvents();
            editorInputManager->Update();

            renderer->ClearColor({0,0,0,1});
            renderer->Clear(window->GetWindowSize());

            renderer->BeginFramebuffer(sceneFramebuffer);
            renderer->ClearColor({0.3f,0.3f,0.3f,1});
            renderer->Clear(window->GetWindowSize());
            renderer->EndFramebuffer();

            editorInterface.Update();

            renderer->UpdateFramebuffer(sceneFramebuffer);
            Time::Update();

            window->SwapBuffers();
        }

    }
} //!namespace