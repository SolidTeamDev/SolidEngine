

#include <iostream>

#include "Core/engine.hpp"
#include "Resources/ressources.hpp"
#include "editor.hpp"
#include <chrono>
#define SA_UTH_DFLT_CSL_LOG 1
#include <Core/Maths/unitTestMaths.hpp>

#include <SA-UnitTestHelper.hpp>

int main(int argc, char** argv)
{
#ifndef NDEBUG
    Solid::AllTestFuncMaths();
#endif

    try
    {
	    Solid::Editor editor;
	    bool b = false;
    	if(argc > 1)
	    {
		    for (int i = 1; i < argc; ++i)
		    {
			    std::string arg = argv[i];
			    if(arg == "-Solid")
			    {
				    b = true;
			    }

			    else if(arg.find("-Project=") != std::string::npos)
			    {
			    	std::size_t pos = arg.find('=');
			    	std::string ProjectPath = arg.substr(pos+1);
			    	ProjectPath.erase(ProjectPath.end());
			    	std::cout << ProjectPath << std::endl;
			    	std::ifstream file(ProjectPath);
			    	editor.CurrentProjectJson.clear();
				    file >> editor.CurrentProjectJson;
				    file.close();
				    editor.InitFromProject(ProjectPath);
			    }
			    Solid::Log::Send(std::string("ARG :") +argv[i]);

		    }

	    }


        editor.Run();

    }
    catch(const Solid::ThrowError& e)
    {
        std::cerr << "Error(" << (int)e.ErrorCode() << ") : " << e.What() << std::endl;
    }
    return EXIT_SUCCESS;
}
