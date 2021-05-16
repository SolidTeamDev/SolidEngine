#include "ECS/Components/transform.hpp"

#include "Core/Maths/Utils/numerics.hpp"

namespace Solid
{
    Transform::Transform():
    transMat(Mat4<>::Identity)
    {}

    Transform::Transform(const Vec3 &_position, const Quat &_rotation, const Vec3 &_scale):
    transMat(Mat4<>::Identity),
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

        hasToUpdateMat = true;
    }

    void Transform::Scale(const Vec3 &_vec)
    {
        scale += _vec;
        hasToUpdateMat = true;
    }

    Vec3 Transform::GetPosition() const
    {
        return position;
    }

    Quat Transform::GetRotation() const
    {
        return rotation;
    }

    Vec3 Transform::GetScale() const
    {
        return scale;
    }



    Vec3 Transform::GetEuler() const
    {
        return euler;
    }

    Mat4<float> Transform::GetMatrix()
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

	void Transform::SetParentMatrix(const Mat4<float>& mat)
	{
		ParentTransMat = mat;
	}

	Mat4<float> Transform::GetParentMatrix()
	{
		return ParentTransMat;
	}

} //!namespace