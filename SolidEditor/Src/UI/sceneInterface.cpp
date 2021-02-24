#include "UI/sceneInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor.hpp"

namespace Solid
{
    void SceneInterface::Draw()
    {
        UI::SetNextWindowSize(ImVec2(250,250));

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

        UI::Begin("Scene", &p_open, windowFlags);

        ImVec2 windowSize = UI::GetWindowSize();
        windowSize.y -= 25.f;
        Editor::sceneFramebuffer.size = {(int)windowSize.x,(int)windowSize.y};
        UI::Image((ImTextureID)(size_t)Editor::sceneFramebuffer.texture,windowSize,ImVec2{1,1},ImVec2{0,0});

        UI::End();
    }
}