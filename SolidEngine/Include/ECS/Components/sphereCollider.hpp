#pragma once

#include "Core/Maths/Vector/vector3.hpp"
#include <PxShape.h>

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/sphereCollider.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() SphereCollider : public Components
    {
        SLDField()
        bool isTrigger = false;
        SLDField()
        Vec3 center;
        SLDField()
        float radius = 1.f;

        physx::PxShape* sphereCollider = nullptr;
    public:

        SphereCollider() = default;
        ~SphereCollider() override = default;

        void Init() override;
        void Release() override;

        SphereCollider_GENERATED
    };
} //!namespace

File_GENERATED