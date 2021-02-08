#include "Core/Maths//Vector/vector2.hpp"
#include "Core/Maths//Vector/vector3.hpp"
#include "Core/Maths//Vector/vector4.hpp"


namespace Solid
{
    Vec2 Vec2::zero{0,0};
    Vec2 Vec2::up{0,1};
    Vec2 Vec2::down{0,-1};
    Vec2 Vec2::left{-1,0};
    Vec2 Vec2::right{1,0};



    constexpr Vec2::Vec2(const float& _value) noexcept :
    x{_value},
    y{_value}
    {}

    constexpr Vec2::Vec2(const float& _x,const float& _y) noexcept :
    x{_x},
    y{_y}
    {}

    constexpr Vec2::Vec2(const Vec2& _copy) noexcept :
    x{_copy.x},
    y{_copy.y}
    {}

    constexpr Vec2::Vec2(const Vec3& _copy) noexcept :
    x{_copy.x},
    y{_copy.y}
    {}

    constexpr Vec2::Vec2(const Vec4& _copy) noexcept :
    x{_copy.x},
    y{_copy.y}
    {}

    constexpr float Vec2::dot(const Vec2& _v1,const Vec2 &_v2) noexcept
    {
        /*TODO*/
        return 0;
    }

    constexpr float Vec2::cross(const Vec2& _v1,const Vec2 &_v2) noexcept
    {
        /*TODO*/
        return 0;
    }

    constexpr float Vec2::sqrtLength() const noexcept
    {
        /*TODO*/
        return 0;
    }

    constexpr float Vec2::length() const noexcept
    {
        /*TODO*/
        return 0;
    }

    Vec2& Vec2::scale(float _scale) noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 Vec2::getScaled(float _scale) const noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    Vec2& Vec2::unscale(float _scale) noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 Vec2::getUnscaled(float _scale) const noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    Vec2& Vec2::normalize() noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 Vec2::getNormalize() const noexcept
    {
        /*TODO*/
        return Vec2::zero;
    }

    constexpr bool Vec2::operator==(const Vec2 &_vec) const noexcept
    {
        /*TODO*/
        return false;
    }

    constexpr bool Vec2::operator!=(const Vec2 &_vec) const noexcept
    {
        /*TODO*/
        return false;
    }

    constexpr Vec2 Vec2::operator+(const Vec2 &_vec) const noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 Vec2::operator-(const Vec2 &_vec) const noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 Vec2::operator*(const Vec2 &_vec) const noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 Vec2::operator/(const Vec2 &_vec) const noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 &Vec2::operator-=(const Vec2 &_vec) noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 &Vec2::operator*=(const Vec2 &_vec) noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 &Vec2::operator/=(const Vec2 &_vec) noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 Vec2::operator+(const float &_value) const noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 Vec2::operator-(const float &_value) const noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 Vec2::operator*(const float &_value) const noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 Vec2::operator/(const float &_value) const noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 &Vec2::operator+=(const float &_value) noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 &Vec2::operator-=(const float &_value) noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 &Vec2::operator*=(const float &_value) noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr Vec2 &Vec2::operator/=(const float &_value) noexcept
    {
        /*TODO*/
        return Vec2::zero;

    }

    constexpr float Vec2::operator[](const size_t _index) const noexcept
    {
        /*TODO*/
        return 0;

    }

    /*constexpr float& Vec2::operator[](const size_t _index) noexcept
    {
        TODO
    }*/

}