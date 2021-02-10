#include "Core/Maths/Vector/vector2.hpp"
#include "Core/Maths/Vector/vector3.hpp"
#include "Core/Maths/Vector/vector4.hpp"

#include "Core/Maths/Utils/numerics.hpp"
#include "Core/Debug/log.hpp"



namespace Solid
{



#pragma region Constructor

    constexpr Vec2::Vec2(const float& _value) noexcept :
    x{_value},
    y{_value}
    {}

    constexpr Vec2::Vec2(float _x,float _y) noexcept :
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

#pragma endregion
#pragma region Static Methods

    Vec2 Vec2::zero{0,0};
    Vec2 Vec2::up{0,1};
    Vec2 Vec2::down{0,-1};
    Vec2 Vec2::left{-1,0};
    Vec2 Vec2::right{1,0};

    constexpr float Vec2::dot(const Vec2& _v1,const Vec2 &_v2) noexcept
    {

        return _v1.x * _v2.x +
                _v1.y * _v2.y;
    }

    constexpr float Vec2::cross(const Vec2& _v1,const Vec2 &_v2) noexcept
    {
        return _v1.x * _v2.y +
                _v1.y * _v2.x;
    }

    Vec2 Vec2::lerp(const Vec2 &_v1, const Vec2 &_v2, float _r) noexcept
    {
        //return Maths::lerp(_v1,_v2, _r);
        return Vec2::zero;
    }

    Vec2 Vec2::Nlerp(const Vec2 &_v1, const Vec2 &_v2, float _r) noexcept
    {
        return Maths::lerp(_v1,_v2,_r).getNormalize();
    }

    Vec2 Vec2::slerp(const Vec2 &_v1, const Vec2 &_v2, float _r) noexcept
    {
        return Maths::slerp(_v1,_v2, _r);
    }

#pragma endregion
#pragma region Methods

    constexpr float Vec2::sqrtLength() const noexcept
    {
        return x * x + y * y;
    }

    constexpr float Vec2::length() const noexcept
    {
        return Maths::sqrt(x * x + y * y);
    }

    Vec2& Vec2::scale(float _scale) noexcept
    {
        x *= _scale;
        y *= _scale;
        return *this;

    }

    constexpr Vec2 Vec2::getScaled(float _scale) const noexcept
    {

        return Vec2(x*_scale, y*_scale);

    }

    Vec2& Vec2::unscale(float _scale) noexcept
    {
        if(_scale == 0)
        {
            Log::Send("unscale function: scale is null is impossible to divide by 0", Log::ELogSeverity::ERROR);
            return *this;
        }
        x /= _scale;
        y /= _scale;
        return *this;
    }

    constexpr Vec2 Vec2::getUnscaled(float _scale) const noexcept
    {
        if(_scale == 0)
        {
            Log::Send("getUnscale function: scale is null is impossible to divide by 0", Log::ELogSeverity::ERROR);
            return *this;
        }
        return Vec2(x/_scale, y/_scale);

    }

    Vec2& Vec2::normalize() noexcept
    {
        float len = length();
        if(len == 0)
        {
            Log::Send("normalize function: length = 0 impossible to compute");
            return *this;
        }

        x /= len;
        y /= len;
        return *this;

    }

    constexpr Vec2 Vec2::getNormalize() const noexcept
    {
        float len = length();
        if(len == 0)
        {
            Log::Send("normalize function: length = 0 impossible to compute");
            return *this;
        }
        return Vec2(x/len, y/len);
    }

    constexpr  bool Vec2::isEquals(const Vec2 &vec) const noexcept
    {
        return Solid::Maths::equals(x,vec.x) && Solid::Maths::equals(y,vec.y);
    }

#pragma endregion
#pragma region Operator

    constexpr bool Vec2::operator==(const Vec2 &_vec) const noexcept
    {
        return isEquals(_vec);
    }

    constexpr bool Vec2::operator!=(const Vec2 &_vec) const noexcept
    {
        return !isEquals(_vec);
    }

    constexpr Vec2 Vec2::operator+(const Vec2 &_vec) const noexcept
    {
        return Vec2(x + _vec.x,y + _vec.y);
    }

    constexpr Vec2 Vec2::operator-(const Vec2 &_vec) const noexcept
    {
        return Vec2(x - _vec.x,y - _vec.y);
    }

    constexpr Vec2 Vec2::operator*(const Vec2 &_vec) const noexcept
    {
        return Vec2(x * _vec.x,y * _vec.y);
    }

    constexpr Vec2 Vec2::operator/(const Vec2 &_vec) const noexcept
    {
        return Vec2(x / _vec.x,y / _vec.y);
    }
    constexpr Vec2 &Vec2::operator+=(const Vec2 &_vec) noexcept
    {
        x += _vec.x;
        y += _vec.y;
        return *this;
    }

    constexpr Vec2 &Vec2::operator-=(const Vec2 &_vec) noexcept
    {
        x -= _vec.x;
        y -= _vec.y;
        return *this;
    }

    constexpr Vec2 &Vec2::operator*=(const Vec2 &_vec) noexcept
    {
        x *= _vec.x;
        y *= _vec.y;
        return *this;
    }

    constexpr Vec2 &Vec2::operator/=(const Vec2 &_vec) noexcept
    {
        x /= _vec.x;
        y /= _vec.y;
        return *this;
    }

    constexpr Vec2 Vec2::operator+(float _value) const noexcept
    {

        return Vec2(x + _value,y + _value);
    }

    constexpr Vec2 Vec2::operator-(float _value) const noexcept
    {
        return Vec2(x - _value,y - _value);
    }

    constexpr Vec2 Vec2::operator*(float _value) const noexcept
    {
        return Vec2(x * _value,y * _value);
    }

    constexpr Vec2 Vec2::operator/(float _value) const noexcept
    {
        return Vec2(x / _value,y / _value);
    }

    constexpr Vec2 &Vec2::operator+=( float _value) noexcept
    {
        x += _value;
        y += _value;
        return *this;
    }

    constexpr Vec2 &Vec2::operator-=(float _value) noexcept
    {
        x -= _value;
        y -= _value;
        return *this;
    }

    constexpr Vec2 &Vec2::operator*=(float _value) noexcept
    {
        x *= _value;
        y *= _value;
        return *this;
    }

    constexpr Vec2 &Vec2::operator/=(float _value) noexcept
    {
        x /= _value;
        y /= _value;
        return *this;
    }

    constexpr Vec2 Vec2::operator-() noexcept
    {
        return Vec2(-x,-y);
    }

    constexpr Vec2 operator * (float _value, const Vec2& _vec) noexcept
    {
        return _vec.getScaled(_value);
    }

    constexpr Vec2 operator / (float _value, const Vec2& _vec)
    {
        return _vec.getUnscaled(_value);
    }

#pragma endregion

}