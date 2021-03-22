#include "UI/sceneInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor.hpp"
#include <algorithm>

namespace Solid
{
    void SceneInterface::Draw()
    {
        if(!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250,250));

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
        windowFlags |= ImGuiWindowFlags_MenuBar;

        UI::Begin("Scene", &p_open, windowFlags);

        ImVec2 windowSize = UI::GetWindowSize();
        windowSize.y -= 50.f;
        Editor::sceneFramebuffer.size = {(int)windowSize.x,(int)windowSize.y};
        UI::Image((ImTextureID)(size_t)Editor::sceneFramebuffer.texture,windowSize,ImVec2{1,1},ImVec2{0,0});
        DrawMenu();


        UI::End();
    }
}

void Solid::SceneInterface::DrawMenu()
{
    UI::BeginMenuBar();

    float speed = Editor::camSpeed/10;
    if (speed < 0.01f)
        speed = 0.01f;
    UI::SliderFloat("Camera Speed", &Editor::camSpeed, 0.1f, 1000.f);
    Editor::camSpeed = std::clamp(Editor::camSpeed, 0.f, 50000.f);


    UI::EndMenuBar();

}
