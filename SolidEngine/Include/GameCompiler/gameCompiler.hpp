//
// Created by ryan1 on 23/02/2021.
//

#ifndef SOLIDEDITOR_GAMECOMPILER_HPP
#define SOLIDEDITOR_GAMECOMPILER_HPP

#ifdef __unix__
#define OS_WIN 0

#elif defined(_WIN32) || defined(WIN32)
#define OS_WIN 1
#include <Windows.h>
#endif
#include "Build/solidAPI.hpp"
#include "Refureku/Refureku.h"
#include <filesystem>
#include <mutex>

namespace fs = std::filesystem;

typedef        int              (__stdcall *f_Entry)();
typedef const  rfk::Class*      (__stdcall *f_GetClass)(const std::string&);
typedef const  rfk::Namespace*  (__stdcall *f_GetNamespace)(const std::string&);

namespace Solid
{

    class SOLID_API GameCompiler {
        static std::mutex mutex;
        static GameCompiler* instance;
    public:
	    f_Entry entryPoint = nullptr;
	    f_GetClass getClass = nullptr;
	    f_GetNamespace getNamespace = nullptr;
	    std::string ProjectName;
	    fs::path srcPath = "";
	    fs::path IncludePath = "";
	    fs::path DllPath = "";
	    fs::path vcVarsP = "";
	    std::string MainScene = "";
	    HMODULE hGetProcIDDLL = nullptr;

    private:
        GameCompiler()=default;
        ~GameCompiler() =default;
    public:
        GameCompiler(GameCompiler&) = delete;
        void operator=(const GameCompiler&) = delete;
        static GameCompiler* GetInstance();
        bool LaunchCompile();
	    void CreateCmake();
	    void CreateToml(fs::path& p);
	    void ReloadCmake();
	    void HotReload();
	    bool Build();
	    void CreateScript(const std::string& _name);
	    void updateVCPath();
	    const rfk::Namespace * GetNamespace(std::string _str);
	    const rfk::Class * GetClass(std::string _str);

	    void GameLoadDLL(std::string name);
	    int EntryPoint();

        void AddLib();
    };

}

#endif //SOLIDEDITOR_GAMECOMPILER_HPP
