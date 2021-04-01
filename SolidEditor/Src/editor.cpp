#include "editor.hpp"
#include "UI/solidUI.hpp"
#include "UI/editorInterface.hpp"
#include "Inputs/editorInputs.hpp"

#include "Resources/ressources.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"

namespace Solid
{

    Framebuffer Editor::sceneFramebuffer;
    Camera Editor::editorCamera;
    float Editor::camSpeed = 2.f;

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
		engine = Engine::GetInstance();
        engine->InitEngineContext(windowParams, rendererParams);

        if(!engine->IsEngineContextInitialized())
            throw ThrowError("Engine not correctly initialized !",ESolidErrorCode::S_INIT_ERROR);

        const Window* window = engine->window;

        editorInputManager = new InputManager<int>(window->GetHandle());
        InitEditorInputList(editorInputManager);

        sceneFramebuffer = engine->renderer->CreateFramebuffer(window->GetWindowSize());
        Compiler = GameCompiler::GetInstance();


    }

    Editor::~Editor()
    {
    	delete editorInputManager;
        //delete engine;
    }

    void Editor::Run()
    {
        Window* window = engine->window;
        Renderer* renderer = engine->renderer;
        EditorInterface editorInterface(window);

        glfwSwapInterval(0);



        while (!glfwWindowShouldClose(window->GetHandle()))
        {
            glfwPollEvents();
            editorInputManager->Update();
            UpdateEditorCamera();

            //TODO: Update engine task in engine
            engine->Update();
            engine->FixedUpdate();
            engine->LateUpdate();

            renderer->ClearColor({0,0,0,1});
            renderer->Clear(window->GetWindowSize());

            renderer->BeginFramebuffer(sceneFramebuffer);
            renderer->ClearColor({0.f,0.f,0.f,1});
            renderer->Clear(sceneFramebuffer.size);
	        renderer->DrawSolidGrid(Editor::editorCamera, 10, Vec3(.5,.5,.5), 1.8);
            engine->rendererSystem->Update(renderer,editorCamera);
            renderer->EndFramebuffer();

            engine->audioSystem->Update(editorCamera);

            editorInterface.Update();

            renderer->UpdateFramebuffer(sceneFramebuffer);
            Time::Update();

            window->SwapBuffers();
        }


    }

    void Editor::LoadResources(bool _solid)
    {
        engine->EnableMultiThread(true);

        ResourcesLoader loader;

        loader.SetManager(&(engine->resourceManager));

        fs::path p = fs::current_path().append("Resources");

        if(_solid)
			p = fs::current_path().append("SolidResources");
        auto before = std::chrono::high_resolution_clock::now();
        loader.LoadResourcesFromFolder(p);
        auto after = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::ratio<1,1000>> fp_s = after - before;
        std::cout << "Loading MT has Taken " << fp_s.count() << " milliseconds\n";
        engine->EnableMultiThread(false);
    }

    void Editor::UpdateEditorCamera()
    {
        editorCamera.UpdateCamera(sceneFramebuffer.size);

        Transform& editorCameraT = editorCamera.transform;

        //== Mouse

        Vec2d mousePos{};
        editorInputManager->GetCursorPos(mousePos.x,mousePos.y);
        Vec2d deltaPos {mousePos.x - sceneFramebuffer.size.x/2,
                          mousePos.y - sceneFramebuffer.size.y/2};

        float mouseSensitivity = 0.1f;

        deltaPos.x *= mouseSensitivity * Time::DeltaTime();
        deltaPos.y *= mouseSensitivity * Time::DeltaTime();

        Vec3 rot = editorCameraT.GetRotation().ToEuler();

        /*rot.x += deltaPos.x;
        rot.y += deltaPos.y;

        if(rot.x >= S_PI_2)
            rot.x = S_PI_2;
        else if (rot.x <= -S_PI_2)
            rot.x = -S_PI_2;

        editorCameraT.SetRotation(Quat(rot));*/

        //== Movement
        float updateCamSpeed = (float)(camSpeed * Time::DeltaTime());
        float forwardVelocity = 0;

        if(editorInputManager->IsPressed(EInputList::FORWARD))
            forwardVelocity = updateCamSpeed;
        if(editorInputManager->IsPressed(EInputList::BACK))
            forwardVelocity = -updateCamSpeed;

        float strafeVelocity = 0;

        if(editorInputManager->IsPressed(EInputList::LEFT))
            strafeVelocity = updateCamSpeed;
        if(editorInputManager->IsPressed(EInputList::RIGHT))
            strafeVelocity = -updateCamSpeed;

        if(editorInputManager->IsPressed(EInputList::UP))
            editorCameraT.Translate(Vec3(0,-updateCamSpeed,0));
        if(editorInputManager->IsPressed(EInputList::DOWN))
            editorCameraT.Translate(Vec3(0,updateCamSpeed,0));


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
