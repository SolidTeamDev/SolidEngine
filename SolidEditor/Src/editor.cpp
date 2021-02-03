#include "editor.hpp"
#include "UI/solidUI.hpp"

namespace Solid
{
    Editor::Editor()
    {

    }

    void Editor::Loop()
    {
        GLFWwindow* window = renderer->GetWindow()->GetHandle();

        while (glfwWindowShouldClose(window) == false)
        {
            glfwPollEvents();

            renderer->Clear();

            UI::BeginFrame();

            UI::ShowDemo();

            UI::RenderFrame();

            glfwSwapBuffers(window);

        }

    }
} //!namespace