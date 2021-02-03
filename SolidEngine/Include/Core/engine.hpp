#pragma once

#include "Build/solidAPI.hpp"
#include "Rendering/renderer.hpp"

namespace Solid
{
    /**
     * @brief Base engine class. Running the lifetime of application
     */
    class SOLID_API Engine
    {
    protected:

        Renderer* renderer;

    public:

        Engine();
        Engine(const Engine& _copy) = delete;
        Engine(Engine&& _move) = delete;

        virtual ~Engine();

        virtual void Loop() = 0;
    };
} //!namespace
