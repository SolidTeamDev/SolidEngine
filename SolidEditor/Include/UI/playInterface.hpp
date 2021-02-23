#pragma once

namespace Solid
{
    class PlayInterface
    {
    public:
        PlayInterface()  = default;
        ~PlayInterface() = default;

        void Draw();

        bool p_open = true; //is the window rendered
        bool cursorInv = false; //is the cursor invisible
    };
}
