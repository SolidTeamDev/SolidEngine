#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Build/SolidAPI.hpp"

#include "Types/extendedTypes.hpp"

namespace Solid
{

    struct SOLID_API WindowParams
    {
        std::string title;
        Int2 windowSize;
    };

    /**
     * @brief Window class. Store and manage GLFWwindow (Only support one instantiation)
     */
    class SOLID_API Window
    {
    private:
        GLFWwindow* window = nullptr;
        std::string title = "SolidWindow";
        Int2 windowSize = {1280,720};

        void Release();
    public:

        Window() = default;
        Window(const Window& _copy) = delete;
        Window(Window&& _move) = delete;

        ~Window();

        static void GLFWErrorCallback(int error_code, const char* description);

        void CreateWindow(const WindowParams& _wParam);

        [[nodiscard]]
        GLFWwindow* GetHandle() const;

        [[nodiscard]]
        Int2 GetWindowSize() const;
    };

} //!namespace


