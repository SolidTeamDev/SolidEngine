#pragma once

#include "Build/SolidAPI.hpp"

namespace Solid
{
    struct Vec3;

    struct Vec4;

    struct SOLID_API Vec2
    {
        float x;
        float y;

        constexpr Vec2() noexcept = default;
        constexpr Vec2(const float& _value)noexcept;
        constexpr Vec2(const float& _x,const float& _y = 0)noexcept;
        constexpr Vec2(const Vec2& _copy)noexcept;
        constexpr Vec2(const Vec3& _copy)noexcept;
        constexpr Vec2(const Vec4& _copy)noexcept;



        ~Vec2() = default;

    };

};
