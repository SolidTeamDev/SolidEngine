#pragma once

#include "Build/solidAPI.hpp"
#include "Rendering/renderer.hpp"
#include "SolidMultiThread.hpp"
#include "Time/time.hpp"
#include "InputManager/inputManager.hpp"

#include "Core/Debug/debug.hpp"

namespace Solid
{
    /**
     * @brief Base engine class. Running the lifetime of application
     */
    class SOLID_API Engine
    {
    private:

        Log log;

    protected:

        bool MTEnabled = false;
        bool engineContextInit = false;

    public:
        Window* window;
        Renderer* renderer = nullptr;

        class ResourceManager* RManager;
        TaskManager MultiTask;
        ThreadManager ThreadPool;
        bool MultiThreadEnabled()const {return MTEnabled;}
        void EnableMultiThread(bool b)
        {
            MTEnabled = b;
            if(b){ThreadPool.PlayAllThreads();}
            else{ThreadPool.PauseAllThreads();}
        }
        Engine();
        Engine(const Engine& _copy) = delete;
        Engine(Engine&& _move) = delete;

        /**
         * @brief Init GLFW window context and Renderer (Minimum required)
         */
        void InitEngineContext(const WindowParams& _windowParams, const RendererParams& _rendererParams);

        /**
         * @brief Return if the engine context is initialized
         * @return Engine context init
         */
        bool IsEngineContextInitialized();

        ~Engine();
    };
} //!namespace
