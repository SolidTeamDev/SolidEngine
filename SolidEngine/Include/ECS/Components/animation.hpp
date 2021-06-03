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
        void SetAnim(AnimResource *_anim);
        void UpdateAnim(float dt);
        void UpdateBone(float currTime, SkeletonResource::Bone* bone = nullptr);
        const AnimResource * GetAnim();
        const std::vector<Mat4f> GetFinalTrans();
		void Init();

    private:
        //SkeletonResource* Skeleton = nullptr;
        float AnimTime;
        AnimResource* anim = nullptr;
        String AnimName;
        float CurrentTime;
        Mat4f InverseRootMat = Mat4f::Identity;
        std::vector<Mat4f> FinalsTrans;
        std::vector<Mat4f> FinalsTransBone;
    public:
		Animation_GENERATED
       

    };
}
File_GENERATED
