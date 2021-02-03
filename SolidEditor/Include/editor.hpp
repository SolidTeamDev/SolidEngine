#pragma once

#include "Core/engine.hpp"

namespace Solid
{

    class Editor final : public Engine
    {
    public:
        Editor();
        ~Editor() final = default;
        
        void Loop() final;
    };

} //!namespace


