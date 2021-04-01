#pragma once

#include "Build/solidAPI.hpp"

#include "ECS/system.hpp"
#include "ECS/Components/camera.hpp"

namespace Solid
{
    class SOLID_API AudioSystem : public System
    {
    public:
        AudioSystem(ECSManager& _ecsManager): System(_ecsManager){};

        void Update(Camera& _camera);
    };
} //!namespace
