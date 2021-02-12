#pragma once

namespace Solid
{
    class FilesInterface
    {
    private:
        bool p_open = true;
    public:
        FilesInterface()  = default;
        ~FilesInterface() = default;

        void Draw();
    };
}