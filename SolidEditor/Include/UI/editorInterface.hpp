#pragma once

#include "Window/window.hpp"
#include <imgui.h>

#include "UI/sceneInterface.hpp"
#include "UI/inspectorInterface.hpp"
#include "UI/hierarchyTreeInterface.hpp"
#include "UI/filesInterface.hpp"

namespace Solid
{
    class EditorInterface
    {
    private:
        bool        p_open = true;
        Window*     window;

        FilesInterface          filesInterface;
        SceneInterface          sceneInterface;
        InspectorInterface      inspectorInterface;
        HierarchyTreeInterface  hierarchyTreeInterface;

    public:
        EditorInterface() = default;
        explicit EditorInterface(Window* window);

        ~EditorInterface() = default;

        /**
         * @debrief updates the whole UI
         */
        void Update();

        void DrawMenuBar();
        void DrawMainFrame();
    };
}