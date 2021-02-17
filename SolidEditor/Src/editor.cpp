#include "editor.hpp"
#include "UI/solidUI.hpp"
#include "UI/editorInterface.hpp"
#include "Inputs/editorInputs.hpp"

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

    void Editor::Loop()
    {
        Window* window = engine.window;
        Renderer* renderer = engine.renderer;
        EditorInterface editorInterface(window);

        glfwSwapInterval(0);

        while (!glfwWindowShouldClose(window->GetHandle()) && !editorInputManager->IsPressed(EInputList::ESCAPE))
        {
            glfwPollEvents();
            editorInputManager->Update();

            renderer->Clear(window->GetWindowSize());

            renderer->BeginFramebuffer(sceneFramebuffer);
            renderer->ClearColor({1,0,0,1});
            renderer->Clear(window->GetWindowSize());
            renderer->EndFramebuffer();
            renderer->ClearColor({0,0,0,1});

            editorInterface.Update();

            renderer->UpdateFramebuffer(sceneFramebuffer);
            Time::Update();
            window->SwapBuffers();
        }

    }
} //!namespace