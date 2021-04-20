#pragma once

#include "Build/solidAPI.hpp"

#include "ECS/Components/transform.hpp"
#include <PxPhysicsAPI.h>

namespace Solid
{
    enum class PhysicsActorType
    {
        STATIC,
        DYNAMIC
    };

    class SOLID_API Physics
    {
    private:
        static physx::PxDefaultErrorCallback gDefaultErrorCallback;
        static physx::PxDefaultAllocator     gDefaultAllocatorCallback;

        static physx::PxCudaContextManager* gCudaContextManager;

        physx::PxFoundation* pxFoundation = nullptr;
        physx::PxPhysics*    pxPhysics    = nullptr;
        physx::PxScene*      pxScene      = nullptr;

        physx::PxMaterial* pxMaterial = nullptr;
    public:

        Physics();
        ~Physics();

        Transform GetTransform(physx::PxActor* _actor) const;
        void SetTransform(physx::PxActor* _actor, const Transform& _transform) const;

        void Update(float _deltaTime) const;

        physx::PxRigidDynamic* CreateDynamic(const Transform& _transform);
        physx::PxRigidStatic* CreateStatic(const Transform& _transform);
        void ConvertActor(physx::PxActor* _actor, const PhysicsActorType& _actorType);
    };

}