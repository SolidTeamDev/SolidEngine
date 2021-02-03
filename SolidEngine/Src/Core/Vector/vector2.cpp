#include "Core/Maths//Vector/vector2.hpp"
#include "Core/Maths//Vector/vector3.hpp"
#include "Core/Maths//Vector/vector4.hpp"


namespace Solid
{
    constexpr Vec2::Vec2(const float& _value) noexcept :
    x{_value},
    y{_value}
    {}

    constexpr Vec2::Vec2(const float& _x,const float& _y) noexcept :
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


}