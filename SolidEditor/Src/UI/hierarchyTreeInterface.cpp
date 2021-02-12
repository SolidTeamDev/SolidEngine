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
        DrawCreateObject();
        UI::End();
    }
}

void Solid::HierarchyTreeInterface::DrawCreateObject()
{
    if(UI::BeginPopupContextWindow("createObject"))
    {
        if(UI::BeginMenu("New"))
        {
            if(UI::MenuItem("Cube"))
            {

            }
            if(UI::MenuItem("Sphere"))
            {

            }
            if(UI::MenuItem("Solid"))
            {

            }
            UI::EndMenu();
        }

        UI::EndPopup();
    }
}
