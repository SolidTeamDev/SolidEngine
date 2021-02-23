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

        UI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.f, 3.f));
        UI::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));

        //UI::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        UI::PushStyleVar(ImGuiStyleVar_TabRounding, 2.f);
        UI::PushStyleVar(ImGuiStyleVar_FrameRounding, 3);
        UI::PushStyleVar(ImGuiStyleVar_WindowRounding, 3);
    }

    void RenderFrame()
    {
        UI::PopStyleVar(5);

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
