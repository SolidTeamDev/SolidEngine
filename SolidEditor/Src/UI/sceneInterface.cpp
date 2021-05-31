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


    float SceneInterface::camSpeed = 2.f;

    SceneInterface::SceneInterface()
    {
        engine = Engine::GetInstance();
        sceneFramebuffer = engine->renderer->CreateFramebuffer(engine->window->GetWindowSize());
        sceneCam.MouseSensitivity = 35.f;

		engine->SetActiveCamera(&sceneCam);
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

        // Focus window if not focused but clicked and hovered
        if(UI::IsMouseDown(1) && UI::IsWindowHovered() && !UI::IsWindowFocused())
            UI::FocusWindow(UI::GetCurrentWindow());

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
	        Mat4<float> Parent = engine->ecsManager.GetComponent<Transform>(go->GetEntity()).GetParentMatrix();
			transMat = (transMat*Parent );
            ImGuizmo::Manipulate(viewMat.elements.data(), projMat.elements.data(),
                                 ButtonInterface::gizmoMode, ButtonInterface::gizmoReferential,
                                 transMat.elements.data() );
            if (ImGuizmo::IsUsing())
            {
            	transMat =   transMat *Parent.GetInversed();
                engine->ecsManager.GetComponent<Transform>(go->GetEntity()).SetTransformMatrix(transMat);
            }
        }

        UI::End();
    }

    void SceneInterface::DrawScene()
    {
        Vec2d mousePos{};
        Editor::editorInputManager->GetCursorPos(mousePos.x,mousePos.y);


        sceneCam.UpdateCamera(sceneFramebuffer.size);

        if(UI::IsWindowFocused() && MouseInSceneInterface(mousePos))
        {
            MovementAndRotationCam(Time::DeltaTime()  * ((int)(engine->window->GetWindowSize().x/2) - (int)mousePos.x ),
                                  Time::DeltaTime()  * ((int)(engine->window->GetWindowSize().y/2) - (int)mousePos.y ));
        }


        engine->renderer->BeginFramebuffer(sceneFramebuffer);
        engine->renderer->ClearColor({0.f,0.f,0.f,1});
        engine->renderer->Clear(sceneFramebuffer.size);
        engine->renderer->DrawSkybox(sceneCam);
        if(ShowSkeleton)
        {
            for(GameObject* gameObject : engine->ecsManager.GetWorld()->childs)
            {
                if(engine->ecsManager.GotComponent<Animation>(gameObject->GetEntity()))
                {
                    std::vector<Vec3> points;
                    std::vector<uint> indices;
                    Animation& anim = engine->ecsManager.GetComponent<Animation>(gameObject->GetEntity());
                    anim.UpdateAnim(Time::DeltaTime());
                    anim.DrawSkeleton(points,indices);
                    engine->renderer->DrawLines(sceneCam, points, indices);
                }
            }
        }

        if(ShowGrid)
            engine->renderer->DrawSolidGrid(sceneCam, 50, Vec3(.3,.3,.3), 1);

        engine->rendererSystem->Update(engine->renderer,sceneCam);
        engine->particleEffectSystem->Update(sceneCam);


        engine->renderer->EndFramebuffer();

        engine->audioSystem->Update(sceneCam);
    }

    void SceneInterface::DrawMenu()
    {
        UI::BeginMenuBar();

        float speed = camSpeed/10;
        if (speed < 0.01f)
            speed = 0.01f;
        UI::Text("Camera Speed");
        UI::SameLine();
        UI::SetNextItemWidth(100.f);
        UI::DragFloat("##CameraSpeed", &camSpeed, 0.1f, 1000.f);
        camSpeed = std::clamp(camSpeed, 0.f, 50000.f);



        if(UI::BeginMenu("Debug"))
        {
            UI::Checkbox("Grid", &ShowGrid);
            UI::Checkbox("Skeleton", &ShowSkeleton);

            UI::EndMenu();
        }
        UI::EndMenuBar();
    }

    bool SceneInterface::MouseInSceneInterface(const Vec2d& mousePos)
    {
        if(Editor::editorInputManager->IsPressed("MOUSE2")
            && mousePos.x >= sceneFramebuffer.pos.x
            && mousePos.x < sceneFramebuffer.pos.x + sceneFramebuffer.size.x
            && mousePos.y >= sceneFramebuffer.pos.y
            && mousePos.y < sceneFramebuffer.pos.y + sceneFramebuffer.size.y)
        {
            Editor::editorInputManager->ShowCursor(false);

            Editor::editorInputManager->SetCursorPos(engine->window->GetWindowSize().x*0.5,
                                                     engine->window->GetWindowSize().y*0.5);

            return true;
        }
        else
        {
            Editor::editorInputManager->ShowCursor(true);
            sceneCam.MouseInCenterScreen = false;
            return false;

        }
    }

    void SceneInterface::MovementAndRotationCam(float xpos, float ypos)
    {
        if (!sceneCam.MouseInCenterScreen)
        {
            sceneCam.MouseInCenterScreen = true;
            return;
        }

        //movement cam
        float updateCamSpeed = (float) (camSpeed * Time::DeltaTime());

        if (Editor::editorInputManager->IsPressed("W"))
            sceneCam.position += sceneCam.Front * updateCamSpeed;
        if (Editor::editorInputManager->IsPressed("S"))
            sceneCam.position -= sceneCam.Front * updateCamSpeed;

        if (Editor::editorInputManager->IsPressed("A"))
            sceneCam.position -= sceneCam.Right * updateCamSpeed;
        if (Editor::editorInputManager->IsPressed("D"))
            sceneCam.position += sceneCam.Right * updateCamSpeed;

        if (Editor::editorInputManager->IsPressed("SPACE"))
            sceneCam.position += Vec3::Up * updateCamSpeed;
        if (Editor::editorInputManager->IsPressed("ALT"))
            sceneCam.position -= Vec3::Up * updateCamSpeed;

        //rotation camera


            sceneCam.Euler.x += xpos * sceneCam.MouseSensitivity;
            sceneCam.Euler.y += ypos * sceneCam.MouseSensitivity;;

            if (sceneCam.Euler.y > 89.0f)
                sceneCam.Euler.y = 89.0f;
            if (sceneCam.Euler.y < -89.0f)
                sceneCam.Euler.y = -89.0f;

            Vec3 _front;
            _front.z = Maths::Cos(Maths::DegToRad(sceneCam.Euler.x)) *
                       Maths::Cos(Maths::DegToRad(sceneCam.Euler.y));

            _front.y = Maths::Sin(Maths::DegToRad(sceneCam.Euler.y));

            _front.x = Maths::Sin(Maths::DegToRad(sceneCam.Euler.x)) *
                       Maths::Cos(Maths::DegToRad(sceneCam.Euler.y));

            sceneCam.Front = _front.GetNormalized();
            sceneCam.Right = Vec3::Cross(sceneCam.Front, Vec3::Up).GetNormalized();
            sceneCam.Up = Vec3::Cross(sceneCam.Right, sceneCam.Front).GetNormalized();

    }
}




