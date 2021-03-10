#include "UI/inspectorInterface.hpp"
#include "UI/editorInterface.hpp"
#include "Core/engine.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
namespace Solid
{
    void InspectorInterface::Draw(Engine* _engine)
    {
        if(!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250,250));

        UI::Begin("Inspector", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

        if(EditorInterface::selectedGO == nullptr)
        {
            UI::End();
            return;
        }

        DrawComponents(_engine);

        AddComponents(_engine);

        UI::End();
    }

    void InspectorInterface::DrawComponents(Engine* _engine)
    {
        GameObject* gameObject = EditorInterface::selectedGO;

        UI::Text("Name: ");
        EditText(gameObject->name, "##name");

        EditTransform(_engine->ecsManager.GetComponent<Transform>(gameObject->GetEntity()));

        if(_engine->ecsManager.GotComponent<MeshRenderer>(gameObject->GetEntity()))
        {
            EditMeshRenderer(_engine->ecsManager.GetComponent<MeshRenderer>(gameObject->GetEntity()));
        }
    }

    void InspectorInterface::AddComponents(Engine* _engine)
    {
        GameObject* gameObject = EditorInterface::selectedGO;

        if(UI::Button("Add Component",ImVec2(-1, 0)))
            UI::OpenPopup("AddComponent");

        if(UI::BeginPopup("AddComponent"))
        {
            if(!_engine->ecsManager.GotComponent<MeshRenderer>(gameObject->GetEntity()))
            {
                if(UI::Button("Mesh renderer"))
                {
                    _engine->ecsManager.AddComponent(gameObject->GetEntity(),MeshRenderer());
                }
            }
            UI::EndPopup();
        }

    }

    void InspectorInterface::EditTransform(Transform& _trs)
    {
        ImVec2 pos = UI::GetCursorPos();
        pos.y += 20;
        UI::SetCursorPos(pos);

        if(UI::CollapsingHeader("Transform",ImGuiTreeNodeFlags_DefaultOpen))
        {
            Vec3 tempPos   = _trs.GetPosition();
            Vec3 tempRot   = _trs.GetRotation().ToEuler();
            Vec3 tempScale = _trs.GetScale();

            UI::Text("Position");UI::SameLine();
            EditVec3(tempPos  , "##trsPos");
            UI::Text("Rotation");UI::SameLine();
            EditVec3(tempRot  , "##trsRot");
            UI::Text("Scale   ");UI::SameLine();
            EditVec3(tempScale, "##trsScl");

            _trs.SetPosition(tempPos);
            _trs.SetRotation(Quat(tempRot));
            _trs.SetScale(tempScale);
        }
    }

    void InspectorInterface::EditMeshRenderer(MeshRenderer& _meshRenderer)
    {
        if(UI::CollapsingHeader("MeshRenderer",ImGuiTreeNodeFlags_DefaultOpen))
        {
            const char* meshName = _meshRenderer.mesh == nullptr ? "" : _meshRenderer.mesh->_name.c_str();

            UI::Text("Mesh  ");UI::SameLine();
            if(UI::BeginCombo("##Mesh", meshName))
            {
                UI::EndCombo();
            }

            const char* shaderName = _meshRenderer.shader == nullptr ? "" : _meshRenderer.shader->_name.c_str();

            UI::Text("Shader");UI::SameLine();
            if(UI::BeginCombo("##Shader", shaderName))
            {
                UI::EndCombo();
            }
        }
    }

    void InspectorInterface::EditVec3(Vec3& _vec, const std::string& _label)
    {
        UI::DragFloat3(_label.c_str(), &_vec.x, 0.01f);
    }

    void InspectorInterface::EditVec2(Vec2& _vec, const std::string& _label)
    {
        UI::DragFloat2(_label.c_str(), &_vec.x, 0.01f);
    }

    void InspectorInterface::EditInt(int& _num, const std::string& _label)
    {
        UI::DragInt(_label.c_str(), &_num);
    }

    void InspectorInterface::EditFloat(float &_num, const std::string& _label)
    {
        UI::DragFloat(_label.c_str(), &_num);
    }

    void InspectorInterface::EditText(std::string &_str, const std::string& _label)
    {
        UI::InputText(_label.c_str(), &_str);
    }
}
