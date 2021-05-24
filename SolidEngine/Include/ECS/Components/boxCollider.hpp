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
        bool isTrigger = false;
        SLDField()
        Vec3 center;
        SLDField()
        Vec3 size {1.f,1.f,1.f};

        SLDField()
        float staticFriction = 0.5f;
        SLDField()
        float dynamicFriction = 0.5f;
        SLDField()
        float restitution = 0.6f;

        physx::PxShape* boxCollider = nullptr;
        physx::PxMaterial* physicMaterial = nullptr;
    public:

        BoxCollider() = default;
        ~BoxCollider() override = default;

        void Init() override;
        void Release() override;

        Vec3 GetCenter();
        void SetCenter(const Vec3& _center);

        Vec3 GetSize();
        void SetSize(const Vec3& _size);

        bool IsTrigger();
        void SetTrigger(bool _trigger);

        BoxCollider_GENERATED
    };
} //!namespace

File_GENERATED