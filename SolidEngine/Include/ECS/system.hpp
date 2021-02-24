#pragma once

#include <set>
#include "Build/solidAPI.hpp"
#include "types.hpp"

class SOLID_API System
{
public:
    std::set<Entity> entities;
    class ECSManager& ecsManager;

    System(ECSManager& _ecsManager) : ecsManager(_ecsManager) {};
};

#include "ECS/ecsManager.hpp"