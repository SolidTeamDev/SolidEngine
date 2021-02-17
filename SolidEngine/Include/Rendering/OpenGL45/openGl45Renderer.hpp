#pragma once

#include "Rendering/renderer.hpp"
#include <glad/glad.h>

namespace Solid
{

    class SOLID_API OpenGL45Renderer : public Renderer
    {
    public:
        OpenGL45Renderer();
        ~OpenGL45Renderer() override = default;

        static void GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                    const GLchar* message, const void* userParam);

        void Clear(const Int2& _windowSize) const final;
        void ClearColor(const Float4& _clearColor) const final;
        [[nodiscard]]
        Framebuffer CreateFramebuffer(const Int2& _size) const final;

        void UpdateFramebuffer(const Framebuffer& _framebuffer) const final;

        void BeginFramebuffer(const Framebuffer& _framebuffer) const final;
        void EndFramebuffer() const final;
    };

} //!namespace




