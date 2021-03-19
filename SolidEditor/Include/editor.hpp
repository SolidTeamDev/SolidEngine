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


        void UpdateEditorCamera();
        void LoadResources();
    public:

        static float camSpeed;
        static Framebuffer sceneFramebuffer;
        static Camera editorCamera;

        Editor();
        ~Editor();
        
        void Run();
    };

} //!namespace


