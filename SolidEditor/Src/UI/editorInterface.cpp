#include <imgui.h>
#include <imgui_internal.h>

#include "UI/editorInterface.hpp"
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
    inspectorInterface.Draw();
    hierarchyTreeInterface.Draw();

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
    windowFlags |= ImGuiWindowFlags_MenuBar;
    windowFlags |= ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize;
    windowFlags |= ImGuiWindowFlags_NoDocking;

    if (UI::Begin("mainFrame", &p_open, windowFlags))
    {
        dockID = UI::GetID("mainDockSpace");
        ImVec2 winSize = UI::GetMainViewport()->Size;
        ImGuiDockNodeFlags dockFlags = 0;
        dockFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
        dockFlags |= ImGuiDockNodeFlags_NoDockingOverMe;
        UI::DockSpace(dockID, winSize, dockFlags);
    }

    DrawMenuBar();
    UI::End();
}
