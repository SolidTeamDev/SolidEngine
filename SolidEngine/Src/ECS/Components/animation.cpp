#include "ECS/Components/animation.hpp"
#include "Rendering/OpenGL45/openGl45Renderer.hpp"

namespace Solid
{
    Animation::Animation(SkeletonResource *_Skeleton):
    Skeleton{_Skeleton}
    {}

    void Animation::DrawSkeleton(std::vector<Vec3>& points, std::vector<uint>& indices,
                                 const SkeletonResource::Bone* bone, uint index)

    {
        if(!Skeleton)
            return;

        if(bone == nullptr)
            DrawSkeleton(points, indices, &Skeleton->rootBone);

        else
        {
            Vec4 parentPos = (bone->Parent == nullptr ? bone->FinalTrans : bone->Parent->FinalTrans) * Vec4::Zero;
            Vec4 childPos = bone->FinalTrans * Vec4::Zero;
            points.push_back(childPos);
            points.push_back(parentPos);
            indices.push_back(index);
            indices.push_back(index+1);
            index += 2;
            for (auto child : bone->Childrens)
            {
                DrawSkeleton(points, indices, child, index);

            }
        }
    }

    void Animation::SetSkeleton(SkeletonResource *_Skeleton)
    {
        Skeleton = _Skeleton;
    }

    const SkeletonResource* Animation::GetSkeleton()
    {
        return Skeleton;
    }

}
