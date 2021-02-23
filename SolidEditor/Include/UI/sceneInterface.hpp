#pragma once

namespace Solid
{
    class SceneInterface
    {
    public:
        SceneInterface()  = default;
        ~SceneInterface() = default;

        void Draw();

        bool p_open = true;
    };
}
