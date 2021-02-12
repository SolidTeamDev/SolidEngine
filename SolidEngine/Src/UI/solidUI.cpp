#include "UI/solidUI.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
namespace Solid::UIContext
{
    void InitializeSolidUI(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport
        io.ConfigWindowsResizeFromEdges = true;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void ReleaseSolidUI()
    {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }


    void BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        UI::NewFrame();
        UI::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.f));
        UI::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.02f, 0.02f, 0.02f, 1.f));
        UI::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.f));

        UI::PushStyleColor(ImGuiCol_DockingPreview, ImVec4(0.25f, 0.25f, 0.25f, 1.f));

        UI::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        UI::PushStyleColor(ImGuiCol_Border, ImVec4(0.1f, 0.1f, 0.1f, 1.f));

        UI::PushStyleColor(ImGuiCol_ResizeGripActive, ImVec4(0.45f, 0.45f, 0.45f, 1.f));
        UI::PushStyleColor(ImGuiCol_ResizeGripHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.f));
        UI::PushStyleColor(ImGuiCol_ResizeGrip, ImVec4(0.35f, 0.35f, 0.35f, 1.f));

        UI::PushStyleColor(ImGuiCol_Separator, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        UI::PushStyleColor(ImGuiCol_SeparatorActive, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        UI::PushStyleColor(ImGuiCol_SeparatorHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.f));


        UI::PushStyleColor(ImGuiCol_Tab, ImVec4(0.2f, 0.2f, 0.2f, 1.f));
        UI::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.25f, 0.25f, 0.25f, 1.f));
        UI::PushStyleColor(ImGuiCol_TabHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.f));
        UI::PushStyleColor(ImGuiCol_TabUnfocused, ImVec4(0.03f, 0.03f, 0.03f, 1.f));
        UI::PushStyleColor(ImGuiCol_TabUnfocusedActive, ImVec4(0.15f, 0.15f, 0.15f, 1.f));

        UI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.5f, 1.5f));
        UI::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));

        UI::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);
        UI::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.8f);
        UI::PushStyleVar(ImGuiStyleVar_TabRounding, 2.f);
    }

    void RenderFrame()
    {
        UI::PopStyleVar(5);
        UI::PopStyleColor(17);

        UI::Render();
        ImGui_ImplOpenGL3_RenderDrawData(UI::GetDrawData());

        if (UI::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupWindow = glfwGetCurrentContext();
            UI::UpdatePlatformWindows();
            UI::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupWindow);
        }
    }
}
