#include "UI/editorInterface.hpp"

#include <imgui.h>
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
    DrawMenuBar();

    UIContext::RenderFrame();
}

void EditorInterface::DrawMenuBar()
{
    if (window == nullptr)
        return;

    if(UI::BeginMainMenuBar())
    {
        if (UI::BeginMenu("File"))
        {
            UI::MenuItem("TestA1");
            if (UI::BeginMenu("TestA2.."))
            {
                UI::MenuItem("TestA2-1");
                UI::EndMenu();
            }
            UI::EndMenu();

        }
        UI::EndMainMenuBar();
    }


}

void EditorInterface::DrawMainFrame()
{
    Int2 windowSize = window->GetWindowSize();
    UI::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    UI::SetNextWindowPos(ImVec2(0.f, 20.f));
    UI::SetNextWindowBgAlpha(0.9f);
    UI::Begin("mainFrame", &p_open, ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    UI::End();
}
