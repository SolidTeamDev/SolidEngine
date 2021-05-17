#include "ECS/Components/boxCollider.hpp"

#include "Core/engine.hpp"

namespace Solid
{
    void BoxCollider::Init()
    {
        Physics& physics = Engine::GetInstance()->physics;
        boxCollider = physics.CreateShape(gameObject->physicsActor,physx::PxBoxGeometry(size.x,size.y,size.z));
    }

    void BoxCollider::Release()
    {
	    Components::Release();
	    Physics& physics = Engine::GetInstance()->physics;
        physics.DeleteShape(gameObject->physicsActor,boxCollider);
        boxCollider = nullptr;
    }
} //!namespace