#include "Core/Maths/Quaternion/quaternion.hpp"

namespace Solid
{
    constexpr Quat::Quat(float _value) noexcept:
    x{_value},
    y{_value},
    z{_value},
    w{_value}
{}

    constexpr Quat::Quat(float _x,float _y, float _z, float _w)noexcept:
     x{_x},
     y{_y},
     z{_z},
     w{_w}
    {}

}
