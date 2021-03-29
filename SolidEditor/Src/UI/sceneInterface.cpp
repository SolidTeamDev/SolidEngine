#include "UI/sceneInterface.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor.hpp"
#include "UI/editorInterface.hpp"
#include <algorithm>
#include "Core/engine.hpp"

#include "ImGuizmo.h"

namespace Solid
{
    void SceneInterface::Draw() {
        if (!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250, 250));

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
        windowFlags |= ImGuiWindowFlags_MenuBar;

        UI::Begin("Scene", &p_open, windowFlags);

        ImVec2 windowSize = UI::GetWindowSize();
        windowSize.y -= 50.f;
        Editor::sceneFramebuffer.size = {(int)windowSize.x,(int)windowSize.y};
        UI::Image((ImTextureID)(size_t)Editor::sceneFramebuffer.texture,windowSize,ImVec2{0,1},ImVec2{1,0});
        DrawMenu();

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(UI::GetForegroundDrawList());
        float width = UI::GetWindowWidth();
        float height = UI::GetWindowHeight();
        ImGuizmo::SetRect(UI::GetWindowPos().x, UI::GetWindowPos().y, width, height);
        Mat4<float> viewMat = Editor::editorCamera.GetView();
        Mat4<float> projMat = Editor::editorCamera.GetProjection();
        Engine* engine = Engine::GetInstance();
        GameObject* go = EditorInterface::selectedGO;
        if (go != nullptr)
        {
            Mat4<float> transMat = engine->ecsManager.GetComponent<Transform>(go->GetEntity()).GetMatrix();

            ImGuizmo::Manipulate(viewMat.elements.data(), projMat.elements.data(),
                                 ImGuizmo::OPERATION::ROTATE, ImGuizmo::LOCAL,
                                 transMat.elements.data());
            if (ImGuizmo::IsUsing())
            {
                engine->ecsManager.GetComponent<Transform>(go->GetEntity()).SetTransformMatrix(transMat);
            }
        }

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

    /*if(UI::Button("Translation"))
        gizmoMode = ImGuizmo::OPERATION::TRANSLATE;
    if(UI::Button("Rotation"))
        gizmoMode = ImGuizmo::OPERATION::ROTATE;
    if(UI::Button("Scale"))
        gizmoMode = ImGuizmo::OPERATION::SCALE;*/

    UI::EndMenuBar();

}
