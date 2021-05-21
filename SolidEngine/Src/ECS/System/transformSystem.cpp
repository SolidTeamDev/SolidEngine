//
// Created by ryan1 on 21/05/2021.
//
#include <functional>
#include "ECS/Components/transform.hpp"
#include "ECS/System/transformSystem.hpp"

using namespace Solid;

void TransformSystem::Update()
{
	GameObject* world =ecsManager.GetWorld();
	std::function<void(GameObject*, const Mat4<float>&)> update = [&](GameObject* object,const Mat4<float>& trans)
	{
		for(GameObject* child : object->childs)
		{

			if(!ecsManager.GotComponent<Transform>(child->GetEntity()))
				continue ;


			auto& transform = ecsManager.GetComponent<Transform>(child->GetEntity());
			transform.SetParentMatrix(trans);
			update(child,transform.GetMatrix()*trans );
		}
	};
	update(world, Mat4<float>::Identity);

}

void TransformSystem::FixedUpdate()
{
	for (auto entity : entities)
	{
		Transform& scriptL = ecsManager.GetComponent<Transform>(entity);



	}
}

void TransformSystem::LateUpdate()
{
	for (auto entity : entities)
	{
		Transform& scriptL = ecsManager.GetComponent<Transform>(entity);



	}
}