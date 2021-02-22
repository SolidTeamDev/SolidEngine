#include "Window/window.hpp"

#include "Core/Debug/debug.hpp"

namespace Solid
{

    Window::Window(const WindowParams &_wParam)
    {
        windowSize  = _wParam.windowSize;
        title       = _wParam.title;

        if(!glfwInit())
            throw ThrowError("GLFW initialisation failed !",ESolidErrorCode::S_INIT_ERROR);

        glfwSetErrorCallback(GLFWErrorCallback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(_wParam.windowSize.x,_wParam.windowSize.y,_wParam.title.c_str(), nullptr, nullptr);

        if(window == nullptr)
            throw ThrowError("GLFW create window failed !",ESolidErrorCode::S_INIT_ERROR);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // v-sync
    }

    Window::~Window()
    {
        if(window != nullptr)
            glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::GLFWErrorCallback(int error_code, const char *description)
    {
        std::cout << "GLFW Error : (" << error_code << ") " << description << std::endl;
    }

    void Window::SwapBuffers()
    {
        glfwGetWindowSize(window,&windowSize.x, &windowSize.y);
        glfwSwapBuffers(window);
    }

    GLFWwindow* Window::GetHandle() const
    {
        return window;
    }

    Int2 Window::GetWindowSize() const
    {
        return windowSize;
    }

} //!namespace