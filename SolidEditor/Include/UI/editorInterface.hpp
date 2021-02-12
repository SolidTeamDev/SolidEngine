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
        bool        colorOpen = false;
        Window*     window;
        ImGuiStyle& editorStyle;

        FilesInterface          filesInterface;
        SceneInterface          sceneInterface;
        InspectorInterface      inspectorInterface;
        HierarchyTreeInterface  hierarchyTreeInterface;

    public:
        EditorInterface();
        explicit EditorInterface(Window* window);

        ~EditorInterface() = default;

        /**
         * @debrief updates the whole UI
         */
        void Update();
        void PushColors();
        void PopColors();

        void DrawMenuBar();
        void DrawMainFrame();
        void DrawChangeColors();
    };
}