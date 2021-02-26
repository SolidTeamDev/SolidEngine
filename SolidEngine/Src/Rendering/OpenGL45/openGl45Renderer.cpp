#include "Ressources/essources.hpp"

#include "Rendering/OpenGL45/openGl45Renderer.hpp"

#include <GLFW/glfw3.h>

#include "Core/Debug/debug.hpp"

namespace Solid
{

    Renderer * OpenGL45Renderer::InitRenderer()
    {
        std::lock_guard<std::mutex>lck(mutex);
        if(pInstance != nullptr)
        {
            ThrowError("Renderer Already Initialzed", ESolidErrorCode::S_INIT_ERROR);
        }
        pInstance = new OpenGL45Renderer();
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

    void OpenGL45Renderer::InitMesh(MeshResource *_m) const
    {
        if(_m->isInit)
            return;

        glGenVertexArrays(1, &_m->VAO);
        for (MeshResource::SubMesh& sub : _m->Meshes) {
            glGenBuffers(1, &sub.VBO);
            glGenBuffers(1, &sub.EBO);
            glBindVertexArray(_m->VAO);
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


        _m->isInit=true;
    }

    void OpenGL45Renderer::DrawMesh(const MeshResource *_mesh) const
    {
        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(_mesh->VAO);
        for (auto& subMesh : _mesh->Meshes)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,subMesh.EBO);
            glDrawElements(GL_TRIANGLES, subMesh.indices.size(),GL_UNSIGNED_INT, nullptr);
        }
    }

    void OpenGL45Renderer::SetShaderMVP(ShaderResource *_shader, Transform& _model, Camera& _camera) const
    {
        unsigned int program = _shader->GetProgram();
        glUseProgram(program);

        glUniformMatrix4fv(glGetUniformLocation(program,"proj"),1,GL_FALSE,_camera.GetProjection().elements.data());
        glUniformMatrix4fv(glGetUniformLocation(program,"view"),1,GL_FALSE,_camera.GetView().elements.data());
        glUniformMatrix4fv(glGetUniformLocation(program,"model"),1,GL_FALSE,_model.GetMatrix().elements.data());
    }




// Type = GL_VERTEX_SHADER / GL_FRAGMENT_SHADER / GL_COMPUTE_SHADER
    Renderer::CShader OpenGL45Renderer::CreateShader(GLenum _type, int _sourceCount, std::vector<char*>& _sources) const
    {
        CShader compute;
        compute.sID = glCreateShader(_type);

        glShaderSource(compute.sID, _sourceCount, _sources.data(), nullptr);
        glCompileShader(compute.sID);
        GLint compileStatus;
        glGetShaderiv(compute.sID, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus == GL_FALSE)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(compute.sID, 1024, nullptr, infoLog);
            printf("Shader compilation error: %s", infoLog);
            compute.error = true;
        }

        return compute;
    }

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
#ifndef ARRAYSIZE
#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

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
        CShader vShader = CreateShader(GL_VERTEX_SHADER, 1, _VertexSources);
        CShader fShader = CreateShader(GL_FRAGMENT_SHADER, 1, _fragSources);

        if(vShader.error || fShader.error)
            return VFShader{true, 0, 0};
        VFShader shader {.error=false, .vID=vShader.sID, .fID=fShader.sID,.pID=0};
        shader.pID = glCreateProgram();
        glAttachShader(shader.pID, vShader.sID);
        glAttachShader(shader.pID, fShader.sID);
        glLinkProgram(shader.pID);
        GLint linkStatus;
        glGetProgramiv(shader.pID, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE)
        {
#ifndef ARRAYSIZE
#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif
            GLchar infoLog[1024];
            glGetProgramInfoLog(shader.pID, ARRAYSIZE(infoLog), nullptr, infoLog);
            printf("Program link error: %s", infoLog);
            //TODO : cleanup at return
            shader.error=true;
        }
        return shader;
    }

    Renderer::ShaderBinary OpenGL45Renderer::GetShaderBinary(uint _PID) const
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