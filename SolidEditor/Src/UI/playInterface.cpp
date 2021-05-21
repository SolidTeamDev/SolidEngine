#include "UI/playInterface.hpp"
#include "Window/window.hpp"

#include <imgui.h>

namespace Solid
{
    PlayInterface::PlayInterface()
    {
        engine = Engine::GetInstance();
    }

    void PlayInterface::Draw()
    {
        if(!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250,250));
        UI::Begin("Play", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

        ImVec2 windowSize = UI::GetContentRegionAvail();

        DrawScene();
        UI::Image((ImTextureID)(size_t)playFramebuffer.texture,windowSize,ImVec2{0,1},ImVec2{1,0});

        UI::End();
    }

    void PlayInterface::DrawScene()
    {
        Camera* camera = engine->activeCamera;

        if(camera != nullptr)
            camera->UpdateCamera(playFramebuffer.size);
        engine->renderer->BeginFramebuffer(playFramebuffer);
        engine->renderer->ClearColor({0.f,0.f,0.f,1});
        engine->renderer->Clear(playFramebuffer.size);
        if(engine->activeCamera != nullptr)
            engine->rendererSystem->Update(engine->renderer,*engine->activeCamera);
        engine->renderer->EndFramebuffer();
    }
}
