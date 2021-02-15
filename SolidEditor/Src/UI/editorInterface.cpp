#include "UI/editorInterface.hpp"
#include "UI/solidUI.hpp"
#include "Time/time.hpp"

#include <iostream>
#include <imgui_internal.h>
#include <string>

using namespace Solid;

EditorInterface::EditorInterface():
        editorStyle (UI::GetStyle())
{
    window = nullptr;
}

EditorInterface::EditorInterface(Window* _window):
        editorStyle  (UI::GetStyle())
{
    window = _window;
}

void EditorInterface::Update()
{
    UIContext::BeginFrame();

    DrawMainFrame();

    filesInterface.Draw();
    sceneInterface.Draw();
    inspectorInterface.Draw();
    hierarchyTreeInterface.Draw();
    if (colorOpen)
        DrawChangeColors();
    DrawPerfOverlay();

    UIContext::RenderFrame();
}

/*
 *  Some cool colors following our theme
 */
void EditorInterface::DarkTheme()
{
    editorStyle.Colors[ImGuiCol_WindowBg]       = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
    editorStyle.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.f);
    editorStyle.Colors[ImGuiCol_ChildBg]        = ImVec4(0.1f, 0.1f, 0.1f, 1.f);

    editorStyle.Colors[ImGuiCol_DockingPreview] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);

    editorStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
    editorStyle.Colors[ImGuiCol_TitleBg]   = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
    editorStyle.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
    editorStyle.Colors[ImGuiCol_Border]        = ImVec4(0.1f, 0.1f, 0.1f, 1.f);

    editorStyle.Colors[ImGuiCol_ResizeGripActive]  = ImVec4(0.45f, 0.45f, 0.45f, 1.f);
    editorStyle.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
    editorStyle.Colors[ImGuiCol_ResizeGrip]        = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

    editorStyle.Colors[ImGuiCol_Separator]        = ImVec4(0.05f, 0.05f, 0.05f, 1.f);
    editorStyle.Colors[ImGuiCol_SeparatorActive]  = ImVec4(0.05f, 0.05f, 0.05f, 1.f);
    editorStyle.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.05f, 0.05f, 0.05f, 1.f);


    editorStyle.Colors[ImGuiCol_Tab]                = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
    editorStyle.Colors[ImGuiCol_TabActive]          = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
    editorStyle.Colors[ImGuiCol_TabHovered]         = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
    editorStyle.Colors[ImGuiCol_TabUnfocused]       = ImVec4(0.03f, 0.03f, 0.03f, 1.f);
    editorStyle.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);

    editorStyle.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.f);

    editorStyle.Colors[ImGuiCol_Button]        = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
    editorStyle.Colors[ImGuiCol_ButtonActive]  = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
    editorStyle.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

    editorStyle.Colors[ImGuiCol_FrameBg]        = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
    editorStyle.Colors[ImGuiCol_FrameBgActive]  = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
    editorStyle.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
}

void EditorInterface::LightTheme() {
    editorStyle.Colors[ImGuiCol_WindowBg]       = ImVec4(0.85f, 0.85f, 0.85f, 1.f);
    editorStyle.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
    editorStyle.Colors[ImGuiCol_ChildBg]        = ImVec4(0.85f, 0.85f, 0.85f, 1.f);

    editorStyle.Colors[ImGuiCol_DockingPreview] = ImVec4(0.6, 0.6f, 0.6f, 1.f);

    editorStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
    editorStyle.Colors[ImGuiCol_TitleBg]   = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
    editorStyle.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);
    editorStyle.Colors[ImGuiCol_Border]        = ImVec4(0.6f, 0.6f, 0.6f, 1.f);

    editorStyle.Colors[ImGuiCol_ResizeGripActive]  = ImVec4(0.45f, 0.45f, 0.45f, 1.f);
    editorStyle.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
    editorStyle.Colors[ImGuiCol_ResizeGrip]        = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

    editorStyle.Colors[ImGuiCol_Separator]        = ImVec4(0.75f, 0.75f, 0.75f, 1.f);
    editorStyle.Colors[ImGuiCol_SeparatorActive]  = ImVec4(0.75f, 0.75f, 0.75f, 1.f);
    editorStyle.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.f);

    editorStyle.Colors[ImGuiCol_Tab]                = ImVec4(0.8f, 0.8f, 0.8f, 1.f);
    editorStyle.Colors[ImGuiCol_TabActive]          = ImVec4(0.85f, 0.85f, 0.85f, 1.f);
    editorStyle.Colors[ImGuiCol_TabHovered]         = ImVec4(0.9f, 0.9f, 0.9f, 1.f);
    editorStyle.Colors[ImGuiCol_TabUnfocused]       = ImVec4(0.75f, 0.75f, 0.75f, 1.f);
    editorStyle.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.8f, 0.8f, 0.8f, 1.f);

    editorStyle.Colors[ImGuiCol_Text] = ImVec4(0.05f, 0.05f, 0.05f, 1.f);

    editorStyle.Colors[ImGuiCol_Button]        = ImVec4(0.6f, 0.6f, 0.6f, 1.f);
    editorStyle.Colors[ImGuiCol_ButtonActive]  = ImVec4(0.55f, 0.55f, 0.55f, 1.f);
    editorStyle.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);

    editorStyle.Colors[ImGuiCol_FrameBg]        = ImVec4(0.6f, 0.6f, 0.6f, 1.f);
    editorStyle.Colors[ImGuiCol_FrameBgActive]  = ImVec4(0.55f, 0.55f, 0.55f, 1.f);
    editorStyle.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.f);
}

void EditorInterface::DrawMenuBar()
{
    if (window == nullptr)
        return;

    if(UI::BeginMenuBar())
    {
        if (UI::BeginMenu("File"))
        {
            UI::MenuItem("Save");
            if (UI::BeginMenu("Build"))
            {
                UI::MenuItem("Windows");
                UI::MenuItem("Linux");
                UI::EndMenu();
            }
            if (UI::MenuItem("Colors Settings"))
            {
                colorOpen = true;
            }
            UI::EndMenu();
        }
        UI::EndMenuBar();
    }


}

void EditorInterface::DrawMainFrame()
{
    Int2 windowSize = window->GetWindowSize();
    ImGuiViewport* mainViewport = UI::GetMainViewport();

    UI::SetNextWindowSize(ImVec2((float)windowSize.x, (float)windowSize.y));
    UI::SetNextWindowPos(ImVec2(mainViewport->GetWorkPos()));
    UI::SetNextWindowViewport(mainViewport->ID);

    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing;
    windowFlags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    windowFlags |= ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize;
    windowFlags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;

    if (UI::Begin("mainFrame", &p_open, windowFlags))
    {
        ImGuiID dockID = UI::GetID("mainDockSpace");
        ImVec2 winSize = UI::GetMainViewport()->Size;
        ImGuiDockNodeFlags dockFlags = 0;
        dockFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
        UI::DockSpace(dockID, winSize, dockFlags);
    }

    DrawMenuBar();
    UI::End();
}

void EditorInterface::DrawChangeColors()
{
    ImVec2 size((float)(window->GetWindowSize().x) / 3.f, (float)(window->GetWindowSize().y) / 1.5f);
    UI::SetNextWindowSize(size);
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_NoDocking;
    UI::Begin("colorsSettings", &colorOpen, flags);

    if (UI::CloseButton(UI::GetID("colorsSettings"), ImVec2(0,0)))
        colorOpen = false;

    if (UI::Button("Dark Theme"))
        DarkTheme();
    UI::SameLine();
    if (UI::Button("Light Theme"))
        LightTheme();

    UI::ColorEdit4("##Text Preview",(float*)&editorStyle.Colors[0],ImGuiColorEditFlags_NoInputs);
    UI::SameLine();
    UI::DragFloat4("Text", (float*)&editorStyle.Colors[0], 0.005f, 0.f, 1.f);

    UI::ColorEdit4("##BG Preview",(float*)&editorStyle.Colors[2],ImGuiColorEditFlags_NoInputs);
    UI::SameLine();
    UI::DragFloat4("Background", (float*)&editorStyle.Colors[2], 0.005f, 0.f, 1.f);
    editorStyle.Colors[ImGuiCol_ChildBg] = editorStyle.Colors[2];

    UI::ColorEdit4("##Border Preview",(float*)&editorStyle.Colors[6],ImGuiColorEditFlags_NoInputs);
    UI::SameLine();
    UI::DragFloat4("Border", (float*)&editorStyle.Colors[6], 0.005f, 0.f, 1.f);

    UI::ColorEdit4("##Tab Unfocused Active Preview",(float*)&editorStyle.Colors[ImGuiCol_TabUnfocusedActive],
                   ImGuiColorEditFlags_NoInputs);
    UI::SameLine();
    UI::DragFloat4("Tab Unfocused Active",
               (float*)&editorStyle.Colors[ImGuiCol_TabUnfocusedActive], 0.005f, 0.f, 1.f);

    UI::ColorEdit4("##Tab Unfocused Active",(float*)&editorStyle.Colors[ImGuiCol_TabActive],
                   ImGuiColorEditFlags_NoInputs);
    UI::SameLine();
    UI::DragFloat4("Tab Active",
                   (float*)&editorStyle.Colors[ImGuiCol_TabActive], 0.005f, 0.f, 1.f);


    UI::End();
}

void EditorInterface::DrawPerfOverlay()
{
    UI::SetNextWindowBgAlpha(0.2);
    UI::SetNextWindowSize(ImVec2(150,150));
    UI::SetNextWindowViewport(UI::GetID(UI::GetMainViewport()));
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;
    flags |= ImGuiWindowFlags_NoTitleBar;
    UI::Begin("PerfOverlay", &p_open, flags);
    std::string fps = std::to_string(Time::Fps()) + " fps";
    UI::Text(fps.c_str());

    UI::End();

}
