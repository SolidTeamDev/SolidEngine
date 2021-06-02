#include "ECS/System/physicsSystem.hpp"

#include "ECS/Components/rigidBody.hpp"

namespace Solid
{
    void Solid::PhysicsSystem::Update(const Physics& _physics, float _deltaTime)
    {
        //Update physics pos
        for (Entity entity : entities)
        {
            GameObject* go = ecsManager.GetGameObjectFromEntity(entity);
            if(go->physicsActor == nullptr)
                continue;

            const Transform& transform = ecsManager.GetComponent<Transform>(entity);

             _physics.SetTransform(go->physicsActor,transform);
        }

        //Update physics frame
        _physics.Update(_deltaTime);

        //Update engine pos
        for (Entity entity : entities)
        {
            GameObject* go = ecsManager.GetGameObjectFromEntity(entity);
            if(go->physicsActor == nullptr)
                continue;

            Transform& transform = ecsManager.GetComponent<Transform>(entity);

            Vec3 scale = transform.GetLocalScale();
            transform = _physics.GetTransform(go->physicsActor);
            transform.SetScale(scale);
        }
    }
} //!namespace
