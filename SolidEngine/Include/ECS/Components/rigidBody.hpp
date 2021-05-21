#pragma once

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/rigidBody.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() RigidBody : public Components
    {
    public:

    	SLDField()
        bool enableGravity = true;
        SLDField()
    	bool isKinematic   = false;
        SLDField()
        float mass = 1.f;
        SLDField()
        float drag = 0;
        SLDField()
        float angularDrag = 0.05;

        RigidBody() = default;
        ~RigidBody() override = default ;

        void Init() override;
        void Release() override;

        RigidBody_GENERATED
    };
} //!namespace

File_GENERATED


