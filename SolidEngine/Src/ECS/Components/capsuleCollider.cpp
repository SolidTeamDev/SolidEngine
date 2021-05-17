#include "ECS/Components/capsuleCollider.hpp"

#include "Core/engine.hpp"

namespace Solid
{
    void CapsuleCollider::Init()
    {
        Physics& physics = Engine::GetInstance()->physics;
        capsuleCollider = physics.CreateShape(gameObject->physicsActor,physx::PxCapsuleGeometry(radius,height));
    }

    void CapsuleCollider::Release()
    {
	    Components::Release();
	    Physics& physics = Engine::GetInstance()->physics;
        physics.DeleteShape(gameObject->physicsActor,capsuleCollider);
        capsuleCollider = nullptr;
    }
} //!namespace