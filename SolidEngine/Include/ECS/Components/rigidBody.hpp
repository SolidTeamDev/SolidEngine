#pragma once

#include "Build/solidAPI.hpp"
#include "EngineGenerated/rigidBody.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() RigidBody
    {
    public:

        bool enableGravity = true;
        bool isKinematic   = false;


        RigidBody() = default;
        ~RigidBody() = default;

        RigidBody_GENERATED
    };
} //!namespace

File_GENERATED