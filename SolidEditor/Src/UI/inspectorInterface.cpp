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

        DrawComponents(_engine);

        UI::End();
    }

    void InspectorInterface::DrawComponents(Engine* _engine)
    {
        GameObject* gameObject = EditorInterface::selectedGO;
        if (gameObject == nullptr)
            return;

        UI::Text("Name: ");
        EditText(gameObject->name, "##name");

        EditTransform(_engine->ecsManager.GetComponent<Transform>(gameObject->GetEntity()));
    }

    void InspectorInterface::EditTransform(Transform& _trs)
    {
        ImVec2 pos = UI::GetCursorPos();
        pos.y += 20;
        UI::SetCursorPos(pos);

        UI::Text("Transform:");


        Vec3 tempPos   = _trs.GetPosition();
        Vec3 tempRot   = _trs.GetRotation().GetEuler();
        Vec3 tempScale = _trs.GetScale();


        EditVec3(tempPos  , "##trsPos");
        EditVec3(tempRot  , "##trsRot");
        EditVec3(tempScale, "##trsScl");

        _trs.SetPosition(tempPos);
        _trs.SetRotation(Quat(tempRot));
        _trs.SetScale(tempScale);
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
