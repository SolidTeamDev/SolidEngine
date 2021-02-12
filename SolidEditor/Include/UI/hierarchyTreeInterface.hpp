#pragma once

namespace Solid
{
    class HierarchyTreeInterface
    {
    private:
        bool p_open = true;
    public:
        HierarchyTreeInterface()  = default;
        ~HierarchyTreeInterface() = default;

        void Draw();
        void DrawCreateObject();
    };
}