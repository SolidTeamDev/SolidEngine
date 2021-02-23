#pragma once

#include "Window/window.hpp"
#include <imgui.h>

#include "UI/playInterface.hpp"
#include "UI/filesInterface.hpp"
#include "UI/sceneInterface.hpp"
#include "UI/inspectorInterface.hpp"
#include "UI/hierarchyTreeInterface.hpp"
#include "UI/logsInterface.hpp"

namespace Solid
{
    class EditorInterface
    {
    private:
        bool        p_open    = true;
        bool        perfOpen  = false;
        bool        demoOpen  = false;
        bool        colorOpen = false;

        Window*     window;
        ImGuiStyle& editorStyle;

        LogsInterface           logsInterface;
        PlayInterface           playInterface;
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

        /**
         * @debrief updates the UI style to a dark theme
         */
        void DarkTheme();

        /**
         * @debrief updates the UI style to a dark theme
         */
        void LightTheme();

        void DrawMenuBar();
        void DrawMainFrame();

        /**
         * @debrief draws the UI window to change editor colors
         */
        void DrawChangeColors();
        void DrawPerfOverlay();
    };
}