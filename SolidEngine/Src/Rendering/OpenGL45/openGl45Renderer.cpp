#include "Rendering/OpenGL45/openGl45Renderer.hpp"

#include <GLFW/glfw3.h>

#include "Core/Debug/debug.hpp"
#include "Ressources/Ressources.hpp"

namespace Solid
{

    OpenGL45Renderer::OpenGL45Renderer()
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            throw ThrowError("Failed to load glad !",ESolidErrorCode::S_INIT_ERROR);

        if (GLAD_GL_KHR_debug)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        }
    }

    void OpenGL45Renderer::GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                           const GLchar *message, const void *userParam)
    {
        Log::ELogSeverity logSeverity = Log::ELogSeverity::DEBUG;
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                logSeverity = Log::ELogSeverity::ERROR;
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                logSeverity = Log::ELogSeverity::WARNING;
                break;
            case GL_DEBUG_SEVERITY_LOW:
                logSeverity = Log::ELogSeverity::INFO;
                break;
        }

        Log::Send("OpenGL Debug : " + std::string(message),logSeverity);
    }

    void OpenGL45Renderer::Clear(const Int2& _windowSize) const
    {
        glViewport(0,0,_windowSize.x,_windowSize.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGL45Renderer::ClearColor(const Float4 &_clearColor) const
    {
        glClearColor(_clearColor.x,_clearColor.y,_clearColor.z,_clearColor.w);
    }

    Framebuffer OpenGL45Renderer::CreateFramebuffer(const Int2& _size) const
    {
        Framebuffer framebuffer;

        //Framebuffer
        framebuffer.size = _size;
        glGenFramebuffers(1, &framebuffer.id);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id);

        //Texture
        glGenTextures(1,&framebuffer.texture);
        glBindTexture(GL_TEXTURE_2D, framebuffer.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebuffer.size.x, framebuffer.size.y, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        //Depth buffer
        glGenRenderbuffers(1,&framebuffer.depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER,framebuffer.depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer.size.x, framebuffer.size.y);
        glBindRenderbuffer(GL_RENDERBUFFER,0);

        //Attachment
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer.texture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer.depthBuffer);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Log::Send("Framebuffer is not complete",Log::ELogSeverity::ERROR);

        glBindFramebuffer(GL_FRAMEBUFFER,0);

        return framebuffer;
    }

    void OpenGL45Renderer::UpdateFramebuffer(const Framebuffer& _framebuffer) const
    {
        glBindTexture(GL_TEXTURE_2D, _framebuffer.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _framebuffer.size.x, _framebuffer.size.y, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     nullptr);
        glBindRenderbuffer(GL_RENDERBUFFER, _framebuffer.depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _framebuffer.size.x, _framebuffer.size.y);
        glBindTexture(GL_TEXTURE_2D,0);
        glBindRenderbuffer(GL_RENDERBUFFER,0);
    }

    void OpenGL45Renderer::BeginFramebuffer(const Framebuffer &_framebuffer) const
    {
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Log::Send("Framebuffer is not complete",Log::ELogSeverity::ERROR);

        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer.id);
    }

    void OpenGL45Renderer::EndFramebuffer() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGL45Renderer::InitMesh(MeshResource *m)
    {
        if(m->isInit)
            return;

        glGenVertexArrays(1, &m->VAO);
        for (MeshResource::SubMesh& sub : m->Meshes) {
            glGenBuffers(1, &sub.VBO);
            glGenBuffers(1, &sub.EBO);
            glBindVertexArray(m->VAO);
            glBindBuffer(GL_ARRAY_BUFFER, sub.VBO);
            glBufferData(GL_ARRAY_BUFFER, sub.vertices.size() * 8 *sizeof(GLfloat), sub.vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);



            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub.EBO);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sub.indices.size()*sizeof(unsigned int), sub.indices.data(), GL_STATIC_DRAW);
        }


        m->isInit=true;
    }

} //!namespace