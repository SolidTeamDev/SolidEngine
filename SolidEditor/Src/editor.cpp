#include "editor.hpp"

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
            glfwSwapBuffers(window);
        }

    }
} //!namespace