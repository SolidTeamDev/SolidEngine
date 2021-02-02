#include "editor.hpp"

#include "Rendering/OpenGL45/openGl45Renderer.hpp"

namespace Solid
{
    Editor::Editor()
    {
        RendererParams rendererParams
        {
            {
                "Solid Engine",
                {1280,720}
            }
        };
        renderer = new OpenGL45Renderer();
        renderer->Initialize(rendererParams);

    }

    void Editor::Loop()
    {
        GLFWwindow* window = renderer->GetWindow()->GetHandle();

        sAssert(false);

        while (glfwWindowShouldClose(window) == false)
        {
            glfwPollEvents();
            glfwSwapBuffers(window);
        }

    }
} //!namespace