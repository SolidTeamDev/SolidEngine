#pragma once

#include "Build/solidAPI.hpp"

#include "ECS/Components/transform.hpp"
#include <PxPhysicsAPI.h>

class GameObject;
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

        static physx::PxCudaContextManager*  gCudaContextManager;

        physx::PxFoundation* pxFoundation = nullptr;
        physx::PxPhysics*    pxPhysics    = nullptr;
        physx::PxScene*      pxScene      = nullptr;

        physx::PxPvd* pxPvd = nullptr;
        physx::PxPvdTransport* pxTransport = nullptr;

        static physx::PxFilterFlags filterShader(physx::PxFilterObjectAttributes attributes0,
                                                 physx::PxFilterData filterData0,
                                                 physx::PxFilterObjectAttributes attributes1,
                                                 physx::PxFilterData filterData1,
                                                 physx::PxPairFlags& pairFlags,
                                                 const void* constantBlock,
                                                 physx::PxU32 constantBlockSize);
    public:

        Physics();
        ~Physics();

        Transform GetTransform(physx::PxActor* _actor) const;
        void SetTransform(physx::PxActor* _actor, const Transform& _transform) const;

        void Update(float _deltaTime) const;

        physx::PxRigidDynamic* CreateDynamic(GameObject* _go, const Transform& _transform = Transform());
        physx::PxRigidStatic* CreateStatic(GameObject* _go, const Transform& _transform   = Transform());
        void ConvertActor(GameObject* _go, PhysicsActorType _actorType);

        physx::PxShape* CreateShape(GameObject* _go, const physx::PxGeometry& _geometry, const physx::PxMaterial* _physicsMaterials);
        void DeleteShape(physx::PxActor* _actor, physx::PxShape* _shape);

        physx::PxMaterial* CreateMaterial(float _staticFriction, float _dynamicFriction, float _restitution);

        void AddForce(const physx::PxActor* _actor, const Vec3& _force, const physx::PxForceMode::Enum& _forceMode);

        Vec3 GetLinearVelocity(const physx::PxActor* _actor) const;
        Vec3 GetAngularVelocity(const physx::PxActor* _actor) const;

        void SetLinearVelocity(const physx::PxActor* _actor, const Vec3& _velocity);
        void SetAngularVelocity(const physx::PxActor* _actor, const Vec3& _velocity);

        void EnableGravity(const physx::PxActor* _actor, bool _enableGravity);

        void SetKinematic(const physx::PxActor* _actor, bool _kinematic);

        void SetMass(const physx::PxActor* _actor, float _mass);

        void SetDrag(const physx::PxActor* _actor, float _drag);

        void SetAngularDrag(const physx::PxActor* _actor, float _angularDrag);

        void FreezePosX(const physx::PxActor *_actor, bool _freezePosX);
        void FreezePosY(const physx::PxActor *_actor, bool _freezePosY);
        void FreezePosZ(const physx::PxActor *_actor, bool _freezePosZ);

        void FreezeRotX(const physx::PxActor *_actor, bool _freezeRotX);
        void FreezeRotY(const physx::PxActor *_actor, bool _freezeRotY);
        void FreezeRotZ(const physx::PxActor *_actor, bool _freezeRotZ);
    };

}