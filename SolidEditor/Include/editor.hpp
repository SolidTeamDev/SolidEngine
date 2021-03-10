#pragma once

#include "Core/engine.hpp"
#include "GameCompiler/gameCompiler.hpp"

#include "ECS/Components/camera.hpp"

namespace Solid
{

    class Editor
    {
    private:
        InputManager<int>* editorInputManager = nullptr;
        Engine* engine = nullptr;
        GameCompiler* Compiler;
        Camera editorCamera;

        void UpdateEditorCamera();
        void LoadResources();
    public:

        static Framebuffer sceneFramebuffer;

        Editor();
        ~Editor();
        
        void Run();
    };

} //!namespace


