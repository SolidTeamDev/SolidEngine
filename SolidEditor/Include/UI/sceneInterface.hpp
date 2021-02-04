#pragma once

namespace Solid
{
    class SceneInterface
    {
    private:
        bool p_open = true;
    public:
        SceneInterface()  = default;
        ~SceneInterface() = default;

        void Draw();
    };
}
