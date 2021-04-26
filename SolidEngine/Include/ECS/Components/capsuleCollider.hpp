#pragma once

#include "Core/Maths/Vector/vector3.hpp"
#include <PxShape.h>

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/capsuleCollider.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() CapsuleCollider : public Components
    {
        SLDField()
        bool isTrigger = false;
        SLDField()
        Vec3 center;
        SLDField()
        float radius = 0.5f;
        SLDField()
        float height = 1;

        physx::PxShape* capsuleCollider = nullptr;
    public:

        CapsuleCollider() = default;
        ~CapsuleCollider() override = default;

        void Init() override;
        void Release() override;

        CapsuleCollider_GENERATED
    };
} //!namespace

File_GENERATED