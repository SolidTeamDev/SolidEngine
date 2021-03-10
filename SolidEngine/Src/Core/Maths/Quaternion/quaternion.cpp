#include "Core/Maths/Quaternion/quaternion.hpp"
#include "Core/Maths/Vector/vector3.hpp"
#include "Core/Debug/debug.hpp"
#include "Core/Maths/Utils/numerics.hpp"

namespace Solid
{
    constexpr Quat::Quat(float _value) noexcept:
            x{_value},
            y{_value},
            z{_value},
            w{_value}
    {}

    constexpr Quat::Quat(float _x, float _y, float _z, float _w) noexcept:
            x{_x},
            y{_y},
            z{_z},
            w{_w}
    {}

    constexpr Quat::Quat(const Vec3 &_axis) noexcept :
    x{0},
    y{0},
    z{0},
    w{0}
    {
        Vec3 half = _axis * 05;

        float cosX = Maths::Cos(half.x);
        float cosY = Maths::Cos(half.y);
        float cosZ = Maths::Cos(half.z);
        float sinX = Maths::Sin(half.x);
        float sinY = Maths::Sin(half.y);
        float sinZ = Maths::Sin(half.z);

        w = cosX * cosY * cosZ - sinX * sinY * sinZ;
        x = sinX * cosY * cosZ + cosX * sinY * sinZ;
        y = cosX * sinY * cosZ - sinX * cosY * sinZ;
        z = cosX * cosY * sinZ + sinX * sinY * cosZ;


    }

#pragma region Static Methods

    Quat Quat::Zero{0, 0, 0, 0};

    Quat Quat::Identity{1, 0, 0, 0};

    constexpr float Quat::Dot(const Quat &_q1, const Quat &_q2) noexcept
    {
        return _q1.w * _q1.w +
               _q1.x * _q1.x +
               _q1.y * _q1.y +
               _q1.z * _q1.z;
    }

    Quat Quat::Lerp(const Quat &_q1, const Quat &_q2, float _r) noexcept
    {
        return Maths::Lerp<Quat>(_q1, _q2, _r);
    }

    Quat Quat::Nlerp(const Quat &_q1, const Quat &_q2, float _r) noexcept
    {
        return Maths::Lerp<Quat>(_q1, _q2, _r).GetNormalized();
    }

    Quat Quat::Slerp(const Quat &_q1, const Quat &_q2, float _r) noexcept
    {
        return Maths::Slerp<Quat>(_q1, _q2, _r);
    }

#pragma endregion
#pragma region Methods

    constexpr bool Quat::IsZero() const noexcept
    {

        return Maths::Equals0(x) && Maths::Equals0(y)
               && Maths::Equals0(z) && Maths::Equals0(w);
    }

    constexpr float Quat::SqrtLength() const noexcept
    {
        return (x * x + y * y + z * z + w * w);
    }

    constexpr float Quat::Length() const noexcept
    {
        return Maths::Sqrt(x * x + y * y + z * z + w * w);
    }

    Quat &Quat::Scale(float _scale) noexcept
    {
        x *= _scale;
        y *= _scale;
        z *= _scale;
        w *= _scale;
        return *this;
    }

    constexpr Quat Quat::GetScaled(float _scale) const noexcept
    {
        return Quat(x * _scale, y * _scale, z * _scale, w * _scale);
    }

    Quat &Quat::Unscale(float _scale) noexcept
    {
        if (_scale == 0)
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

    constexpr Quat Quat::GetUnscaled(float _scale) const noexcept
    {
        if (_scale == 0)
        {
            Log::Send("getUnscale function: scale is null is impossible to divide by 0", Log::ELogSeverity::ERROR);
            return *this;
        }
        return Quat(x / _scale, y / _scale, z / _scale, w / _scale);

    }

    Quat &Quat::Normalize() noexcept
    {
        float len = Length();
        if (len == 0)
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

    constexpr Quat Quat::GetNormalized() const noexcept
    {
        float len = Length();
        if (len == 0)
        {
            Log::Send("normalize function: length = 0 impossible to compute");
            return *this;
        }
        return Quat(x / len, y / len, z / len, w / len);
    }

    constexpr Quat& Quat::Inverse()
    {
        if (IsNormalized())
        {
            Log::Send("GetInversed function: Quaternion must be normalized");
            return *this;
        }
        x = -x;
        y = -y;
        z = -z;

        return *this;
    }

    constexpr Quat Quat::GetInversed() const
    {
        if (IsNormalized())
        {
            Log::Send("GetInversed function: Quaternion must be normalized");
            return *this;
        }

        return Quat(-x, -y, -z, w);
    }

    constexpr bool Quat::IsNormalized() const noexcept
    {
        return Maths::Equals<float>(SqrtLength(), 3.f * S_EPSILON);
    }


    constexpr bool Quat::IsEquals(const Quat &_quat) const noexcept
    {
        return Solid::Maths::Equals(x, _quat.x) &&
               Solid::Maths::Equals(y, _quat.y) &&
               Solid::Maths::Equals(z, _quat.z) &&
               Solid::Maths::Equals(w, _quat.w);
    }

    constexpr Vec3 Quat::Rotate(const Vec3 &_vec) const
    {
        if(IsNormalized())
        {
            Log::Send("Rotate function: Quaternion must be normalized");
            return Vec3::Zero;
        }

        Vec3 v1 = Vec3(x,y,z);

        Vec3 v2 = 2.f * Vec3::Cross(v1, _vec);

        return _vec + (w * v2) + Vec3::Cross(v1,v2);
    }

    constexpr Quat Quat::Rotate(const Quat &_quat) const
    {
        if(IsNormalized() || _quat.IsNormalized())
        {
            Log::Send("Rotate function: Quaternion must be normalized");
            return *this;
        }

        return Quat(w * _quat.w - x * _quat.x - y * _quat.y - z * _quat.z,
                    w * _quat.x + x * _quat.w + y * _quat.z - z * _quat.y,
                    w * _quat.y - x * _quat.z + y * _quat.w + z * _quat.x,
                    w * _quat.z + x * _quat.y - y * _quat.x + z * _quat.w);
    }

    constexpr Vec3 Quat::Unrotate(const Vec3 &_vec) const
    {
        if(IsNormalized())
        {
            Log::Send("Rotate function: Quaternion must be normalized");
            return Vec3::Zero;
        }

        return GetInversed().Rotate(_vec);
    }

    constexpr Quat Quat::Unrotate(const Quat &_quat) const
    {
        if(IsNormalized() || _quat.IsNormalized())
        {
            Log::Send("Rotate function: Quaternion must be normalized");
            return *this;
        }

        return GetInversed().Rotate(_quat);
    }

    Vec3 Quat::ToEuler() const
    {
        Vec3 eulerAngle;

        // roll (x-axis rotation)
        double sinr_cosp = 2 * (w * x + y * z);
        double cosr_cosp = 1 - 2 * (x * x + y * y);
        eulerAngle.x = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        double sinp = 2 * (w * y - z * x);
        if (std::abs(sinp) >= 1)
            eulerAngle.y = std::copysign(S_PI / 2, sinp); // use 90 degrees if out of range
        else
            eulerAngle.y = std::asin(sinp);

        // yaw (z-axis rotation)
        double siny_cosp = 2 * (w * z + x * y);
        double cosy_cosp = 1 - 2 * (y * y + z * z);
        eulerAngle.z = std::atan2(siny_cosp, cosy_cosp);

        return eulerAngle;
    }

    std::string Quat::ToString() noexcept
    {
        return "[" + std::to_string(x) + "," + std::to_string(y) + "," +
               std::to_string(z) + "," + std::to_string(w) + "]";
    }

#pragma endregion
#pragma region Operator

    constexpr Quat Quat::operator*(float _scale) const noexcept
    {
        return GetScaled(_scale);
    }

    constexpr Quat Quat::operator/(float _scale) const noexcept
    {
        return GetUnscaled(_scale);
    }

    constexpr Quat Quat::operator+(float _scale) const noexcept
    {
        return Quat(x + _scale, y + _scale, z + _scale, w + _scale);
    }

    constexpr Quat Quat::operator-(float _scale) const noexcept
    {
        return Quat(x - _scale, y - _scale, z - _scale, w - _scale);

    }

    constexpr Quat Quat::operator*(const Quat &_quat) const noexcept
    {
        return Rotate(_quat);
    }

    constexpr Vec3 Quat::operator*(const Vec3 &_vec) const noexcept
    {
        return Rotate(_vec);
    }

    constexpr Quat Quat::operator/(const Quat &_quat) const noexcept
    {
        return Unrotate(_quat);

    }

    constexpr Vec3 Quat::operator/(const Vec3 &_vec) const noexcept
    {
        return Unrotate(_vec);

    }

    Quat &Quat::operator*=(float _scale)
    {
        Scale(_scale);
        return *this;

    }

    Quat &Quat::operator/=(float _scale)
    {
        Unscale(_scale);
        return *this;
    }

    Quat &Quat::operator*=(const Quat &_quat)
    {
        return *this = *this * _quat;
    }

    Quat &Quat::operator/=(const Quat &_quat)
    {
        return *this = *this * _quat;
    }

    Quat &Quat::operator+=(const Quat &_quat)
    {
        x += _quat.x;
        y += _quat.y;
        z += _quat.z;
        w += _quat.w;

        return *this;

    }

    Quat &Quat::operator-=(const Quat &_quat)
    {
        x -= _quat.x;
        y -= _quat.y;
        z -= _quat.z;
        w -= _quat.w;

        return *this;
    }

    constexpr bool Quat::operator==(const Quat &_quat)
    {
        return IsEquals(_quat);
    }

    constexpr bool Quat::operator!=(const Quat &_quat)
    {
        return !IsEquals(_quat);

    }

    constexpr Quat Quat::operator-() const noexcept
    {
        return Quat(-x, -y, -z, -w);
    }
    constexpr Quat operator * (float _value, const Quat& _quat) noexcept
    {
        return _quat * _value;
    }
    constexpr Quat operator / (float _value, const Quat& _quat)
    {
        return Quat(_value/_quat.x,
                    _value/_quat.y,
                    _value/_quat.z,
                    _value/_quat.w);
    }

    constexpr Quat Quat::operator+(const Quat &_quat) const noexcept
    {
        return Quat(x + _quat.x,
                    y + _quat.y,
                    z + _quat.z,
                    w + _quat.w);
    }

    constexpr Quat Quat::operator-(const Quat &_quat) const noexcept
    {
        return Quat(x - _quat.x,
                    y - _quat.y,
                    z - _quat.z,
                    w - _quat.w);
    }

    Quat &Quat::operator+=(float _scale)
    {
        x += _scale;
        y += _scale;
        z += _scale;
        w += _scale;

        return *this;

    }

    Quat &Quat::operator-=(float _scale)
    {
        x -= _scale;
        y -= _scale;
        z -= _scale;
        w -= _scale;

        return *this;
    }

#pragma endregion
}
