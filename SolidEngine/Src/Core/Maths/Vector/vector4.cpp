#include "Core/Maths/Vector/vector2.hpp"
#include "Core/Maths/Vector/vector3.hpp"
#include "Core/Maths/Vector/vector4.hpp"

#include "Core/Maths/Utils/numerics.hpp"
#include "Core/Debug/log.hpp"

namespace Solid
{

#pragma region Constructor
    constexpr Vec4::Vec4(float _value)noexcept :
    x{_value},
    y{_value},
    z{_value},
    w{1}
    {}

    constexpr Vec4::Vec4(float _x, float _y, float _z, float _w)noexcept:
    x{_x},
    y{_y},
    z{_z},
    w{_w}
    {}

    constexpr Vec4::Vec4(const Vec3& _copy)noexcept:
    x{_copy.x},
    y{_copy.y},
    z{_copy.z},
    w{1}
    {}

    constexpr Vec4::Vec4(const Vec4& _copy)noexcept:
    x{_copy.x},
    y{_copy.y},
    z{_copy.z},
    w{_copy.w}
    {}

#pragma endregion
#pragma region Static Methods

    Vec4 const Vec4::Zero{0,0,0,1};
    Vec4 const Vec4::Up{0,1,0,1};
    Vec4 const Vec4::Down{0,-1,0,1};
    Vec4 const Vec4::Left{-1,0,0,1};
    Vec4 const Vec4::Right{1,0,0,1};
    Vec4 const Vec4::Forward{1,0,1,1};
    Vec4 const Vec4::Back{0,0,-1,1};

    constexpr float Vec4::Dot(const Vec4& _v1,const Vec4 &_v2) noexcept
    {

        return _v1.x * _v2.x +
               _v1.y * _v2.y +
               _v1.z * _v2.z +
               _v1.w * _v2.w;
    }

    constexpr Vec4 Vec4::Cross(const Vec4& _v1,const Vec4 &_v2) noexcept
    {
        return Vec4(_v1.y * _v2.z - _v1.z * _v2.y,
                _v1.z * _v2.x - _v1.x * _v2.z,
                _v1.x * _v2.y - _v1.y * _v2.x, _v1.w * _v2.w);
    }

    Vec4 Vec4::Lerp(const Vec4 &_v1, const Vec4 &_v2, float _r) noexcept
    {
        return Maths::Lerp(_v1,_v2, _r);
    }

    Vec4 Vec4::Nlerp(const Vec4 &_v1, const Vec4 &_v2, float _r) noexcept
    {
        return Maths::Lerp(_v1,_v2,_r).GetNormalize();
    }

    Vec4 Vec4::Slerp(const Vec4 &_v1, const Vec4 &_v2, float _r) noexcept
    {
        return Maths::Slerp(_v1,_v2, _r);
    }

#pragma endregion
#pragma region Methods

    constexpr float Vec4::SqrtLength() const noexcept
    {
        return x * x + y *  + z * z + w * w;
    }

    constexpr float Vec4::Length() const noexcept
    {
        return Maths::Sqrt(x * x + y * y + z * z + w * w);
    }

    Vec4& Vec4::Scale(float _scale) noexcept
    {
        x *= _scale;
        y *= _scale;
        z *= _scale;
        w *= _scale;
        return *this;

    }

    constexpr Vec4 Vec4::GetScaled(float _scale) const noexcept
    {

        return Vec4(x*_scale, y*_scale, z * _scale, w * _scale);

    }

    Vec4& Vec4::Unscale(float _scale) noexcept
    {
        if(_scale == 0)
        {
            Log::Send("unscale function: scale is null is impossible to divide by 0", Log::ELogSeverity::ERROR);
            return *this;
        }
        x /= _scale;
        y /= _scale;
        z /= _scale;
        w /= _scale;
        return *this;
    }

    constexpr Vec4 Vec4::GetUnscaled(float _scale) const noexcept
    {
        if(_scale == 0)
        {
            Log::Send("getUnscale function: scale is null is impossible to divide by 0", Log::ELogSeverity::ERROR);
            return *this;
        }
        return Vec4(x/_scale, y/_scale, z/_scale, w / _scale);

    }

    Vec4& Vec4::Normalize() noexcept
    {
        float len = Length();
        if(len == 0)
        {
            Log::Send("normalize function: length = 0 impossible to compute");
            return *this;
        }

        x /= len;
        y /= len;
        z /= len;
        w /= len;
        return *this;

    }

    constexpr Vec4 Vec4::GetNormalize() const noexcept
    {
        float len = Length();
        if(len == 0)
        {
            Log::Send("normalize function: length = 0 impossible to compute");
            return *this;
        }
        return Vec4(x/len, y/len,z/len,1);
    }

    constexpr bool Vec4::IsNormalized() const noexcept
    {
        return Maths::Equals<float>(SqrtLength(), 3.f * S_EPSILON);
    }

    constexpr bool Vec4::IsEquals(const Vec4& vec) const noexcept
    {
        return  Solid::Maths::Equals(x,vec.x, 0.0001f) &&
                Solid::Maths::Equals(y,vec.y, 0.0001f) &&
                Solid::Maths::Equals(z,vec.z, 0.0001f) &&
                Solid::Maths::Equals(w,vec.w, 0.0001f) ;
    }

    constexpr float Vec4::Dist(const Vec4 &vec) const noexcept
    {
        return (*this - vec).Length();
    }

    constexpr float Vec4::SqrtDist(const Vec4 &vec) const noexcept
    {
        return (*this - vec).SqrtLength();
    }

    constexpr Vec3 Vec4::ToVector3() const noexcept
    {
        return Vec3(x/w,y/w,z/w);
    }

    const std::string Vec4::ToString() const noexcept
    {
        return  "[" + std::to_string(x) + "," + std::to_string(y) + "," +
                std::to_string(z) + "," + std::to_string(w) + "]";
    }

#pragma endregion
#pragma region Operator

    constexpr bool Vec4::operator==(const Vec4 &_vec) const noexcept
    {
        return IsEquals(_vec);
    }

    constexpr bool Vec4::operator!=(const Vec4 &_vec) const noexcept
    {
        return !IsEquals(_vec);
    }

    constexpr Vec4 Vec4::operator+(const Vec4 &_vec) const noexcept
    {
        return Vec4(x + _vec.x,y + _vec.y, z + _vec.z, w + _vec.w);
    }

    constexpr Vec4 Vec4::operator-(const Vec4 &_vec) const noexcept
    {
        return Vec4(x - _vec.x,y - _vec.y, z - _vec.z, w - _vec.w);
    }

    constexpr Vec4 Vec4::operator*(const Vec4 &_vec) const noexcept
    {
        return Vec4(x * _vec.x,y * _vec.y, z * _vec.z, w * _vec.w);
    }

    constexpr Vec4 Vec4::operator/(const Vec4 &_vec) const noexcept
    {
        return Vec4(x / _vec.x,y / _vec.y, z / _vec.z, w / _vec.w);
    }

    Vec4 &Vec4::operator+=(const Vec4 &_vec) noexcept
    {
        x += _vec.x;
        y += _vec.y;
        z += _vec.z;
        w += _vec.w;
        return *this;
    }

    Vec4 &Vec4::operator-=(const Vec4 &_vec) noexcept
    {
        x -= _vec.x;
        y -= _vec.y;
        z -= _vec.z;
        w -= _vec.w;

        return *this;
    }

    Vec4 &Vec4::operator*=(const Vec4 &_vec) noexcept
    {
        x *= _vec.x;
        y *= _vec.y;
        z *= _vec.z;
        w *= _vec.w;

        return *this;
    }

    Vec4 &Vec4::operator/=(const Vec4 &_vec) noexcept
    {
        x /= _vec.x;
        y /= _vec.y;
        z /= _vec.z;
        w /= _vec.w;

        return *this;
    }

    constexpr Vec4 Vec4::operator+(float _value) const noexcept
    {

        return Vec4(x + _value,y + _value, z + _value, w + _value);
    }

    constexpr Vec4 Vec4::operator-(float _value) const noexcept
    {
        return Vec4(x - _value,y - _value, z - _value, w - _value);
    }

    constexpr Vec4 Vec4::operator*(float _value) const noexcept
    {
        return Vec4(x * _value,y * _value, z *_value, w * _value);
    }

    constexpr Vec4 Vec4::operator/(float _value) const noexcept
    {
        return Vec4(x / _value,y / _value, z / _value, w / _value);
    }

    Vec4 &Vec4::operator+=( float _value) noexcept
    {
        x += _value;
        y += _value;
        z += _value;
        w += _value;
        return *this;
    }

    Vec4 &Vec4::operator-=(float _value) noexcept
    {
        x -= _value;
        y -= _value;
        z -= _value;
        w -= _value;
        return *this;
    }

    Vec4 &Vec4::operator*=(float _value) noexcept
    {
        x *= _value;
        y *= _value;
        z *= _value;
        w *= _value;
        return *this;
    }

    Vec4 &Vec4::operator/=(float _value) noexcept
    {
        x /= _value;
        y /= _value;
        z /= _value;
        w /= _value;
        return *this;
    }

    constexpr Vec4 Vec4::operator-() noexcept
    {
        return Vec4(-x,-y, -z, -w);
    }

    constexpr Vec4 operator * (float _value, const Vec4& _vec) noexcept
    {
        return _vec.GetScaled(_value);
    }

    constexpr Vec4 operator / (float _value, const Vec4& _vec)
    {
        return _vec.GetUnscaled(_value);
    }

#pragma endregion

}
