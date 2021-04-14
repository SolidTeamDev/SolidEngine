#pragma once

#include <string>

#include "Build/solidAPI.hpp"
#include "../Utils/constants.hpp"
#include "EngineGenerated/quaternion.sld.hpp"

namespace Solid SLDNamespace()
{
    class Vec3;
    class SOLID_API SLDClass() Quat
    {
    public:
        constexpr Quat()noexcept:x(0),y(0),z(0),w(1){}
        constexpr Quat(float _value) noexcept;
        constexpr Quat(float _x,float _y, float _z, float _w = 1) noexcept;
        constexpr Quat(const Vec3& _angles) noexcept;
        constexpr Quat(const Quat& _copy) noexcept = default;
        Quat(float _angle, const Vec3& _axis) noexcept;

        ~Quat() = default;

        SLDField()
        float x;
	    SLDField()
        float y;
	    SLDField()
        float z;
	    SLDField()
        float w;

#pragma region Static Methods

        static Quat Zero;

        static Quat Identity;

        SLDMethod()
        static constexpr float Dot(const Quat &_q1,const Quat &_q2) noexcept;

		SLDMethod()
        static Quat Lerp(const Quat& _q1,const Quat& _q2, float _r) noexcept;

		SLDMethod()
        static Quat Nlerp(const Quat& _q1,const Quat& _q2, float _r) noexcept;

		SLDMethod()
        static Quat Slerp(const Quat& _q1,const Quat& _q2, float _r) noexcept;

#pragma endregion
#pragma region Methods

		SLDMethod()
        constexpr bool IsZero() const noexcept;

		SLDMethod()
        constexpr float SqrtLength() const noexcept;

		SLDMethod()
        constexpr float Length() const noexcept;

		SLDMethod()
        Quat &Scale(float _scale) noexcept;

		SLDMethod()
        constexpr Quat GetScaled(float _scale) const noexcept;

		SLDMethod()
        Quat &Unscale(float _scale) noexcept;

		SLDMethod()
        constexpr Quat GetUnscaled(float _scale) const noexcept;

		SLDMethod()
        Quat &Normalize() noexcept;

		SLDMethod()
        constexpr Quat GetNormalized() const noexcept;

		SLDMethod()
        constexpr Quat& Inverse();

		SLDMethod()
        constexpr Quat GetInversed() const;

		SLDMethod()
        constexpr bool IsNormalized() const noexcept;

		SLDMethod()
        constexpr bool IsEquals(const Quat& _quat) const noexcept;

		SLDMethod()
        constexpr Vec3 Rotate(const Vec3& _vec) const;
		SLDMethod()
        constexpr Quat Rotate(const Quat& _quat) const;

		SLDMethod()
        constexpr Vec3 Unrotate(const Vec3& _vec) const;
		SLDMethod()
        constexpr Quat Unrotate(const Quat& _quat) const;

		SLDMethod()
        Vec3 ToEuler() const;
		SLDMethod()
        Quat FromEuler(const Vec3& _angles) const;


		SLDMethod()
        const std::string ToString() const noexcept;

#pragma endregion
#pragma region Operator

		SLDMethod()
        Quat& operator = (const Quat& _quat) = default;

		SLDMethod()
        Quat& operator = (Quat&& _quat) = default;

		SLDMethod()
        constexpr Quat operator * (float _scale) const noexcept;

		SLDMethod()
        constexpr Quat operator / (float _scale) const noexcept;

		SLDMethod()
        constexpr Quat operator + (float _scale) const noexcept;

		SLDMethod()
        constexpr Quat operator - (float _scale) const noexcept;

		SLDMethod()
        constexpr Quat operator * (const Quat& _quat) const noexcept;

		SLDMethod()
        constexpr Vec3 operator * (const Vec3& _vec) const noexcept;


		SLDMethod()
        constexpr Quat operator / (const Quat& _quat) const noexcept;

		SLDMethod()
        constexpr Vec3 operator / (const Vec3& _vec) const noexcept;

		SLDMethod()
        constexpr Quat operator + (const Quat& _quat) const noexcept;

		SLDMethod()
        constexpr Quat operator - (const Quat& _quat) const noexcept;

		SLDMethod()
        Quat& operator *= (float _scale) ;

		SLDMethod()
        Quat& operator /= (float _scale);

		SLDMethod()
        Quat& operator += (float _scale);

		SLDMethod()
        Quat& operator -= (float _scale);

		SLDMethod()
        Quat& operator *= (const Quat& _quat);

		SLDMethod()
        Quat& operator /= (const Quat& _quat);

		SLDMethod()
        Quat& operator += (const Quat& _quat);

		SLDMethod()
        Quat& operator -= (const Quat& _quat);

		SLDMethod()
        constexpr bool operator == (const Quat& _quat);

		SLDMethod()
        constexpr bool operator != (const Quat& _quat);

		SLDMethod()
        constexpr Quat operator - () const noexcept;
#pragma endregion
		Quat_GENERATED
    };

    constexpr Quat operator * (float _value, const Quat& _quat) noexcept;
    constexpr Quat operator / (float _value, const Quat& _quat) ;
}

File_GENERATED