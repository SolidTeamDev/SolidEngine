#pragma once

namespace Solid
{
    class PlayInterface
    {
    public:
        PlayInterface()  = default;
        ~PlayInterface() = default;

        void Draw();

        bool p_open = true;
    };
}
