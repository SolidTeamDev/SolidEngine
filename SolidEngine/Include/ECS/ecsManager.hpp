#pragma once

#include <memory>
#include "Build/solidAPI.hpp"
#include "componentManager.hpp"
#include "entityManager.hpp"
#include "systemManager.hpp"
#include "sceneGraphManager.hpp"
#include "ECS/Components/transform.hpp"

namespace Solid
{
	struct FromSceneGraphMgr{};
    class SOLID_API ECSManager
    {
    private:
        std::unique_ptr<ComponentManager> componentManager;
        std::unique_ptr<EntityManager> entityManager;
        std::unique_ptr<SystemManager> systemManager;
        std::unique_ptr<SceneGraphManager> sceneGraphManager;
    public:

        void Init()
        {
            componentManager = std::make_unique<ComponentManager>();
            entityManager = std::make_unique<EntityManager>();
            systemManager = std::make_unique<SystemManager>();
            sceneGraphManager = std::make_unique<SceneGraphManager>();
        }

        GameObject* CreateEntity()
        {
            Entity temp = entityManager->CreateEntity();

            return sceneGraphManager->GetWorld()->AddToCurrent(temp);
        }

	    GameObject* CreateEntity(Entity _parent)
        {
            Entity temp = entityManager->CreateEntity();
            return sceneGraphManager->GetNodeFromEntity(_parent)->AddToCurrent(temp);
        }
	    GameObject* CreateEntity(std::string _name)
	    {
		    Entity temp = entityManager->CreateEntity();
		    GameObject* obj =sceneGraphManager->GetWorld()->AddToCurrent(temp);
		    obj->name = _name;
		    return obj;
	    }
	    GameObject* CreateEntity(std::string _name,Entity _parent)
	    {
		    Entity temp = entityManager->CreateEntity();
		    GameObject* obj = sceneGraphManager->GetNodeFromEntity(_parent)->AddToCurrent(temp);
		    obj->name = _name;
		    return obj;
	    }

        void DestroyEntity(Entity _entity)
        {
            entityManager->DestroyEntity(_entity);

            componentManager->EntityDestroyed(_entity);

            systemManager->EntityDestroyed(_entity);

            sceneGraphManager->GetNodeFromEntity(_entity)->RemoveCurrent();
        }

	    void DestroyEntity(Entity _entity, FromSceneGraphMgr _noDel)
	    {
		    entityManager->DestroyEntity(_entity);

		    componentManager->EntityDestroyed(_entity);

		    systemManager->EntityDestroyed(_entity);

	    }
        GameObject* GetGameObjectFromEntity(Entity _e)
        {
            return sceneGraphManager->GetNodeFromEntity(_e);
        }
        GameObject* GetWorld()
        {
            return sceneGraphManager->GetWorld();
        }

        template<typename T>
        void RegisterComponent()
        {
            componentManager->RegisterComponent<T>();
        }

        template<typename T>
        T* AddComponent(GameObject* _entity, T _component)
        {
        	//HERE
            Components* c =componentManager->AddComponent<T>(_entity->GetEntity(),_component);
            c->gameObject = _entity;
			_entity->compsList.push_back(c);
            auto signature = entityManager->GetSignature(_entity->GetEntity());
            signature.set(componentManager->GetComponentType<T>(), true);
            entityManager->SetSignature(_entity->GetEntity(),signature);

            systemManager->EntitySignatureChanged(_entity->GetEntity(),signature);
            c->Init();
            return (T*)c;
        }

        template<typename T>
        void RemoveComponent(GameObject* _entity)
        {
            Components* c = componentManager->RemoveComponent<T>(_entity);
            c->Release();
	        for (auto it= _entity->compsList.begin(); it != _entity->compsList.end(); ++it)
	        {
		        if(*it == c)
		        {
		        	_entity->compsList.erase(it);
		        	break;
		        }
	        }

            auto signature = entityManager->GetSignature(_entity->GetEntity());
            signature.set(componentManager->GetComponentType<T>(), false);
            entityManager->SetSignature(_entity->GetEntity(),signature);

            systemManager->EntitySignatureChanged(_entity->GetEntity(),signature);
        }

        template<typename T>
        T& GetComponent(Entity _entity)
        {
            return componentManager->GetComponent<T>(_entity);

        }

        template<typename T>
        ComponentType GetComponentType()
        {
            return componentManager->GetComponentType<T>();
        }

        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            return systemManager->RegisterSystem<T>(*this);
        }

        template<typename T>
        void SetSystemSignature(Signature _signature)
        {
            systemManager->SetSignature<T>(_signature);
        }

        template<typename T>
        bool GotComponent(Entity _entity)
        {
            return componentManager->GotComponent<T>(_entity);
        }
    };
} //!namespace
