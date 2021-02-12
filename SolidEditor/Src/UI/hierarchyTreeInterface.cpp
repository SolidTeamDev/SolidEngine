#include "UI/hierarchyTreeInterface.hpp"

#include <imgui.h>


namespace Solid
{
    void HierarchyTreeInterface::Draw()
    {
        UI::SetNextWindowSize(ImVec2(250,250));

        UI::Begin("Hierarchy", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        UI::TextWrapped("Welcome on the Solid hierarchy tree!");
        UI::End();
    }
}

