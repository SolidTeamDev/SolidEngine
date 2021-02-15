#pragma once

#include "Build/solidAPI.hpp"
#include "Rendering/renderer.hpp"
#include "Time/time.hpp"

namespace Solid
{
    /**
     * @brief Base engine class. Running the lifetime of application
     */
    class SOLID_API Engine
    {
    private:

        void InitializeRenderer();

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
