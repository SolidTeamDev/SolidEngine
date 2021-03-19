#include "Physics/physics.hpp"

#include "Core/Debug/throwError.hpp"

using namespace physx;

namespace Solid
{

    Physics::Physics()
    {
        static physx::PxDefaultErrorCallback gDefaultErrorCallback;
        static physx::PxDefaultAllocator     gDefaultAllocatorCallback;

        pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION,gDefaultAllocatorCallback,gDefaultErrorCallback);
        if(pxFoundation == nullptr)
            throw ThrowError("Physx initialization failed ! (PxCreateFoundation)",ESolidErrorCode::S_INIT_ERROR);

        pxPvd = nullptr;/*PxCreatePvd(*pxFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    pxPvd->connect(*transport, PxPvdInstrumentationFlag::Enum::eALL);*/

        pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, PxTolerancesScale(), true, pxPvd);
        if(pxPhysics == nullptr)
            throw ThrowError("Physx initialization failed ! (PxCreatePhysics)",ESolidErrorCode::S_INIT_ERROR);

    }

    Physics::~Physics()
    {
        pxPhysics->release();
        pxFoundation->release();
    }

} //!namespace

