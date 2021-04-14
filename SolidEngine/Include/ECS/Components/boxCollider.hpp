#pragma once

#include "Core/Maths/Vector/vector3.hpp"
#include <PxShape.h>

namespace Solid
{
    class BoxCollider
    {
        bool isTrigger;
        Vec3 center;
        Vec3 size {1.f,1.f,1.f};
        physx::PxBoxGeometry* boxCollider = nullptr;
    public:

        BoxCollider();
        ~BoxCollider() = default;
    };
} //!namespace