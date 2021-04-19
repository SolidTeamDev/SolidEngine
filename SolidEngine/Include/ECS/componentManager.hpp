#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <string_view>
#include "types.hpp"
#include "componentArray.hpp"

namespace Solid
{
    class SOLID_API ComponentManager
    {
    private:

        std::unordered_map<std::string_view, ComponentType> componentTypes{};

        std::unordered_map<std::string_view, std::shared_ptr<IComponentArray>> componentArrays{};

        ComponentType nextComponentType{};

        template<typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            std::string_view typeName = typeid(T).name();

            try
            {
                return std::static_pointer_cast<ComponentArray<T>>(componentArrays.at(typeName));
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error : Cannot get component array (Component not registered)" << std::endl;
                return nullptr;
            }
        }

    public:

        template<typename T>
        void RegisterComponent()
        {
            const char *typeName = typeid(T).name();

            if (componentTypes.find(typeName) != componentTypes.end())
            {
                std::cerr << "Error : Cannot register component type (Component type already registered)" << std::endl;
                return;
            }

            componentTypes.insert({typeName, nextComponentType});

            componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

            nextComponentType++;
        }

        template<typename T>
        ComponentType GetComponentType()
        {
            std::string_view typeName = typeid(T).name();

            if (componentTypes.find(typeName) == componentTypes.end())
            {
                std::cerr << "Error : Cannot get component type (Component not registered)" << std::endl;
                return 0;
            }

            return componentTypes[typeName];
        }

        template<typename T>
        Components* AddComponent(Entity _entity, T _component)
        {
           return GetComponentArray<T>()->InsertData(_entity, _component);
        }

        template<typename T>
        Components* RemoveComponent(Entity _entity)
        {
            return GetComponentArray<T>()->RemoveData(_entity);
        }

        template<typename T>
        T &GetComponent(Entity _entity)
        {
            return GetComponentArray<T>()->GetData(_entity);
        }

        template<typename T>
        bool GotComponent(Entity _entity)
        {
            return GetComponentArray<T>()->GotComponent(_entity);
        }

        void EntityDestroyed(Entity _entity)
        {
            for (auto const &pair : componentArrays)
            {
                auto const &component = pair.second;

                component->EntityDestroyed(_entity);
            }
        }
    };
} //!namespace