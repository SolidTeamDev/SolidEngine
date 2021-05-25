#include "ECS/Components/rigidBody.hpp"
#include "Core/engine.hpp"
#include "Core/Maths/Vector/vector3.hpp"

namespace Solid
{
    void RigidBody::Init()
    {
        Physics& physics = Engine::GetInstance()->physics;
        physics.ConvertActor(gameObject,PhysicsActorType::DYNAMIC);

        EnableGravity(gravity);
        SetKinematic(isKinematic);
        SetMass(mass);
        SetDrag(drag);
        SetAngularDrag(angularDrag);

        FreezePosX(freezePosX);
        FreezePosY(freezePosY);
        FreezePosZ(freezePosZ);

        FreezeRotX(freezeRotX);
        FreezeRotY(freezeRotY);
        FreezeRotZ(freezeRotZ);
    }
	void Solid::RigidBody::Release()
	{
        Physics& physics = Engine::GetInstance()->physics;
        physics.ConvertActor(gameObject,PhysicsActorType::STATIC);

		Components::Release();
	}

    void RigidBody::AddForce(const Vec3& _force)
    {
        Physics& physics = Engine::GetInstance()->physics;

        physics.AddForce(gameObject->physicsActor,_force,physx::PxForceMode::Enum::eFORCE);
    }

    void RigidBody::AddImpulse(const Vec3& _force)
    {
        Physics& physics = Engine::GetInstance()->physics;

        physics.AddForce(gameObject->physicsActor,_force,physx::PxForceMode::Enum::eIMPULSE);
    }

    void RigidBody::ResetVelocity()
    {
        SetLinearVelocity(Vec3(0));
        SetAngularVelocity(Vec3(0));
    }

    Vec3 RigidBody::GetLinearVelocity()
    {
        Physics& physics = Engine::GetInstance()->physics;

        return physics.GetLinearVelocity(gameObject->physicsActor);
    }

    Vec3 RigidBody::GetAngularVelocity()
    {
        Physics& physics = Engine::GetInstance()->physics;

        return physics.GetAngularVelocity(gameObject->physicsActor);
    }

    void RigidBody::SetLinearVelocity(const Vec3 &_velocity)
    {
        Physics& physics = Engine::GetInstance()->physics;

        physics.SetLinearVelocity(gameObject->physicsActor,_velocity);
    }

    void RigidBody::SetAngularVelocity(const Vec3 &_velocity)
    {
        Physics& physics = Engine::GetInstance()->physics;

        physics.SetAngularVelocity(gameObject->physicsActor,_velocity);
    }

    bool RigidBody::IsGravityEnabled()
    {
        return gravity;
    }

    void RigidBody::EnableGravity(bool _enableGravity)
    {
        Physics& physics = Engine::GetInstance()->physics;

        gravity = _enableGravity;
        physics.EnableGravity(gameObject->physicsActor,gravity);
    }

    bool RigidBody::IsKinematic()
    {
        return isKinematic;
    }

    void RigidBody::SetKinematic(bool _kinematic)
    {
        Physics& physics = Engine::GetInstance()->physics;

        isKinematic = _kinematic;
        physics.SetKinematic(gameObject->physicsActor, isKinematic);
    }

    float RigidBody::GetMass()
    {
        return mass;
    }

    void RigidBody::SetMass(float _mass)
    {
        Physics& physics = Engine::GetInstance()->physics;

        mass = _mass;
        physics.SetMass(gameObject->physicsActor, mass);
    }

    float RigidBody::GetDrag()
    {
        return drag;
    }

    void RigidBody::SetDrag(float _drag)
    {
        Physics& physics = Engine::GetInstance()->physics;

        drag = _drag;
        physics.SetDrag(gameObject->physicsActor, drag);
    }

    float RigidBody::GetAngularDrag()
    {
        return angularDrag;
    }

    void RigidBody::SetAngularDrag(float _angularDrag)
    {
        Physics& physics = Engine::GetInstance()->physics;

        angularDrag = _angularDrag;
        physics.SetAngularDrag(gameObject->physicsActor, angularDrag);
    }

    bool RigidBody::IsFreezePosX()
    {
        return freezePosX;
    }

    bool RigidBody::IsFreezePosY()
    {
        return freezePosY;
    }

    bool RigidBody::IsFreezePosZ()
    {
        return freezePosZ;
    }

    void RigidBody::FreezePosX(bool _freezePosX)
    {
        Physics& physics = Engine::GetInstance()->physics;

        freezePosX = _freezePosX;
        physics.FreezePosX(gameObject->physicsActor, freezePosX);
    }

    void RigidBody::FreezePosY(bool _freezePosY)
    {
        Physics& physics = Engine::GetInstance()->physics;

        freezePosY = _freezePosY;
        physics.FreezePosY(gameObject->physicsActor, freezePosY);
    }

    void RigidBody::FreezePosZ(bool _freezePosZ)
    {
        Physics& physics = Engine::GetInstance()->physics;

        freezePosZ = _freezePosZ;
        physics.FreezePosZ(gameObject->physicsActor, freezePosZ);
    }

    bool RigidBody::IsFreezeRotX()
    {
        return freezeRotX;
    }

    bool RigidBody::IsFreezeRotY()
    {
        return freezeRotY;
    }

    bool RigidBody::IsFreezeRotZ()
    {
        return freezeRotZ;
    }

    void RigidBody::FreezeRotX(bool _freezeRotX)
    {
        Physics& physics = Engine::GetInstance()->physics;

        freezeRotX = _freezeRotX;
        physics.FreezeRotX(gameObject->physicsActor, freezeRotX);
    }

    void RigidBody::FreezeRotY(bool _freezeRotY)
    {
        Physics& physics = Engine::GetInstance()->physics;

        freezeRotY = _freezeRotY;
        physics.FreezeRotY(gameObject->physicsActor, freezeRotY);
    }

    void RigidBody::FreezeRotZ(bool _freezeRotZ)
    {
        Physics& physics = Engine::GetInstance()->physics;

        freezeRotZ = _freezeRotZ;
        physics.FreezeRotZ(gameObject->physicsActor, freezeRotZ);
    }
} //!namespace