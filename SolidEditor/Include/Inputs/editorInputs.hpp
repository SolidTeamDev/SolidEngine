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
    //class InputManager;
    void InitEditorInputList(InputManager<int>* _inputManager);

} // !namespace
