#include "editor.hpp"
#include "UI/solidUI.hpp"
#include "UI/editorInterface.hpp"

namespace Solid
{
    Editor::Editor()
    {

    }

    void Editor::Loop()
    {
        GLFWwindow* window = renderer->GetWindow()->GetHandle();
        EditorInterface editorInterface(renderer->GetWindow());

        while (glfwWindowShouldClose(window) == false)
        {
            glfwPollEvents();

            renderer->Clear();

            editorInterface.Update();

            renderer->GetWindow()->update();

            glfwSwapBuffers(window);
        }

    }
} //!namespace