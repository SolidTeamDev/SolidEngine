#include "Core/Maths/Vector/vector2.hpp"
#include "Core/Maths/Vector/vector3.hpp"
#include "Core/Maths/Vector/vector4.hpp"

namespace Solid
{

    constexpr Vec4::Vec4(const float& _value)noexcept :
    x{_value},
    y{_value},
    z{_value},
    w{1}
    {}

    constexpr Vec4::Vec4(const float& _x,const float& _y, const float& _z, const float& _w)noexcept:
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

}
