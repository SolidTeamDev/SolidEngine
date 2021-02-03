#pragma once

#include "Window/window.hpp"

namespace Solid
{
    class EditorInterface
    {
    private:
        Window* window;
        bool    p_open = true;
    public:
        EditorInterface() = default;
        EditorInterface(Window* window);

        ~EditorInterface() = default;

        void Update();
        void DrawMenuBar();
        void DrawMainFrame();
    };
}