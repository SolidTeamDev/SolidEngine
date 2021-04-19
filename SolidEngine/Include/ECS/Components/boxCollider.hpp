#pragma once

#include "Core/Maths/Vector/vector3.hpp"
#include <PxShape.h>

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/boxCollider.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() BoxCollider : public Components
    {
        SLDField()
        bool isTrigger;
        SLDField()
        Vec3 center;
        SLDField()
        Vec3 size {1.f,1.f,1.f};
        physx::PxBoxGeometry* boxCollider = nullptr;
    public:

        BoxCollider();
        ~BoxCollider() = default;

        BoxCollider_GENERATED
    };
} //!namespace

File_GENERATED