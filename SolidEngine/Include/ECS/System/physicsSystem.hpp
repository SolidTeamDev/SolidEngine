#pragma once

#include "Physics/physics.hpp"
#include "ECS/system.hpp"

namespace Solid
{
    class PhysicsSystem : public System
    {
    public:
        PhysicsSystem(ECSManager& _ecsManager): System(_ecsManager){};

        void Update(const Physics& _physics, float _deltaTime);
    };
} //!namespace
