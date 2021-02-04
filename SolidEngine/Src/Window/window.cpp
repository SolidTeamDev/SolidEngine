#include "Window/window.hpp"

namespace Solid
{

    Window::~Window()
    {
        Release();
    }

    void Window::GLFWErrorCallback(int error_code, const char *description)
    {
        std::cout << "GLFW Error : (" << error_code << ") " << description << std::endl;
    }

    void Window::CreateWindow(const WindowParams &_wParam)
    {
        windowSize  = _wParam.windowSize;
        title       = _wParam.title;

        if(!glfwInit())
            throw std::runtime_error("GLFW initialisation failed !");

        glfwSetErrorCallback(GLFWErrorCallback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(_wParam.windowSize.x,_wParam.windowSize.y,_wParam.title.c_str(), nullptr, nullptr);

        if(window == nullptr)
            throw std::runtime_error("GLFW create window failed !");

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // v-sync
    }

    void Window::update()
    {
        glfwGetWindowSize(window,&windowSize.x, &windowSize.y);
    }

    GLFWwindow* Window::GetHandle() const
    {
        return window;
    }

    Int2 Window::GetWindowSize() const
    {
        return windowSize;
    }

    void Window::Release()
    {
        if(window != nullptr)
            glfwDestroyWindow(window);
        glfwTerminate();
    }

} //!namespace