#include "ECS/Components/boxCollider.hpp"

#include "Core/engine.hpp"

namespace Solid
{
    void BoxCollider::Init()
    {
        Physics& physics = Engine::GetInstance()->physics;
        boxCollider = physics.CreateShape(gameObject->physicsActor,physx::PxBoxGeometry(1,1,1));
    }
} //!namespace