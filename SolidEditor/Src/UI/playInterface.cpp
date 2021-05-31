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

        engine->RenderToBuffer();
        UI::Image((ImTextureID)(size_t)engine->PlayBuffer.texture,windowSize,ImVec2{0,1},ImVec2{1,0});

        UI::End();
    }
}
