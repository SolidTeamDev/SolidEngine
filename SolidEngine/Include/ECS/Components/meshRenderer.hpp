#pragma once

#include "Ressources/Ressources.hpp"

namespace Solid
{
    struct MeshRenderer
    {
        MeshResource* mesh = nullptr;
        ShaderResource* shader = nullptr;
    };
} //!namespace