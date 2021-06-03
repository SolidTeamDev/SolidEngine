#include <iostream>
#include "Core/Maths/Quaternion/quaternion.hpp"
#include "Core/Maths/Vector/vector3.hpp"
#include "Core/Debug/debug.hpp"
#include "Core/Maths/Utils/numerics.hpp"


namespace Solid
{
#pragma region Constructors

    Quat::Quat(float _value) noexcept:
            x{_value},
            y{_value},
            z{_value},
            w{_value}
    {}

    Quat::Quat(float _x, float _y, float _z, float _w) noexcept:
            x{_x},
            y{_y},
            z{_z},
            w{_w}
    {}

    Quat::Quat(const Vec3 &_angles) noexcept :
    x{0},
    y{0},
    z{0},
    w{0}
    {

        Vec3 half = _angles * 0.5;

        float cosPitch = Maths::Cos(half.y);
        float sinPitch = Maths::Sin(half.y);

        float cosYaw = Maths::Cos(half.x);
        float sinYaw = Maths::Sin(half.x);

        float cosRoll = Maths::Cos(half.z);
        float sinRoll = Maths::Sin(half.z);

        w = cosPitch * cosYaw * cosRoll + sinPitch * sinYaw * sinRoll;
        x = sinYaw * cosRoll * cosPitch - cosYaw * sinRoll * sinPitch;
        y = cosYaw * cosRoll * sinPitch + sinYaw * sinRoll * cosPitch;
        z = cosYaw * sinRoll * cosPitch - sinYaw * cosRoll * sinPitch;

    }

    Quat::Quat(float _angle, const Vec3& _axis) noexcept
    {
        float _angleRad = Maths::DegToRad(_angle)*0.5f;

        Vec3 vn = _axis;
        float sinAngle = sin(_angleRad);

        if(!_axis.IsNormalized())
            vn = _axis.GetNormalized();

        w = Maths::Cos(_angleRad);
        x = vn.x * sinAngle;
        y = vn.y * sinAngle;
        z = vn.z * sinAngle;
    }
#pragma endregion
#pragma region Static Methods

    Quat Quat::Zero{0, 0, 0, 0};

    Quat Quat::Identity{0, 0, 0, 1};

    float Quat::Dot(const Quat &_q1, const Quat &_q2) noexcept
    {
        return (_q1.w * _q2.w) +
               (_q1.x * _q2.x) +
               (_q1.y * _q2.y) +
               (_q1.z * _q2.z);
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
        return Maths::Slerp<Quat>(_q1, _q2, _r).GetNormalized();
    }

#pragma endregion
#pragma region Methods

    bool Quat::IsZero() const noexcept
    {

        return Maths::Equals0(x) && Maths::Equals0(y)
               && Maths::Equals0(z) && Maths::Equals0(w);
    }

    float Quat::SqrtLength() const noexcept
    {
        return (x * x + y * y + z * z + w * w);
    }

    float Quat::Length() const noexcept
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

    Quat Quat::GetScaled(float _scale) const noexcept
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

    Quat Quat::GetUnscaled(float _scale) const noexcept
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

    Quat Quat::GetNormalized() const noexcept
    {
        float len = Length();
        if (len == 0)
        {
            Log::Send("normalize function: length = 0 impossible to compute");
            return *this;
        }
        return Quat(x / len, y / len, z / len, w / len);
    }

    Quat& Quat::Inverse()
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

    Quat Quat::GetInversed() const
    {
        if (IsNormalized())
        {
            Log::Send("GetInversed function: Quaternion must be normalized");
            return *this;
        }

        return Quat(-x, -y, -z, w);
    }

    bool Quat::IsNormalized() const noexcept
    {
        return Maths::Equals<float>(SqrtLength(), 3.f * S_EPSILON);
    }


    bool Quat::IsEquals(const Quat& _quat) const noexcept
    {
        return Solid::Maths::Equals(x, _quat.x, 0.0001f) &&
               Solid::Maths::Equals(y, _quat.y,  0.0001f) &&
               Solid::Maths::Equals(z, _quat.z,  0.0001f) &&
               Solid::Maths::Equals(w, _quat.w,  0.0001f);
    }

    Vec3 Quat::Rotate(const Vec3 &_vec) const
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

    Quat Quat::Rotate(const Quat &_quat) const
    {
        if(IsNormalized() || _quat.IsNormalized())
        {
            Log::Send("Rotate function: Quaternion must be normalized",Log::ELogSeverity::WARNING);
            return *this;
        }

        return Quat(w * _quat.x + x * _quat.w + y * _quat.z - z * _quat.y,
                    w * _quat.y - x * _quat.z + y * _quat.w + z * _quat.x,
                    w * _quat.z + x * _quat.y - y * _quat.x + z * _quat.w,
                    w * _quat.w - x * _quat.x - y * _quat.y - z * _quat.z);
    }

    Vec3 Quat::Unrotate(const Vec3 &_vec) const
    {
        if(IsNormalized())
        {
            Log::Send("Rotate function: Quaternion must be normalized");
            return Vec3::Zero;
        }

        return GetInversed().Rotate(_vec);
    }

    Quat Quat::Unrotate(const Quat &_quat) const
    {
        if(IsNormalized() || _quat.IsNormalized())
        {
            Log::Send("Rotate function: Quaternion must be normalized");
            return *this;
        }

        return GetInversed().Rotate(_quat);
    }


    /// TO Euler Convention is ZYX
    Vec3 Quat::ToEuler() const
    {

        Vec3 eulerAngle;
        const float SINGULARITY_THRESHOLD = 0.4999995f;
        const float SingularityTest = ( w * y - z * x );
        const float YawY = 2.f*(w*z+x*y);
        const float Yawx = (1.f-2.f*(y*y + z*z));

        // roll (z-axis rotation)
        eulerAngle.z = Maths::Atan2(YawY, Yawx);
        // pitch (x-axis rotation)
        // yaw (y-axis rotation)

        if (SingularityTest < -SINGULARITY_THRESHOLD)
        {
            eulerAngle.x = -eulerAngle.z - (2.f * Maths::Atan2(x, w));
            eulerAngle.y = Maths::DegToRad(-90.f);
        }
        else if(SingularityTest > SINGULARITY_THRESHOLD)
        {
            eulerAngle.x =eulerAngle.z - (2.f * Maths::Atan2(x, w));
            eulerAngle.y = Maths::DegToRad(90.f);
        }
        else
        {
            eulerAngle.x = Maths::Atan2(2.f*(w*x+y*z), (1.f-2.f*(x*x + y*y)));
            eulerAngle.y = Maths::Asin(2.0 * SingularityTest);
        }

        return eulerAngle;
    }

    Quat Quat::FromEuler(const Vec3& _angles) const
    {
        Vec3 half = _angles * 05;

        float cosPitch = Maths::Cos(half.x);
        float cosYaw = Maths::Cos(half.y);
        float cosRoll = Maths::Cos(half.z);
        float sinPitch = Maths::Sin(half.x);
        float sinYaw = Maths::Sin(half.y);
        float sinRoll = Maths::Sin(half.z);

        Quat result;
        result.w = cosPitch * cosYaw * cosRoll - sinPitch * sinYaw * sinRoll;
        result.x = sinPitch * cosYaw * cosRoll + cosPitch * sinYaw * sinRoll;
        result.y = cosPitch * sinYaw * cosRoll - sinPitch * cosYaw * sinRoll;
        result.z = cosPitch * cosYaw * sinRoll + sinPitch * sinYaw * cosRoll;

        return result;
    }
    const std::string Quat::ToString() const noexcept
    {
        return "[" + std::to_string(x) + "," + std::to_string(y) + "," +
               std::to_string(z) + "," + std::to_string(w) + "]";
    }

#pragma endregion
#pragma region Operator

    Quat Quat::operator*(float _scale) const noexcept
    {
        return GetScaled(_scale);
    }

    Quat Quat::operator/(float _scale) const noexcept
    {
        return GetUnscaled(_scale);
    }

    Quat Quat::operator+(float _scale) const noexcept
    {
        return Quat(x + _scale, y + _scale, z + _scale, w + _scale);
    }

    Quat Quat::operator-(float _scale) const noexcept
    {
        return Quat(x - _scale, y - _scale, z - _scale, w - _scale);

    }

    Quat Quat::operator*(const Quat &_quat) const noexcept
    {
        return Rotate(_quat);
    }

    Vec3 Quat::operator*(const Vec3 &_vec) const noexcept
    {
        return Rotate(_vec);
    }

    Quat Quat::operator/(const Quat &_quat) const noexcept
    {
        return Unrotate(_quat);

    }

    Vec3 Quat::operator/(const Vec3 &_vec) const noexcept
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

    bool Quat::operator==(const Quat &_quat)
    {
        return IsEquals(_quat);
    }

    bool Quat::operator!=(const Quat &_quat)
    {
        return !IsEquals(_quat);

    }

    Quat Quat::operator-() const noexcept
    {
        return Quat(-x, -y, -z, -w);
    }
    Quat operator * (float _value, const Quat& _quat) noexcept
    {
        return _quat * _value;
    }
    Quat operator / (float _value, const Quat& _quat)
    {
        return Quat(_value/_quat.x,
                    _value/_quat.y,
                    _value/_quat.z,
                    _value/_quat.w);
    }

    Quat Quat::operator+(const Quat &_quat) const noexcept
    {
        return Quat(x + _quat.x,
                    y + _quat.y,
                    z + _quat.z,
                    w + _quat.w);
    }

    Quat Quat::operator-(const Quat &_quat) const noexcept
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
