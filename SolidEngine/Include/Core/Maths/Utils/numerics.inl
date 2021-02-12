#include "numerics.hpp"


#include <cmath>
#include <type_traits>
#include "Core/Debug/log.hpp"

namespace Solid
{
    namespace Maths
    {
        template<typename T>
        constexpr T Clamp(T _value, T _min, T _max) noexcept
        {
            if (_min > _max)
            {
                Log::Send("the minimum input is higher than the maximum", Log::ELogSeverity::ERROR);
                return T(0);
            }
            return Min(_max, Max(_min,_value));
        }

        template<typename T>
        T Lerp(T _start, T _end, float _ratio) noexcept
        {
            return (1.f - _ratio) * _start + _ratio *_end;
        }

        template<typename T>
        T Slerp(T _start, T _end, float _ratio) noexcept
        {
             float dot = T::Dot(_start,_end);
             if(dot < 0.f)
             {
                _end = -_end;
                dot = -dot;
             }
             else if (dot < 1+S_EPSILON && dot > 1-S_EPSILON)
                 return Lerp(_start,_end,_ratio);

            float ang = Acos(dot);
            float step = ang * _ratio;
            float s = Sin(ang);
            float sRatio = Sin(step)/s;

            return (Cos(step) - dot * sRatio)* _start + sRatio * _end;
        }

        template<typename T>
        constexpr T DegToRad(T _value) noexcept
        {
            return _value * S_PI/180;
        }

        template<typename T>
        constexpr T RadToDeg(T _value) noexcept
        {
            return _value * 180/S_PI;
        }

        template<typename T>
        constexpr T Abs(T _value) noexcept
        {
            if(std::is_unsigned<T>::value)
            {
                Log::Send("the value input is unsigned", Log::ELogSeverity::WARNING);
                return _value;
            }
            return _value > T(0) ? _value : -_value;
        }

        template<typename T>
        constexpr T Min(T _a, T _b) noexcept
        {
            return _a > _b ? _b : _a;
        }

        template<typename T>
        constexpr T Max(T _a, T _b) noexcept
        {
            return _a > _b ? _a : _b;
        }

        template<typename T>
        constexpr T Pow(T _base, T _exp) noexcept
        {
            return std::pow(_base,_exp);
        }

        template<typename T>
        constexpr T Pow(T _base, int _exp) noexcept
        {
            return std::pow(_base,_exp);
        }


        template<typename T>
        constexpr T Sqrt(T _value) noexcept
        {
            return std::sqrt(_value);
        }


        template<typename T>
        constexpr T Cos(T _rad) noexcept
        {
            return std::cos(_rad);
        }

        template<typename T>
        constexpr T Acos(T _value) noexcept
        {
            return std::acos(_value);
        }


        template<typename T>
        constexpr T Sin(T _rad) noexcept
        {
            return std::sin(_rad);
        }

        template<typename T>
        constexpr T Asin(T _value) noexcept
        {
            return std::asin(_value);
        }

        template<typename T>
        constexpr T Tan(T _rad) noexcept
        {
            return std::tan(_rad);
        }

        template<typename T>
        constexpr T Atan(T _value) noexcept
        {
            return std::atan(_value);
        }

        template<typename T>
        constexpr bool Equals(T _a, T _b, float _epsilon)
        {
            return Abs(_a - _b) < _epsilon;
        }
    }
}