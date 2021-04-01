#include "ECS/System/rendererSystem.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"

namespace Solid
{
    void RendererSystem::Update(const Renderer* _renderer, Camera& _camera)
    {
        for (auto entity : entities)
        {
            auto& meshRenderer = ecsManager.GetComponent<MeshRenderer>(entity);
            if(meshRenderer.mesh == nullptr)
                continue;

            auto transform = ecsManager.GetComponent<Transform>(entity);
			//meshRenderer.shader->SetMVP(transform,_camera);
			meshRenderer.mesh->DrawMesh(meshRenderer.materials, transform, _camera);

            //_renderer->SetShaderMVP(meshRenderer.shader,transform,_camera);
            //_renderer->DrawMesh(meshRenderer.mesh);
        }
    }
} //!namespace
