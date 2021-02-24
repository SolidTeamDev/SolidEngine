#pragma once

#include "Core/engine.hpp"
#include "GameCompiler/GameCompiler.hpp"

namespace Solid
{

    class Editor
    {
    private:
        InputManager<int>* editorInputManager = nullptr;
        Engine engine;
        GameCompiler Compiler;
    public:

        static Framebuffer sceneFramebuffer;

        Editor();
        ~Editor();
        
        void Run();
    };

} //!namespace


