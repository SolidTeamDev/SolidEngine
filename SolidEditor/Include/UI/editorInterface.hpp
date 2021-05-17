#pragma once


#include "Window/window.hpp"
#include <imgui.h>

#include "Core/engine.hpp"
#include "UI/playInterface.hpp"
#include "UI/filesInterface.hpp"
#include "UI/sceneInterface.hpp"
#include "UI/inspectorInterface.hpp"
#include "UI/hierarchyTreeInterface.hpp"
#include "UI/logsInterface.hpp"
#include "UI/buttonInterface.hpp"

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
        Renderer*   renderer;
        ImGuiStyle& editorStyle;

        //All interfaces
        LogsInterface           logsInterface;
        PlayInterface           playInterface;
        FilesInterface          filesInterface;
        SceneInterface          sceneInterface;
        InspectorInterface      inspectorInterface;
        HierarchyTreeInterface  hierarchyTreeInterface;
        ButtonInterface         buttonInterface;

    public:
        EditorInterface();
        explicit EditorInterface(Window* window, Renderer* renderer);

        ~EditorInterface() = default;

        static Solid::GameObject* selectedGO;
        static bool        draggingEnt;


        /**
         * @debrief Updates the whole UI
         */
        void Update();

        /**
         * @debrief Updates the UI style to a dark theme
         */
        void DarkTheme();

        /**
         * @debrief Updates the UI style to a light theme
         */
        void LightTheme();

        /**
         * @brief Creates the main menu bar
         */
        void DrawMenuBar();

        /**
         * @brief Creates the Files menu
         */
        void DrawMenuFiles();

        /**
         * @brief Creates the Windows menu
         */
        void DrawMenuWindows();

        /**
         * @brief Creates the Modules menu
         */
        void DrawMenuModules();

        /**
         * @brief Creates the Tools menu
         */
        void DrawMenuTools();


        /**
         * @brief Creates the main frame in which the editor will be drawn
         */
        void DrawMainFrame();

        /**
         * @debrief Draws the window to change editor colors
         */
        void DrawChangeColors();

        /**
         * @brief Draws the additional informations overlay
         */
        void AddInfoOverlay();
    };
}
