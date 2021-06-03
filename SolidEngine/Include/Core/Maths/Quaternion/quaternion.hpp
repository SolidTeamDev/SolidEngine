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
        Quat()noexcept:x(0),y(0),z(0),w(1){}
        Quat(float _value) noexcept;
        Quat(float _x,float _y, float _z, float _w = 1) noexcept;
        Quat(const Vec3& _angles) noexcept;
        Quat(const Quat& _copy) noexcept = default;
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
        static float Dot(const Quat &_q1,const Quat &_q2) noexcept;

		SLDMethod()
        static Quat Lerp(const Quat& _q1,const Quat& _q2, float _r) noexcept;

		SLDMethod()
        static Quat Nlerp(const Quat& _q1,const Quat& _q2, float _r) noexcept;

		SLDMethod()
        static Quat Slerp(const Quat& _q1,const Quat& _q2, float _r) noexcept;

#pragma endregion
#pragma region Methods

		SLDMethod()
        bool IsZero() const noexcept;

		SLDMethod()
        float SqrtLength() const noexcept;

		SLDMethod()
        float Length() const noexcept;

		SLDMethod()
        Quat &Scale(float _scale) noexcept;

		SLDMethod()
        Quat GetScaled(float _scale) const noexcept;

		SLDMethod()
        Quat &Unscale(float _scale) noexcept;

		SLDMethod()
        Quat GetUnscaled(float _scale) const noexcept;

		SLDMethod()
        Quat &Normalize() noexcept;

		SLDMethod()
        Quat GetNormalized() const noexcept;

		SLDMethod()
        Quat& Inverse();

		SLDMethod()
        Quat GetInversed() const;

		SLDMethod()
        bool IsNormalized() const noexcept;

		SLDMethod()
        bool IsEquals(const Quat& _quat) const noexcept;

		SLDMethod()
        Vec3 Rotate(const Vec3& _vec) const;
		SLDMethod()
        Quat Rotate(const Quat& _quat) const;

		SLDMethod()
        Vec3 Unrotate(const Vec3& _vec) const;
		SLDMethod()
        Quat Unrotate(const Quat& _quat) const;

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
        Quat operator * (float _scale) const noexcept;

		SLDMethod()
        Quat operator / (float _scale) const noexcept;

		SLDMethod()
        Quat operator + (float _scale) const noexcept;

		SLDMethod()
        Quat operator - (float _scale) const noexcept;

		SLDMethod()
        Quat operator * (const Quat& _quat) const noexcept;

		SLDMethod()
        Vec3 operator * (const Vec3& _vec) const noexcept;


		SLDMethod()
        Quat operator / (const Quat& _quat) const noexcept;

		SLDMethod()
        Vec3 operator / (const Vec3& _vec) const noexcept;

		SLDMethod()
        Quat operator + (const Quat& _quat) const noexcept;

		SLDMethod()
        Quat operator - (const Quat& _quat) const noexcept;

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
        bool operator == (const Quat& _quat);

		SLDMethod()
        bool operator != (const Quat& _quat);

		SLDMethod()
        Quat operator - () const noexcept;
#pragma endregion
		Quat_GENERATED
    };

    Quat operator * (float _value, const Quat& _quat) noexcept;
    Quat operator / (float _value, const Quat& _quat) ;
}

File_GENERATED