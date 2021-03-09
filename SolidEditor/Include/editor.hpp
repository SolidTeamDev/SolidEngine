#pragma once

#include "Core/engine.hpp"

#include "ECS/Components/camera.hpp"

namespace Solid
{

    class Editor
    {
    private:
        InputManager<int>* editorInputManager = nullptr;
        Engine engine;
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


