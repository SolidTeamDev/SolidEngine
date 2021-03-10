//
// Created by ryan1 on 23/02/2021.
//

#ifndef SOLIDEDITOR_GAMECOMPILER_HPP
#define SOLIDEDITOR_GAMECOMPILER_HPP

#include <filesystem>
#include <mutex>

namespace fs = std::filesystem;

namespace Solid
{

    class GameCompiler {
        static std::mutex mutex;
        static GameCompiler* instance;
    public:
        std::string ProjectName;
    private:
        fs::path p = fs::current_path().append("SolidGameSrc");
        GameCompiler()=default;
        ~GameCompiler() =default;
    public:
        GameCompiler(GameCompiler&) = delete;
        void operator=(const GameCompiler&) = delete;
        static GameCompiler* GetInstance();
        void LaunchCompile();
        void CreateCmake();
    };

}

#endif //SOLIDEDITOR_GAMECOMPILER_HPP
