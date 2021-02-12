#include "UI/sceneInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace Solid
{
    void SceneInterface::Draw()
    {
        UI::SetNextWindowSize(ImVec2(250,250));

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;


        UI::Begin("Scene", &p_open, windowFlags);

        UI::TextWrapped("Welcome on the Solid Scene!");
        UI::End();
    }
}