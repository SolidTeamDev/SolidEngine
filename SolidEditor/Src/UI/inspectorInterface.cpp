#include "UI/inspectorInterface.hpp"

#include <imgui.h>

namespace Solid
{
    void InspectorInterface::Draw()
    {
        UI::SetNextWindowSize(ImVec2(250,250));

        UI::Begin("InspectorInterface", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        UI::Text("Welcome on the Solid inspector!");
        UI::End();
    }
}
