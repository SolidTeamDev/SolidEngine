//
// Created by ryan1 on 23/02/2021.
//

#ifndef SOLIDEDITOR_AMECOMPILER_HPP
#define SOLIDEDITOR_AMECOMPILER_HPP

#include <filesystem>

namespace fs = std::filesystem;

namespace Solid
{

    class GameCompiler {
    public:
        std::string ProjectName;
    private:
        fs::path p = fs::current_path().append("SolidGameSrc");
    public:
        GameCompiler()=default;
        ~GameCompiler() =default;
        void LaunchCompile();
        void CreateCmake();
    };

}

#endif //SOLIDEDITOR_AMECOMPILER_HPP
