#pragma once

#include "Core/Maths/solidMaths.hpp"
#include "ECS/Components/transform.hpp"
#include <string>

namespace Solid
{
    class InspectorInterface
    {
    private:
        Transform fakeTRS = Transform();
        std::string fakeName = "";
    public:
        InspectorInterface()  = default;
        ~InspectorInterface() = default;

        void Draw();
        void DrawComponent();

        void EditTransform(Transform& _trs);

        void EditInt(int&   _num, const std::string& _label);
        void EditVec3(Vec3& _vec, const std::string& _label);
        void EditVec2(Vec2& _vec, const std::string& _label);
        void EditText(std::string& _str, const std::string& _label);
        void EditFloat(float&      _num, const std::string& _label);

        bool p_open = true;
    };

}