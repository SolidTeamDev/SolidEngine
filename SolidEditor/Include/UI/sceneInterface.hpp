#pragma once

namespace Solid
{
    class SceneInterface
    {
    public:
        SceneInterface()  = default;
        ~SceneInterface() = default;

        void Draw();
        void DrawMenu();

        bool p_open = true;
    };
}
