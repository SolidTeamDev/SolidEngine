#pragma once

#include "Build/SolidAPI.hpp"

namespace Solid
{
    struct Vec2;

    struct Vec4;

    struct SOLID_API Vec3
    {
        float x;
        float y;
        float z;

        constexpr Vec3() noexcept = default;
        constexpr Vec3(const float& _value)noexcept;
        constexpr Vec3(const float& _x,const float& _y = 0, const float& _z = 0)noexcept;
        constexpr Vec3(const Vec2& _copy)noexcept;
        constexpr Vec3(const Vec3& _copy)noexcept;
        constexpr Vec3(const Vec4& _copy)noexcept;



        ~Vec3() = default;

    };

};