#include "ECS/Components/animation.hpp"
#include "Rendering/OpenGL45/openGl45Renderer.hpp"

namespace Solid
{
    Animation::Animation(AnimResource* _anim):
    anim{_anim}
    {
    }

    /// don't forget const bone*
    void Animation::DrawSkeleton(std::vector<Vec3>& points, std::vector<uint>& indices,
                                 SkeletonResource::Bone* bone, uint index)

    {
        if(!anim || !anim->Root)
            return;

        if(bone == nullptr)
        {
            points.clear();
            points.resize(anim->numOfBones);
            indices.clear();
            DrawSkeleton(points, indices, anim->Root);
        }

        else
        {
            Vec4 parentPos = Vec4::Zero;
            Vec4 childPos = Vec4::Zero;
            for (auto& boneChannel : anim->Channels)
            {
                SkeletonResource::Bone* bonetoMod = boneChannel.BoneToMod;
                Mat4<float> newTF = Mat4<float>::Transform(boneChannel.Frames[0].pos,boneChannel.Frames[0].Rot,boneChannel.Frames[0].Scale);
                //bonetoMod->LocalTrans = newTF;
            }
            if(bone->Parent == nullptr)
            {
                bone->GlobalTrans = bone->LocalTrans.GetTransposed();
                parentPos = (bone->GlobalTrans.GetInversed()) * bone->GlobalTrans * bone->offset * Vec4::Zero;
                points[bone->id] = parentPos ;
            }
            else
            {
                bone->GlobalTrans = (bone->Parent->GlobalTrans * bone->LocalTrans).GetTransposed();

                childPos = bone->GlobalTrans.GetInversed() * bone->GlobalTrans * bone->offset * Vec4::Zero;
                if(bone->isAnimated)
                {
                    points[bone->id] = childPos;

                    indices.push_back(bone->Parent->id);
                    indices.push_back(bone->id);
                }


            }
            //Vec4 parentPos = (bone->Parent == nullptr ? bone->FinalTrans : bone->Parent->FinalTrans) * Vec4::Zero;
            //Vec4 childPos = ((bone->Parent == nullptr ? bone->FinalTrans : (bone->Parent->FinalTrans* bone->transfo)*bone->offset) ) * Vec4::Zero;


            for (auto child : bone->Childrens)
            {
                DrawSkeleton(points, indices, child, index);
            }
        }
    }

    void Animation::SetAnim(AnimResource* _anim)
    {
        anim = _anim;
    }

    const AnimResource * Animation::GetAnim()
    {
        return anim;
    }

}
