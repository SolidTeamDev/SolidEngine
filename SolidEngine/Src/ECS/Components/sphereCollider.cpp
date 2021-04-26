#include "ECS/Components/sphereCollider.hpp"

#include "Core/engine.hpp"

namespace Solid
{
    void SphereCollider::Init()
    {
        Physics& physics = Engine::GetInstance()->physics;
        sphereCollider = physics.CreateShape(gameObject->physicsActor,physx::PxSphereGeometry(radius));
    }

    void SphereCollider::Release()
    {
        Physics& physics = Engine::GetInstance()->physics;
        physics.DeleteShape(gameObject->physicsActor,sphereCollider);
        sphereCollider = nullptr;
    }
} //!namespace