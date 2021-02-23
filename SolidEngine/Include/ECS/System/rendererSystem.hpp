#pragma once

#include "Build/solidAPI.hpp"

#include "ECS/system.hpp"

namespace Solid
{
    class SOLID_API RendererSystem : public System
    {
    public:
        RendererSystem(ECSManager& _ecsManager): System(_ecsManager){};

        void Update();
    };
} //!namespace
