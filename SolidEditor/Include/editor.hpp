#pragma once

#include "Core/engine.hpp"

namespace Solid
{

    class Editor
    {
    private:
        InputManager<int>* editorInputManager = nullptr;
        Engine engine;
    public:

        static Framebuffer sceneFramebuffer;

        Editor();
        ~Editor();
        
        void Run();
    };

} //!namespace


