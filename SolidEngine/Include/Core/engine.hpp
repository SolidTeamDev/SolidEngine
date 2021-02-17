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

        void InitializeRenderer();

    protected:

        Renderer* renderer;
        bool MTEnabled = false;
        InputManager<int> inputManager;

    public:

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

        virtual ~Engine();

        virtual void Loop() = 0;
    };
} //!namespace
