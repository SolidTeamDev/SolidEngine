#include "editor.hpp"
#include "UI/solidUI.hpp"
#include "UI/editorInterface.hpp"
#include "Inputs/editorInputs.hpp"

#include "Ressources/essources.hpp"

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

        loader.SetManager(engine.resourceManager);
        fs::path p = fs::current_path().append("Resources");

        auto before = std::chrono::high_resolution_clock::now();
        loader.LoadResourcesFromFolder(p);
        auto after = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::ratio<1,1000>> fp_s = after - before;
        std::cout << "Loading MT has Taken " << fp_s.count() << " milliseconds\n";
        engine.EnableMultiThread(false);
        //std::vector<Resource*>* Meshes= engine.resourceManager->GetResourcesVecByType<MeshResource>();
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
                .mesh   = dynamic_cast<MeshResource*>(engine.resourceManager->GetResourceByName("towerWNorms.obj")),
                .shader = dynamic_cast<ShaderResource*>(engine.resourceManager->GetResourceByName("ZShader"))
        });

        glfwSwapInterval(0);

        while (!glfwWindowShouldClose(window->GetHandle()))
        {
            glfwPollEvents();
            editorInputManager->Update();
            UpdateEditorCamera();

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

    void Editor::UpdateEditorCamera()
    {
        editorCamera.UpdateCamera(sceneFramebuffer.size);

        Transform& editorCameraT = editorCamera.transform;

        //== Mouse

        Double2 mousePos{};
        editorInputManager->GetCursorPos(mousePos.x,mousePos.y);
        Double2 deltaPos {mousePos.x - sceneFramebuffer.size.x/2,
                          mousePos.y - sceneFramebuffer.size.y/2};

        float mouseSensitivity = 0.1f;

        deltaPos.x *= mouseSensitivity * Time::DeltaTime();
        deltaPos.y *= mouseSensitivity * Time::DeltaTime();

        Vec3 rot = editorCameraT.GetRotation().GetEuler();

        /*rot.x += deltaPos.x;
        rot.y += deltaPos.y;

        if(rot.x >= S_PI_2)
            rot.x = S_PI_2;
        else if (rot.x <= -S_PI_2)
            rot.x = -S_PI_2;

        editorCameraT.SetRotation(Quat(rot));*/

        //== Movement
        float camSpeed = (float)(4 * Time::DeltaTime());
        float forwardVelocity = 0;

        if(editorInputManager->IsPressed(EInputList::UP))
            forwardVelocity = camSpeed;
        if(editorInputManager->IsPressed(EInputList::DOWN))
            forwardVelocity = -camSpeed;

        float strafeVelocity = 0;

        if(editorInputManager->IsPressed(EInputList::LEFT))
            strafeVelocity = -camSpeed;
        if(editorInputManager->IsPressed(EInputList::RIGHT))
            strafeVelocity = camSpeed;

        /*editorCameraT.Translate(Vec3(forwardVelocity,
                                     -forwardVelocity,
                                     forwardVelocity));

        editorCameraT.Translate(Vec3(strafeVelocity,
                                     0,
                                     strafeVelocity));*/

        editorCameraT.Translate(Vec3(sin(-rot.y) * cos(-rot.x) * forwardVelocity,
                                     -(sin(rot.x) * forwardVelocity),
                                     cos(rot.y) * cos(rot.x) * forwardVelocity));

        editorCameraT.Translate(Vec3(cos(rot.y)  * strafeVelocity,
                                     0,
                                     sin(rot.y)  * strafeVelocity));
    }
} //!namespace
