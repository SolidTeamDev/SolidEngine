#pragma once

#include "Build/SolidAPI.hpp"

namespace Solid
{
    struct Vec3;

    struct Vec4;

    struct SOLID_API Vec3
    {
        float x;
        float y;
        float z;

        constexpr Vec3() noexcept = default;
        constexpr Vec3(float _value)noexcept;
        constexpr Vec3(float _x, float _y, float _z)noexcept;
        constexpr Vec3(const Vec4& _copy)noexcept;
        constexpr Vec3(const Vec3& _copy)noexcept;

        ~Vec3() = default;

#pragma region Static Methods

        static const Vec3 Zero;
        static const Vec3 One;
        static const Vec3 Up;
        static const Vec3 Down;
        static const Vec3 Left;
        static const Vec3 Right;
        static const Vec3 Forward;
        static const Vec3 Back;

        static constexpr float Dot(const Vec3 &_v1,const Vec3 &_v2) noexcept;

        static constexpr Vec3 Cross(const Vec3 &_v1,const Vec3 &_v2) noexcept;

        static Vec3 Lerp(const Vec3& _v1,const Vec3& _v2, float _r) noexcept;

        static Vec3 Nlerp(const Vec3& _v1,const Vec3& _v2, float _r) noexcept;

        static Vec3 Slerp(const Vec3& _v1,const Vec3& _v2, float _r) noexcept;

#pragma endregion
#pragma region Methods

        constexpr float SqrtLength() const noexcept;

        constexpr float Length() const noexcept;

        Vec3 &Scale(float _scale) noexcept;

        constexpr Vec3 GetScaled(float _scale) const noexcept;

        Vec3 &Unscale(float _scale) noexcept;

        constexpr Vec3 GetUnscaled(float _scale) const noexcept;

        Vec3 &Normalize() noexcept;

        constexpr Vec3 GetNormalize() const noexcept;

        constexpr bool IsEquals(const Vec3& vec) const noexcept;

        constexpr float Dist(const Vec3& vec) const noexcept;

        constexpr float SqrtDist(const Vec3& vec) const noexcept;

        std::string ToString() noexcept;

#pragma endregion
#pragma region Operator

        constexpr Vec3 &operator = (const Vec3& vec) noexcept = default;

        constexpr bool operator == (const Vec3 &_vec) const noexcept;

        constexpr bool operator != (const Vec3 &_vec) const noexcept;

        constexpr Vec3 operator + (const Vec3 &_vec) const noexcept;

        constexpr Vec3 operator - (const Vec3 &_vec) const noexcept;

        constexpr Vec3 operator * (const Vec3 &_vec) const noexcept;

        constexpr Vec3 operator / (const Vec3 &_vec) const noexcept;

        constexpr Vec3 &operator += (const Vec3 &_vec) noexcept;

        constexpr Vec3 &operator -= (const Vec3 &_vec) noexcept;

        constexpr Vec3 &operator *= (const Vec3 &_vec) noexcept;

        constexpr Vec3 &operator /= (const Vec3 &_vec) noexcept;

        constexpr Vec3 operator + (float _value) const noexcept;

        constexpr Vec3 operator - (float _value) const noexcept;

        constexpr Vec3 operator * (float _value) const noexcept;

        constexpr Vec3 operator / (float _value) const noexcept;

        constexpr Vec3 &operator += (float _value) noexcept;

        constexpr Vec3 &operator -= (float _value) noexcept;

        constexpr Vec3 &operator *= (float _value) noexcept;

        constexpr Vec3 &operator /= (float _value) noexcept;

        constexpr Vec3 operator -() noexcept;

#pragma endregion

    };
    constexpr Vec3 operator * (float _value, const Vec3& vec) noexcept;
    constexpr Vec3 operator / (float _value, const Vec3& vec) ;
};