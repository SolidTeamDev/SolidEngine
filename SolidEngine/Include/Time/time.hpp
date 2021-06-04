#pragma once

#include "Build/solidAPI.hpp"

namespace Solid
{
    class SOLID_API Time
    {
    private:
        /// DeltaTime
        static double lastTime;
        static double deltaTime;

        static double globalTime;

        /// FPS
        static int fps;
        static int fpsCount;
        static double lastTimePassed;

        static void UpdateDeltaTime();
        static void UpdateFps();

    public:
        static void Update();
        static double DeltaTime();
        static double GlobalTime();


        static int Fps();
    };
} //!namespace
