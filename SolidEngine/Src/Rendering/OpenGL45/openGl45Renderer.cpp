#include "Rendering/OpenGL45/openGl45Renderer.hpp"

namespace Solid
{

    OpenGL45Renderer::~OpenGL45Renderer()
    {
        Release();
    }

    void OpenGL45Renderer::GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                           const GLchar *message, const void *userParam)
    {
        std::cout << "OpenGL Debug : " << message << std::endl;
    }

    void OpenGL45Renderer::Initialize(const RendererParams &_rParams)
    {
        window = new Window();
        window->CreateWindow(_rParams.windowParams);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            throw std::runtime_error("Failed to load glad !");

        if (GLAD_GL_KHR_debug)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        }
    }

    void OpenGL45Renderer::Clear(const Float4& _clearColor) const
    {
        glViewport(0,0,window->GetWindowSize().x,window->GetWindowSize().y);
        glClearColor(_clearColor.x,_clearColor.y,_clearColor.z,_clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGL45Renderer::Release()
    {
        delete window;
    }

} //!namespace