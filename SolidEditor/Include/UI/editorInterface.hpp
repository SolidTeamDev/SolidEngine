#pragma once

#include "Window/window.hpp"
#include "UI/sceneInterface.hpp"

namespace Solid
{
    class EditorInterface
    {
    private:
        bool             p_open = true;
        Window*          window;
        SceneInterface   sceneInterface;
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