#pragma once

#include "engine.hpp"

#include "Rendering/renderer.hpp"

namespace Solid
{

    class Editor final : public Engine
    {
    public:
        Editor();
        ~Editor() final = default;

        Renderer* renderer;

        void Loop();
    };

} //!namespace


