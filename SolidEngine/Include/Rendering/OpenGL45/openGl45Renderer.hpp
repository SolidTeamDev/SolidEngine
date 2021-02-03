#pragma once

#include "Rendering/renderer.hpp"
#include <glad/glad.h>

namespace Solid
{

    class SOLID_API OpenGL45Renderer : public Renderer
    {
    private:

        void Release() final;

    public:

        ~OpenGL45Renderer() override;

        void Initialize(const RendererParams& _rParams) final;

        static void GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                    const GLchar* message, const void* userParam);

        void Clear(const Float4& _clearColor = {0,0,0, 1}) const final;
    };

} //!namespace




