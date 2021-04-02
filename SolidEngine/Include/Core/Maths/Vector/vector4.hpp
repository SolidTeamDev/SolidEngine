#pragma once

#include "Build/SolidAPI.hpp"

namespace Solid
{
    struct Vec4;

    struct Vec4;

    struct SOLID_API Vec4
    {
        float x;
        float y;
        float z;
        float w;

        constexpr Vec4() noexcept = default;

        constexpr Vec4(float _value) noexcept;

        constexpr Vec4(float _x, float _y, float _z, float _w) noexcept;

        constexpr Vec4(const Vec3& _copy) noexcept;

        constexpr Vec4(const Vec4& _copy) noexcept;

        ~Vec4() = default;

#pragma region Static Methods

        static const Vec4 Zero;
        static const Vec4 Up;
        static const Vec4 Down;
        static const Vec4 Left;
        static const Vec4 Right;
        static const Vec4 Forward;
        static const Vec4 Back;

        static constexpr float Dot(const Vec4& _v1, const Vec4& _v2) noexcept;

        static constexpr Vec4 Cross(const Vec4& _v1, const Vec4& _v2) noexcept;

        static Vec4 Lerp(const Vec4& _v1, const Vec4& _v2, float _r) noexcept;

        static Vec4 Nlerp(const Vec4& _v1, const Vec4& _v2, float _r) noexcept;

        static Vec4 Slerp(const Vec4& _v1, const Vec4& _v2, float _r) noexcept;

#pragma endregion
#pragma region Methods

        constexpr float SqrtLength() const noexcept;

        constexpr float Length() const noexcept;

        Vec4& Scale(float _scale) noexcept;

        constexpr Vec4 GetScaled(float _scale) const noexcept;

        Vec4& Unscale(float _scale) noexcept;

        constexpr Vec4 GetUnscaled(float _scale) const noexcept;

        Vec4& Normalize() noexcept;

        constexpr Vec4 GetNormalized() const noexcept;

        constexpr bool IsNormalized() const noexcept;

        constexpr bool IsEquals(const Vec4& vec) const noexcept;

        constexpr float Dist(const Vec4& vec) const noexcept;

        constexpr float SqrtDist(const Vec4& vec) const noexcept;

        constexpr Vec3 ToVector3() const noexcept;

        const std::string ToString() const noexcept;

#pragma endregion
#pragma region Operator

        constexpr Vec4& operator=(const Vec4& vec) noexcept = default;

        constexpr bool operator==(const Vec4& _vec) const noexcept;

        constexpr bool operator!=(const Vec4& _vec) const noexcept;

        constexpr Vec4 operator+(const Vec4& _vec) const noexcept;

        constexpr Vec4 operator-(const Vec4& _vec) const noexcept;

        constexpr Vec4 operator*(const Vec4& _vec) const noexcept;

        constexpr Vec4 operator/(const Vec4& _vec) const noexcept;

        Vec4& operator+=(const Vec4& _vec) noexcept;

        Vec4& operator-=(const Vec4& _vec) noexcept;

        Vec4& operator*=(const Vec4& _vec) noexcept;

        Vec4& operator/=(const Vec4& _vec) noexcept;

        constexpr Vec4 operator+(float _value) const noexcept;

        constexpr Vec4 operator-(float _value) const noexcept;

        constexpr Vec4 operator*(float _value) const noexcept;

        constexpr Vec4 operator/(float _value) const noexcept;

        Vec4& operator+=(float _value) noexcept;

        Vec4& operator-=(float _value) noexcept;

        Vec4& operator*=(float _value) noexcept;

        Vec4& operator/=(float _value) noexcept;

        constexpr Vec4 operator-() noexcept;

#pragma endregion

    };

    constexpr Vec4 operator*(float _value, const Vec4& vec) noexcept;

    constexpr Vec4 operator/(float _value, const Vec4& vec);

};