#include <iostream>
#include <Core/Maths/Vector/vector2.hpp>
#include <Core/Maths/Vector/unitTestVec2.hpp>
#include <SA-UnitTestHelper.hpp>

using namespace Sa;

namespace Solid::Vec2UnitTest
{
    Vec2 GenerateRandomVec()
    {
        return Vec2(UTH::Rand(-50.f,50.f), UTH::Rand(-50.f,50.f));
    }
    void TestConstructors()
    {
        //Test default contructor
        Vec2 v0;
        SA_UTH_EQ(v0.x,0.f);
        SA_UTH_EQ(v0.y,0.f);

    }

    void TestConstants()
    {

    }

    void TestStatics()
    {

    }

    void TestMethods()
    {

    }

    void TestOperators()
    {

    }

    void AllTest()
    {
        SA_UTH_GP(TestConstructors());
    }

}



