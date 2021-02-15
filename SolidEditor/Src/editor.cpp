#include "editor.hpp"
#include "UI/solidUI.hpp"
#include "UI/editorInterface.hpp"
#include "Inputs/editorInputs.hpp"

#include <string>

namespace Solid
{
    Editor::Editor() : editorInputManager(renderer->GetWindow()->GetHandle())
    {
        InitEditorInputList(editorInputManager);
    }

    void Editor::Loop()
    {
        GLFWwindow* window = renderer->GetWindow()->GetHandle();
        EditorInterface editorInterface(renderer->GetWindow());

        glfwSwapInterval(0);

        while (!glfwWindowShouldClose(window) && !editorInputManager.IsPressed(EInputList::ESCAPE))
        {
            glfwPollEvents();
            editorInputManager.Update();

            renderer->Clear();

            editorInterface.Update();

            renderer->GetWindow()->update();
            Time::Update();

            glfwSetWindowTitle(window,std::string("Solid Editor - " + std::to_string(Time::Fps()) + "fps").c_str());

            glfwSwapBuffers(window);
        }

    }
} //!namespace