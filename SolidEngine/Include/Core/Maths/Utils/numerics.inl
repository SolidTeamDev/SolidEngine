#include "numerics.hpp"


#include <cmath>
#include <type_traits>
#include "Core/Debug/log.hpp"

namespace Solid
{
    namespace Maths
    {
        template<typename T>
        constexpr T clamp(T _value, T _min, T _max) noexcept
        {
            if (_min > _max)
            {
                Log::Send("the minimum input is higher than the maximum", Log::ELogSeverity::ERROR);
                return T(0);
            }
            return min(_max, max(_min,_value));
        }

        template<typename T>
        T lerp(T _start, T _end, float _ratio) noexcept
        {
            return (1.f - _ratio) * _start + _ratio *_end;
        }

        template<typename T>
        T slerp(T _start, T _end, float _ratio) noexcept
        {
             float dot = T::dot(_start,_end);
             if(dot < 0.f)
             {
                _end = -_end;
                dot = -dot;
             }
             else if (dot < 1+S_EPSILON && dot > 1-S_EPSILON)
                 return lerp(_start,_end,_ratio);

            float ang = acos(dot);
            float step = ang * _ratio;
            float s = sin(ang);
            float sRatio = sin(step)/s;

            return (cos(step) - dot * sRatio)* _start + sRatio * _end;
        }

        template<typename T>
        constexpr T degToRad(T _value) noexcept
        {
            return _value * S_PI/180;
        }

        template<typename T>
        constexpr T radToDeg(T _value) noexcept
        {
            return _value * 180/S_PI;
        }

        template<typename T>
        constexpr T abs(T _value) noexcept
        {
            if(std::is_unsigned<T>::value)
            {
                Log::Send("the value input is unsigned", Log::ELogSeverity::WARNING);
                return _value;
            }
            return _value > T(0) ? _value : -_value;
        }

        template<typename T>
        constexpr T min(T _a, T _b) noexcept
        {
            return _a > _b ? _b : _a;
        }

        template<typename T>
        constexpr T max(T _a, T _b) noexcept
        {
            return _a > _b ? _a : _b;
        }

        template<typename T>
        constexpr T pow(T _base, T _exp) noexcept
        {
            return std::pow(_base,_exp);
        }

        template<typename T>
        constexpr T pow(T _base, int _exp) noexcept
        {
            return std::pow(_base,_exp);
        }


        template<typename T>
        constexpr T sqrt(T _value) noexcept
        {
            return std::sqrt(_value);
        }


        template<typename T>
        constexpr T cos(T _rad) noexcept
        {
            return std::cos(_rad);
        }

        template<typename T>
        constexpr T acos(T _value) noexcept
        {
            return std::acos(_value);
        }


        template<typename T>
        constexpr T sin(T _rad) noexcept
        {
            return std::sin(_rad);
        }

        template<typename T>
        constexpr T asin(T _value) noexcept
        {
            return std::asin(_value);
        }

        template<typename T>
        constexpr T tan(T _rad) noexcept
        {
            return std::tan(_rad);
        }

        template<typename T>
        constexpr T atan(T _value) noexcept
        {
            return std::atan(_value);
        }

        template<typename T>
        constexpr bool equals(T _a, T _b, float _epsilon)
        {
            return abs(_a - _b) < _epsilon;
        }
    }
}