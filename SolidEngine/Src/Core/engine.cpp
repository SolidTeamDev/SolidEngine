#include "Core/engine.hpp"

#include <iostream>
#include "Rendering/OpenGL45/openGl45Renderer.hpp"
#include "UI/solidUI.hpp"
#include "Ressources/Ressources.hpp"

#include "ECS/Components/transform.hpp"

namespace Solid
{
    Engine::Engine() :
    ThreadPool(&MultiTask)
    {
        RManager = new ResourceManager(this);
        InitEcs();
    }

    Engine::~Engine()
    {

        UIContext::ReleaseSolidUI();
        delete window;
        delete renderer;
        ThreadPool.TerminateAllThreads();
    }

    void Engine::InitEcs()
    {
        ecsManager.Init();

        ecsManager.RegisterComponent<Transform>();
    }

    void Engine::InitEngineContext(const WindowParams& _windowParams, const RendererParams& _rendererParams)
    {
        window   = new Window(_windowParams);
        switch (_rendererParams.rendererType)
        {
            case ERendererType::OpenGl45:
                renderer = new OpenGL45Renderer();
                break;
        }

        /// TEMPORARY
        UIContext::InitializeSolidUI(window->GetHandle());
        ///

        if(window != nullptr && renderer != nullptr)
            engineContextInit = true;
    }

    bool Engine::IsEngineContextInitialized() const
    {
        return engineContextInit;
    }
} //!namespace
