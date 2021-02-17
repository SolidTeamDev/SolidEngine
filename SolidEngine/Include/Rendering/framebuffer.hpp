#pragma once

#include "Types/extendedTypes.hpp"

namespace Solid
{

    struct Framebuffer
    {
        GLuint id           = 0;
        GLuint texture      = 0;
        GLuint depthBuffer  = 0;

        Int2 size = {0,0};
    };

} //!namespace

