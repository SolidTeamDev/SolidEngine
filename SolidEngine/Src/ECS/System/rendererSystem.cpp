#include "ECS/System/rendererSystem.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"

namespace Solid
{
    void RendererSystem::Update(const Renderer* _renderer, Camera& _camera)
    {

			    auto& transform = ecsManager.GetComponent<Transform>(child->GetEntity());
			    transform.SetParentMatrix(trans);
			    if(ecsManager.GotComponent<Animation>(child->GetEntity()))
                {
			        auto& anim = ecsManager.GetComponent<Animation>(child->GetEntity());
                    mesh->DrawMesh(meshRenderer.GetMaterials(), transform, _camera, &anim);
                }
                else
			        mesh->DrawMesh(meshRenderer.GetMaterials(), transform, _camera);
			    update(child,transform.GetMatrix()*trans );
	    	}
	    };


	    update(world, Mat4<float>::Identity);


	    /*for (auto entity : entities)
        {
	        auto& meshRenderer = ecsManager.GetComponent<MeshRenderer>(entity);
	        auto mesh = meshRenderer.GetMesh();
	        if(mesh == nullptr)
            {
	            continue;
            }

	        auto transform = ecsManager.GetComponent<Transform>(entity);
			mesh->DrawMesh(meshRenderer.GetMaterials(), transform, _camera);
        }
    }
} //!namespace
