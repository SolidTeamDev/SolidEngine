#pragma once

#include "Rendering/renderer.hpp"
#include <glad/glad.h>

namespace Solid
{
    // RC: Rendering architecture?
    class SOLID_API OpenGL45Renderer : public Renderer
    {
    private:
        OpenGL45Renderer();
        ~OpenGL45Renderer() override = default;
        uint gridProgram;
        uint grid_vbo;
        uint grid_ebo;
        void InitGridGL();

    public:
        static Renderer* InitRenderer();


        static void GLDebugCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length,
                                    const GLchar* _message, const void* _userParam);

        void Clear(const Vec2i& _windowSize) const final;
        void ClearColor(const Vec4& _clearColor) const final;
        [[nodiscard]]
        Framebuffer CreateFramebuffer(const Vec2i& _size) const final;

        void UpdateFramebuffer(const Framebuffer& _framebuffer) const final;

        void BeginFramebuffer(const Framebuffer& _framebuffer) const final;
        void EndFramebuffer() const final;
        ShaderBinary GetShaderBinary(uint _PID) const final;
        uint CreateShaderFromBinary(ShaderBinary _binary) const final;

	    virtual void DrawSolidGrid(Camera &_camera, float _gridSize, Vec3 _color, float _thickness) override;
    };

} //!namespace




