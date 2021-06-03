#pragma once
#include <string>
#include "Build/solidAPI.hpp"
#include "EngineGenerated/vector4.sld.hpp"

namespace Solid SLDNamespace()
{
    class Vec2;

    class Vec3;

    class SOLID_API SLDClass() Vec4i
    {
    public:
        SLDField()
        int x = 0;
        SLDField()
        int y = 0;
        SLDField()
        int z = 0;
        SLDField()
        int w = 0;

        Vec4i_GENERATED
    };

    class SOLID_API SLDClass() Vec4
    {
    public:

    	SLDField()
	    float x = 0;
	    SLDField()
        float y = 0;
	    SLDField()
        float z = 0;
	    SLDField()
        float w = 0;


        Vec4() noexcept = default;

        Vec4(float _value) noexcept;

        Vec4(float _x, float _y, float _z, float _w) noexcept;

        Vec4(const Vec3& _copy) noexcept;

        Vec4(const Vec4& _copy) noexcept;

        ~Vec4() = default;

#pragma region Static Methods

        static const Vec4 Zero;
        static const Vec4 Up;
        static const Vec4 Down;
        static const Vec4 Left;
        static const Vec4 Right;
        static const Vec4 Forward;
        static const Vec4 Back;

		SLDMethod()
		static float Dot(const Vec4& _v1, const Vec4& _v2) noexcept;

		SLDMethod()
		static Vec4 Cross(const Vec4& _v1, const Vec4& _v2) noexcept;

		SLDMethod()
		static Vec4 Lerp(const Vec4& _v1, const Vec4& _v2, float _r) noexcept;

		SLDMethod()
		static Vec4 Nlerp(const Vec4& _v1, const Vec4& _v2, float _r) noexcept;

		SLDMethod()
		static Vec4 Slerp(const Vec4& _v1, const Vec4& _v2, float _r) noexcept;

#pragma endregion
#pragma region Methods

		SLDMethod()
		float SqrtLength() const noexcept;

		SLDMethod()
		float Length() const noexcept;

		SLDMethod()
		Vec4& Scale(float _scale) noexcept;

		SLDMethod()
		Vec4 GetScaled(float _scale) const noexcept;

		SLDMethod()
		Vec4& Unscale(float _scale) noexcept;

		SLDMethod()
		Vec4 GetUnscaled(float _scale) const noexcept;

		SLDMethod()
		Vec4& Normalize() noexcept;

		SLDMethod()
		Vec4 GetNormalized() const noexcept;

		SLDMethod()
		bool IsNormalized() const noexcept;

		SLDMethod()
		bool IsEquals(const Vec4& vec) const noexcept;

		SLDMethod()
		float Dist(const Vec4& vec) const noexcept;

		SLDMethod()
		float SqrtDist(const Vec4& vec) const noexcept;

		SLDMethod()
		Vec3 ToVector3() const noexcept;

		SLDMethod()
		const std::string ToString() const noexcept;

#pragma endregion
#pragma region Operator

		SLDMethod()
		Vec4& operator=(const Vec4& vec) noexcept = default;

		SLDMethod()
		bool operator==(const Vec4& _vec) const noexcept;

		SLDMethod()
		bool operator!=(const Vec4& _vec) const noexcept;

		SLDMethod()
		Vec4 operator+(const Vec4& _vec) const noexcept;

		SLDMethod()
		Vec4 operator-(const Vec4& _vec) const noexcept;

		SLDMethod()
		Vec4 operator*(const Vec4& _vec) const noexcept;

		SLDMethod()
		Vec4 operator/(const Vec4& _vec) const noexcept;

		SLDMethod()
		Vec4& operator+=(const Vec4& _vec) noexcept;

		SLDMethod()
		Vec4& operator-=(const Vec4& _vec) noexcept;

		SLDMethod()
		Vec4& operator*=(const Vec4& _vec) noexcept;

		SLDMethod()
		Vec4& operator/=(const Vec4& _vec) noexcept;

		SLDMethod()
		Vec4 operator+(float _value) const noexcept;

		SLDMethod()
		Vec4 operator-(float _value) const noexcept;

		SLDMethod()
		Vec4 operator*(float _value) const noexcept;

		SLDMethod()
		Vec4 operator/(float _value) const noexcept;

		SLDMethod()
		Vec4& operator+=(float _value) noexcept;

		SLDMethod()
		Vec4& operator-=(float _value) noexcept;

		SLDMethod()
		Vec4& operator*=(float _value) noexcept;

		SLDMethod()
		Vec4& operator/=(float _value) noexcept;

		SLDMethod()
		Vec4 operator-() noexcept;

        float& operator[](unsigned int index);
        float  operator[](unsigned int index) const;
#pragma endregion
		Vec4_GENERATED
    };

    Vec4 operator*(float _value, const Vec4& vec) noexcept;

    Vec4 operator/(float _value, const Vec4& vec);

};

File_GENERATED