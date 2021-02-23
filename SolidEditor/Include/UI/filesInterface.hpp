#pragma once

namespace Solid
{
    class FilesInterface
    {
    public:
        FilesInterface()  = default;
        ~FilesInterface() = default;

        void Draw();
        void DrawCreateFile();

        bool p_open = true;
    };
}