#pragma once

#include <PxPhysicsAPI.h>

namespace Solid
{
    class Physics
    {
    public:

        physx::PxFoundation* pxFoundation = nullptr;
        physx::PxPvd*        pxPvd        = nullptr;
        physx::PxPhysics*    pxPhysics    = nullptr;


        Physics();
        ~Physics();
    };
}