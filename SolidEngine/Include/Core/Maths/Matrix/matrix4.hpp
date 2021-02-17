#pragma once

#include "Build/SolidAPI.hpp"
#include <string>
#include <array>
#include "Core/Maths/Vector/vector3.hpp"
#include "Core/Maths/Utils/constants.hpp"

namespace Solid
{
    struct Quat;


    template<typename T = float>
    struct SOLID_API Mat4
    {
        std::array<T, 16> elements;

        Mat4();

        Mat4(const Mat4 &_copy) = default;

        Mat4(Mat4 &&_move) = default;

        Mat4(T e0 , T e1 , T e2  , T e3,
             T e4 , T e5 , T e6  , T e7,
             T e8 , T e9 , T e10 , T e11,
             T e12, T e13, T e14 , T e15);

        Mat4(std::array<T, 16> _elements);

        ~Mat4() = default;

#pragma region Static Methods

        static Mat4 Zero;
        static Mat4 Identity;

        static Mat4 Translate(const Vec3& _pos);
        static Mat4 Rotation(const Quat& _rot);
        static Mat4 Scale(const Vec3& _scale);
        static Mat4 Transform(const Vec3& _pos,const Quat& _rot, const Vec3& _scale = Vec3::One);
        static Mat4 Perspective(float _fov = 90.f, float _aspect = 1.f,float _near = 0.25f,float _far = 100.f);
        //static Mat4 Orthogonal();

#pragma endregion
#pragma region Methods

        constexpr bool IsZero() const noexcept;
        constexpr bool IsIdentity() const noexcept;
        constexpr bool IsEquals(const Mat4& _other,T _epsilon = S_EPSILON) const noexcept;

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
        constexpr Mat4 CoMatrix() noexcept;
        Mat4& Inverse() noexcept;
        constexpr Mat4 GetInverse() const noexcept;

        constexpr float Determinant() const noexcept;

        std::string ToString() noexcept;

    private:
        constexpr float det_2(unsigned x, unsigned y, unsigned z, unsigned w) noexcept;



#pragma endregion
#pragma region Operator
    public:

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

        T& Mat4<T>::operator[](unsigned int _index);

#pragma endregion

    };
    template <typename T>
    constexpr Mat4<T> operator*(T _value, const Mat4<T>& _mat) noexcept;


    template <typename T>
    constexpr Mat4<T> operator/(T _value, const Mat4<T>& _mat);

    using Mat4i = Mat4<int>;
    using Mat4f = Mat4<float>;
    using Mat4d = Mat4<double>;
}

#include "matrix4.inl"
