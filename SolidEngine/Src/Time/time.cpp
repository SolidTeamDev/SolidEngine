#include "Time/time.hpp"

#include <GLFW/glfw3.h>

namespace Solid
{
    double Time::lastTime = 0;
    double Time::deltaTime = 0;

    void Time::Update()
    {
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }

    double Time::DeltaTime()
    {
        return deltaTime;
    }

} //!namespace