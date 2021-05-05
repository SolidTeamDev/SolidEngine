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

    private:


        static float camSpeed;
        Camera sceneCam;
        Framebuffer sceneFramebuffer;
        Engine* engine;

        bool ShowGrid = true;
        bool ShowSkeleton = false;
        void DrawScene();
        bool MouseInSceneInterface(const Vec2d& mousePos);
        void MovementAndRotationCam(float xpos, float ypos);
    };
}
