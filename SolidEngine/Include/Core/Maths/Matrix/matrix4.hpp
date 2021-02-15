#pragma once

#include "Build/SolidAPI.hpp"
#include <array>
#include "Core/Maths/Vector/vector3.hpp"

namespace Solid
{
    struct Quat;


    template<typename T = float>
    struct Mat4
    {
        std::array<T, 16> elements;

        Mat4() = default;

        Mat4(const Mat4 &_copy) = default;

        Mat4(Mat4 &&_move) = default;

        Mat4(T e0 , T e1 , T e2  , T e3,
             T e4 , T e5 , T e6  , T e7,
             T e8 , T e9 , T e10 , T e11
             T e12, T e13, T e14 , T e15);

        Mat4(std::array<T, 16> _elements);

        ~Mat4() = default;

#pragma region Static Methods

        static Mat4 Zero;
        static Mat4 Identity;

        static Translate(const Vec3& _pos);
        static Mat4 Rotation(const Quat& _rot);
        static Mat4 Scale(const Vec3& _scale);
        static Mat4 Transform(const Vec3& _pos,const Quat& _rot, const Vec3& _scale = Vec3::One);
        static Mat4 Perspective(T _fov = T(90.f), T _aspect = T(1.f), T  _near = (0.25f), T _far = (100.f));

#pragma endregion
#pragma region Methods

        constexpr bool IsZero() const noexcept;
        constexpr bool IsIdentity() const noexcept;
        constexpr bool IsEquals() const noexcept;

        T& At(unsigned int _index);
        constexpr T& At(unsigned int _index)const;
        T& At(unsigned int _n, unsigned int _m);
        constexpr T& At(unsigned int _n, unsigned int _m)const;

        Mat4& Multiply(T _scale) noexcept;
        constexpr Mat4 GetMultiply(T _scale) const noexcept;

        Mat4& Divide(T _scale) noexcept;
        constexpr Mat4 GetDivide(T _scale) const noexcept;

        Mat4& Transpose() noexcept;
        constexpr Mat4 GetTranspose() const noexcept;
        Mat4& Inverse() noexcept;
        constexpr Mat4 GetInverse() const noexcept;

        constexpr float Determinant() const noexcept;



#pragma endregion
#pragma region Operator

        Mat4& operator = (Mat4&& _move) = default;
        Mat4& operator = (Mat4& _copy)  = default;

        constexpr Mat4 operator * (T _scale) const noexcept;
        constexpr Mat4 operator / (T _scale) const noexcept;
        Mat4& operator *= (T _scale) noexcept;
        Mat4& operator /= (T _scale) noexcept;

        constexpr Mat4 operator + (const Mat4& _mat) const noexcept;
        constexpr Mat4 operator - (const Mat4& _mat) const noexcept;
        constexpr Mat4 operator * (const Mat4& _mat) const noexcept;
        constexpr Mat4 operator / (const Mat4& _mat) const noexcept;
        Mat4& operator += (Mat4& _mat) noexcept;
        Mat4& operator -= (Mat4& _mat) noexcept;
        Mat4& operator *= (Mat4& _mat) noexcept;
        Mat4& operator /= (Mat4& _mat) noexcept;

#pragma endregion

    };
}

include "matrix.inl";
