#pragma once

namespace Solid
{
    class PlayInterface
    {
    private:
        bool p_open = true;
    public:
        PlayInterface()  = default;
        ~PlayInterface() = default;

        void Draw();
    };
}
