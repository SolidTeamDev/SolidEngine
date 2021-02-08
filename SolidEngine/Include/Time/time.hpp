#pragma once

namespace Solid
{
    class Time
    {
    private:
        static double lastTime;
        static double deltaTime;

    public:
        static void Update();
        static double DeltaTime();
    };
} //!namespace
