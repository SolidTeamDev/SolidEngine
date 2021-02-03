#pragma once

#include "Build/solidAPI.hpp"

namespace Solid
{

    namespace UI
    {
        /**
         * @brief Begins a UI frame
         */
        void SOLID_API BeginFrame();
        /**
         * @brief Ends and render a UI frame
         */
        void SOLID_API RenderFrame();


    };
} //!namespace
