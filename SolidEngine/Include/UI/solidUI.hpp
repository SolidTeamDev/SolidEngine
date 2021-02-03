#pragma once

#include "Build/solidAPI.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>

namespace Solid
{

    namespace UI
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

        /**
         * @brief Shows an UI demo window
         */
        void SOLID_API ShowDemo();

    };
} //!namespace
