#include "Core/Maths/Vector/vector2.hpp"
#include "Core/Maths/Vector/vector3.hpp"
#include "Core/Maths/Vector/vector4.hpp"
#include "Core/Maths/Quaternion/quaternion.hpp"
#include "Core/Maths/Utils/numerics.hpp"
#include "Core/Debug/log.hpp"
namespace Solid
{

#pragma region Constructor

    Vec3::Vec3(float _value)noexcept :
    x{_value},
    y{_value},
    z{_value}
    {}

    Vec3::Vec3(float _x,float _y, float _z)noexcept:
    x{_x},
    y{_y},
    z{_z}
    {}

    Vec3::Vec3(const Vec4& _copy)noexcept:
    x{_copy.x},
    y{_copy.y},
    z{_copy.z}
    {}

    Vec3::Vec3(const Vec3& _copy)noexcept:
    x{_copy.x},
    y{_copy.y},
    z{_copy.z}
    {}

    Vec3::Vec3(const Quat& _q) noexcept
    {
        *this =  _q.ToEuler();
    }

#pragma endregion
#pragma region Static Methods

    Vec3 const Vec3::Zero{0,0,0};
    Vec3 const Vec3::One{1,1,1};
    Vec3 const Vec3::Up{0,1,0};
    Vec3 const Vec3::Down{0,-1,0};
    Vec3 const Vec3::Left{-1,0,0};
    Vec3 const Vec3::Right{1,0,0};
    Vec3 const Vec3::Forward{1,0,1};
    Vec3 const Vec3::Back{0,0,-1};

    float Vec3::Dot(const Vec3& _v1,const Vec3 &_v2) noexcept
    {

        return _v1.x * _v2.x +
               _v1.y * _v2.y +
               _v1.z * _v2.z;
    }

    Vec3 Vec3::Cross(const Vec3& _v1,const Vec3 &_v2) noexcept
    {
        return Vec3(_v1.y * _v2.z - _v1.z * _v2.y,
                _v1.z * _v2.x - _v1.x * _v2.z,
                _v1.x * _v2.y - _v1.y * _v2.x);
    }

    Vec3 Vec3::Lerp(const Vec3 &_v1, const Vec3 &_v2, float _r) noexcept
    {
        return Maths::Lerp(_v1,_v2, _r);
    }

    Vec3 Vec3::Nlerp(const Vec3 &_v1, const Vec3 &_v2, float _r) noexcept
    {
        return Maths::Lerp(_v1,_v2,_r).GetNormalized();
    }

    Vec3 Vec3::Slerp(const Vec3 &_v1, const Vec3 &_v2, float _r) noexcept
    {
        return Maths::Slerp(_v1,_v2, _r);
    }

#pragma endregion
#pragma region Methods

    float Vec3::SqrtLength() const noexcept
    {
        return x * x + y *  + z * z;
    }

    float Vec3::Length() const noexcept
    {
        return Maths::Sqrt(x * x + y * y + z * z);
    }

    Vec3& Vec3::Scale(float _scale) noexcept
    {
        x *= _scale;
        y *= _scale;
        z *= _scale;
        return *this;

    }

    Vec3 Vec3::GetScaled(float _scale) const noexcept
    {

        return Vec3(x*_scale, y*_scale, z * _scale);

    }

    Vec3& Vec3::Unscale(float _scale) noexcept
    {
        if(_scale == 0)
        {
            Log::Send("unscale function: scale is null is impossible to divide by 0", Log::ELogSeverity::ERROR);
            return *this;
        }
        x /= _scale;
        y /= _scale;
        z /= _scale;
        return *this;
    }

    Vec3 Vec3::GetUnscaled(float _scale) const noexcept
    {
        if(_scale == 0)
        {
            Log::Send("getUnscale function: scale is null is impossible to divide by 0", Log::ELogSeverity::ERROR);
            return *this;
        }
        return Vec3(x/_scale, y/_scale, z/_scale);

    }

    Vec3& Vec3::Normalize() noexcept
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
        return *this;

    }

    Vec3 Vec3::GetNormalized() const noexcept
    {
        float len = Length();
        if(len == 0)
        {
            Log::Send("normalize function: length = 0 impossible to compute");
            return *this;
        }
        return Vec3(x/len, y/len,z/len);
    }

    bool Vec3::IsNormalized() const noexcept
    {
        return Maths::Equals<float>(SqrtLength(), 3.f * S_EPSILON);
    }

    bool Vec3::IsEquals(const Vec3& vec) const noexcept
    {
        return  Solid::Maths::Equals(x,vec.x, 0.01f) &&
                Solid::Maths::Equals(y,vec.y, 0.01f) &&
                Solid::Maths::Equals(z,vec.z, 0.01f);
    }

    float Vec3::Dist(const Vec3 &vec) const noexcept
    {
        return (*this - vec).Length();
    }

    float Vec3::SqrtDist(const Vec3 &vec) const noexcept
    {
        return (*this - vec).SqrtLength();
    }

    const std::string Vec3::ToString() const noexcept
    {
        //return "[" + std::to_string(Maths::RadToDeg(x)) + "," + std::to_string(Maths::RadToDeg(y)) + "," + std::to_string(Maths::RadToDeg(z)) + "]";
        return "[" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "]";

    }

#pragma endregion
#pragma region Operator

    bool Vec3::operator==(const Vec3 &_vec) const noexcept
    {
        return IsEquals(_vec);
    }

    bool Vec3::operator!=(const Vec3 &_vec) const noexcept
    {
        return !IsEquals(_vec);
    }

    Vec3 Vec3::operator+(const Vec3 &_vec) const noexcept
    {
        return Vec3(x + _vec.x,y + _vec.y, z + _vec.z);
    }

    Vec3 Vec3::operator-(const Vec3 &_vec) const noexcept
    {
        return Vec3(x - _vec.x,y - _vec.y, z - _vec.z);
    }

    Vec3 Vec3::operator*(const Vec3 &_vec) const noexcept
    {
        return Vec3(x * _vec.x,y * _vec.y, z * _vec.z);
    }

    Vec3 Vec3::operator/(const Vec3 &_vec) const noexcept
    {
        return Vec3(x / _vec.x,y / _vec.y, z / _vec.z);
    }

    Vec3 &Vec3::operator+=(const Vec3 &_vec) noexcept
    {
        x += _vec.x;
        y += _vec.y;
        z += _vec.z;
        return *this;
    }

    Vec3 &Vec3::operator-=(const Vec3 &_vec) noexcept
    {
        x -= _vec.x;
        y -= _vec.y;
        z -= _vec.z;
        return *this;
    }

    Vec3 &Vec3::operator*=(const Vec3 &_vec) noexcept
    {
        x *= _vec.x;
        y *= _vec.y;
        z *= _vec.z;
        return *this;
    }

    Vec3 &Vec3::operator/=(const Vec3 &_vec) noexcept
    {
        x /= _vec.x;
        y /= _vec.y;
        z /= _vec.z;
        return *this;
    }

    Vec3 Vec3::operator+(float _value) const noexcept
    {

        return Vec3(x + _value,y + _value, z + _value);
    }

    Vec3 Vec3::operator-(float _value) const noexcept
    {
        return Vec3(x - _value,y - _value, z - _value);
    }

    Vec3 Vec3::operator*(float _value) const noexcept
    {
        return Vec3(x * _value,y * _value, z *_value);
    }

    Vec3 Vec3::operator/(float _value) const noexcept
    {
        return Vec3(x / _value,y / _value, z / _value);
    }

    Vec3 &Vec3::operator+=( float _value) noexcept
    {
        x += _value;
        y += _value;
        z += _value;
        return *this;
    }

    Vec3 &Vec3::operator-=(float _value) noexcept
    {
        x -= _value;
        y -= _value;
        z -= _value;
        return *this;
    }

    Vec3 &Vec3::operator*=(float _value) noexcept
    {
        x *= _value;
        y *= _value;
        z *= _value;
        return *this;
    }

    Vec3 &Vec3::operator/=(float _value) noexcept
    {
        x /= _value;
        y /= _value;
        z /= _value;
        return *this;
    }

    Vec3 Vec3::operator-() noexcept
    {
        return Vec3(-x,-y, -z);
    }

    Vec3 operator * (float _value, const Vec3& _vec) noexcept
    {
        return _vec.GetScaled(_value);
    }

    Vec3 operator / (float _value, const Vec3& _vec)
    {
        return _vec.GetUnscaled(_value);
    }

#pragma endregion
}
