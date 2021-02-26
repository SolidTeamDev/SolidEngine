#pragma once

#include "Ressources/ressources.hpp"

namespace Solid
{
    struct MeshRenderer
    {
        MeshResource* mesh = nullptr;
        ShaderResource* shader = nullptr;
    };
} //!namespace