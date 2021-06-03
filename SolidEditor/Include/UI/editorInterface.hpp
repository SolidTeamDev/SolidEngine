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
        bool        computeDemo = false;
	    bool        colorOpen = false;
	    bool        openScenePopup = false;
	    bool        saveScenePopup = false;
	    std::shared_ptr<ICompute> currentComputeShader = nullptr;
	    SceneResource* currentScenePtr = nullptr;
	    SceneResource* choice = nullptr;
	    std::string str ;
		std::string currentOpenedScene = "Untitled";
		fs::path    currentScenePath;
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
	    struct CodeEditor
	    {
		    enum class ECodeType
		    {
			    COMPUTE
		    };
		    TextEditor imCodeEditor;
		    ECodeType codeType;
		    bool isCodeEditorOpen = false;
	    }codeEditor;
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

	    /**
		 * @brief Draws the compute Demo overlay
		 */
	    void AddComputeOverlay();


        void LoadSceneCall(Resource* scene);
    };
}
