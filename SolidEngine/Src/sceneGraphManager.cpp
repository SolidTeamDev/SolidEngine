//
// Created by ryan1 on 14/04/2021.
//
#include "ECS/sceneGraphManager.hpp"

#include "Core/engine.hpp"

Solid::GameObject::GameObject(Solid::Entity _entity)
{

	entity = _entity;
}

Solid::GameObject::GameObject(Solid::GameObject *_parent, Solid::Entity _entity)
{
	parent = _parent;
	entity = _entity;
}

Solid::GameObject::~GameObject()
{
	for (GameObject* child : childs)
	{
		delete child;
	}
}

Solid::GameObject *Solid::GameObject::AddToCurrent(Solid::Entity _entity)
{
	GameObject* obj = new GameObject(this,_entity);
	childs.push_back(obj);
	return obj;
}

void Solid::GameObject::ReParentCurrent(Solid::GameObject *Parent)
{
	GameObject* ParentTemp =this->parent;
	this->parent = Parent;
	for (auto it = ParentTemp->childs.begin(); it != ParentTemp->childs.end(); it++)
	{
		if((*it) == this)
		{
			ParentTemp->childs.erase(it);
			break;
		}
	}
	Parent->childs.push_back(this);
}

void Solid::GameObject::RemoveCurrent()
{
	if(parent != nullptr)
	{
		for (size_t i = 0 ; i < parent->childs.size() ; ++i)
		{
			if (parent->childs.at(i) == this)
			{
				parent->childs.erase(parent->childs.begin() + i);
				Engine::GetInstance()->ecsManager.DestroyEntity(entity,FromSceneGraphMgr{});
				delete this;
				break;
			}
		}
	}
}

Solid::Entity Solid::GameObject::GetEntity() const
{
	return entity;
}

Solid::GameObject *Solid::SceneGraphManager::GetNodeFromEntity(Solid::Entity _entity, Solid::GameObject *node)
{
	if(node == nullptr)
		return nullptr;

	GameObject* result = nullptr;

	for (GameObject* node : node->childs)
	{
		if(node->GetEntity() == _entity)
		{
			result = node;
			break;
		}
		else
		{
			result = GetNodeFromEntity(_entity,node);
			if(result != nullptr)
				break;
		}
	}

	return result;
}

Solid::SceneGraphManager::SceneGraphManager() :
		world(-1)
{}

Solid::GameObject *Solid::SceneGraphManager::GetWorld()
{
	return &world;
}

Solid::GameObject *Solid::SceneGraphManager::GetNodeFromEntity(Solid::Entity _entity)
{
	return GetNodeFromEntity(_entity,&world);
}
