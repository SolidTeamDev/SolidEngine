#pragma once

namespace Solid
{
    class HierarchyTreeInterface
    {
    public:
        HierarchyTreeInterface()  = default;
        ~HierarchyTreeInterface() = default;

        void Draw();
        void DrawCreateObject();

        bool p_open = true;
    };
}