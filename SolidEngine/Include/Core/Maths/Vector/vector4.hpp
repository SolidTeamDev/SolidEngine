#pragma once

#include "Build/SolidAPI.hpp"

namespace Solid
{
    struct Vec3;

    struct Vec2;

    struct SOLID_API Vec4
    {
        float x;
        float y;
        float z;
        float w;

        constexpr Vec4() noexcept = default;
        constexpr Vec4(const float& _value)noexcept;
        constexpr Vec4(const float& _x,const float& _y = 0, const float& _z = 0, const float& _w = 1)noexcept;
        constexpr Vec4(const Vec3& _copy)noexcept;
        constexpr Vec4(const Vec4& _copy)noexcept;



        ~Vec4() = default;

    };

};