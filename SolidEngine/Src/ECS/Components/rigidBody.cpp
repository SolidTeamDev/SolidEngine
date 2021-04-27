#include "ECS/Components/rigidBody.hpp"

#include "Core/engine.hpp"

namespace Solid
{
    void RigidBody::Init()
    {
        Physics& physics = Engine::GetInstance()->physics;
        physics.ConvertActor(gameObject->physicsActor,PhysicsActorType::DYNAMIC);
    }
} //!namespace