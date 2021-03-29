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
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        float width = UI::GetWindowWidth();
        float height = UI::GetWindowHeight();

        ImGuizmo::SetRect(UI::GetWindowPos().x, UI::GetWindowPos().y, width, height);
        Mat4<float> viewMat = Editor::editorCamera.GetView().GetTransposed();
        Mat4<float> projMat = Editor::editorCamera.GetProjection().GetTransposed();

        Engine* engine = Engine::GetInstance();
        GameObject* go = EditorInterface::selectedGO;
        if (go != nullptr)
        {
            Mat4<float> pos = engine->ecsManager.GetComponent<Transform>(go->GetEntity()).GetMatrix().GetTransposed();
            ImGuizmo::Manipulate(viewMat.elements.data(), projMat.elements.data(),
                                 ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL,
                                 pos.elements.data());
            if (ImGuizmo::IsUsing())
            {
                pos = pos.GetTransposed();
                Vec3 newPos = Vec3(pos.At(12), pos.At(13), pos.At(14));
                engine->ecsManager.GetComponent<Transform>(go->GetEntity()).SetPosition(newPos);
            }
        }
        ImVec2 windowSize = UI::GetWindowSize();
        windowSize.y -= 50.f;
        Editor::sceneFramebuffer.size = {(int)windowSize.x,(int)windowSize.y};
        UI::Image((ImTextureID)(size_t)Editor::sceneFramebuffer.texture,windowSize,ImVec2{1,1},ImVec2{0,0});
        DrawMenu();


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


    UI::EndMenuBar();

}
