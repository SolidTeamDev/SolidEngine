#pragma once

#include "Build/SolidAPI.hpp"
#include <iostream>
#include <string>

namespace Solid
{
    struct Vec3;

    struct Vec4;

    struct SOLID_API Vec2
    {
        float x;
        float y;

        constexpr Vec2() noexcept = default;

        constexpr Vec2(const float &_value) noexcept;

        constexpr Vec2(float _x, float _y = 0) noexcept;

        constexpr Vec2(const Vec2 &_copy) noexcept;

        constexpr Vec2(const Vec3 &_copy) noexcept;

        constexpr Vec2(const Vec4 &_copy) noexcept;

        ~Vec2() = default;

#pragma region Static Methods

        static Vec2 zero;
        static Vec2 up;
        static Vec2 down;
        static Vec2 left;
        static Vec2 right;

        static constexpr float dot(const Vec2 &_v1,const Vec2 &_v2) noexcept;

        static constexpr float cross(const Vec2 &_v1,const Vec2 &_v2) noexcept;

        static Vec2 lerp(const Vec2& _v1,const Vec2& _v2, float _r) noexcept;

        static Vec2 Nlerp(const Vec2& _v1,const Vec2& _v2, float _r) noexcept;

        static Vec2 slerp(const Vec2& _v1,const Vec2& _v2, float _r) noexcept;

#pragma endregion
#pragma region Methods

        constexpr float sqrtLength() const noexcept;

        constexpr float length() const noexcept;

        Vec2 &scale(float _scale) noexcept;

        constexpr Vec2 getScaled(float _scale) const noexcept;

        Vec2 &unscale(float _scale) noexcept;

        constexpr Vec2 getUnscaled(float _scale) const noexcept;

        Vec2 &normalize() noexcept;

        constexpr Vec2 getNormalize() const noexcept;

        constexpr bool isEquals(const Vec2& vec) const noexcept;

#pragma endregion
#pragma region Operator

        constexpr Vec2 &operator = (const Vec2& vec) noexcept = default;

        constexpr bool operator == (const Vec2 &_vec) const noexcept;

        constexpr bool operator != (const Vec2 &_vec) const noexcept;

        constexpr Vec2 operator + (const Vec2 &_vec) const noexcept;

        constexpr Vec2 operator - (const Vec2 &_vec) const noexcept;

        constexpr Vec2 operator * (const Vec2 &_vec) const noexcept;

        constexpr Vec2 operator / (const Vec2 &_vec) const noexcept;

        constexpr Vec2 &operator += (const Vec2 &_vec) noexcept;

        constexpr Vec2 &operator -= (const Vec2 &_vec) noexcept;

        constexpr Vec2 &operator *= (const Vec2 &_vec) noexcept;

        constexpr Vec2 &operator /= (const Vec2 &_vec) noexcept;

        constexpr Vec2 operator + (float _value) const noexcept;

        constexpr Vec2 operator - (float _value) const noexcept;

        constexpr Vec2 operator * (float _value) const noexcept;

        constexpr Vec2 operator / (float _value) const noexcept;

        constexpr Vec2 &operator += (float _value) noexcept;

        constexpr Vec2 &operator -= (float _value) noexcept;

        constexpr Vec2 &operator *= (float _value) noexcept;

        constexpr Vec2 &operator /= (float _value) noexcept;

        constexpr Vec2 operator -() noexcept;

#pragma endregion

    };
    constexpr Vec2 operator * (float _value, const Vec2& vec) noexcept;
    constexpr Vec2 operator / (float _value, const Vec2& vec) ;

};

