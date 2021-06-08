#pragma once
#include "Build/SolidAPI.hpp"
#include "constants.hpp"

namespace Solid
{
/// site where to find the curves : https://easings.net/ ///
    namespace FunctionCurve
    {
        float SOLID_API easeOutQuart(float _t);

        float SOLID_API easeInQuart(float _t);

        float SOLID_API easeInOutQuart(float _t);

        float SOLID_API easeOutExpo(float _t);

        float SOLID_API easeInExpo(float _t);

        float SOLID_API easeInOutExpo(float _t);

        float SOLID_API easeOutBack(float _t);

        float SOLID_API easeInBack(float _t);

        float SOLID_API easeInOutBack(float _t);

        float SOLID_API easeOutBounce(float _t);

        float SOLID_API easeInBounce(float _t);

        float SOLID_API easeInOutBounce(float _t);

        float SOLID_API easeInElastic(float _t);

        float SOLID_API easeOutElastic(float _t);

        float SOLID_API easeInOutElastic(float _t);

    }
}