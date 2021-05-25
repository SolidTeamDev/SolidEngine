#include "ECS/System/rendererSystem.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"

namespace Solid
{
    void RendererSystem::Update(const Renderer* _renderer, Camera& _camera)
    {
	    GameObject* world =ecsManager.GetWorld();
	    std::function<void(GameObject*, const Mat4<float>&)> update = [&](GameObject* object,const Mat4<float>& trans)
	    {
	    	for(GameObject* child : object->childs)
	    	{
	    		if(!ecsManager.GotComponent<MeshRenderer>(child->GetEntity()))
				    continue ;
			    if(!ecsManager.GotComponent<Transform>(child->GetEntity()))
				    continue ;
			    auto& meshRenderer = ecsManager.GetComponent<MeshRenderer>(child->GetEntity());
			    auto mesh = meshRenderer.GetMesh();
			    if(mesh == nullptr)
			    {
				    continue;
			    }

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
        }*/
    }
} //!namespace
