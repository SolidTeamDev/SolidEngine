#include "ECS/Components/transform.hpp"

#include "Core/Maths/Utils/numerics.hpp"

namespace Solid
{
    Transform::Transform():
    transMat(Mat4<>::Identity),
    parentTransform(nullptr)
    {}

    Transform::Transform(const Vec3 &_position, const Quat &_rotation, const Vec3 &_scale):
    transMat(Mat4<>::Identity),
    parentTransform(nullptr),
    position(_position),
    rotation(_rotation),
    euler(Maths::RadToDeg(rotation.ToEuler())),
    scale(_scale)
    {}

    void Transform::SetPosition(const Vec3 &_vec)
    {
        position = _vec;
        hasToUpdateMat = true;
    }

    void Transform::SetRotation(const Quat &_quat)
    {
        rotation = _quat;
        euler = _quat.ToEuler();
        euler.x=Maths::RadToDeg(euler.x);
        euler.y=Maths::RadToDeg(euler.y);
        euler.z=Maths::RadToDeg(euler.z);

        hasToUpdateMat = true;
    }

    void Transform::SetEuler(const Vec3& _euler)
    {
        euler=_euler;
        Vec3 temp {Maths::DegToRad(_euler.x),Maths::DegToRad(_euler.y),Maths::DegToRad(_euler.z)};
        rotation = Quat(temp);
    }

    void Transform::SetScale(const Vec3 &_vec)
    {
        scale = _vec;
        hasToUpdateMat = true;
    }

    void Transform::Translate(const Vec3 &_vec)
    {
        position += _vec;
        hasToUpdateMat = true;
    }

    void Transform::Rotate(const Quat &_quat)
    {
        rotation = rotation.Rotate(_quat);

        euler = rotation.ToEuler();
        euler.x=Maths::RadToDeg(euler.x);
        euler.y=Maths::RadToDeg(euler.y);
        euler.z=Maths::RadToDeg(euler.z);

        hasToUpdateMat = true;
    }

    void Transform::Scale(const Vec3 &_vec)
    {
        scale += _vec;
        hasToUpdateMat = true;
    }

    Vec3 Transform::GetLocalPosition() const
    {
        return position;
    }

    Quat Transform::GetLocalRotation() const
    {
        return rotation;
    }

    Vec3 Transform::GetLocalScale() const
    {
        return scale;
    }



    Vec3 Transform::GetLocalEuler() const
    {
        return euler;
    }

    Mat4<float> Transform::GetLocalMatrix()
    {
        if(hasToUpdateMat)
        {
            UpdateTransformMatrix();
            hasToUpdateMat = false;
        }

        return transMat;
    }

    void Transform::UpdateTransformMatrix()
    {
        transMat = Mat4<float>::Transform(position,rotation,scale);
    }

    void Transform::SetTransformMatrix(const Mat4<float> &_mat)
    {
        transMat = _mat;
        Mat4<float>::DecomposeTransform(_mat,position,rotation,scale);
        Vec3 eulerRad = rotation.ToEuler();
        euler = Vec3(Maths::RadToDeg(eulerRad.x),Maths::RadToDeg(eulerRad.y),Maths::RadToDeg(eulerRad.z));
        hasToUpdateMat = false;
    }


	void Transform::Release()
	{
		Components::Release();
	}

	void Transform::SetParentTransform( Transform *_parent)
	{
		parentTransform = _parent;
	}

	Vec3 Transform::GetGlobalPosition()
	{
		if (parentTransform == nullptr)
			return position;
		return  parentTransform->GetGlobalMatrix()* Vec4(position);
	}

	Mat4<float> Transform::GetGlobalMatrix()
	{
    	if(parentTransform == nullptr)
		    return GetLocalMatrix();

		return GetLocalMatrix() *parentTransform->GetGlobalMatrix() ;
	}

	Quat Transform::GetGlobalRotation() const
	{
    	if(parentTransform == nullptr)
    		return rotation;
		return parentTransform->GetGlobalRotation() *rotation ;
	}

	Vec3 Transform::GetGlobalScale() const
	{
		if(parentTransform == nullptr)
			return scale;
		return scale * parentTransform->GetGlobalScale();
	}

	Vec3 Transform::GetGlobalEuler() const
	{
		if(parentTransform == nullptr)
			return euler;
		return euler + parentTransform->GetGlobalEuler();
	}

	Transform *Transform::GetParentTransform()
	{
		return parentTransform;
	}

	Vec3 Transform::GetLocalForward() const
	{
		return rotation.Rotate(Vec3(0,0,1));
	}

	Vec3 Transform::GetLocalRight() const
	{
		return rotation.Rotate(Vec3(-1,0,0));
	}

	Vec3 Transform::GetLocalUp() const
	{
		return rotation.Rotate(Vec3(0,1,0));
	}

	Vec3 Transform::GetGlobalForward() const
	{
		return GetGlobalRotation().Rotate(Vec3(0,0,1));
	}

	Vec3 Transform::GetGlobalRight() const
	{
		return GetGlobalRotation().Rotate(Vec3(-1,0,0));
	}

	Vec3 Transform::GetGlobalUp() const
	{
		return GetGlobalRotation().Rotate(Vec3(0,1,0));
	}


} //!namespace