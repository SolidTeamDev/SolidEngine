#pragma once
#include "Build/solidAPI.hpp"
#include "ECS/sceneGraphManager.hpp"
#include "Core/engine.hpp"

namespace Solid
{
    class HierarchyTreeInterface
    {
    public:
        HierarchyTreeInterface()  = default;
        ~HierarchyTreeInterface() = default;

        void Draw(Engine* _engine);
        void DrawCreateObject(Engine* _engine);

        void DrawEntities(GameObject* child, uint it);
        void DrawEntity(GameObject* child);

        bool p_open = true;
    };
}