#pragma once

#include "Build/solidAPI.hpp"

#include "ECS/system.hpp"
#include "ECS/Components/camera.hpp"
#include "Rendering/renderer.hpp"

namespace Solid
{
    class SOLID_API RendererSystem : public System
    {
    public:
        RendererSystem(ECSManager& _ecsManager): System(_ecsManager){};

        void Update(const Renderer* _renderer, Camera& _camera);

    };
} //!namespace
