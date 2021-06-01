#pragma once

#include "Core/engine.hpp"

namespace Solid
{
    class PlayInterface
    {
    private:
        Framebuffer playFramebuffer;
        Engine* engine = nullptr;

    public:
        PlayInterface();
        ~PlayInterface() = default;

        void Draw();

        bool p_open = true; //is the window rendered
        bool cursorInv = false; //is the cursor invisible
    };
}
