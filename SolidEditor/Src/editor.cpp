#include "editor.hpp"
#include "UI/solidUI.hpp"
#include "UI/editorInterface.hpp"
#include "Inputs/editorInputs.hpp"

#include "Ressources/Ressources.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"

#include <string>

namespace Solid
{
    Framebuffer Editor::sceneFramebuffer;

    Editor::Editor()
    {
        WindowParams windowParams
        {
            .title = "Solid Editor",
            .windowSize = {1280,720}
        };

        RendererParams rendererParams
        {
            .rendererType = ERendererType::OpenGl45
        };

        engine.InitEngineContext(windowParams, rendererParams);

        if(!engine.IsEngineContextInitialized())
            throw ThrowError("Engine not correctly initialized !",ESolidErrorCode::S_INIT_ERROR);

        const Window* window = engine.window;

        editorInputManager = new InputManager<int>(window->GetHandle());
        InitEditorInputList(editorInputManager);

        sceneFramebuffer = engine.renderer->CreateFramebuffer(window->GetWindowSize());
    }

    Editor::~Editor()
    {
        delete editorInputManager;
    }

    void Editor::Run()
    {
        Window* window = engine.window;
        Renderer* renderer = engine.renderer;
        EditorInterface editorInterface(window);

        engine.EnableMultiThread(true);

        ResourcesLoader loader;

        loader.SetManager(engine.RManager);
        fs::path p = fs::current_path().append("Resources");

        auto before = std::chrono::high_resolution_clock::now();
        loader.LoadResourcesFromFolder(p);
        auto after = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::ratio<1,1000>> fp_s = after - before;
        std::cout << "Loading MT has Taken " << fp_s.count() << " milliseconds\n";
        engine.EnableMultiThread(false);
        //std::vector<Resource*>* Meshes= engine.RManager->GetResourcesVecByType<MeshResource>();
        //if(Meshes == nullptr);
            //Ressource Manager do not have this type of resourcesList
        //else;
             //Resource Manager Have a ResourceList for this type

        Entity entity = engine.ecsManager.CreateEntity();
        engine.ecsManager.AddComponent(entity,Transform{
                {0,0,-10},
                {0,0,0,1},
                {1,1,1}
        });
        engine.ecsManager.AddComponent(entity,MeshRenderer{
                .mesh   = dynamic_cast<MeshResource*>(engine.RManager->GetResourceByName("towerWNorms.obj")),
                .shader = dynamic_cast<ShaderResource*>(engine.RManager->GetResourceByName("ZShader"))
        });

        glfwSwapInterval(0);

        while (!glfwWindowShouldClose(window->GetHandle()))
        {
            glfwPollEvents();
            editorInputManager->Update();
            editorCamera.UpdateCamera(sceneFramebuffer.size);

            renderer->ClearColor({0,0,0,1});
            renderer->Clear(window->GetWindowSize());

            renderer->BeginFramebuffer(sceneFramebuffer);
            renderer->ClearColor({0.f,0.f,0.f,1});
            renderer->Clear(sceneFramebuffer.size);
            engine.rendererSystem->Update(renderer,editorCamera);
            renderer->EndFramebuffer();

            editorInterface.Update();

            renderer->UpdateFramebuffer(sceneFramebuffer);
            Time::Update();

            window->SwapBuffers();
        }

    }
} //!namespace
