#pragma once

#include "Core/Maths/solidMaths.hpp"
#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"
#include <string>

namespace Solid
{
    class Engine;
    class InspectorInterface
    {
    public:
        InspectorInterface()  = default;
        ~InspectorInterface() = default;

        void Draw();
        void DrawComponents();
        void AddComponents();

        void EditTransform(Transform& _trs);
        void EditMeshRenderer(MeshRenderer& _meshRenderer);

        void EditInt(int&   _num, const std::string& _label);
        void EditVec3(Vec3& _vec, const std::string& _label);
        void EditVec2(Vec2& _vec, const std::string& _label);
        void EditText(std::string& _str, const std::string& _label);
        void EditFloat(float&      _num, const std::string& _label);

        bool p_open = true;
    };

}