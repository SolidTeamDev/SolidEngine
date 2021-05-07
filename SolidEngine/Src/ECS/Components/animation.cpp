#include "ECS/Components/animation.hpp"
#include "Rendering/OpenGL45/openGl45Renderer.hpp"

namespace Solid
{
    Animation::Animation(SkeletonResource *_Skeleton):
    Skeleton{_Skeleton}
    {}

    /// don't forget const bone*
    void Animation::DrawSkeleton(std::vector<Vec3>& points, std::vector<uint>& indices,
                                 SkeletonResource::Bone* bone, uint index)

    {
        if(!Skeleton)
            return;

        if(bone == nullptr)
            DrawSkeleton(points, indices, &Skeleton->rootBone);

        else
        {
            Vec4 parentPos = Vec4::Zero;
            Vec4 childPos = Vec4::Zero;
            if(bone->Parent == nullptr)
            {
                bone->GlobalTrans = bone->LocalTrans;

                parentPos = (bone->GlobalTrans * bone->offset) * Vec4::Zero;
                childPos = (bone->GlobalTrans * bone->offset) * Vec4::Zero;
            }
            else
            {
                bone->GlobalTrans = bone->Parent->GlobalTrans * bone->LocalTrans;

                parentPos = (bone->Parent->GlobalTrans * bone->offset) * Vec4::Zero;
                childPos = (bone->GlobalTrans * bone->offset) * Vec4::Zero;
            }
            //Vec4 parentPos = (bone->Parent == nullptr ? bone->FinalTrans : bone->Parent->FinalTrans) * Vec4::Zero;
            //Vec4 childPos = ((bone->Parent == nullptr ? bone->FinalTrans : (bone->Parent->FinalTrans* bone->transfo)*bone->offset) ) * Vec4::Zero;

            points.push_back(parentPos);
            points.push_back(childPos);
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
