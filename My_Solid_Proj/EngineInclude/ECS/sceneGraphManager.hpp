#pragma once

#include <deque>
#include <string>
#include "Build/solidAPI.hpp"

#include "ECS/types.hpp"
#include <PxActor.h>
#include "ECS/Components/transform.hpp"

namespace Solid
{
    class SOLID_API GameObject
    {
        Entity entity;
    public:

        std::string name = "New GameObject";
        std::string tag = "";
        physx::PxActor* physicsActor = nullptr;

        Transform* transform = nullptr;
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

	    template<class T>
	    T * AddComp(const std::string &className, std::vector<char> &buffer, std::uint64_t &readPos,
	                 GameObject *go, Components *cmp, std::size_t FieldNum, std::size_t cmpNameSize);

        void AddAllComps(GameObject *elt, std::vector<char> &buffer, uint64_t &ReadPos);
    public:

        SceneGraphManager();

        GameObject* GetWorld();

        GameObject* GetNodeFromEntity(Entity _entity);

        GameObject* Instantiate(std::string _prefabName, GameObject* parent, std::string _name  );
    };
} //!namespace
