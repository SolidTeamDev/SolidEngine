#pragma once

#include "Window/window.hpp"
#include "UI/sceneInterface.hpp"
#include "UI/inspectorInterface.hpp"
#include "UI/hierarchyTreeInterface.hpp"

namespace Solid
{
    class EditorInterface
    {
    private:
        bool        p_open = true;
        Window*     window;

        SceneInterface          sceneInterface;
        InspectorInterface      inspectorInterface;
        HierarchyTreeInterface  hierarchyTreeInterface;
    public:
        EditorInterface() = default;
        EditorInterface(Window* window);

        ~EditorInterface() = default;

        /**
         * @debrief updates the whole UI
         */
        void Update();

        void DrawMenuBar();
        void DrawMainFrame();
    };
}