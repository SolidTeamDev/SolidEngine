#pragma once

#include "Core/engine.hpp"

namespace Solid
{

    class Editor final : public Engine
    {
    private:
        InputManager<int> editorInputManager;
    public:
        Editor();
        ~Editor() final = default;
        
        void Loop() final;
    };

} //!namespace


