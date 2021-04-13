#include "Physics/physics.hpp"

#include "Core/Debug/throwError.hpp"

using namespace physx;

namespace Solid
{
    physx::PxDefaultErrorCallback Physics::gDefaultErrorCallback = PxDefaultErrorCallback();
    physx::PxDefaultAllocator     Physics::gDefaultAllocatorCallback = PxDefaultAllocator();
    physx::PxCudaContextManager* Physics::gCudaContextManager = nullptr;

    Physics::Physics()
    {
        pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION,gDefaultAllocatorCallback,gDefaultErrorCallback);
        if(!pxFoundation)
            throw ThrowError("Physx initialization failed ! (PxCreateFoundation)",ESolidErrorCode::S_INIT_ERROR);

        pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, PxTolerancesScale(), true, nullptr);
        if(!pxPhysics)
            throw ThrowError("Physx initialization failed ! (PxCreatePhysics)",ESolidErrorCode::S_INIT_ERROR);

        /// Create physx scene

        PxSceneDesc sceneDesc(pxPhysics->getTolerancesScale());

        sceneDesc.gravity = PxVec3(0,-9.81f,0);
        sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(4);
        sceneDesc.filterShader  = PxDefaultSimulationFilterShader;

        pxScene = pxPhysics->createScene(sceneDesc);
        if(!pxScene)
            throw ThrowError("Physx create scene failed !",ESolidErrorCode::S_INIT_ERROR);
        ///

        pxMaterial = pxPhysics->createMaterial(.5f,.5f,.6f);

        PxRigidStatic* groundPlane = PxCreatePlane(*pxPhysics,PxPlane(0,1,0,0), *pxMaterial);
        AddCollider(*groundPlane);
    }

    Physics::~Physics()
    {
        pxPhysics->release();
        pxFoundation->release();
    }

    void Physics::Update(float _deltaTime)
    {
        if(_deltaTime <= 0)
            return;

        pxScene->simulate(_deltaTime);
        pxScene->fetchResults(true);
    }

    void Physics::AddCollider(PxActor& _actor)
    {
        pxScene->addActor(_actor);
    }

} //!namespace

