#pragma once

#include "Build/solidAPI.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>

namespace Solid::UIContext
{
        /**
         * @brief Initializes SolidUI
         * @param window required to bind the context to the window
         */
        void InitializeSolidUI(GLFWwindow* window);
        /**
         * @brief Releases SolidUI
         */
        void ReleaseSolidUI();


        /**
         * @brief Begins a UI frame
         */
        void SOLID_API BeginFrame();
        /**
         * @brief Ends and render a UI frame
         */
        void SOLID_API RenderFrame();
} //!namespace