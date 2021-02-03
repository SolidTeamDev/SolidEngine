#include "Core/engine.hpp"

#include <iostream>
#include "Rendering/OpenGL45/openGl45Renderer.hpp"
#include "UI/solidUI.hpp"
namespace Solid
{
    Engine::Engine()
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
        UI::InitializeSolidUI(renderer->GetWindow()->GetHandle());

    }

    Engine::~Engine()
    {
        UI::ReleaseSolidUI();
    }
} //!namespace
