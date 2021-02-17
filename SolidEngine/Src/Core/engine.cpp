#include "Core/engine.hpp"

#include <iostream>
#include "Rendering/OpenGL45/openGl45Renderer.hpp"
#include "UI/solidUI.hpp"

namespace Solid
{
    Engine::Engine()
    {
    }

    Engine::~Engine()
    {
        UIContext::ReleaseSolidUI();
        delete window;
        delete renderer;
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
        /// \brief

        if(window != nullptr && renderer != nullptr)
            engineContextInit = true;
    }

    bool Engine::IsEngineContextInitialized()
    {
        return engineContextInit;
    }
} //!namespace
