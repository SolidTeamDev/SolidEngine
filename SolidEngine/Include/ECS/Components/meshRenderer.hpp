#pragma once

#include "Ressources/essources.hpp"

namespace Solid
{
    struct MeshRenderer
    {
        MeshResource* mesh = nullptr;
        ShaderResource* shader = nullptr;
    };
} //!namespace