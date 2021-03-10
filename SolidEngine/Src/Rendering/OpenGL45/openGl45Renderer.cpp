#include "Resources/ressources.hpp"

#include "Rendering/OpenGL45/openGl45Renderer.hpp"

#include <GLFW/glfw3.h>

#include "Core/Debug/debug.hpp"

namespace Solid
{
#ifndef ARRAYSIZE
#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

    Renderer * OpenGL45Renderer::InitRenderer()
    {
        if(pInstance != nullptr)
        {
            ThrowError("Renderer Already Initialzed", ESolidErrorCode::S_INIT_ERROR);
        }
        pInstance = new OpenGL45Renderer();
        pInstance->SetRendererType(ERendererType::OpenGl45);
        return pInstance;
    }
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

    void OpenGL45Renderer::GLDebugCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length,
                                           const GLchar *_message, const void *_userParam)
    {
        Log::ELogSeverity logSeverity = Log::ELogSeverity::DEBUG;
        switch (_severity)
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

        Log::Send("OpenGL Debug : " + std::string(_message), logSeverity);
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
        glGenFramebuffers(1, &framebuffer.id);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer.texture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer.depthBuffer);
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);
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


/*




//



    Renderer::CShader OpenGL45Renderer::CreateComputeProgram(std::vector<char*>& _sources) const
    {

        CShader compute = CreateShader(GL_COMPUTE_SHADER, 1, _sources);
        if(compute.error)
            return compute;
        compute.pID = glCreateProgram();
        glAttachShader(compute.pID, compute.sID);
        glLinkProgram(compute.pID);
        GLint linkStatus;
        glGetProgramiv(compute.pID, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE)
        {

            GLchar infoLog[1024];
            glGetProgramInfoLog(compute.pID, ARRAYSIZE(infoLog), nullptr, infoLog);
            printf("Program link error: %s", infoLog);
            //TODO : Cleanup at return
            compute.error = true;
        }
        return compute;
    }

    Renderer::VFShader OpenGL45Renderer::CreateVertFragProgram(std::vector<char*>& _VertexSources, std::vector<char*>& _fragSources) const
    {

    }*/

    ShaderBinary OpenGL45Renderer::GetShaderBinary(uint _PID) const
    {

        int pSize = 0;
        glGetProgramiv(_PID, GL_PROGRAM_BINARY_LENGTH, &pSize);
        char* binary = new char[pSize];
        GLenum BFormat = 0;
        glGetProgramBinary(_PID, pSize, nullptr, &BFormat, (void*)binary);
        return {.size=(uint)pSize,.format=BFormat, .b=binary};
    }

    uint OpenGL45Renderer::CreateShaderFromBinary(ShaderBinary _binary) const
    {
        uint ID = glCreateProgram();
        glProgramBinary(ID, _binary.format, (void*)_binary.b, _binary.size);
        GLenum ErrorCode = glGetError();
        while (ErrorCode != GL_NO_ERROR)
        {

            if(ErrorCode == GL_INVALID_ENUM)
            {

                printf("Binary Format = %u is not a value recognized by the implementation.\n", _binary.format);
            }
            if(ErrorCode == GL_INVALID_OPERATION )
            {
                printf("programID = %u is not the name of an existing program object .\n", ID);
            }
            ErrorCode = glGetError();
        }
        return ID;
    }
} //!namespace