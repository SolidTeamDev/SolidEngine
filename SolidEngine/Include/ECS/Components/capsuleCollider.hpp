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
    public:
        enum class SLDEnum() ECapsuleDirection
        {
            X_AXIS,
            Y_AXIS,
            Z_AXIS
        };
    private:

        SLDField()
        bool isTrigger = false;
        SLDField()
        Vec3 center;
        SLDField()
        float radius = 0.5f;
        SLDField()
        float height = 1;

        SLDField()
        float staticFriction = 0.5f;
        SLDField()
        float dynamicFriction = 0.5f;
        SLDField()
        float restitution = 0.6f;

        SLDField()
        ECapsuleDirection direction = ECapsuleDirection::Y_AXIS;

        physx::PxShape* capsuleCollider = nullptr;
        physx::PxMaterial* physicMaterial = nullptr;
    public:

        CapsuleCollider() = default;
        ~CapsuleCollider() override = default;

        void Init() override;
        void Release() override;

        Vec3 GetCenter();
        void SetCenter(const Vec3& _center);

        float GetRadius();
        void SetRadius(float _radius);

        float GetHeight();
        void SetHeight(float _height);

        ECapsuleDirection GetDirection();
        void SetCapsuleDirection(const ECapsuleDirection& _capsuleDirection);

        bool IsTrigger();
        void SetTrigger(bool _trigger);

        float GetStaticFriction();
        void SetStaticFriction(float _staticFriction);

        float GetDynamicFriction();
        void SetDynamicFriction(float _dynamicFriction);

        float GetRestitution();
        void SetRestitution(float _restitution);

        CapsuleCollider_GENERATED
    };
} //!namespace

File_GENERATED