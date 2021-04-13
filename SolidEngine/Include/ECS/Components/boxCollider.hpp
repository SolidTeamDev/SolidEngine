#pragma once

#include "Core/Maths/Vector/vector3.hpp"

namespace Solid
{
    class BoxCollider
    {
        bool isTrigger;
        Vec3 center;
        Vec3 size {1.f,1.f,1.f};
    public:

        BoxCollider();
        ~BoxCollider() = default;
    };
} //!namespace