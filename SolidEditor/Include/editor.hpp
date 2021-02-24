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
    public:

        static Framebuffer sceneFramebuffer;

        Editor();
        ~Editor();
        
        void Run();
    };

} //!namespace


