#pragma once

#include "Resources/ressources.hpp"
#include "Resources/graphicalResource.hpp"
#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/animation.sld.hpp"


namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass()  Animation : public Components
    {
    public:
        Animation() = default;
        Animation(SkeletonResource* _Skeleton);
        ~Animation() = default;

    private:
        SkeletonResource* Skeleton = nullptr;

    };
}
