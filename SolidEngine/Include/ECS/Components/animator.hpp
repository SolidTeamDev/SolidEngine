#pragma once

#include "animation.hpp"
#include "EngineGenerated/animator.sld.hpp"


namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass()  Animator : public Components
    {
    public:
        Animator() = default;
        Animator(AnimResource* _anim);
        ~Animator() = default;

    private:

    };
}