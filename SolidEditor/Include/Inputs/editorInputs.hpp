#pragma once

#include "InputManager/inputManager.hpp"

namespace Solid
{

    namespace EInputList
    {
        enum : int
        {
            FORWARD,
            LEFT,
            BACK,
            RIGHT,
            UP,
            DOWN,
            Mouse0
        };

    } // !namespace

    /**
     * @brief Add every editor input list
     * @param _inputManager
     */
    void InitEditorInputList(InputManager<int>* _inputManager)
    {
        _inputManager->AddKeyInput(EInputList::FORWARD,GLFW_KEY_W,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput(EInputList::LEFT,GLFW_KEY_A,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput(EInputList::BACK,GLFW_KEY_S,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput(EInputList::RIGHT,GLFW_KEY_D,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput(EInputList::UP,GLFW_KEY_SPACE,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput(EInputList::DOWN,GLFW_KEY_LEFT_ALT,ImEnumDetectionType::PRESSED);
        _inputManager->AddMouseInput(EInputList::Mouse0,GLFW_MOUSE_BUTTON_1,ImEnumDetectionType::PRESSED);

    }

} // !namespace
