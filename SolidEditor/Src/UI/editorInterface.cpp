#include "UI/editorInterface.hpp"
#include "UI/solidUI.hpp"

#include <iostream>
#include <imgui_internal.h>


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

    UIContext::RenderFrame();
}

/*
 *  Some cool colors following our theme
 */
void EditorInterface::DarkTheme()
{
    editorStyle.Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
    editorStyle.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.f);
    editorStyle.Colors[ImGuiCol_ChildBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);

    editorStyle.Colors[ImGuiCol_DockingPreview]= ImVec4(0.25f, 0.25f, 0.25f, 1.f);

    editorStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
    editorStyle.Colors[ImGuiCol_Border] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);

    editorStyle.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.f);
    editorStyle.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
    editorStyle.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

    editorStyle.Colors[ImGuiCol_Separator] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
    editorStyle.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
    editorStyle.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);


    editorStyle.Colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
    editorStyle.Colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
    editorStyle.Colors[ImGuiCol_TabHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
    editorStyle.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.03f, 0.03f, 0.03f, 1.f);
    editorStyle.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
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


    UI::ColorButton("Text Preview", editorStyle.Colors[0]);
    UI::SameLine();
    UI::DragFloat4("Text", (float*)&editorStyle.Colors[0], 0.005f, 0.f, 1.f);

    UI::ColorButton("BG Preview", editorStyle.Colors[2]);
    UI::SameLine();
    UI::DragFloat4("Background", (float*)&editorStyle.Colors[2], 0.005f, 0.f, 1.f);
    editorStyle.Colors[ImGuiCol_ChildBg] = editorStyle.Colors[2];

    UI::ColorButton("Border Preview", editorStyle.Colors[6]);
    UI::SameLine();
    UI::DragFloat4("Border", (float*)&editorStyle.Colors[6], 0.005f, 0.f, 1.f);

    UI::ColorButton("Tab Unfocused Active Preview",
                    editorStyle.Colors[ImGuiCol_TabUnfocusedActive]);
    UI::SameLine();
    UI::DragFloat4("Tab Unfocused Active",
               (float*)&editorStyle.Colors[ImGuiCol_TabUnfocusedActive], 0.005f, 0.f, 1.f);

    UI::ColorButton("Tab Active Preview",
                    editorStyle.Colors[ImGuiCol_TabActive]);
    UI::SameLine();
    UI::DragFloat4("Tab Active",
                   (float*)&editorStyle.Colors[ImGuiCol_TabActive], 0.005f, 0.f, 1.f);


    UI::End();
}


