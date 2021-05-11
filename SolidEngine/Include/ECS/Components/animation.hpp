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
        Animation(AnimResource* _anim);
        ~Animation() = default;
        void DrawSkeleton(std::vector<Vec3>& points, std::vector<uint>& indices,
                          SkeletonResource::Bone* bone = nullptr, uint index = 0);
        void SetAnim(AnimResource *_anim);
        const AnimResource * GetAnim();



    private:
        //SkeletonResource* Skeleton = nullptr;
        AnimResource* anim = nullptr;



    };
}
