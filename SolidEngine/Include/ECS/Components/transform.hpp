#pragma once

#include "Core/Maths/Quaternion/quaternion.hpp"
#include "Core/Maths/Vector/vector3.hpp"

namespace Solid
{
    struct Transform
    {
        Vec3 position;
        Vec3 scale;
        Quat rotation;
    };
} //!namespace

