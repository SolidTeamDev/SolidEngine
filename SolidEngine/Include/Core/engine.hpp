#pragma once

#include "Build/solidAPI.hpp"
#include "Rendering/renderer.hpp"
#include "Window/window.hpp"
#include "solidMultiThread.hpp"
#include "Time/time.hpp"
#include "InputManager/inputManager.hpp"

#include "Core/Debug/debug.hpp"

#include "ECS/ecsManager.hpp"
#include "ECS/System/rendererSystem.hpp"
#include "Resources/graphicalResourceMgr.hpp"

namespace Solid
{
    /**
     * @brief Base engine class. Running the lifetime of application
     */
    class SOLID_API Engine
    {
    private:

        Log log;

        bool mtEnabled = false;
        bool engineContextInit = false;


        void InitEcs();
	    Engine();
	    static Engine* instance;

    public:
    	//Engine** test = &instance;
        Window* window;
        Renderer* renderer = nullptr;
        ECSManager ecsManager;
        std::shared_ptr<RendererSystem> rendererSystem;
        class ResourceManager* resourceManager;
        TaskManager taskManager;
        ThreadManager threadPool;
        GraphicalResourceMgr graphicsResourceMgr;
        static Engine* GetInstance();
        bool MultiThreadEnabled()const {return mtEnabled;}
        void EnableMultiThread(bool _b);
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
        bool IsEngineContextInitialized() const;

        ~Engine();
    };
} //!namespace
