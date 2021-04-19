#pragma once

#include "Core/engine.hpp"
#include "ECS/Components/camera.hpp"

namespace Solid
{
    class SceneInterface
    {
    public:
        SceneInterface();
        ~SceneInterface() = default;

        void Draw();
        void DrawMenu();

        bool p_open = true;
        inline const Framebuffer& getSceneFrameBuffer() {return sceneFramebuffer;};

        Vec2 prevMousePos = Vec2::Zero;
    private:


        static float camSpeed;
        Camera sceneCam;
        Framebuffer sceneFramebuffer;
        Engine* engine;

        void DrawScene();
        bool MouseInSceneInterface(const Vec2d& mousePos);
        void MovementAndRotationCam(const Vec2d& mousePos);
    };
}
