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
        float mass = 1.f;
        float drag = 0;
        float angularDrag = 0.05;

        RigidBody() = default;
        ~RigidBody() = default;
    };
} //!namespace
