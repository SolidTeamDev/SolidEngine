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


        UI::Begin("SceneInterface", &p_open, windowFlags);

        /*ImGuiID currID = UI::GetID("SceneDock");

        UI::DockBuilderRemoveNode(currID);
        UI::DockBuilderFinish(currID);

        UI::DockSpace(currID, ImVec2(0,0));*/

        UI::Text("Welcome on the Solid Scene!");
        UI::End();
    }
}