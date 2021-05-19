#pragma once


#include "Core/Maths/Quaternion/quaternion.hpp"
#include "Core/Maths/Vector/vector3.hpp"
#include "Core/Maths/Matrix/matrix4.hpp"

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/transform.sld.hpp"


namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() Transform : public Components
    {
    private:
    	SLDField()
        Vec3 position {0,0,0};
        SLDField()
        Quat rotation {};
        SLDField()
    	Vec3 scale {1,1,1};
        Vec3 euler{};
        Mat4<float> transMat;
	    Mat4<float> ParentTransMat;
        bool hasToUpdateMat = true;

        void UpdateTransformMatrix();

    public:


        Transform();
        Transform(const Vec3& _position, const Quat& _rotation, const Vec3& _scale);

        void SetPosition(const Vec3& _vec);
        void SetRotation(const Quat& _quat);
        void SetScale(const Vec3& _vec);
        void SetEuler(const Vec3& _euler);
        void SetTransformMatrix(const Mat4<float>& _mat);
        void Translate(const Vec3& _vec);
        void Rotate(const Quat& _quat);
        void Scale(const Vec3& _vec);

        Vec3 GetPosition() const;
        Vec3 GetEuler()const;
        Quat GetRotation() const;
        Vec3 GetScale() const;

        Mat4<float> GetMatrix();
	    void SetParentMatrix(const Mat4<float>& mat);
	    Mat4<float> GetParentMatrix();

	    void Release()override;

        Transform_GENERATED
    };
} //!namespace


File_GENERATED