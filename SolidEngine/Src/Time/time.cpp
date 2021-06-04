#include "Time/time.hpp"

#include <GLFW/glfw3.h>

namespace Solid
{
    /// DeltaTime
    double Time::lastTime = 0;
    double Time::deltaTime = 0;
    double Time::globalTime = 0;

    /// Fps
    int Time::fps = 0;
    int Time::fpsCount = 0;
    double Time::lastTimePassed = glfwGetTime();


    void Time::Update()
    {
        globalTime += glfwGetTime();
        UpdateDeltaTime();
        UpdateFps();
    }

    void Time::UpdateDeltaTime()
    {
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }

    void Time::UpdateFps()
    {
        fpsCount++;

        double currTime = glfwGetTime();

        if(currTime - lastTimePassed >= 1.0)// each 1 second
        {
            fps = fpsCount;

            lastTimePassed = currTime;

            fpsCount = 0;
        }
    }

    double Time::DeltaTime()
    {
        return deltaTime;
    }

    double Time::GlobalTime()
    {
        return globalTime;
    }

    int Time::Fps()
    {
        return fps;
    }

} //!namespace