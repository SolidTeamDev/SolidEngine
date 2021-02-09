#include "UI/sceneInterface.hpp"

#include <imgui.h>

namespace Solid
{
    void SceneInterface::Draw()
    {

        UI::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Set window background to red
        UI::Begin("SceneInterface", &p_open,
        ImGuiWindowFlags_NoCollapse);
        UI::PopStyleColor();
        UI::Text("YOOOOOOOOOO");
        UI::End();
    }
}