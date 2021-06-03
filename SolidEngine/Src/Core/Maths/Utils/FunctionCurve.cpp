#include "Core/Maths/Utils/numerics.hpp"
#include "Core/Maths/Utils/FunctionCurve.hpp"

namespace Solid
{
    namespace FunctionCurve
    {
        float easeOutQuart(float _t)
        {
            return 1 - Maths::Pow(1-_t, 4);
        }

        float easeInQuart(float _t)
        {
            return _t * _t * _t * _t;
        }

        float easeInOutQuart(float _t)
        {

            return _t < 0.5 ? 8 * _t * _t * _t * _t : (float)(1 - pow(-2 * _t + 2, 4) / 2);
        }

        float easeOutExpo(float _t)
        {
            return _t == 1 ? 1 : (float)(1 - pow(2, -10 * _t));
        }

        float easeInExpo(float _t)
        {

            return _t == 0 ? 0 : pow(2, 10 * _t - 10);
        }

        float easeInOutExpo(float _t)
        {
            return  _t == 0 ? 0 :
                    _t == 1 ? 1 :
                    _t < 0.5 ? pow(2, 20 * _t - 10) / 2 : (2 - pow(2, -20 * _t + 10)) / 2;
        }

        float easeOutBack(float _t)
        {
            const float c1 = 1.70158;
            const float c3 = c1 + 1;

            return 1 + c3 * pow(_t - 1, 3) + c1 * pow(_t - 1, 2);
        }

        float easeInBack(float _t)
        {
            const float c1 = 1.70158;
            const float c3 = c1 + 1;

            return c3 * _t * _t * _t - c1 * _t * _t;
        }

        float easeInOutBack(float _t)
        {
            const float c1 = 1.70158;
            const float c2 = c1 * 1.525;

            return _t < 0.5
                   ? (float)((pow(2 * _t, 2) * ((c2 + 1) * 2 * _t - c2)) / 2)
                   : (float)((pow(2 * _t - 2, 2) * ((c2 + 1) * (_t * 2 - 2) + c2) + 2) / 2);
        }

        float easeOutBounce(float _t)
        {
            const float n1 = 7.5625;
            const float d1 = 2.75;

            if (_t < 1 / d1)
                return n1 * _t * _t;

            else if (_t < 2 / d1)
                return n1 * (_t -= 1.5 / d1) * _t + 0.75;

            else if (_t < 2.5 / d1)
                return n1 * (_t -= 2.25 / d1) * _t + 0.9375;

            else
                return n1 * (_t -= 2.625 / d1) * _t + 0.984375;

        }

        float easeInBounce(float _t)
        {
            return 1 - easeOutBounce(1 - _t);
        }

        float easeInOutBounce(float _t)
        {

            return _t < 0.5
                   ? (1 - easeOutBounce(1 - 2 * _t)) / 2
                   : (1 + easeOutBounce(2 * _t - 1)) / 2;
        }

        float easeInElastic(float _t)
        {
            const float c4 = (2 * 3.14) / 3;

            return  _t == 0 ? 0 :
                    _t == 1 ? 1 : Maths::Pow(2, -10 * _t) * Maths::Sin((_t * 10 - 0.75) * c4) + 1;
        }

        float easeOutElastic(float _t)
        {
            const float c4 = (2 * 3.14) / 3;

            return  _t == 0 ? 0 :
                    _t == 1 ? 1 : -Maths::Pow(2, 10 * _t - 10) * Maths::Sin((_t * 10 - 10.75) * c4);
        }

        float easeInOutElastic(float _t)
        {
            const float c5 = (2 *3.14) / 4.5;

            return  _t == 0 ? 0 :
                    _t == 1 ? 1 :
                    _t < 0.5 ? -(Maths::Pow(2, 20 * _t - 10) * Maths::Sin((20 * _t - 11.125) * c5)) / 2
                                   : (Maths::Pow(2, -20 * _t + 10) * Maths::Sin((20 * _t - 11.125) * c5)) / 2 + 1;
        }
    }
}
