#include "UI/sceneInterface.hpp"

#include <imgui.h>

namespace Solid
{
    void SceneInterface::Draw()
    {

        UI::Begin("SceneInterface", &p_open,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        UI::Text("Welcome on Solid Engine");
        UI::End();
    }
}