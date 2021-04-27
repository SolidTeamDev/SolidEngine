//
// Created by ryan1 on 23/02/2021.
//

#include "GameCompiler/gameCompiler.hpp"
#include <fstream>

// ifstream = read  / ofstream = write


namespace Solid
{
    GameCompiler* GameCompiler::instance = nullptr;
    std::mutex GameCompiler::mutex = std::mutex();
    GameCompiler * GameCompiler::GetInstance()
    {
        std::lock_guard<std::mutex> lck(mutex);
        if(instance == nullptr)
            instance = new GameCompiler();
        return instance;
    }

    void GameCompiler::LaunchCompile()
    {
        fs::path log = srcPath;
	    fs::path CmakeP= srcPath;
	    fs::path build= srcPath;
	    build.append("Build");
        CmakeP.append("CMakeLists.txt");
        //log.append("SolidGameCompile.log");
        //if(fs::exists(log))
        //    fs::remove(log);
        std::string Compile = "@call vcvarsall.bat x64 >"+srcPath.string()+"\\vcvars.log && cmake --build "+build.string()+" --target "+ ProjectName+" >"+srcPath.string()+"\\SolidGameCompile.log";
        std::system(Compile.c_str());

    }
    void GameCompiler::CreateCmake()
    {
        //CMAKE_CURRENT_LIST_DIR CMAKE_CXX_COMPILER
        //
	    fs::path CmakeP= srcPath;
	    fs::path src= srcPath;
	    fs::path inc= srcPath;
	    src.append("Src");
	    inc.append("Include");
	    fs::create_directory(src);
	    fs::create_directory(inc);
	    src.append("main.cpp");
        CmakeP.append("CMakeLists.txt");
        std::ofstream CmakeFile (CmakeP, std::fstream::binary | std::fstream::trunc);
	    std::ofstream main (src, std::fstream::binary | std::fstream::trunc);
        std::string RawFile = "cmake_minimum_required(VERSION 3.17)\n"
                              "project("+ProjectName+")\n"
                              "\n"
                              "set(CMAKE_CXX_STANDARD 20)\n"
                              "### Include dirs\n"
                              "\n"
                              "include_directories(Include)\n"
							  "include_directories(${PROJECT_SOURCE_DIR}/../EngineInclude)\n"
                              "\n"
                              "### Engine Sources Files\n"
                              "\n"
                              "# Add source files\n"
                              "file(GLOB_RECURSE GAME_SOURCE_FILES\n"
                              "        Src/*.c\n"
                              "        Src/*.cpp)\n"
                              "file(GLOB LIBS_COMMON\n"
                              "        Dlls/*.lib)\n"
                              "file(GLOB LIBS_DEBUG\n"
                              "        Dlls/Debug/*.lib)\n"
                              "file(GLOB LIBS_RELEASE\n"
	                          "        Dlls/Release/*.lib)\n"
                              "\n"
                              "link_directories(Dlls)\n"
                              "#add_executable("+ProjectName+" ${GAME_SOURCE_FILES})\n"
							  "add_library("+ProjectName+" SHARED ${GAME_SOURCE_FILES})\n"
							  "#Extern Lib\n"
			                  "\n"
	                          "if(CMAKE_BUILD_TYPE MATCHES Debug)#DEBUG\n"
                              "target_link_libraries("+ProjectName+" PUBLIC\n"
                              "${LIBS_COMMON}\n"
                              "debug     ${LIBS_DEBUG} )\n"
							  "else()#Release/Other\n"
                              "target_link_libraries("+ProjectName+" PUBLIC\n"
                              "${LIBS_COMMON}\n"
                              "optimized ${LIBS_RELEASE})\n"
                              "endif()\n"
							  "target_link_libraries("+ProjectName+" ${GAME_LIBS})\n"
							  "# ========= DLL ==========\n"
                              "\n"
                              "#COMMON\n"
                              "ADD_CUSTOM_COMMAND (TARGET "+ProjectName+" POST_BUILD\n"
                              "\n"
                              "        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Dlls/glfw3.dll ${CMAKE_BINARY_DIR}/\n"
                              "        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Dlls/OpenAL32.dll ${CMAKE_BINARY_DIR}/\n"
                              "        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Dlls/sndfile.dll ${CMAKE_BINARY_DIR}/\n"
                              "\n"
                              "COMMENT \"Copying common binaries\" VERBATIM)\n"
                              "\n"
                              "if(CMAKE_BUILD_TYPE MATCHES Debug)#DEBUG\n"
                              "    ADD_CUSTOM_COMMAND (TARGET "+ProjectName+" POST_BUILD\n"
                              "\n"
                              "            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Dlls/Debug ${CMAKE_BINARY_DIR}/\n"
                              "\n"
                              "    COMMENT \"Copying debug binaries\" VERBATIM)\n"
                              "else()#RELEASE/OTHER\n"
                              "\n"
                              "    ADD_CUSTOM_COMMAND (TARGET "+ProjectName+" POST_BUILD\n"
                              "\n"
                              "            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Dlls/Release ${CMAKE_BINARY_DIR}/\n"
                              "\n"
                              "    COMMENT \"Copying release binaries\" VERBATIM)\n"
                              "endif()";

	    std::string MainCpp=  "#include <iostream>\n"
	                          "\n"
							  "int main()\n"
			                  "{\n"
					          "     std::cout << \"Hello World \" << std::endl;\n"
                              "     return 0;\n"
                              "}\n"
                              "\n"
                              "\n"
                              "\n"
                              "\n"
                              "\n"
                              "\n"
                              "\n"
                              "\n"
                              "\n"
                              "\n"
                              "\n"
                              "\n"
                              "\n"
                              "";



        if(CmakeFile.is_open())
        {
            CmakeFile.write(RawFile.c_str(), RawFile.size());
            CmakeFile.close();
        }
	    if(main.is_open())
	    {
		    main.write(MainCpp.c_str(), MainCpp.size());
		    main.close();
	    }
		auto buildP = srcPath;
	    buildP.append("Build");
	    if(!fs::exists(buildP))
	        fs::create_directory(buildP);
	    std::string t = buildP.string();
        std::string Gen = " @call vcvarsall.bat x64 >" + srcPath.string()+"\\vcvars.log && cmake.exe -DCMAKE_BUILD_TYPE=Debug -G \"CodeBlocks - NMake Makefiles\" -S "+ srcPath.string()+" -B "+buildP.string()+
        		" >" + srcPath.string()+"\\SolidCMakeProjectGen.log\n";

        std::system(Gen.c_str());


    }

	void GameCompiler::AddLib()
	{
		auto curr = fs::current_path();
		auto SEngine = curr;
		SEngine.append("SolidEngine.dll");

	}

}