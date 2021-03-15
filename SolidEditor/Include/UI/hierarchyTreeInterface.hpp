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

        void Draw();
        void DrawCreateObject();

        void DrawEntities();
        void DrawEntity(GameObject* child);

        void CheckEntities(GameObject* child, uint it);

        bool p_open = true;
    };
}