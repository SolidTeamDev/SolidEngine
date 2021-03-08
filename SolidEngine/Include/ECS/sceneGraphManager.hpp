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
            name += " PAR:"+std::to_string(_parent->entity)+" Ent:"+std::to_string(_entity);
        }

        ~GameObject()
        {
            for (GameObject* child : childs)
            {
                delete child;
            }
        }

        void AddToCurrent(Entity _entity)
        {
            childs.push_back(new GameObject(this,_entity));
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
                    result = node;
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
