#pragma once

#include "Resources/ressources.hpp"
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

        void CalculateBoneTransform (SkeletonResource::Bone* bone, const Solid::Mat4f& _parent);
        void DrawSkeleton(std::vector<Vec3>& points, std::vector<uint>& indices,
                          SkeletonResource::Bone* bone = nullptr);
        void SetAnim(AnimResource *_anim,bool _loop = true);
        void SetAnim(String _animName,bool _loop = true);

        void UpdateAnim(float dt);
        void UpdateBone(float currTime, SkeletonResource::Bone* bone = nullptr);
        const AnimResource * GetAnim();
        const std::vector<Mat4f> GetFinalTrans();
		void Init();

        bool Loop = false;
        bool IsFinish = false;
        bool ShowSkeleton = false;
    private:
        float AnimTime;
        AnimResource* anim = nullptr;
	    SLDField()
        String AnimName;
        float CurrentTime = 0;
        int CurrentIndex = 0;
        std::vector<Mat4f> FinalsTrans;
        std::vector<Mat4f> FinalsTransBone;
    public:
		Animation_GENERATED
       

    };
}
File_GENERATED
