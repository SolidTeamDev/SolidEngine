#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Build/solidAPI.hpp"
#include "Core/Maths/Vector/vector2.hpp"

namespace Solid
{

    struct SOLID_API WindowParams
    {
        std::string title;
        Vec2i windowSize;
    };

    /**
     * @brief Window class. Store and manage GLFWwindow (Only support one window)
     */
    class SOLID_API Window
    {
    private:
        GLFWwindow* window = nullptr;
        std::string title = "SolidWindow";
        Vec2i windowSize = {1280,720};
    public:

        Window(const WindowParams& _wParam);
        Window(const Window& _copy) = delete;
        Window(Window&& _move) = delete;

        ~Window();

        static void GLFWErrorCallback(int error_code, const char* description);

        void SwapBuffers();

        [[nodiscard]]
        GLFWwindow* GetHandle() const;

        [[nodiscard]]
        Vec2i GetWindowSize() const;

    };

    void SOLID_API SetCursorPos(float _x, float _y);

} //!namespace


