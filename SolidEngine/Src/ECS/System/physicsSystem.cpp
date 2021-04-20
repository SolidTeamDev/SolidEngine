#include "ECS/System/physicsSystem.hpp"

#include "ECS/Components/rigidBody.hpp"

namespace Solid
{
    void Solid::PhysicsSystem::Update(const Physics& _physics, float _deltaTime)
    {
        //Update physics pos
        for (Entity entity : entities)
        {
            const Transform& transform = ecsManager.GetComponent<Transform>(entity);
            GameObject* go = ecsManager.GetGameObjectFromEntity(entity);

             _physics.SetTransform(go->physicsActor,transform);
        }

        //Update physics frame
        _physics.Update(_deltaTime);

        //Update engine pos
        for (Entity entity : entities)
        {
            Transform& transform = ecsManager.GetComponent<Transform>(entity);
            RigidBody& rigidbody = ecsManager.GetComponent<RigidBody>(entity);
            GameObject* go = ecsManager.GetGameObjectFromEntity(entity);

            Vec3 scale = transform.GetScale();
            transform = _physics.GetTransform(go->physicsActor);
            transform.SetScale(scale);
        }
    }
} //!namespace
