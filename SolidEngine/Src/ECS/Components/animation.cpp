#include "Core/engine.hpp"
#include "ECS/Components/animation.hpp"
#include "Rendering/OpenGL45/openGl45Renderer.hpp"

namespace Solid
{
    Animation::Animation(AnimResource* _anim):
    anim{_anim}
    {
        FinalsTrans.reserve(anim->numOfBones);
        for(int i = 0; i < anim->numOfBones; i++)
            FinalsTrans.push_back(Mat4f::Identity);
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
                parentPos = FinalsTrans[bone->id] * Vec4::Zero;
                points[bone->id] = parentPos ;
            }
            else
            {

                parentPos = FinalsTrans[bone->Parent->id] * Vec4::Zero;
                childPos = FinalsTrans[bone->id] * Vec4::Zero;
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

    void Animation::CalculateBoneTransform (SkeletonResource::Bone* bone, Mat4f _parentTRS)
    {
        const char* nodeName = bone->name.c_str();
        Mat4f nodeTransform = Mat4f::Identity;

        SkeletonResource::Bone* newBone = anim->Root->FindBoneByName(nodeName);

        if (newBone)
        {
           UpdateBone(CurrentTime, newBone);
            nodeTransform = newBone->LocalTrans;
        }

        Mat4f GlobalTrans = _parentTRS * nodeTransform;

        //auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
        //if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            FinalsTrans[bone->id] =  ( GlobalTrans * newBone->offset );
        }

        for (auto child : newBone->Childrens)
            CalculateBoneTransform(child, GlobalTrans);
    }

    void Animation::SetAnim(AnimResource* _anim)
    {
        anim = _anim;
        AnimName = anim->name;
        CurrentTime = 0;
        AnimTime = anim->numTicks/anim->ticksPerSeconds;
        std::cout << AnimTime << std::endl;
        InverseRootMat = (anim->Root->LocalTrans ) ;
        FinalsTrans.clear();
        FinalsTrans.reserve(anim->numOfBones);
        for(int i = 0; i < anim->numOfBones; i++)
            FinalsTrans.push_back(Mat4f::Identity);

        CalculateBoneTransform(anim->Root, Mat4f::Identity);

    }
    void Animation::UpdateAnim(float dt)
    {
        if (anim)
        {
            CurrentTime += anim->ticksPerSeconds * dt;
            CurrentTime = fmod(CurrentTime, anim->numTicks);
            if(CurrentTime >= AnimTime)
                CurrentTime = 0;
            CalculateBoneTransform(anim->Root, Mat4f::Identity);
        }
    }

    void Animation::UpdateBone(float currTime, SkeletonResource::Bone* bone)
    {
        //for (auto& boneChannel : anim->Channels)
        {
            SkeletonResource::Bone* newBone = anim->Root->FindBoneByName(bone->name.c_str());
            if(bone->channel.Frames.size() != 0)
            {
                int index = 0;
                if(currTime <= bone->channel.Frames.size())
                    index = (int)currTime;
                else
                    index = (int)bone->channel.Frames.size()-1;

                //SkeletonResource::Bone* bonetoMod = boneChannel.BoneToMod;
                Vec3 translation = Vec3::Zero;
                Quat rotation = Quat::Identity;
                Vec3 scale = Vec3(1,1,1);

                if(bone->channel.Frames[index].usePos)
                    translation = bone->channel.Frames[index].pos;

                if(bone->channel.Frames[index].useRot)
                    rotation = bone->channel.Frames[index].Rot;

                if(bone->channel.Frames[index].useScale)
                    scale = bone->channel.Frames[index].Scale;

                Mat4f newTF = Mat4f::Transform(translation, rotation,scale);

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

