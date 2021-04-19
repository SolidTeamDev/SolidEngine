#include "UI/sceneInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor.hpp"
#include "UI/editorInterface.hpp"
#include "Inputs/editorInputs.hpp"

#include <algorithm>


#include "ImGuizmo.h"

namespace Solid
{
    //TODO: Replace static
    static ImGuizmo::OPERATION gizmoMode = ImGuizmo::OPERATION::TRANSLATE;
    static ImGuizmo::MODE      gizmoReferential = ImGuizmo::MODE::LOCAL;

    float SceneInterface::camSpeed = 2.f;

    SceneInterface::SceneInterface()
    {
        engine = Engine::GetInstance();
        sceneFramebuffer = engine->renderer->CreateFramebuffer(engine->window->GetWindowSize());
    }

    void SceneInterface::Draw()
    {
        if (!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250, 250));

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
        windowFlags |= ImGuiWindowFlags_MenuBar;

        UI::Begin("Scene", &p_open, windowFlags);

        ImVec2 windowSize = UI::GetContentRegionAvail();
        sceneFramebuffer.size = {(int)windowSize.x+5,(int)windowSize.y};
        sceneFramebuffer.pos = {(int)(UI::GetWindowPos().x - UI::GetMainViewport()->Pos.x) ,
                                (int)(UI::GetWindowPos().y -UI::GetMainViewport()->Pos.y + 42)};

        DrawScene();
        UI::Image((ImTextureID)(size_t)sceneFramebuffer.texture,windowSize,ImVec2{0,1},ImVec2{1,0});

        ImVec2 imgPos = UI::GetItemRectMin();

        DrawMenu();

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(imgPos.x, imgPos.y, windowSize.x, windowSize.y);
        Mat4<float> viewMat = sceneCam.GetView();
        Mat4<float> projMat = sceneCam.GetProjection();

        GameObject* go = EditorInterface::selectedGO;
        if (go != nullptr && engine->ecsManager.GotComponent<Transform>(go->GetEntity()))
        {
            Mat4<float> transMat = engine->ecsManager.GetComponent<Transform>(go->GetEntity()).GetMatrix();

            ImGuizmo::Manipulate(viewMat.elements.data(), projMat.elements.data(),
                                 gizmoMode, gizmoReferential,
                                 transMat.elements.data());
            if (ImGuizmo::IsUsing())
            {
                engine->ecsManager.GetComponent<Transform>(go->GetEntity()).SetTransformMatrix(transMat);
            }
        }
        //Show grid
        //::DrawGrid(viewMat.elements.data(),projMat.elements.data(),Mat4<float>::Identity.elements.data(),10);

        UI::End();
    }

    void SceneInterface::DrawScene()
    {
        Vec2d mousePos{};
        Editor::editorInputManager->GetCursorPos(mousePos.x,mousePos.y);

        if(UI::IsWindowFocused() && MouseInSceneInterface(mousePos))
            MovementAndRotationCam(mousePos);

        engine->renderer->BeginFramebuffer(sceneFramebuffer);
        engine->renderer->ClearColor({0.f,0.f,0.f,1});
        engine->renderer->Clear(sceneFramebuffer.size);
        engine->renderer->DrawSolidGrid(sceneCam, 50, Vec3(.3,.3,.3), 1);
        engine->rendererSystem->Update(engine->renderer,sceneCam);
        engine->renderer->EndFramebuffer();

        engine->audioSystem->Update(sceneCam);
    }

    void SceneInterface::DrawMenu()
    {
        UI::BeginMenuBar();

        float speed = camSpeed/10;
        if (speed < 0.01f)
            speed = 0.01f;
        UI::SliderFloat("Camera Speed", &camSpeed, 0.1f, 1000.f);
        camSpeed = std::clamp(camSpeed, 0.f, 50000.f);

        if(UI::Button("Local"))
            gizmoReferential = ImGuizmo::MODE::LOCAL;
        if(UI::Button("Global"))
            gizmoReferential =  ImGuizmo::MODE::WORLD;

        if(UI::Button("Translation"))
            gizmoMode = ImGuizmo::OPERATION::TRANSLATE;
        if(UI::Button("Rotation"))
            gizmoMode = ImGuizmo::OPERATION::ROTATE;
        if(UI::Button("Scale"))
            gizmoMode = ImGuizmo::OPERATION::SCALE; //TODO: Force Local

        UI::EndMenuBar();
    }

    bool SceneInterface::MouseInSceneInterface(const Vec2d& mousePos)
    {
        if(Editor::editorInputManager->IsPressed(EInputList::Mouse0)
            && mousePos.x >= sceneFramebuffer.pos.x
            && mousePos.x < sceneFramebuffer.pos.x + sceneFramebuffer.size.x
            && mousePos.y >= sceneFramebuffer.pos.y
            && mousePos.y < sceneFramebuffer.pos.y + sceneFramebuffer.size.y)
        {
            return true;
        }
        else
            return false;
    }

    void SceneInterface::MovementAndRotationCam(const Vec2d& mousePos)
    {
        sceneCam.UpdateCamera(sceneFramebuffer.size);

        Transform& sceneCamT = sceneCam.transform;

        //== Mouse


        Vec2d deltaPos {mousePos.x - sceneFramebuffer.size.x/2,
                        mousePos.y - sceneFramebuffer.size.y/2};

        float mouseSensitivity = 0.1f;

        deltaPos.x *= mouseSensitivity * Time::DeltaTime();
        deltaPos.y *= mouseSensitivity * Time::DeltaTime();

        //Rotation camera

        Vec3 rot = sceneCamT.GetRotation().ToEuler();


        //Movement camera
        float updateCamSpeed = (float)(camSpeed * Time::DeltaTime());
        float forwardVelocity = 0;

        if(Editor::editorInputManager->IsPressed(EInputList::FORWARD))
            forwardVelocity = updateCamSpeed;
        if(Editor::editorInputManager->IsPressed(EInputList::BACK))
            forwardVelocity = -updateCamSpeed;

        float strafeVelocity = 0;

        if(Editor::editorInputManager->IsPressed(EInputList::LEFT))
            strafeVelocity = updateCamSpeed;
        if(Editor::editorInputManager->IsPressed(EInputList::RIGHT))
            strafeVelocity = -updateCamSpeed;

        if(Editor::editorInputManager->IsPressed(EInputList::UP))
            sceneCamT.Translate(Vec3(0,-updateCamSpeed,0));
        if(Editor::editorInputManager->IsPressed(EInputList::DOWN))
            sceneCamT.Translate(Vec3(0,updateCamSpeed,0));


        sceneCamT.Translate(Vec3(sin(-rot.y) * cos(-rot.x) * forwardVelocity,
                                 -(sin(rot.x) * forwardVelocity),
                                 cos(rot.y) * cos(rot.x) * forwardVelocity));

        sceneCamT.Translate(Vec3(cos(rot.y)  * strafeVelocity,
                                 0,
                                 sin(rot.y)  * strafeVelocity));
    }


}




