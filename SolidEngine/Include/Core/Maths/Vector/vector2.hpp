#pragma once
#include "Build/SolidAPI.hpp"
#include <Core/Maths/Utils/constants.hpp>

#include "EngineGenerated/vector2.sld.hpp"
#include <iostream>
#include <string>

namespace Solid SLDNamespace()
{
    class Vec3;

    class Vec4;

    //TODO: SEE TO TEMPLATE VECTOR

    struct SOLID_API SLDStruct() Vec2i
    {
        int x;
        int y;
        Vec2i_GENERATED
    };
    struct SOLID_API SLDStruct() Vec2d
    {
        double x;
        double y;
	    Vec2d_GENERATED
    };
    class SOLID_API SLDClass() Vec2
    {
    public:

	    float x;
        float y;

        constexpr Vec2() noexcept = default;

        constexpr Vec2(const float &_value) noexcept;

        constexpr Vec2(float _x, float _y) noexcept;

        constexpr Vec2(const Vec2 &_copy) noexcept;

        constexpr Vec2(const Vec3 &_copy) noexcept;

        constexpr Vec2(const Vec4 &_copy) noexcept;

        ~Vec2() = default;

#pragma region Static Methods

        static const Vec2 Zero;
        static const Vec2 Up;
        static const Vec2 Down;
        static const Vec2 Left;
        static const Vec2 Right;

        static constexpr float Dot(const Vec2 &_v1,const Vec2 &_v2) noexcept;

        static constexpr float Cross(const Vec2 &_v1,const Vec2 &_v2) noexcept;

        static Vec2 Lerp(const Vec2& _v1,const Vec2& _v2, float _r) noexcept;

        static Vec2 Nlerp(const Vec2& _v1,const Vec2& _v2, float _r) noexcept;

        static Vec2 Slerp(const Vec2& _v1,const Vec2& _v2, float _r) noexcept;

#pragma endregion
#pragma region Methods

        constexpr float SqrtLength() const noexcept;

        constexpr float Length() const noexcept;

        Vec2 &Scale(float _scale) noexcept;

        constexpr Vec2 GetScaled(float _scale) const noexcept;

        Vec2 &Unscale(float _scale) noexcept;

        constexpr Vec2 GetUnscaled(float _scale) const noexcept;

        Vec2 &Normalize() noexcept;

        constexpr Vec2 GetNormalized() const noexcept;

        constexpr bool IsNormalized() const noexcept;

        constexpr bool IsEquals(const Vec2& vec) const noexcept;

        constexpr float Dist(const Vec2& vec) const noexcept;

        constexpr float SqrtDist(const Vec2& vec) const noexcept;

        const std::string ToString() const noexcept;

#pragma endregion
#pragma region Operator

        constexpr Vec2 &operator = (const Vec2& vec) noexcept = default;

        constexpr bool operator == (const Vec2 &_vec) const noexcept;

        constexpr bool operator != (const Vec2 &_vec) const noexcept;

        constexpr Vec2 operator + (const Vec2 &_vec) const noexcept;

        constexpr Vec2 operator - (const Vec2 &_vec) const noexcept;

        constexpr Vec2 operator * (const Vec2 &_vec) const noexcept;

        constexpr Vec2 operator / (const Vec2 &_vec) const noexcept;

        Vec2 &operator += (const Vec2 &_vec) noexcept;

        Vec2 &operator -= (const Vec2 &_vec) noexcept;

        Vec2 &operator *= (const Vec2 &_vec) noexcept;

        Vec2 &operator /= (const Vec2 &_vec) noexcept;

        constexpr Vec2 operator + (float _value) const noexcept;

        constexpr Vec2 operator - (float _value) const noexcept;

        constexpr Vec2 operator * (float _value) const noexcept;

        constexpr Vec2 operator / (float _value) const noexcept;

        Vec2 &operator += (float _value) noexcept;

        Vec2 &operator -= (float _value) noexcept;

        Vec2 &operator *= (float _value) noexcept;

        Vec2 &operator /= (float _value) noexcept;

        constexpr Vec2 operator -() noexcept;

#pragma endregion
		Vec2_GENERATED
    };
    constexpr Vec2 operator * (float _value, const Vec2& vec) noexcept;
    constexpr Vec2 operator / (float _value, const Vec2& vec) ;

};

File_GENERATED
