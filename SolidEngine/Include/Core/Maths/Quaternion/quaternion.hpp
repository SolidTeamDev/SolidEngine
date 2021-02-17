#pragma once

#include "Build/SolidAPI.hpp"

namespace Solid
{
    struct SOLID_API Quat
    {
        constexpr Quat() noexcept = default;
        constexpr Quat(float _value)noexcept;
        constexpr Quat(float _x,float _y = 0, float _z = 0, float _w = 1)noexcept;
        constexpr Quat(const Quat& _copy)noexcept = default;

        ~Quat() = default;

        float x;
        float y;
        float z;
        float w;
    };
}