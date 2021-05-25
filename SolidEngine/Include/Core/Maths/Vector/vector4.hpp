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

		SLDMethod()
		static constexpr float Dot(const Vec4& _v1, const Vec4& _v2) noexcept;

		SLDMethod()
		static constexpr Vec4 Cross(const Vec4& _v1, const Vec4& _v2) noexcept;

		SLDMethod()
		static Vec4 Lerp(const Vec4& _v1, const Vec4& _v2, float _r) noexcept;

		SLDMethod()
		static Vec4 Nlerp(const Vec4& _v1, const Vec4& _v2, float _r) noexcept;

		SLDMethod()
		static Vec4 Slerp(const Vec4& _v1, const Vec4& _v2, float _r) noexcept;

#pragma endregion
#pragma region Methods

		SLDMethod()
		constexpr float SqrtLength() const noexcept;

		SLDMethod()
		constexpr float Length() const noexcept;

		SLDMethod()
		Vec4& Scale(float _scale) noexcept;

		SLDMethod()
		constexpr Vec4 GetScaled(float _scale) const noexcept;

		SLDMethod()
		Vec4& Unscale(float _scale) noexcept;

		SLDMethod()
		constexpr Vec4 GetUnscaled(float _scale) const noexcept;

		SLDMethod()
		Vec4& Normalize() noexcept;

		SLDMethod()
		constexpr Vec4 GetNormalized() const noexcept;

		SLDMethod()
		constexpr bool IsNormalized() const noexcept;

		SLDMethod()
		constexpr bool IsEquals(const Vec4& vec) const noexcept;

		SLDMethod()
		constexpr float Dist(const Vec4& vec) const noexcept;

		SLDMethod()
		constexpr float SqrtDist(const Vec4& vec) const noexcept;

		SLDMethod()
		constexpr Vec3 ToVector3() const noexcept;

		SLDMethod()
		const std::string ToString() const noexcept;

#pragma endregion
#pragma region Operator

		SLDMethod()
		constexpr Vec4& operator=(const Vec4& vec) noexcept = default;

		SLDMethod()
		constexpr bool operator==(const Vec4& _vec) const noexcept;

		SLDMethod()
		constexpr bool operator!=(const Vec4& _vec) const noexcept;

		SLDMethod()
		constexpr Vec4 operator+(const Vec4& _vec) const noexcept;

		SLDMethod()
		constexpr Vec4 operator-(const Vec4& _vec) const noexcept;

		SLDMethod()
		constexpr Vec4 operator*(const Vec4& _vec) const noexcept;

		SLDMethod()
		constexpr Vec4 operator/(const Vec4& _vec) const noexcept;

		SLDMethod()
		Vec4& operator+=(const Vec4& _vec) noexcept;

		SLDMethod()
		Vec4& operator-=(const Vec4& _vec) noexcept;

		SLDMethod()
		Vec4& operator*=(const Vec4& _vec) noexcept;

		SLDMethod()
		Vec4& operator/=(const Vec4& _vec) noexcept;

		SLDMethod()
		constexpr Vec4 operator+(float _value) const noexcept;

		SLDMethod()
		constexpr Vec4 operator-(float _value) const noexcept;

		SLDMethod()
		constexpr Vec4 operator*(float _value) const noexcept;

		SLDMethod()
		constexpr Vec4 operator/(float _value) const noexcept;

		SLDMethod()
		Vec4& operator+=(float _value) noexcept;

		SLDMethod()
		Vec4& operator-=(float _value) noexcept;

		SLDMethod()
		Vec4& operator*=(float _value) noexcept;

		SLDMethod()
		Vec4& operator/=(float _value) noexcept;

		SLDMethod()
		constexpr Vec4 operator-() noexcept;

        float& operator[](unsigned int index);
        float  operator[](unsigned int index) const;
#pragma endregion
		Vec4_GENERATED
    };

    constexpr Vec4 operator*(float _value, const Vec4& vec) noexcept;

    constexpr Vec4 operator/(float _value, const Vec4& vec);

};

File_GENERATED