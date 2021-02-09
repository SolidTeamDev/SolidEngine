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

    Int2 windowSize = window->GetWindowSize();
    ImGuiViewport* mainViewport = UI::GetMainViewport();

    UI::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    UI::SetNextWindowPos(ImVec2(mainViewport->GetWorkPos()));
    UI::SetNextWindowBgAlpha(0.9f);
    if (UI::Begin("mainFrame", &p_open,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_DockNodeHost))
    {
        ImGuiID sceneDock = ImGui::GetID("sceneDock");
        dockID = UI::GetID("mainDockSpace");
        UI::DockBuilderRemoveNodeChildNodes(dockID);
        UI::DockSpace(dockID, ImVec2(0.f, 0.f));
    }
    DrawMenuBar();
    UI::End();
}
