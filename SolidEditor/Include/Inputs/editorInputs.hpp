#pragma once

#include "InputManager/inputManager.hpp"

namespace Solid
{

    namespace EInputList
    {
        enum : int
        {
            ESCAPE
        };

    } // !namespace

    /**
     * @brief Add every editor input list
     * @param _inputManager
     */
    void InitEditorInputList(InputManager<int>& _inputManager)
    {
        _inputManager.AddKeyInput(EInputList::ESCAPE,GLFW_KEY_ESCAPE,ImEnumDetectionType::PRESSED);
    }

} // !namespace
