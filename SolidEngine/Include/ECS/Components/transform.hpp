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
	    Transform* parentTransform = nullptr;
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

        Vec3 GetLocalPosition() const;
        Vec3 GetLocalEuler()const;
        Quat GetLocalRotation() const;
	    Vec3 GetLocalScale() const;
	    Vec3 GetLocalForward() const;
	    Vec3 GetLocalRight() const;
	    Vec3 GetLocalUp() const;
	    Vec3 GetGlobalPosition() const;
	    Vec3 GetGlobalEuler()const;
	    Quat GetGlobalRotation() const;
	    Vec3 GetGlobalScale() const;
	    Vec3 GetGlobalForward() const;
	    Vec3 GetGlobalRight() const;
	    Vec3 GetGlobalUp() const;
        Mat4<float> GetLocalMatrix();
	    Mat4<float> GetGlobalMatrix();
	    void SetParentTransform( Transform* _parent);
	    Transform* GetParentTransform();
	    void Release()override;


        Transform_GENERATED
    };
} //!namespace


File_GENERATED