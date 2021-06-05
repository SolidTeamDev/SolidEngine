#include "ECS/System/rendererSystem.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"

namespace Solid
{
	void RendererSystem::Update(const Renderer* _renderer, Camera& _camera)
    {
        for (auto entity : entities)
        {
            auto &meshRenderer = ecsManager.GetComponent<MeshRenderer>(entity);
            auto mesh = meshRenderer.GetMesh();
            if (mesh == nullptr)
            {
                continue;
            }

            auto& transform = ecsManager.GetComponent<Transform>(entity);
            if(ecsManager.GotComponent<Animation>(entity))
            {
                auto& anim = ecsManager.GetComponent<Animation>(entity);
                mesh->DrawMesh(meshRenderer.GetMaterials(), transform, _camera, &anim);
            }
            else
                mesh->DrawMesh(meshRenderer.GetMaterials(), transform, _camera);
        }
    }
} //!namespace
