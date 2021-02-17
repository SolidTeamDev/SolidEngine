#pragma once

#include "Build/solidAPI.hpp"
#include "Rendering/renderer.hpp"
#include "Time/time.hpp"
#include "InputManager/inputManager.hpp"

namespace Solid
{
    /**
     * @brief Base engine class. Running the lifetime of application
     */
    class SOLID_API Engine
    {
    private:
        bool engineContextInit = false;
    public:
        Window* window;
        Renderer* renderer = nullptr;

        Engine();
        Engine(const Engine& _copy) = delete;
        Engine(Engine&& _move) = delete;

        /**
         * @brief Init GLFW window context and Renderer (Minimum required)
         */
        void InitEngineContext(const WindowParams& _windowParams, const RendererParams& _rendererParams);

        /**
         * @brief
         * @return
         */
        bool IsEngineContextInitialized();

        ~Engine();
    };
} //!namespace
