#pragma once

#include "Core/engine.hpp"
#include "GameCompiler/GameCompiler.hpp"

#include "ECS/Components/camera.hpp"

namespace Solid
{

    class Editor
    {
    private:
        InputManager<int>* editorInputManager = nullptr;
        Engine engine;
        GameCompiler Compiler;
        Camera editorCamera;

        void UpdateEditorCamera();
    public:

        static Framebuffer sceneFramebuffer;

        Editor();
        ~Editor();
        
        void Run();
    };

} //!namespace


