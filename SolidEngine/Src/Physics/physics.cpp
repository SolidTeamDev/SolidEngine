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
        pxScene->addActor(*groundPlane);
    }

    Physics::~Physics()
    {
        pxPhysics->release();
        pxFoundation->release();
    }

    Transform Physics::GetTransform(physx::PxActor* _actor) const
    {
        PxTransform pxT;

        switch(_actor->getType())
        {
            case PxActorType::Enum::eRIGID_STATIC:
                pxT = ((PxRigidStatic*) _actor)->getGlobalPose();
                break;
            case PxActorType::Enum::eRIGID_DYNAMIC:
                pxT = ((PxRigidDynamic*) _actor)->getGlobalPose();
                break;
        }

        return Transform(Vec3(pxT.p.x,pxT.p.y,pxT.p.z),
                         Quat(pxT.q.x,pxT.q.y,pxT.q.z,pxT.q.w),Vec3(1));
    }

    void Physics::SetTransform(physx::PxActor* _actor, const Transform& _transform) const
    {
        Vec3 pos = _transform.GetPosition();
        Quat rot = _transform.GetRotation();
        PxTransform pxT = PxTransform(PxVec3(pos.x,pos.y,pos.z),PxQuat(rot.x,rot.y,rot.z,rot.w));

        switch(_actor->getType())
        {
            case PxActorType::Enum::eRIGID_STATIC:
                ((PxRigidStatic*) _actor)->setGlobalPose(pxT);
                break;
            case PxActorType::Enum::eRIGID_DYNAMIC:
                ((PxRigidDynamic*) _actor)->setGlobalPose(pxT);
                break;
        }
    }

    void Physics::Update(float _deltaTime) const
    {
        if(_deltaTime <= 0)
            return;

        pxScene->simulate(_deltaTime);
        pxScene->fetchResults(true);
    }

    physx::PxRigidDynamic* Physics::CreateDynamic(const Transform& _transform)
    {
        Vec3 pos = _transform.GetPosition();
        Quat rot = _transform.GetRotation();
        PxTransform pxT = PxTransform(PxVec3(pos.x,pos.y,pos.z),PxQuat(rot.x,rot.y,rot.z,rot.w));
        PxRigidDynamic* dynamicActor = pxPhysics->createRigidDynamic(pxT);
        pxScene->addActor(*dynamicActor);

        return dynamicActor;
    }

    physx::PxRigidStatic* Physics::CreateStatic(const Transform& _transform)
    {
        Vec3 pos = _transform.GetPosition();
        Quat rot = _transform.GetRotation();
        PxTransform pxT = PxTransform(PxVec3(pos.x,pos.y,pos.z),PxQuat(rot.x,rot.y,rot.z,rot.w));
        PxRigidStatic* staticActor = pxPhysics->createRigidStatic(pxT);
        pxScene->addActor(*staticActor);

        return staticActor;
    }

    //TODO: TEST THIS
    void Physics::ConvertActor(PxActor* _actor, const PhysicsActorType& _actorType)
    {
        //Check if actor exist
        if(!_actor)
        {
            Log::Send("Cannot convert actor : Actor not created !",Log::ELogSeverity::WARNING);
            return;
        }

        //Change actor type (STATIC <-> DYNAMIC)
        switch(_actor->getType())
        {
            case PxActorType::Enum::eRIGID_STATIC:
                if(_actorType == PhysicsActorType::DYNAMIC)
                {
                    PxTransform pxT;
                    PxRigidStatic* staticActor = (PxRigidStatic*) _actor;
                    PxRigidDynamic* dynamicActor = nullptr;
                    PxShape** shapeList;
                    size_t nbShape = 0;
                    // Save data
                    pxT = staticActor->getGlobalPose();
                    staticActor->getShapes(shapeList,sizeof(PxShape));
                    nbShape = staticActor->getNbShapes();
                    // Remove
                    pxScene->removeActor(*staticActor);
                    staticActor->release();
                    // Create
                    dynamicActor = pxPhysics->createRigidDynamic(pxT);
                    // Restore
                    for (size_t i = 1; i < nbShape; ++i)
                        dynamicActor->attachShape(*shapeList[i]);

                    staticActor->release();

                    _actor = dynamicActor;

                    break;
                }

                return;

            case PxActorType::Enum::eRIGID_DYNAMIC:
                if(_actorType == PhysicsActorType::STATIC)
                {
                    PxTransform pxT;
                    PxRigidStatic* staticActor = nullptr;
                    PxRigidDynamic* dynamicActor = (PxRigidDynamic*) _actor;
                    PxShape** shapeList;
                    size_t nbShape = 0;

                    pxT = dynamicActor->getGlobalPose();
                    dynamicActor->getShapes(shapeList,sizeof(PxShape));
                    nbShape = dynamicActor->getNbShapes();

                    pxScene->removeActor(*dynamicActor);

                    staticActor = pxPhysics->createRigidStatic(pxT);

                    for (size_t i = 1; i < nbShape; ++i)
                        staticActor->attachShape(*shapeList[i]);

                    dynamicActor->release(); ///WARN; maybe Pointer Data Lost For Shape

                    _actor = staticActor;

                    break;
                }

                return;
        }

        // Add switched actor
        pxScene->addActor(*_actor);
    }

} //!namespace

