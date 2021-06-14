#pragma once
#include <string>
#include "Build/solidAPI.hpp"
#include "EngineGenerated/vector3.sld.hpp"
namespace Solid SLDNamespace()
{
    class Vec2;

    class Vec4;

    class Quat;

    struct SOLID_API SLDStruct() Vec3i
    {
        SLDField()
        int x;
        SLDField()
        int y;
        SLDField()
        int z;
        Vec3i_GENERATED
    };
    struct SOLID_API SLDStruct() Vec3ui
    {
        SLDField()
        uint x;
        SLDField()
        uint y;
        SLDField()
        uint z;
        Vec3ui_GENERATED
    };

    class SOLID_API SLDClass() Vec3
    {
    public:
		SLDField()
	    float x = 0;
	    SLDField()
        float y = 0;
	    SLDField()
        float z = 0;

		SLDMethod()
        Vec3() noexcept = default;


	    SLDMethod()
        Vec3(float _value) noexcept;

	    SLDMethod()
        Vec3(float _x, float _y, float _z) noexcept;

	    SLDMethod()
	    Vec3(const Vec4& _copy) noexcept;

	    SLDMethod()
	    Vec3(const Vec3& _copy) noexcept;

        /**
         * make a euler angle
        **/
	    SLDMethod()
	    Vec3(const Quat& _q) noexcept;

	    SLDMethod()
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

		SLDMethod()
		static float Dot(const Vec3& _v1, const Vec3& _v2) noexcept;

		SLDMethod()
		static Vec3 Cross(const Vec3& _v1, const Vec3& _v2) noexcept;

		SLDMethod()
		static Vec3 Lerp(const Vec3& _v1, const Vec3& _v2, float _r) noexcept;

		SLDMethod()
		static Vec3 Nlerp(const Vec3& _v1, const Vec3& _v2, float _r) noexcept;

		SLDMethod()
		static Vec3 Slerp(const Vec3& _v1, const Vec3& _v2, float _r) noexcept;

#pragma endregion
#pragma region Methods

		SLDMethod()
		float SqrtLength() const noexcept;

		SLDMethod()
		float Length() const noexcept;

		SLDMethod()
		Vec3& Scale(float _scale) noexcept;

		SLDMethod()
		Vec3 GetScaled(float _scale) const noexcept;

		SLDMethod()
		Vec3& Unscale(float _scale) noexcept;

		SLDMethod()
		Vec3 GetUnscaled(float _scale) const noexcept;

		SLDMethod()
		Vec3& Normalize() noexcept;

		SLDMethod()
		Vec3 GetNormalized() const noexcept;

		SLDMethod()
		bool IsNormalized() const noexcept;

		SLDMethod()
		bool IsEquals(const Vec3& vec) const noexcept;

		SLDMethod()
		float Dist(const Vec3& vec) const noexcept;

		SLDMethod()
		float SqrtDist(const Vec3& vec) const noexcept;

		SLDMethod()
		const std::string ToString() const noexcept;

#pragma endregion
#pragma region Operator

		SLDMethod()
		Vec3& operator=(const Vec3& vec) noexcept = default;

		SLDMethod()
		bool operator==(const Vec3& _vec) const noexcept;

		SLDMethod()
		bool operator!=(const Vec3& _vec) const noexcept;

		SLDMethod()
		Vec3 operator+(const Vec3& _vec) const noexcept;

		SLDMethod()
		Vec3 operator-(const Vec3& _vec) const noexcept;

		SLDMethod()
		Vec3 operator*(const Vec3& _vec) const noexcept;

		SLDMethod()
		Vec3 operator/(const Vec3& _vec) const noexcept;

		SLDMethod()
		Vec3& operator+=(const Vec3& _vec) noexcept;

		SLDMethod()
		Vec3& operator-=(const Vec3& _vec) noexcept;

		SLDMethod()
		Vec3& operator*=(const Vec3& _vec) noexcept;

		SLDMethod()
		Vec3& operator/=(const Vec3& _vec) noexcept;

		SLDMethod()
		Vec3 operator+(float _value) const noexcept;

		SLDMethod()
		Vec3 operator-(float _value) const noexcept;

		SLDMethod()
		Vec3 operator*(float _value) const noexcept;

		SLDMethod()
		Vec3 operator/(float _value) const noexcept;

		SLDMethod()
		Vec3& operator+=(float _value) noexcept;

		SLDMethod()
		Vec3& operator-=(float _value) noexcept;

		SLDMethod()
		Vec3& operator*=(float _value) noexcept;

		SLDMethod()
		Vec3& operator/=(float _value) noexcept;

		SLDMethod()
		Vec3 operator-() noexcept;

#pragma endregion
		Vec3_GENERATED
    };

    Vec3 operator*(float _value, const Vec3& vec) noexcept;

    Vec3 operator/(float _value, const Vec3& vec);
}

File_GENERATED