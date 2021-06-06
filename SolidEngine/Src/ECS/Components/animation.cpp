#include "Core/engine.hpp"
#include "ECS/Components/animation.hpp"
#include "Rendering/OpenGL45/openGl45Renderer.hpp"
#include "Core/Debug/log.hpp"
namespace Solid
{
    Animation::Animation(AnimResource* _anim):
    anim{_anim}
    {
        FinalsTrans.reserve(anim->numOfBones);
        FinalsTransBone.reserve(anim->numOfBones);
        for(int i = 0; i < anim->numOfBones; i++)
        {
            FinalsTrans.push_back(Mat4f::Identity);
            FinalsTransBone.push_back(Mat4f::Identity);
        }

	    AnimName = anim->name;
    }

    /// don't forget const bone*
    void Animation::DrawSkeleton(std::vector<Vec3>& points, std::vector<uint>& indices,
                                 SkeletonResource::Bone* bone)

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

            if(bone->Parent == nullptr)
            {
                parentPos = FinalsTransBone[bone->id] * Vec4::Zero;
                points[bone->id] = parentPos ;
            }
            else
            {

                parentPos = FinalsTransBone[bone->Parent->id] * Vec4::Zero;
                childPos = FinalsTransBone[bone->id] * Vec4::Zero;
                if(bone->isAnimated)
                {
                    points[bone->id] = childPos;

                    indices.push_back(bone->Parent->id);
                    indices.push_back(bone->id);
                }


            }
            for (auto child : bone->Childrens)
            {
                DrawSkeleton(points, indices, child);
            }
        }
    }

    void Animation::CalculateBoneTransform (SkeletonResource::Bone* bone, const Mat4f& _parentTRS)
    {
        Mat4f nodeTransform = Mat4f::Identity;

        if (bone)
        {
            UpdateBone(CurrentTime*1000, bone);
            nodeTransform = bone->LocalTrans;
        }

        Mat4f GlobalTrans =  _parentTRS * nodeTransform;
        {
            FinalsTrans[bone->id] =  ( GlobalTrans * bone->offset);
            if(ShowSkeleton)
                FinalsTransBone[bone->id] =  ( GlobalTrans * bone->offset.GetTransposed().GetInversed());

        }

        for (auto child : bone->Childrens)
            CalculateBoneTransform(child, GlobalTrans);
    }

    void Animation::SetAnim(String _animName,bool _loop)
    {
        AnimResource* anim = Engine::GetInstance()->resourceManager.GetRawAnimByName(AnimName.c_str());
        if(anim)
            SetAnim(anim);
    }
    void Animation::SetAnim(AnimResource* _anim,bool _loop)
    {
        anim = _anim;
        AnimName = anim->name;
        AnimTime = anim->numTicks/1000;
        Loop = _loop;
        IsFinish = false;
        CurrentTime = 0;
        CurrentIndex = 0;
        FinalsTrans.clear();
        FinalsTransBone.clear();
        FinalsTrans.reserve(anim->numOfBones);
        FinalsTransBone.reserve(anim->numOfBones);
        for(int i = 0; i < anim->numOfBones; i++)
        {
            FinalsTrans.push_back(Mat4f::Identity);
            FinalsTransBone.push_back(Mat4f::Identity);
        }

        CalculateBoneTransform(anim->Root, Mat4f::Identity);

    }
    void Animation::UpdateAnim(float dt)
    {
        if (anim)
        {
            CurrentTime += dt;
            CurrentTime = fmod(CurrentTime, anim->numTicks);
            if(CurrentTime >= AnimTime && Loop)
            {
                CurrentTime = 0;
                CurrentIndex = 0;
                if(IsFinish)
                    IsFinish = false;
            }
            else if(CurrentTime >= AnimTime && !IsFinish && !Loop)
                IsFinish = true;
            CalculateBoneTransform(anim->Root, Mat4f::Identity);
        }
    }

    void Animation::UpdateBone(float currTime, SkeletonResource::Bone* bone)
    {
        {
            if(bone->channel.Frames.size() != 0)
            {
                int index = 0;
                if(currTime <= bone->channel.Frames.size())
                    index = currTime;
                else
                    index = bone->channel.Frames.size()-1;

                for(int i = CurrentIndex; i < bone->channel.Frames.size(); )
                {
                    SkeletonResource::Bone::KeyFrame& keyFrame = bone->channel.Frames[i];
                    if(keyFrame.time <= currTime)
                    {
                        ++i;

                    }
                    else
                    {
                        CurrentIndex = i;
                        break;
                    }
                }
                SkeletonResource::Bone::KeyFrame& keyFrame = bone->channel.Frames[CurrentIndex];
                Vec3 translation = keyFrame.usePos   ? keyFrame.pos   : Vec3::Zero;
                Quat rotation    = keyFrame.useRot   ? keyFrame.Rot   : Quat::Identity;
                Vec3 scale       = keyFrame.useScale ? keyFrame.Scale : Vec3(1,1,1);

                Mat4f newTF = Mat4f::Transform(translation, rotation, scale);

                bone->LocalTrans = newTF.GetTransposed();
                return;
            }
        }
    }

    const AnimResource * Animation::GetAnim()
    {
        return anim;
    }

   const std::vector<Mat4f> Animation::GetFinalTrans()
   {
        return  FinalsTrans;
   }

	void Animation::Init()
	{
		if(AnimName.empty())
			return;
		AnimResource* anim = Engine::GetInstance()->resourceManager.GetRawAnimByName(AnimName.c_str());
		if(anim)
			SetAnim(anim);
	}
}

