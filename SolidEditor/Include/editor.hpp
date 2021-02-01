#pragma once

#include "Core/engine.hpp"

#include "Rendering/renderer.hpp"

namespace Solid
{

    class Editor final : public Engine
    {
    private:
        Renderer* renderer;
    public:
        Editor();
        ~Editor() final = default;
        
        void Loop() final;
    };

} //!namespace


