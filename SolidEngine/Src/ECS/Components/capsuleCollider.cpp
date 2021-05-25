#include "ECS/Components/capsuleCollider.hpp"

#include "Core/engine.hpp"

namespace Solid
{
    void CapsuleCollider::Init()
    {
        Physics& physics = Engine::GetInstance()->physics;
        physicMaterial = physics.CreateMaterial(staticFriction,dynamicFriction,restitution);
        capsuleCollider = physics.CreateShape(gameObject,physx::PxCapsuleGeometry(radius,height),physicMaterial);
        SetCenter(center);
        SetHeight(height);
        SetRadius(radius);
        SetTrigger(isTrigger);
        SetCapsuleDirection(direction);
    }

    void CapsuleCollider::Release()
    {
	    Physics& physics = Engine::GetInstance()->physics;
        physics.DeleteShape(gameObject->physicsActor,capsuleCollider);
        physicMaterial->release();
        physicMaterial = nullptr;
        capsuleCollider = nullptr;

        Components::Release();
    }

    Vec3 CapsuleCollider::GetCenter()
    {
        return center;
    }

    void CapsuleCollider::SetCenter(const Vec3 &_center)
    {
        center = _center;
    }

    float CapsuleCollider::GetRadius()
    {
        return radius;
    }

    void CapsuleCollider::SetRadius(float _radius)
    {
        radius = _radius;
        capsuleCollider->setGeometry(physx::PxCapsuleGeometry(radius,height));
    }

    float CapsuleCollider::GetHeight()
    {
        return height;
    }

    void CapsuleCollider::SetHeight(float _height)
    {
        height = _height;
        capsuleCollider->setGeometry(physx::PxCapsuleGeometry(radius,height));
    }

    CapsuleCollider::ECapsuleDirection CapsuleCollider::GetDirection()
    {
        return direction;
    }

    void CapsuleCollider::SetCapsuleDirection(const CapsuleCollider::ECapsuleDirection &_capsuleDirection)
    {
        //TODO: ROTATE NOT WORKING
        physx::PxTransform transform = capsuleCollider->getLocalPose();

        switch (direction)
        {
            case ECapsuleDirection::X_AXIS:
                transform.rotate(physx::PxVec3(-1,0,0));
                break;
            case ECapsuleDirection::Y_AXIS:
                transform.rotate(physx::PxVec3(0,-1,0));
                break;
            case ECapsuleDirection::Z_AXIS:
                transform.rotate(physx::PxVec3(0,0,-1));
                break;
        }

        direction = _capsuleDirection;

        switch (direction)
        {
            case ECapsuleDirection::X_AXIS:
                transform.rotate(physx::PxVec3(1,0,0));
                break;
            case ECapsuleDirection::Y_AXIS:
                transform.rotate(physx::PxVec3(0,1,0));
                break;
            case ECapsuleDirection::Z_AXIS:
                transform.rotate(physx::PxVec3(0,0,1));
                break;
        }

        capsuleCollider->setLocalPose(transform);
    }

    bool CapsuleCollider::IsTrigger()
    {
        return isTrigger;
    }

    void CapsuleCollider::SetTrigger(bool _trigger)
    {
        isTrigger = _trigger;

        if(isTrigger)
        {
            capsuleCollider->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
            capsuleCollider->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
        }
        else
        {
            capsuleCollider->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
            capsuleCollider->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
        }
    }
} //!namespace