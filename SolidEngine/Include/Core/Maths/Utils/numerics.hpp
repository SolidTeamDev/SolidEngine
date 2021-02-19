#pragma once
#include "Build/SolidAPI.hpp"
#include "constants.hpp"

namespace Solid
{
    namespace Maths
    {
        /**
        * @brief Clamp the value between min and max
        * @tparam T Type of value
        * @param _value Value to clamp
        * @param _min Minimum value
        * @param _max Maximum value
        * @return Clamped value
        */
        template<typename T>
        constexpr T Clamp(T _value, T _min, T _max) noexcept;

        /**
        * @brief lerp from _start to _end at time.
        * @tparam T Type of value
        * @param _start Current value
        * @param _end Destination value
        * @param _ratio Ration value (Between 0 and 1)
        * @return interpolation between _start and _end
        */
        template<typename T>
        T Lerp(const T& _start, const T& _end, float _ratio) noexcept;

        /**
         * @brief  slerp from _start to _end at time.
         * @tparam T Type of value
         * @param _start Current value
         * @param _end Destination value
         * @param _ratio Ration value (Between 0 and 1)
         * @return interpolation between _start and _end
         */
        template<typename T>
        T Slerp(const T& _start, T _end, float _ratio) noexcept;

        /**
         * @brief Convert degrees value into radiant
         * @tparam T Type of data
         * @param _value Value to convert
         * @return value converted in radiant
         */
        template<typename T>
        constexpr T DegToRad(T _value) noexcept;

        /**
         * @brief Convert radiant value into degrees
         * @tparam T Type of data
         * @param _value Value to convert
         * @return value  converted in degrees
         */
        template<typename T>
        constexpr T RadToDeg(T _value) noexcept;

        /**
         * @brief compute the absolute value
         * @tparam T Type of value
         * @param _value Input number to compute
         * @return absolute of the number
         */
        template<typename T>
        constexpr T Abs(T _value) noexcept;

        /**
         * @brief getter of the minimum value
         * @tparam T Type of value
         * @param _a First value to compare
         * @param _b Second value to compare
         * @return Minimum value of _a and _b
         */
        template<typename T>
        constexpr T Min(T _a, T _b) noexcept;

        /**
         * @brief getter of the maximum value
         * @tparam T Type of value
         * @param _a First value to compare
         * @param _b Second value to compare
         * @return Maximum value of _a and _b
         */
        template<typename T>
        constexpr T Max(T _a, T _b) noexcept;

        /**
         * @brief Compute pow of base with exponent
         * @tparam T Type of value
         * @param _base Base value
         * @param _exp Exponent value
         * @return Base raised to exponent
         */
        template<typename T>
        constexpr T Pow(T _base, T _exp) noexcept;

        /**
        * @brief Compute pow of base with exponent
        * @tparam T Type of value
        * @param _base Base value
        * @param _exp Exponent value
        * @return Base raised to exponent
        */
        template<typename T>
        constexpr T Pow(T _base, int _exp) noexcept;


        /**
         * @brief Compute the square root
         * @tparam T Type of value
         * @param _value Input number to compute
         * @return Square root of the number
         */
        template<typename T>
        constexpr T Sqrt(T _value) noexcept;

        /**
         * @brief Compute the cosine
         * @tparam T Type of value
         * @param _rad input a radian value to compute
         * @return Cosine of the number
         */
        template<typename T>
        constexpr T Cos(T _rad) noexcept;

        /**
         * @brief Compute the arc-cosine
         * @tparam T Type of value
         * @param _value Input number to compute
         * @return Arc-cosine in radian
         */
        template<typename T>
        constexpr T Acos(T _value) noexcept;

        /**
        * @brief Compute the sinus
        * @tparam T Type of value
        * @param _rad input a radian value to compute
        * @return Sinus of the number
        */
        template<typename T>
        constexpr T Sin(T _rad) noexcept;

        /**
        * @brief Compute the arc-sinus
        * @tparam T Type of value
        * @param _value Input number to compute
        * @return Arc-sinus in radian
        */
        template<typename T>
        constexpr T Asin(T _value) noexcept;


        /**
        * @brief Compute the tangent
        * @tparam T Type of value
        * @param _rad input a radian value to compute
        * @return Tangent of the number
        */
        template<typename T>
        constexpr T Tan(T _rad) noexcept;

        /**
        * @brief Compute the arc-tangent
        * @tparam T Type of value
        * @param _value Input number to compute
        * @return Arc-tangent in radian
        */
        template<typename T>
        constexpr T Atan(T _value) noexcept;


        /**
         * @brief test the equality of 2 values
         * @tparam T Type of value
         * @param _a First value
         * @param _b Second value
         * @param _epsilon Allowed threshold to accept equality
         * @return if a and b are equal
         */
        template <typename T>
        constexpr bool Equals(T _a, T _b, float _epsilon = S_EPSILON);

        /**
         * @brief test if the number is equal to 0
         * @tparam T Type of value
         * @param  _value is value
         * @return if value is equal to 0
         */
        template <typename T>
        constexpr bool Equals0(T _value);


    }
}

#include "numerics.inl"