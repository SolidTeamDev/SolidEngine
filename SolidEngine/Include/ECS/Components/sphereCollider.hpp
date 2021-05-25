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

        SLDField()
        float staticFriction = 0.5f;
        SLDField()
        float dynamicFriction = 0.5f;
        SLDField()
        float restitution = 0.6f;

        physx::PxShape* sphereCollider = nullptr;
        physx::PxMaterial* physicMaterial = nullptr;
    public:

        SphereCollider() = default;
        ~SphereCollider() override = default;

        void Init() override;
        void Release() override;

        Vec3 GetCenter();
        void SetCenter(const Vec3& _center);

        float GetRadius();
        void SetRadius(float _radius);

        bool IsTrigger();
        void SetTrigger(bool _trigger);

        SphereCollider_GENERATED
    };
} //!namespace

File_GENERATED