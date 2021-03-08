#pragma once

#include <set>
#include "Build/solidAPI.hpp"
#include "types.hpp"

namespace Solid
{
    class SOLID_API System
    {
    public:
        std::set<Entity> entities;
        class ECSManager& ecsManager;

        System(ECSManager& _ecsManager) : ecsManager(_ecsManager) {};
    };
} //!namespace

#include "ECS/ecsManager.hpp"