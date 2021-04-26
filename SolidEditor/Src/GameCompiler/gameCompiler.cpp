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
        fs::path log = p;
        fs::path CmakeP= p;
        CmakeP.append("CMakeLists.txt");
        //log.append("SolidGameCompile.log");
        //if(fs::exists(log))
        //    fs::remove(log);
        std::string Compile = "@call vcvarsall.bat x64 >"+p.string()+"\\vcvars.log && cmake --build "+p.string()+" --target Souls >"+p.string()+"\\SolidGameCompile.log";
        std::system(Compile.c_str());

    }
    void GameCompiler::CreateCmake()
    {
        //CMAKE_CURRENT_LIST_DIR CMAKE_CXX_COMPILER

        //
        fs::path CmakeP= p;
        CmakeP.append("CMakeLists.txt");
        std::ofstream CmakeFile (CmakeP, std::fstream::binary | std::fstream::trunc);
        ProjectName = "Souls";
        std::string RawFile = "cmake_minimum_required(VERSION 3.17)\n"
                              "project("+ProjectName+")\n"
                              "\n"
                              "set(CMAKE_CXX_STANDARD 20)\n"
                              "### Include dirs\n"
                              "\n"
                              "include_directories(Include)\n"
                              "\n"
                              "### Engine Sources Files\n"
                              "\n"
                              "# Add source files\n"
                              "file(GLOB_RECURSE GAME_SOURCE_FILES\n"
                              "        Src/*.c\n"
                              "        Src/*.cpp)\n"
                              "\n"
                              "\n"
                              "link_directories(Lib)\n"
                              "set(GAME_LIBS PUBLIC SolidEngine)\n"
                              "add_executable("+ProjectName+" ${GAME_SOURCE_FILES})\n"
                              "target_link_libraries("+ProjectName+" ${GAME_LIBS})";



        if(CmakeFile.is_open())
        {
            CmakeFile.write(RawFile.c_str(), RawFile.size());
            CmakeFile.close();
        }

        std::string Gen = " @call vcvarsall.bat x64 >" + p.string()+"\\vcvars.log && cmake.exe -DCMAKE_BUILD_TYPE=Debug -G \"CodeBlocks - NMake Makefiles\" "
                          + p.string()+"\\CMakeLists.txt >" + p.string()+"\\SolidCMakeProjectGen.log\n";

        std::system(Gen.c_str());


    }

}