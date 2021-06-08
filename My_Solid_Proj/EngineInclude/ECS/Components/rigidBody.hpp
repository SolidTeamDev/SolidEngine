#pragma once

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"

#include "EngineGenerated/rigidBody.sld.hpp"

namespace Solid SLDNamespace()
{
    class Vec3;
    class SOLID_API SLDClass() RigidBody : public Components
    {
    private:
        SLDField()
        bool gravity = true;
        SLDField()
        bool isKinematic = false;
        SLDField()
        float mass = 1.f;
        SLDField()
        float drag = 0;
        SLDField()
        float angularDrag = 0.05;

        SLDField()
        bool freezePosX = false;
        SLDField()
        bool freezePosY = false;
        SLDField()
        bool freezePosZ = false;

        SLDField()
        bool freezeRotX = false;
        SLDField()
        bool freezeRotY = false;
        SLDField()
        bool freezeRotZ = false;

    public:

        RigidBody() = default;
        ~RigidBody() override = default;

        void Init() override;
        void Release() override;

        void AddForce(const Vec3& _force);
        void AddImpulse(const Vec3& _force);

        void ResetVelocity();

        Vec3 GetLinearVelocity();
        Vec3 GetAngularVelocity();

        void SetLinearVelocity(const Vec3& _velocity);
        void SetAngularVelocity(const Vec3& _velocity);

        bool IsGravityEnabled();
        void EnableGravity(bool _enableGravity);

        bool IsKinematic();
        void SetKinematic(bool _kinematic);

        float GetMass();
        void SetMass(float _mass);

        float GetDrag();
        void SetDrag(float _drag);

        float GetAngularDrag();
        void SetAngularDrag(float _angularDrag);

        bool IsFreezePosX();
        bool IsFreezePosY();
        bool IsFreezePosZ();

        void FreezePosX(bool _freezePosX);
        void FreezePosY(bool _freezePosY);
        void FreezePosZ(bool _freezePosZ);

        bool IsFreezeRotX();
        bool IsFreezeRotY();
        bool IsFreezeRotZ();

        void FreezeRotX(bool _freezeRotX);
        void FreezeRotY(bool _freezeRotY);
        void FreezeRotZ(bool _freezeRotZ);


        RigidBody_GENERATED
    };
} //!namespace

File_GENERATED


