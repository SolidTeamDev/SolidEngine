#pragma once

#include <Build/solidAPI.hpp>


namespace Solid
{
    struct Vec2;
    namespace Vec2UnitTest
    {
        Vec2 SOLID_API GenerateRandomVec();
        void SOLID_API TestConstructors();
        void SOLID_API TestConstants();
        void SOLID_API TestStatics();
        void SOLID_API TestMethods();
        void SOLID_API TestOperators();
        void SOLID_API AllTest();
    }
}

