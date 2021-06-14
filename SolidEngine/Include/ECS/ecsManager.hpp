#pragma once

#include <memory>
#include <functional>
#include "Build/solidAPI.hpp"
#include "componentManager.hpp"
#include "entityManager.hpp"
#include "systemManager.hpp"
#include "sceneGraphManager.hpp"
#include "ECS/Components/transform.hpp"

namespace Solid
{
	struct FromSceneGraphMgr{ void* destrotroyedEnt = nullptr;};
    class SOLID_API ECSManager
    {
    private:
        std::unique_ptr<ComponentManager> componentManager;
        std::unique_ptr<EntityManager> entityManager;
        std::unique_ptr<SystemManager> systemManager;
        std::unique_ptr<SceneGraphManager> sceneGraphManager;
	    std::vector<std::function<void(Entity, void*)>> DestroyedEntityCallbacks;
    public:

        void Init()
        {
            componentManager = std::make_unique<ComponentManager>();
            entityManager = std::make_unique<EntityManager>();
            systemManager = std::make_unique<SystemManager>();
            sceneGraphManager = std::make_unique<SceneGraphManager>();
        }

		void AddDestroyedEntityCallback(const std::function<void(Entity, void*)>& _func)
		{
			DestroyedEntityCallbacks.push_back(_func);
		}

        GameObject* CreateEntity()
        {
            Entity temp = entityManager->CreateEntity();
            GameObject* go = sceneGraphManager->GetWorld()->AddToCurrent(temp);
            go->transform = AddComponent<Transform>(go,Transform());
            return go;
        }

	    GameObject* CreateEntity(Entity _parent)
        {
            Entity temp = entityManager->CreateEntity();
            GameObject* go = sceneGraphManager->GetNodeFromEntity(_parent)->AddToCurrent(temp);
            go->transform = AddComponent<Transform>(go,Transform());
            return go;
        }
	    GameObject* CreateEntity(std::string _name)
	    {
		    Entity temp = entityManager->CreateEntity();
		    GameObject* go =sceneGraphManager->GetWorld()->AddToCurrent(temp);
		    go->name = _name;
		    go->transform = AddComponent<Transform>(go,Transform());
		    return go;
	    }
	    GameObject* CreateEntity(std::string _name,Entity _parent)
	    {
		    Entity temp = entityManager->CreateEntity();
		    GameObject* obj = sceneGraphManager->GetNodeFromEntity(_parent)->AddToCurrent(temp);
		    obj->name = _name;
		    obj->transform = AddComponent<Transform>(obj,Transform());
		    return obj;
	    }

	    GameObject* Instantiate(std::string _prefabName, GameObject* parent, std::string _name ="" )
	    {
        	return sceneGraphManager->Instantiate(_prefabName, parent, _name);
	    }
	    GameObject* Instantiate(GameObject* duplicatedGO, GameObject* parent, std::string _name ="" )
	    {
		    return sceneGraphManager->Instantiate(duplicatedGO, parent, _name);
	    }
        void DestroyEntity(Entity _entity)
        {
            entityManager->DestroyEntity(_entity);

            componentManager->EntityDestroyed(_entity);

            systemManager->EntityDestroyed(_entity);
			GameObject* go = sceneGraphManager->GetNodeFromEntity(_entity);
            go->RemoveCurrent();
	        for(auto& elt : DestroyedEntityCallbacks)
	        {
		        elt(_entity, go);
	        }
        }

	    void DestroyEntity(Entity _entity, FromSceneGraphMgr _noDel)
	    {
		    entityManager->DestroyEntity(_entity);

		    componentManager->EntityDestroyed(_entity);

		    systemManager->EntityDestroyed(_entity);
		    for(auto& elt : DestroyedEntityCallbacks)
		    {
			    elt(_entity, _noDel.destrotroyedEnt);
		    }

	    }
        GameObject* GetGameObjectFromEntity(Entity _e)
        {
            return sceneGraphManager->GetNodeFromEntity(_e);
        }

        GameObject* FindGameObjectByName(std::string name, GameObject* _go = nullptr)
        {
            GameObject* go = nullptr;
            if(_go == nullptr)
                 go = sceneGraphManager->GetWorld();
            else
                go = _go;

            for (auto entity : go->childs)
            {
                if(entity->name == name)
                    return entity;
            }

            for (auto entity : go->childs)
                FindGameObjectByName(name, entity);

            Log::Send("No GameObject found", Log::ELogSeverity::WARNING);
            return nullptr;
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
	    template<>
	    Script** AddComponent(GameObject* _entity, Script* _component)
	    {
		    //HERE
		    Components* c =componentManager->AddComponent<Script*>(_entity->GetEntity(),_component);
		    c->gameObject = _entity;
		    _entity->compsList.push_back(c);
		    auto signature = entityManager->GetSignature(_entity->GetEntity());
		    signature.set(componentManager->GetComponentType<Script*>(), true);
		    entityManager->SetSignature(_entity->GetEntity(),signature);

		    systemManager->EntitySignatureChanged(_entity->GetEntity(),signature);
		    c->Init();
		    return (Script**)c;
	    }
	    template<typename T>
	    std::shared_ptr<ComponentArray<T>> GetCompArray()
	    {

		    return componentManager->GetComponentArray<T>();
	    }
        template<typename T>
        void RemoveComponent(GameObject* _entity)
        {
            Components* c = componentManager->RemoveComponent<T>(_entity->GetEntity());

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
