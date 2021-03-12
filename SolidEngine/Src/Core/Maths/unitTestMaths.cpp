#include <Core/Maths/unitTestMaths.hpp>
#include <Core/Maths/Vector/unitTestVec2.hpp>
#include <SA-UnitTestHelper.hpp>
#include <iostream>


namespace Solid
{
    void TestVec3()
    {
        /*TODO*/
    }

    void TestVec4()
    {
        /*TODO*/
    }

    void TestQuat()
    {
        /*TODO*/
    }

    void TestMat4()
    {
        /*TODO*/
    }

    int AllTestFuncMaths()
    {
        std::cout << "Test vec2" << std::endl;
        SA_UTH_INIT();
        Solid::Vec2UnitTest::AllTest();
        SA_UTH_EXIT();
    }

}
