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

        GameObject(Entity _entity);

        GameObject(GameObject* _parent, Entity _entity);

        ~GameObject();

        GameObject* AddToCurrent(Entity _entity);

        void ReParentCurrent(GameObject* Parent);

        void RemoveCurrent();

        Entity GetEntity() const;
    };

    class SOLID_API SceneGraphManager
    {
        GameObject world;

        GameObject* GetNodeFromEntity(Entity _entity, GameObject* node);
    public:

        SceneGraphManager();

        GameObject* GetWorld();

        GameObject* GetNodeFromEntity(Entity _entity);
    };
} //!namespace
