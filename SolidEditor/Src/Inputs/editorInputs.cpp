
#include "Inputs/editorInputs.hpp"

namespace Solid
{
    void InitEditorInputList(InputManager* _inputManager)
    {
        _inputManager->AddKeyInput("W",GLFW_KEY_W,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput("A",GLFW_KEY_A,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput("S",GLFW_KEY_S,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput("D",GLFW_KEY_D,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput("SPACE",GLFW_KEY_SPACE,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput("ALT",GLFW_KEY_LEFT_ALT,ImEnumDetectionType::PRESSED);

        _inputManager->AddMouseInput("MOUSE2",GLFW_MOUSE_BUTTON_2,ImEnumDetectionType::PRESSED);

    }
}
