#include "Core/engine.hpp"

#include <iostream>
#include "Rendering/OpenGL45/openGl45Renderer.hpp"
#include "UI/solidUI.hpp"
#include "Ressources/Ressources.hpp"
namespace Solid
{
    Engine::Engine() :
    ThreadPool(&MultiTask)
    {
        RManager = new ResourceManager(this);
        InitializeRenderer();
        inputManager = InputManager<int>(renderer->GetWindow()->GetHandle());
        UIContext::InitializeSolidUI(renderer->GetWindow()->GetHandle());
    }

    Engine::~Engine()
    {

        UIContext::ReleaseSolidUI();
        delete renderer;
        ThreadPool.TerminateAllThreads();
    }

    void Engine::InitializeRenderer()
    {
        RendererParams rendererParams
        {
            {
                "Solid Engine",
                {1280,720}
            }
        };
        renderer = new OpenGL45Renderer();
        renderer->Initialize(rendererParams);
    }

} //!namespace
