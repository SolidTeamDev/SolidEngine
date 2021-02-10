#include "UI/editorInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include "UI/solidUI.hpp"

using namespace Solid;

EditorInterface::EditorInterface(Window* _window)
{
    window = _window;
}

void EditorInterface::Update()
{
    UIContext::BeginFrame();

    DrawMainFrame();
    sceneInterface.Draw();

    UIContext::RenderFrame();
}

void EditorInterface::DrawMenuBar()
{
    if (window == nullptr)
        return;

    if(UI::BeginMenuBar())
    {
        if (UI::BeginMenu("Project"))
        {
            UI::MenuItem("Save");
            if (UI::BeginMenu("Build"))
            {
                UI::MenuItem("Windows");
                UI::MenuItem("Linux");
                UI::EndMenu();
            }
            UI::EndMenu();
        }
        UI::EndMenuBar();
    }


}

void EditorInterface::DrawMainFrame()
{
    static ImGuiID dockID = 0;
    static bool init = false;
    Int2 windowSize = window->GetWindowSize();
    ImGuiViewport* mainViewport = UI::GetMainViewport();

    UI::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    UI::SetNextWindowPos(ImVec2(mainViewport->GetWorkPos()));
    UI::SetNextWindowViewport(mainViewport->ID);

    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing;
    windowFlags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_DockNodeHost;
    windowFlags |= ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize;
    windowFlags |= ImGuiWindowFlags_NoDocking;

    if (UI::Begin("mainFrame", &p_open, windowFlags))
    {
        dockID = UI::GetID("mainDockSpace");

            /*ImGui::DockBuilderRemoveNode(dockID); // Clear out existing layout
            ImGuiID fullNode = ImGui::DockBuilderAddNode(dockID); // Add empty node
            UI::DockBuilderSetNodeSize(fullNode, UI::GetMainViewport()->Size);
            UI::DockBuilderSetNodePos(fullNode, ImVec2(0,0));

            ImGuiID leftNode = ImGui::DockBuilderSplitNode(fullNode, ImGuiDir_Down, 0.20f, NULL, nullptr);

            ImGui::DockBuilderDockWindow("SceneInterface", leftNode);
            ImGui::DockBuilderFinish(dockID);*/


        UI::DockSpace(dockID, ImVec2(0.f, 0.f));
    }

    DrawMenuBar();
    UI::End();
}
