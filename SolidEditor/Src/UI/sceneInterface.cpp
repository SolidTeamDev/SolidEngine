#include "UI/sceneInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor.hpp"
#include "UI/editorInterface.hpp"
#include <algorithm>
#include <imgui_impl_opengl3.h>
#include "Core/engine.hpp"

#include "ImGuizmo.h"

namespace Solid
{
    //TODO: Replace static
    static ImGuizmo::OPERATION gizmoMode = ImGuizmo::OPERATION::TRANSLATE;
    static ImGuizmo::MODE      gizmoReferential = ImGuizmo::MODE::LOCAL;
    void SceneInterface::Draw() {
        if (!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250, 250));

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
        windowFlags |= ImGuiWindowFlags_MenuBar;

        UI::Begin("Scene", &p_open, windowFlags);
	    Mat4<float> viewMat = Editor::editorCamera.GetView();
	    Mat4<float> projMat = Editor::editorCamera.GetProjection();

	    ImVec4 ClipRect = ((ImGuiWindow*)gizmoWin)->DrawList->_CmdHeader.ClipRect;
	    auto winList = ((ImGuiWindow*)gizmoWin)->DrawList;

	    GridList->PushClipRect(ImVec2(0, 0), ImVec2(Editor::sceneFramebuffer.size.x,Editor::sceneFramebuffer.size.y));
	    GridList->PushTextureID(winList->_CmdHeader.TextureId);
	    ImGuizmo::DrawGrid(viewMat.elements.data(),projMat.elements.data(),Mat4<float>::Identity.elements.data(),10, GridList);
	    Engine::GetInstance()->renderer->BeginFramebuffer(Editor::sceneFramebuffer);
	    int x,y;
	    glfwGetWindowPos(Engine::GetInstance()->window->GetHandle(),&x,&y);
	    Engine::GetInstance()->renderer->ClearColor({0.f,0.f,0.f,1});
	    Engine::GetInstance()->renderer->Clear(Editor::sceneFramebuffer.size);
	    ImGui_ImplOpenGL3_RenderDrawList(ImVec2(x,y), ImVec2(Editor::sceneFramebuffer.size.x,Editor::sceneFramebuffer.size.y), ImVec2(1,1), GridList);
	    Engine::GetInstance()->rendererSystem->Update(Engine::GetInstance()->renderer,Editor::editorCamera);
	    Engine::GetInstance()->renderer->EndFramebuffer();
	    Engine::GetInstance()->renderer->EndFramebuffer();

        ImVec2 windowSize = UI::GetContentRegionAvail();
        Editor::sceneFramebuffer.size = {(int)windowSize.x,(int)windowSize.y};
        UI::Image((ImTextureID)(size_t)Editor::sceneFramebuffer.texture,windowSize,ImVec2{0,1},ImVec2{1,0});
        ImVec2 imgPos = UI::GetItemRectMin();
        DrawMenu();

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(imgPos.x, imgPos.y, windowSize.x, windowSize.y);

        Engine* engine = Engine::GetInstance();
        GameObject* go = EditorInterface::selectedGO;
        if (go != nullptr)
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


        UI::End();
    }
}

void Solid::SceneInterface::DrawMenu()
{
    UI::BeginMenuBar();

    float speed = Editor::camSpeed/10;
    if (speed < 0.01f)
        speed = 0.01f;
    UI::SliderFloat("Camera Speed", &Editor::camSpeed, 0.1f, 1000.f);
    Editor::camSpeed = std::clamp(Editor::camSpeed, 0.f, 50000.f);

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
