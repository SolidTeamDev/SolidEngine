#include "Physics/physicsEventCallback.hpp"

#include <PxRigidActor.h>
#include "ECS/sceneGraphManager.hpp"
#include "Core/Debug/log.hpp"

void Solid::PhysicsEventCallback::onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs)
{
    Log::Send("On contact",Log::ELogSeverity::DEBUG);
}

void Solid::PhysicsEventCallback::onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count)
{
    Log::Send("On trigger", Log::ELogSeverity::DEBUG);
    GameObject* go1 = (GameObject*)pairs->triggerActor->userData;
    GameObject* go2 = (GameObject*)pairs->otherActor->userData;
    //TODO: get scripts and call trigger function
}
