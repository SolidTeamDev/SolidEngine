#include <iostream>

#include "Core/engine.hpp"
#include "Resources/ressources.hpp"
#include "editor.hpp"
#include <chrono>
#define SA_UTH_DFLT_CSL_LOG 1
#include <Core/Maths/unitTestMaths.hpp>

#include <SA-UnitTestHelper.hpp>

int main()
{

    Solid::AllTestFuncMaths();

    try
    {
        Solid::Editor editor;
        editor.Run();
    }
    catch(const Solid::ThrowError& e)
    {
        std::cerr << "Error(" << (int)e.ErrorCode() << ") : " << e.What() << std::endl;
    }
    return EXIT_SUCCESS;
}
