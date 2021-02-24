#pragma once

#include "InputManager/inputManager.hpp"

namespace Solid
{

    namespace EInputList
    {
        enum : int
        {
            UP,
            LEFT,
            DOWN,
            RIGHT,
        };

    } // !namespace

    /**
     * @brief Add every editor input list
     * @param _inputManager
     */
    void InitEditorInputList(InputManager<int>* _inputManager)
    {
        _inputManager->AddKeyInput(EInputList::UP,GLFW_KEY_W,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput(EInputList::LEFT,GLFW_KEY_A,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput(EInputList::DOWN,GLFW_KEY_S,ImEnumDetectionType::PRESSED);
        _inputManager->AddKeyInput(EInputList::RIGHT,GLFW_KEY_D,ImEnumDetectionType::PRESSED);
    }

} // !namespace
