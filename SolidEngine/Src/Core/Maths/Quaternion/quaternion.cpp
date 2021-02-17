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

    constexpr Quat::Quat(float _x, float _y = 0, float _z = 0, float _w = 1) noexcept:
            x{_x},
            y{_y},
            z{_z},
            w{_w}
    {}

#pragma region Static Methods

    Quat const Quat::Zero{0, 0, 0, 0};

    Quat const Quat::Identity{1, 0, 0, 0};

    constexpr float Quat::Dot(const Quat &_q1, const Quat &_q2)
    {
        return _q1.w * _q1.w +
               _q1.x * _q1.x +
               _q1.y * _q1.y +
               _q1.z * _q1.z;
    }

    Quat Quat::Lerp(const Quat &_q1, const Quat &_q2, float _r) noexcept
    {
        return Maths::Lerp(_q1, _q2, _r);
    }

    Quat Quat::Nlerp(const Quat &_q1, const Quat &_q2, float _r) noexcept
    {
        return Maths::Lerp(_q1, _q2, _r).GetNormalize();
    }

    Quat Quat::Slerp(const Quat &_q1, const Quat &_q2, float _r) noexcept
    {
        return Maths::Slerp(_q1, _q2, _r);
    }

#pragma endregion
#pragma region Methods

    constexpr bool Quat::IsZero() const noexcept
    {
        return Solid::Maths::Equals0(x) && Solid::Maths::Equals0(y)
               && Solid::Maths::Equals0(z) && Solid::Maths::Equals0(w);
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
        return Vec4(x * _scale, y * _scale, z * _scale, w * _scale);
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
        return Vec4(x / _scale, y / _scale, z / _scale, w / _scale);

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

    constexpr Quat Quat::GetNormalize() const noexcept
    {
        float len = Length();
        if (len == 0)
        {
            Log::Send("normalize function: length = 0 impossible to compute");
            return *this;
        }
        return Vec4(x / len, y / len, z / len);
    }

    constexpr Quat Quat::Inverse()
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
        return Maths::Equals(SqrtLength(), 3.f * S_EPSILON);
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
            return *this;
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
            return *this;
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

    std::string ToString() noexcept
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
        return Quat(x + _scale, y + _scale, z + _scale, w + _scale)
    }

    constexpr Quat Quat::operator-(float _scale) const noexcept
    {
        return Quat(x - _scale, y - _scale, z - _scale, w - _scale)

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
        return Maths::Equals(*this, _quat);
    }

    constexpr bool Quat::operator!=(const Quat &_quat)
    {
        return !Maths::Equals(*this, _quat);

    }

    constexpr Quat Quat::operator-() const noexcept
    {
        return Quat(-x, -y, -z, -w);
    }

#pragma endregion
}
