#include "Core/engine.hpp"

#include <iostream>
#include "Rendering/OpenGL45/openGl45Renderer.hpp"
#include "UI/solidUI.hpp"
#include "Resources/ressources.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"
#include "ECS/Components/camera.hpp"

namespace Solid
{
	__declspec(dllexport) Engine* Engine::instance = nullptr;
    Engine::Engine() :
            threadPool(&taskManager),
            resourceManager(this)
    {
        InitEcs();
    }

    Engine::~Engine()
    {

        UIContext::ReleaseSolidUI();
        delete window;
        threadPool.TerminateAllThreads();
    }

    void Engine::InitEcs()
    {
        ecsManager.Init();

        //Components Registration
        ecsManager.RegisterComponent<Transform>();
        ecsManager.RegisterComponent<MeshRenderer>();
        ecsManager.RegisterComponent<Camera>();

        //Register Signature
        rendererSystem = ecsManager.RegisterSystem<RendererSystem>();
        {
            Signature signature;
            signature.set(ecsManager.GetComponentType<Transform>());
            signature.set(ecsManager.GetComponentType<MeshRenderer>());
            ecsManager.SetSystemSignature<RendererSystem>(signature);
        }
    }

    void Engine::InitEngineContext(const WindowParams& _windowParams, const RendererParams& _rendererParams)
    {
        window   = new Window(_windowParams);
        switch (_rendererParams.rendererType)
        {
            case ERendererType::OpenGl45:
                renderer = OpenGL45Renderer::InitRenderer();
                break;
        }

        /// TEMPORARY
        UIContext::InitializeSolidUI(window->GetHandle());
        ///

        if(window != nullptr && renderer != nullptr)
            engineContextInit = true;
	    graphicsResourceMgr.Init(&resourceManager, renderer);
    }

    bool Engine::IsEngineContextInitialized() const
    {

        return engineContextInit;
    }

	Engine *Engine::GetInstance()
	{
    	if(instance == nullptr)
    		instance = new Engine();

    	return instance;
	}

	void Engine::EnableMultiThread(bool _b)
	{
		mtEnabled = _b;
		if(_b){threadPool.PlayAllThreads();}
		else{threadPool.PauseAllThreads();}
	}
} //!namespace


