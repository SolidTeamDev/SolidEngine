#pragma once

#include "Build/SolidAPI.hpp"
#include "../Utils/constants.hpp"

namespace Solid
{
    struct Vec3;
    struct SOLID_API Quat
    {
        constexpr Quat() noexcept = default;
        constexpr Quat(float _value) noexcept;
        constexpr Quat(float _x,float _y = 0, float _z = 0, float _w = 1) noexcept;
        constexpr Quat(float _angle, const Vec3& _axis) noexcept;
        constexpr Quat(const Quat& _copy) noexcept = default;

        ~Quat() = default;

        float x;
        float y;
        float z;
        float w;

#pragma region Static Methods

        static Quat Zero;

        static Quat Identity;

        static constexpr float Dot(const Quat &_q1,const Quat &_q2) noexcept;

        static Quat Lerp(const Quat& _q1,const Quat& _q2, float _r) noexcept;

        static Quat Nlerp(const Quat& _q1,const Quat& _q2, float _r) noexcept;

        static Quat Slerp(const Quat& _q1,const Quat& _q2, float _r) noexcept;

#pragma endregion
#pragma region Methods

        constexpr bool IsZero() const noexcept;

        constexpr float SqrtLength() const noexcept;

        constexpr float Length() const noexcept;

        Quat &Scale(float _scale) noexcept;

        constexpr Quat GetScaled(float _scale) const noexcept;

        Quat &Unscale(float _scale) noexcept;

        constexpr Quat GetUnscaled(float _scale) const noexcept;

        Quat &Normalize() noexcept;

        constexpr Quat GetNormalize() const noexcept;

        constexpr Quat Inverse();

        constexpr Quat GetInversed() const;

        constexpr bool IsNormalized() const noexcept;

        constexpr bool IsEquals(const Quat& _quat) const noexcept;

        constexpr Vec3 Rotate(const Vec3& _vec) const;
        constexpr Quat Rotate(const Quat& _quat) const;

        constexpr Vec3 Unrotate(const Vec3& _vec) const;
        constexpr Quat Unrotate(const Quat& _quat) const;

        std::string ToString() noexcept;

#pragma endregion
#pragma region Operator

        Quat& operator = (const Quat& _quat) = default;

        Quat& operator = (Quat&& _quat) = default;

        constexpr Quat operator * (float _scale) const noexcept;

        constexpr Quat operator / (float _scale) const noexcept;

        constexpr Quat operator + (float _scale) const noexcept;

        constexpr Quat operator - (float _scale) const noexcept;

        constexpr Quat operator * (const Quat& _quat) const noexcept;

        constexpr Vec3 operator * (const Vec3& _vec) const noexcept;


        constexpr Quat operator / (const Quat& _quat) const noexcept;

        constexpr Vec3 operator / (const Vec3& _vec) const noexcept;

        constexpr Quat operator + (const Quat& _quat) const noexcept;

        constexpr Quat operator - (const Quat& _quat) const noexcept;

        Quat& operator *= (float _scale) ;

        Quat& operator /= (float _scale);

        Quat& operator += (float _scale);

        Quat& operator -= (float _scale);

        Quat& operator *= (const Quat& _quat);

        Quat& operator /= (const Quat& _quat);

        Quat& operator += (const Quat& _quat);

        Quat& operator -= (const Quat& _quat);

        constexpr bool operator == (const Quat& _quat);

        constexpr bool operator != (const Quat& _quat);

        constexpr Quat operator - () const noexcept;
#pragma endregion

    };
}