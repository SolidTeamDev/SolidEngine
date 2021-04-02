#pragma once

#include "Build/solidAPI.hpp"
#include "EngineGenerated/rigidBody.sld.hpp"

namespace Solid
{
    class SOLID_API RigidBody
    {
    public:

        bool enableGravity = true;
        bool isKinematic   = false;


        RigidBody() = default;
        ~RigidBody() = default;
    };
} //!namespace