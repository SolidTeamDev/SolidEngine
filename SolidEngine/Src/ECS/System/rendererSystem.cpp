#include "ECS/System/rendererSystem.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"

namespace Solid
{
    void RendererSystem::Update()
    {
        for (auto entity : entities)
        {
            auto meshRenderer = ecsManager.GetComponent<MeshRenderer>(entity);
            if(meshRenderer.shader == nullptr || meshRenderer.mesh == nullptr)
                continue;

            auto transform = ecsManager.GetComponent<Transform>(entity);

            for (auto& subMesh : meshRenderer.mesh->Meshes)
            {

            }
        }
    }
} //!namespace