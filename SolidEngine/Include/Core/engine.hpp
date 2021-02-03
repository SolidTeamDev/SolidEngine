#pragma once

#include "Build/solidAPI.hpp"

namespace Solid
{
    /**
     * @brief Base engine class. Running the lifetime of application
     */
    class SOLID_API Engine
    {
    public:
        Engine() = default;
        virtual ~Engine() = default;

        virtual void Loop() = 0;
    };
} //!namespace
