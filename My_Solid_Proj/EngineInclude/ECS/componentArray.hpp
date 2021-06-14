#pragma once

#include "Build/solidAPI.hpp"
#include <array>
#include <unordered_map>

#include "Core/Debug/debug.hpp"

#include "ECS/types.hpp"
#include "ECS/Components/script.hpp"
namespace Solid
{
    class SOLID_API IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;

        virtual void EntityDestroyed(Entity _entity) = 0;
    };


    template<typename T>
    class ComponentArray : public IComponentArray
    {
    private:

        std::array<T, MAX_ENTITIES> componentArray {};

        std::unordered_map<Entity, size_t> entityToIndexMap {};
        std::unordered_map<size_t, Entity> indexToEntityMap {};

        size_t size {};
        int destroyEntityNumber = -1;

    public:

	    std::array<T, MAX_ENTITIES>& GetArray()
	    {
		    return componentArray;
	    }
	    std::unordered_map<Entity, size_t>& GetIndexesArray()
	    {
		    return entityToIndexMap;
	    }
	    std::unordered_map<size_t, Entity>& GetEntitiesArray()
	    {
		    return indexToEntityMap;
	    }
	    uint GetNumOfUsedComps()
	    {
	    	return size;
	    }

	    void SetArray(std::array<T, MAX_ENTITIES>&& _array)
	    {
	    	componentArray = _array;
	    }

    	Components* InsertData(Entity _entity, T _component)
        {
            if(entityToIndexMap.find(_entity) != entityToIndexMap.end())
            {
                //Log::Send("Cannot add component (Component already exist)",Log::ELogSeverity::WARNING);
                return &GetData(_entity);
            }

            size_t newIndex             = size;
            entityToIndexMap[_entity]   = newIndex;
            indexToEntityMap[newIndex]  = _entity;
            componentArray[newIndex]    = _component;

            size++;
            return &(componentArray[newIndex]);
        }

        Components* RemoveData(Entity _entity)
        {
            if(entityToIndexMap.find(_entity) == entityToIndexMap.end())
            {
                Log::Send("Error : Cannot remove component (Component doesn't exist)",Log::ELogSeverity::ERROR);
                return nullptr;
            }

            size_t indexOfRemovedEntity             = entityToIndexMap[_entity];
            size_t indexOfLastElement               = size-1;
            Components* c = (Components*)&(componentArray[indexOfRemovedEntity]);
	        c->Release();
            componentArray[indexOfRemovedEntity]    = componentArray[indexOfLastElement];

            Entity entityOfLastElement              = indexToEntityMap[indexOfLastElement];
            entityToIndexMap[entityOfLastElement]   = indexOfRemovedEntity;
            indexToEntityMap[indexOfRemovedEntity]  = entityOfLastElement;

            entityToIndexMap.erase(_entity);
            indexToEntityMap.erase(indexOfLastElement);

            size--;
            return c;
        }

        T& GetData(Entity _entity)
        {
            try
            {
                return componentArray.at(entityToIndexMap.at(_entity));
            }
            catch (const std::exception& e)
            {
                throw ThrowError("Error : Cannot get component (Component doesn't exist)",ESolidErrorCode::S_ASSERT);
            }
        }

        bool GotComponent(Entity _entity)
        {
            return entityToIndexMap.find(_entity) != entityToIndexMap.end();
        }

        void EntityDestroyed(Entity _entity) final
        {
            if(entityToIndexMap.find(_entity) != entityToIndexMap.end())
            {
                Components* c =RemoveData(_entity);
                destroyEntityNumber = _entity;
            }
        }

        int IsEntityDestroyed()
        {
	        int tmp = destroyEntityNumber;

	        destroyEntityNumber = -1;

            return tmp;
        }


    };

	template<>
	class ComponentArray<Script*> : public IComponentArray
	{
	private:

		std::array<Script*, MAX_ENTITIES> componentArray {};

		std::unordered_map<Entity, size_t> entityToIndexMap {};
		std::unordered_map<size_t, Entity> indexToEntityMap {};

		size_t size {};

	public:

		std::array<Script*, MAX_ENTITIES>& GetArray()
		{
			return componentArray;
		}
		std::unordered_map<Entity, size_t>& GetIndexesArray()
		{
			return entityToIndexMap;
		}
		std::unordered_map<size_t, Entity>& GetEntitiesArray()
		{
			return indexToEntityMap;
		}
		uint GetNumOfUsedComps()
		{
			return size;
		}

		void SetArray(std::array<Script*, MAX_ENTITIES>&& _array)
		{
			componentArray = _array;
		}

		Components* InsertData(Entity _entity, Script* _component)
		{
			if(entityToIndexMap.find(_entity) != entityToIndexMap.end())
			{
				Log::Send("Cannot add component (Component already exist)",Log::ELogSeverity::ERROR);
				return nullptr;
			}

			size_t newIndex             = size;
			entityToIndexMap[_entity]   = newIndex;
			indexToEntityMap[newIndex]  = _entity;
			componentArray[newIndex]    = _component;

			size++;
			return (componentArray[newIndex]);
		}

		Components* RemoveData(Entity _entity)
		{
			if(entityToIndexMap.find(_entity) == entityToIndexMap.end())
			{
				Log::Send("Error : Cannot remove component (Component doesn't exist)",Log::ELogSeverity::ERROR);
				return nullptr;
			}

			size_t indexOfRemovedEntity             = entityToIndexMap[_entity];
			size_t indexOfLastElement               = size-1;
			Components* c = (Components*)&(componentArray[indexOfRemovedEntity]);
			componentArray[indexOfRemovedEntity]    = componentArray[indexOfLastElement];

			Entity entityOfLastElement              = indexToEntityMap[indexOfLastElement];
			entityToIndexMap[entityOfLastElement]   = indexOfRemovedEntity;
			indexToEntityMap[indexOfRemovedEntity]  = entityOfLastElement;

			entityToIndexMap.erase(_entity);
			indexToEntityMap.erase(indexOfLastElement);

			size--;
			return c;
		}

		Script*& GetData(Entity _entity)
		{
			try
			{
				return componentArray.at(entityToIndexMap.at(_entity));
			}
			catch (const std::exception& e)
			{
				throw ThrowError("Error : Cannot get component (Component doesn't exist)",ESolidErrorCode::S_ASSERT);
			}
		}

		bool GotComponent(Entity _entity)
		{
			return entityToIndexMap.find(_entity) != entityToIndexMap.end();
		}

		void EntityDestroyed(Entity _entity) final
		{
			if(entityToIndexMap.find(_entity) != entityToIndexMap.end())
			{
				RemoveData(_entity);
			}
		}


	};
} //!namespace
