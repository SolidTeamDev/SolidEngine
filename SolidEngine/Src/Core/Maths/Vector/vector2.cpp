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

    Vec2 const Vec2::Zero{0,0};
    Vec2 const Vec2::Up{0,1};
    Vec2 const Vec2::Down{0,-1};
    Vec2 const Vec2::Left{-1,0};
    Vec2 const Vec2::Right{1,0};

    constexpr float Vec2::Dot(const Vec2& _v1,const Vec2 &_v2) noexcept
    {

        return _v1.x * _v2.x +
                _v1.y * _v2.y;
    }

    constexpr float Vec2::Cross(const Vec2& _v1,const Vec2 &_v2) noexcept
    {
        return _v1.x * _v2.y +
                _v1.y * _v2.x;
    }

    Vec2 Vec2::Lerp(const Vec2 &_v1, const Vec2 &_v2, float _r) noexcept
    {
        return Maths::Lerp(_v1,_v2, _r);
    }

    Vec2 Vec2::Nlerp(const Vec2 &_v1, const Vec2 &_v2, float _r) noexcept
    {
        return Maths::Lerp(_v1,_v2,_r).GetNormalized();
    }

    Vec2 Vec2::Slerp(const Vec2 &_v1, const Vec2 &_v2, float _r) noexcept
    {
        return Maths::Slerp(_v1,_v2, _r);
    }

#pragma endregion
#pragma region Methods

    constexpr float Vec2::SqrtLength() const noexcept
    {
        return x * x + y * y;
    }

    constexpr float Vec2::Length() const noexcept
    {
        return Maths::Sqrt(x * x + y * y);
    }

    Vec2& Vec2::Scale(float _scale) noexcept
    {
        x *= _scale;
        y *= _scale;
        return *this;

    }

    constexpr Vec2 Vec2::GetScaled(float _scale) const noexcept
    {

        return Vec2(x*_scale, y*_scale);

    }

    Vec2& Vec2::Unscale(float _scale) noexcept
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

    constexpr Vec2 Vec2::GetUnscaled(float _scale) const noexcept
    {
        if(_scale == 0)
        {
            Log::Send("getUnscale function: scale is null is impossible to divide by 0", Log::ELogSeverity::ERROR);
            return *this;
        }
        return Vec2(x/_scale, y/_scale);

    }

    Vec2& Vec2::Normalize() noexcept
    {
        float len = Length();
        if(len == 0)
        {
            Log::Send("normalize function: length = 0 impossible to compute");
            return *this;
        }

        x /= len;
        y /= len;
        return *this;

    }

#if _DEBUG

#define SE_WARN(_pred, str) { if(!_pred) Log::Send(str); }

#else

#define SE_WARN(...) {}

#endif

    constexpr Vec2 Vec2::GetNormalized() const noexcept
    {

        float len = Length();

        SE_WARN(len != 0, "normalize function: length = 0 impossible to compute");

        //if(len == 0)
        //{
        //    Log::Send("normalize function: length = 0 impossible to compute");
        //    return *this;
        //}
        return Vec2(x/len, y/len);
    }

    constexpr bool Vec2::IsNormalized() const noexcept
    {
        return Maths::Equals<float>(SqrtLength(), 3.f * S_EPSILON);
    }

    constexpr bool Vec2::IsEquals(const Vec2& vec) const noexcept
    {
        return Solid::Maths::Equals(x,vec.x, 0.0001f) &&
               Solid::Maths::Equals(y,vec.y, 0.0001f);
    }

    constexpr float Vec2::Dist(const Vec2 &vec) const noexcept
    {
        return (*this - vec).Length();
    }

    constexpr float Vec2::SqrtDist(const Vec2 &vec) const noexcept
    {
        return (*this - vec).SqrtLength();
    }

    const std::string Vec2::ToString() const noexcept
    {
        return "[" + std::to_string(x) + "," + std::to_string(y) + "]";
    }

#pragma endregion
#pragma region Operator

    constexpr bool Vec2::operator==(const Vec2 &_vec) const noexcept
    {
        return IsEquals(_vec);
    }

    constexpr bool Vec2::operator!=(const Vec2 &_vec) const noexcept
    {
        return !IsEquals(_vec);
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

    Vec2 &Vec2::operator+=(const Vec2 &_vec) noexcept
    {
        x += _vec.x;
        y += _vec.y;
        return *this;
    }

    Vec2 &Vec2::operator-=(const Vec2 &_vec) noexcept
    {
        x -= _vec.x;
        y -= _vec.y;
        return *this;
    }

    Vec2 &Vec2::operator*=(const Vec2 &_vec) noexcept
    {
        x *= _vec.x;
        y *= _vec.y;
        return *this;
    }

    Vec2 &Vec2::operator/=(const Vec2 &_vec) noexcept
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

    Vec2 &Vec2::operator+=( float _value) noexcept
    {
        x += _value;
        y += _value;
        return *this;
    }

    Vec2 &Vec2::operator-=(float _value) noexcept
    {
        x -= _value;
        y -= _value;
        return *this;
    }

    Vec2 &Vec2::operator*=(float _value) noexcept
    {
        x *= _value;
        y *= _value;
        return *this;
    }

    Vec2 &Vec2::operator/=(float _value) noexcept
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
        return _vec.GetScaled(_value);
    }

    constexpr Vec2 operator / (float _value, const Vec2& _vec)
    {
        return _vec.GetUnscaled(_value);
    }

#pragma endregion

}