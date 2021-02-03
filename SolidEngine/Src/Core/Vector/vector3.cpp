#include "Core/Maths//Vector/vector2.hpp"
#include "Core/Maths//Vector/vector3.hpp"
#include "Core/Maths//Vector/vector4.hpp"

namespace Solid
{

    constexpr Vec3::Vec3(const float& _value)noexcept :
    x{_value},
    y{_value},
    z{_value}
    {}

    constexpr Vec3::Vec3(const float& _x,const float& _y, const float& _z)noexcept:
    x{_x},
    y{_y},
    z{_z}
    {}

    constexpr Vec3::Vec3(const Vec2& _copy)noexcept:
    x{_copy.x},
    y{_copy.y},
    z{0}
    {}

    constexpr Vec3::Vec3(const Vec3& _copy)noexcept:
    x{_copy.x},
    y{_copy.y},
    z{_copy.z}
    {}

    constexpr Vec3::Vec3(const Vec4& _copy)noexcept:
    x{_copy.x},
    y{_copy.y},
    z{_copy.z}
    {}

}
