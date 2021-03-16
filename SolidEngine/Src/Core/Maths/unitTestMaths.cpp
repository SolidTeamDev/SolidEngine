#include <Core/Maths/unitTestMaths.hpp>
#define SA_UTH_DFLT_CSL_LOG 1
#include <SA-UnitTestHelper.hpp>
#include <iostream>


namespace Solid
{

    int AllTestFuncMaths()
    {

        SA_UTH_INIT();
        Sa::UTH::verbosity = Sa::UTH::Verbosity::Default;

        SA_UTH_GP(Solid::Vec2UnitTest::Main());
        SA_UTH_GP(Solid::QuatUnitTest::Main());
        SA_UTH_EXIT();
    }

}
