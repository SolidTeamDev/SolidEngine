#include "ECS/Components/sphereCollider.hpp"

#include "Core/engine.hpp"

namespace Solid
{
    void SphereCollider::Init()
    {
        Physics& physics = Engine::GetInstance()->physics;
        physicMaterial = physics.CreateMaterial(staticFriction,dynamicFriction,restitution);
        sphereCollider = physics.CreateShape(gameObject,physx::PxSphereGeometry(radius),physicMaterial);
    }

    void SphereCollider::Release()
    {
	    Physics& physics = Engine::GetInstance()->physics;
        physics.DeleteShape(gameObject->physicsActor,sphereCollider);
        physicMaterial->release();
        physicMaterial = nullptr;
        sphereCollider = nullptr;

        Components::Release();
    }

    Vec3 SphereCollider::GetCenter()
    {
        return center;
    }

    void SphereCollider::SetCenter(const Vec3 &_center)
    {
        center = _center;
    }

    float SphereCollider::GetRadius()
    {
        return radius;
    }

    void SphereCollider::SetRadius(float _radius)
    {
        radius = _radius;
        sphereCollider->setGeometry(physx::PxSphereGeometry(radius));
    }

    bool SphereCollider::IsTrigger()
    {
        return isTrigger;
    }

    void SphereCollider::SetTrigger(bool _trigger)
    {
        isTrigger = _trigger;

        if(isTrigger)
        {
            sphereCollider->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
            sphereCollider->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
        }
        else
        {
            sphereCollider->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
            sphereCollider->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
        }
    }
} //!namespace