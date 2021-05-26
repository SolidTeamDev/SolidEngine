#include "ECS/Components/boxCollider.hpp"

#include "Core/engine.hpp"

namespace Solid
{
    void BoxCollider::Init()
    {
        Physics& physics = Engine::GetInstance()->physics;
        physicMaterial = physics.CreateMaterial(staticFriction,dynamicFriction,restitution);
        boxCollider = physics.CreateShape(gameObject,physx::PxBoxGeometry(size.x,size.y,size.z),physicMaterial);
        SetCenter(center);
        SetSize(size);
        SetTrigger(isTrigger);
    }

    void BoxCollider::Release()
    {
	    Physics& physics = Engine::GetInstance()->physics;
        physics.DeleteShape(gameObject->physicsActor,boxCollider);
        physicMaterial->release();
        physicMaterial = nullptr;
        boxCollider = nullptr;

        Components::Release();
    }

    Vec3 BoxCollider::GetCenter()
    {
        return center;
    }

    void BoxCollider::SetCenter(const Vec3& _center)
    {
        center = _center;
    }

    Vec3 BoxCollider::GetSize()
    {
        return size;
    }

    void BoxCollider::SetSize(const Vec3& _size)
    {
        size = _size;
        boxCollider->setGeometry(physx::PxBoxGeometry(size.x,size.y,size.z));
    }

    bool BoxCollider::IsTrigger()
    {
        return isTrigger;
    }

    void BoxCollider::SetTrigger(bool _trigger)
    {
        isTrigger = _trigger;

        if(isTrigger)
        {
            boxCollider->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
            boxCollider->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
        }
        else
        {
            boxCollider->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
            boxCollider->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
        }
    }

} //!namespace