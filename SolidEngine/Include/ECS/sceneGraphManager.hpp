#pragma once

#include <deque>
#include <string>
#include "Build/solidAPI.hpp"

#include "ECS/types.hpp"

namespace Solid
{
    class SOLID_API GameObject
    {
        Entity entity;
    public:

        std::string name = "New GameObject";

        std::vector<Components*> compsList;

        GameObject* parent = nullptr;
        std::deque<GameObject*> childs;

        GameObject(Entity _entity)
        {

            entity = _entity;
        }

        GameObject(GameObject* _parent, Entity _entity)
        {
            parent = _parent;
            entity = _entity;
        }

        ~GameObject()
        {
            for (GameObject* child : childs)
            {
                delete child;
            }
        }

        GameObject* AddToCurrent(Entity _entity)
        {
	        GameObject* obj = new GameObject(this,_entity);
            childs.push_back(obj);
            return obj;
        }

        void ReParentCurrent(GameObject* Parent)
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

        void RemoveCurrent()
        {
            if(parent != nullptr)
            {
                for (size_t i = 0 ; i < parent->childs.size() ; ++i)
                {
                    if (parent->childs.at(i) == this)
                    {
                        parent->childs.erase(parent->childs.begin() + i);
                        delete this;
                        break;
                    }
                }
            }
        }

        Entity GetEntity() const
        {
            return entity;
        }
    };

    class SOLID_API SceneGraphManager
    {
        GameObject world;

        GameObject* GetNodeFromEntity(Entity _entity, GameObject* node)
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
    public:

        SceneGraphManager():
        world(-1)
        {}

        GameObject* GetWorld()
        {
            return &world;
        }

        GameObject* GetNodeFromEntity(Entity _entity)
        {
            return GetNodeFromEntity(_entity,&world);
        }
    };
} //!namespace
