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
    public:
        static Renderer* InitRenderer();


        static void GLDebugCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length,
                                    const GLchar* _message, const void* _userParam);

        void Clear(const Int2& _windowSize) const final;
        void ClearColor(const Float4& _clearColor) const final;
        [[nodiscard]]
        Framebuffer CreateFramebuffer(const Int2& _size) const final;

        void UpdateFramebuffer(const Framebuffer& _framebuffer) const final;

        void BeginFramebuffer(const Framebuffer& _framebuffer) const final;
        void EndFramebuffer() const final;
        void InitMesh(class MeshResource* _m)const final;
        void DrawMesh(const class MeshResource* _mesh)const final;
        void SetShaderMVP(class ShaderResource* _shader, Transform& _model, Camera& _camera)const final;

        VFShader CreateVertFragProgram(std::vector<char *> &_VertexSources, std::vector<char *> &_fragSources)const final;

        CShader CreateComputeProgram(std::vector<char *> &_sources)const final;

        CShader CreateShader(GLenum _type, int _sourceCount, std::vector<char *> &_sources)const final;
        ShaderBinary GetShaderBinary(uint _PID) const final;
        uint CreateShaderFromBinary(ShaderBinary _binary) const final;
    };

} //!namespace




