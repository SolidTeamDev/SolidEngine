#pragma once

#include "Core/Maths/Vector/vector2.hpp"

#include <glad/glad.h>

namespace Solid
{

    struct Framebuffer
    {
        GLuint id           = 0;
        GLuint texture      = 0;
        GLuint depthBuffer  = 0;

        Vec2i size = {0,0};
    };

} //!namespace

