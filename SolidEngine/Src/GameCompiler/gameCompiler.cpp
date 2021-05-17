//
// Created by ryan1 on 23/02/2021.
//

#include "Core/engine.hpp"
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

    bool GameCompiler::LaunchCompile()
    {
        fs::path log = srcPath;
	    fs::path CmakeP= srcPath;
	    fs::path build= srcPath;
	    fs::path Toml= srcPath;
	    build.append("Build");


	    CmakeP.append("CMakeLists.txt");
        Toml.append("Dlls").append("RefurekuSettings.toml");
        CreateToml(Toml);

        std::string Compile = "@call vcvarsall.bat x64 >"+srcPath.string()+"\\vcvars.log && cmake --build "+build.string()+" --target "+ ProjectName+" >"+srcPath.string()+"\\SolidGameCompile.log";
	    std::string Gen2 = "\""+vcVarsP.string()+" x64  && cmake.exe --build "+build.string()+" --target "+ ProjectName+"\"";

	    std::string output;
#if SOLID_WIN


	    if(wincmd(Gen2, output, 1024000) != 0)
	    {
		    Log::Send(output, Log::ELogSeverity::ERROR);
			return false;
	    }

#else
	    if(std::system(Compile.c_str()) != 0)
        {
        	return false;
        }

#endif
	    Log::Send(output, Log::ELogSeverity::INFO);


        HotReload();
	    return true;

    }

	void GameCompiler::CreateToml(fs::path &p)
	{
		std::ofstream file(p, std::fstream::trunc);
		std::string TOMLFile = "[FileGenerationSettings]\n"
		                       "# Generated files will use this extension\n"
		                       "generatedFilesExtension = \".sld.hpp\"\n"
		                       "\n"
		                       "# List of supported extensions\n"
		                       "supportedExtensions = [\".h\", \".hpp\"]\n"
		                       "\n"
		                       "# Generated files will be located here\n"
		                       "outputDirectory = '''Include/EngineGenerated'''\n"
		                       "\n"
		                       "# Files contained in the directories of this list will be parsed\n"
		                       "toParseDirectories = [\n"
		                       "    '''Include'''\n"

		                       "]\n"
		                       "\n"
		                       "# Files to parse which are not included in any directory of toParseDirectories\n"
		                       "toParseFiles = [\n"
		                       "   \n"
		                       "   \n"
		                       "]\n"
		                       "\n"
		                       "# Files contained in the directories of this list will be ignored\n"
		                       "ignoredDirectories = [\n"
		                       "\t'''Include/EngineGenerated''',\n"
		                       "#\t'''Path/To/Directory/To/Ignore'''\n"
		                       "]\n"
		                       "\n"
		                       "# Files not to parse which are not included in any directory of ignoredDirectories\n"
		                       "ignoredFiles = []\n"
		                       "\n"
		                       "\n"
		                       "[FileParsingSettings]\n"
		                       "# Abort parsing on first encountered error\n"
		                       "shouldAbortParsingOnFirstError = true\n"
		                       "\n"
		                       "# Should all entities be parsed whether they are tagged with the macro or not\n"
		                       "shouldParseAllEntities = false\n"
		                       "\n"
		                       "# Include directories of the project\n"
		                       "projectIncludeDirectories = [\n"
		                       "\t'''../EngineInclude/Refureku''',\n"
		                       "\t'''../EngineInclude'''\n"
		                       "]\n"
		                       "\n"
		                       "# Must be one of \"msvc\", \"clang++\", \"g++\"\n"
		                       "compilerExeName = \"msvc\"\n"
		                       "\n"
		                       "[FileParsingSettings.Properties]\n"
		                       "namespaceMacroName = \"SLDNamespace\"\n"
		                       "classMacroName = \"SLDClass\"\n"
		                       "structMacroName = \"SLDStruct\"\n"
		                       "fieldMacroName = \"SLDField\"\n"
		                       "methodMacroName = \"SLDMethod\"\n"
		                       "enumMacroName = \"SLDEnum\"\n"
		                       "enumValueMacroName = \"SLDEnumVal\"\n"
		                       "variableMacroName = \"SLDVariable\"\n"
		                       "functionMacroName = \"SLDFunction\"";
		file.write(TOMLFile.c_str(), TOMLFile.size());
		file.close();
	}
    void GameCompiler::CreateCmake()
    {
	    updateVCPath();
        //CMAKE_CURRENT_LIST_DIR CMAKE_CXX_COMPILER
        //
	    fs::path CmakeP= srcPath;
	    fs::path src= srcPath;
	    fs::path inc= srcPath;
	    src.append("Src");
	    inc.append("Include");
	    fs::create_directory(src);
	    fs::create_directory(inc);
	    src.append("EntryPoint.cpp");
	    inc.append("entry.hpp");
        CmakeP.append("CMakeLists.txt");
        std::ofstream CmakeFile (CmakeP, std::fstream::binary | std::fstream::trunc);
	    std::ofstream main (src, std::fstream::binary | std::fstream::trunc);
	    std::ofstream incmain (inc, std::fstream::binary | std::fstream::trunc);
        std::string RawFile = "cmake_minimum_required(VERSION 3.17)\n"
                              "project("+ProjectName+")\n"
                              "\n"
                              "set(CMAKE_CXX_STANDARD 20)\n"
                              "### Include dirs\n"
                              "\n"
                              "include_directories(Include)\n"
							  "include_directories(${PROJECT_SOURCE_DIR}/../EngineInclude)\n"
                              "include_directories(${PROJECT_SOURCE_DIR}/../EngineInclude/Physx)\n"
                              "### Engine Sources Files\n"
                              "#OPENGL\n"
                              "find_package(OpenGL REQUIRED)\n"
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
							  "target_compile_definitions("+ProjectName+" PUBLIC \"IMGUI_API=__declspec(dllimport)\")\n"
		                      "target_compile_definitions("+ProjectName+" PUBLIC \"SOLID_API=__declspec(dllimport)\")\n"
                              "target_compile_definitions("+ProjectName+" PUBLIC \"SOLID_SCRIPT_API=__declspec(dllexport)\")"
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
                              "endif()\n"
							  "add_custom_target(RunGenerator\n"
                              "        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}\n"
                              "        COMMAND Dlls/RefurekuGenerator\n"
                              "        )\n"
                              "\n"
							  "add_dependencies("+ProjectName+" RunGenerator)\n"
	                          "project(Game)\n"
                              "include_directories(Include)\n"
                              "include_directories(${PROJECT_SOURCE_DIR}/../EngineInclude)\n"
							  "include_directories(${PROJECT_SOURCE_DIR}/../EngineInclude/Physx)\n"
	                          "set(Game_libs PUBLIC glfw3dll OpenGL::GL SolidEngine)\n"
                              "link_directories(Dlls)\n"
	                          "add_executable(Game main.cpp)\n"
                              "target_link_libraries(Game ${Game_libs})\n"
	                          "target_compile_definitions(Game PUBLIC \"IMGUI_API=__declspec(dllimport)\")\n"
						      "target_compile_definitions(Game PUBLIC \"SOLID_API=__declspec(dllimport)\")\n"
                              "if(CMAKE_BUILD_TYPE MATCHES Debug)#DEBUG\n"
                              "target_link_libraries(Game PUBLIC\n"
                              "${LIBS_COMMON}\n"
                              "debug     ${LIBS_DEBUG} )\n"
                              "else()#Release/Other\n"
                              "target_link_libraries(Game PUBLIC\n"
                              "${LIBS_COMMON}\n"
							  "optimized ${LIBS_RELEASE})\n"
                              "endif()\n"
	                          "\n"
	                          "\n"
	                          "\n"
	                          "\n";

	    std::string MainCpp=  "#include <iostream>\n"
						      "#include \"Core/engine.hpp\"\n"
	                          "#include \"Refureku/Refureku.h\"\n"
						      "#include \"entry.hpp\"\n"
							  "int Entry()\n"
			                  "{\n"
					          "     std::cout << \"Hello World \" << std::endl;\n"
                              "     return 0;\n"
                              "}\n"
                              "std::size_t  GetClass(const std::string& _s)\n"
	                          "{\n"
	                          "     return (std::size_t) rfk::Database::getClass(_s);\n"
	                          "}\n"
	                          "std::size_t  GetNamespace(const std::string& _s)\n"
	                          "{\n"
	                          "     return (std::size_t) rfk::Database::getNamespace(_s);\n"
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
                              "";


	    std::string mainhpp = "//\n"
	                          "// Created by ryan1 on 22/01/2021.\n"
	                          "//\n"
	                          "\n"
	                          "#ifndef DLL_LOADER_GAMETEST_H\n"
	                          "#define DLL_LOADER_GAMETEST_H\n"
	                          "\n"
	                          "#ifdef __cplusplus\n"
	                          "\n"
	                          "#include <string>\n"
	                          "#include \"Refureku/Refureku.h\"\n"
	                          "\n"
	                          "extern \"C\" int __declspec(dllexport) __stdcall Entry(); \n"
	                          "extern \"C\" std::size_t __declspec(dllexport) __stdcall GetClass(const std::string& );\n"
	                          "extern \"C\" std::size_t __declspec(dllexport) __stdcall GetNamespace(const std::string& );\n"
	                          "\n"
	                          "#endif\n"
	                          "\n"
	                          "\n"
	                          "\n"
	                          "\n"
	                          "#endif //DLL_LOADER_GAMETEST_H";

	    fs::path cpp = srcPath.string() + "/main" + ".cpp";
	    std::ofstream file(cpp);

	    std::string RawCpp = "#include \"Core/engine.hpp\"\n"
	                         "#include \"Resources/ressources.hpp\"\n"
	                         "#include <filesystem>\n"
	                         "\n"
	                         "namespace fs = std::filesystem;\n"
	                         "using namespace Solid;\n"
	                         "\n"
	                         "int main()\n"
	                         "{\n"
	                         "\t""Engine* engine = Engine::GetInstance();\n"
	                         "\t""WindowParams windowParams\n"
	                         "\t""{\n"
	                         "\t""    .title = \"Solid "+ProjectName+"\",\n"
	                                                                 "\t""    .windowSize = {1280,720}\n"
	                                                                 "\t""};\n"
	                                                                 "\t""RendererParams rendererParams\n"
	                                                                 "\t""{\n"
	                                                                 "\t""    .rendererType = ERendererType::OpenGl45\n"
	                                                                 "\t""};\n"
	                                                                 "\t""engine->InitEngineContext(windowParams, rendererParams);\n"
	                                                                 "\t""if(!engine->IsEngineContextInitialized())\n"
	                                                                 "\t\t""throw ThrowError(\"Engine not correctly initialized !\",ESolidErrorCode::S_INIT_ERROR);\n"
	                                                                 "\t""Window* window = engine->window;\n"
	                                                                 "\t""Renderer* renderer = engine->renderer;\n"
	                                                                 "\t""InputManager<int> * GameInputManager = new InputManager<int>(window->GetHandle());\n"
	                                                                 "\t""glfwSwapInterval(0);\n"
	                                                                 "\t""ResourcesLoader loader;\n"
	                                                                 "\t""loader.SetManager(&(engine->resourceManager));\n"
	                                                                 "\t""engine->EnableMultiThread(true);\n"
	                                                                 "\t""fs::path p = fs::current_path();\n"
	                                                                 "\t""p.append(\"Assets\");\n"
	                                                                 "\t""loader.LoadResourcesFromFolder(p);\n"
	                                                                 "\t""engine->LoadScene(fs::current_path());\n"
	                                                                 "\t""engine->EnableMultiThread(false);\n"
	                                                                 "\t""Camera sceneCam;\n"
	                                                                 "\t""while (!glfwWindowShouldClose(window->GetHandle()))\n"
	                                                                 "\t""{\n"
	                                                                 "\t\t""glfwPollEvents();\n"
	                                                                 "\t\t""GameInputManager->Update();\n"
	                                                                 "\t\t""engine->Update();\n"
	                                                                 "\t\t""engine->FixedUpdate();\n"
	                                                                 "\t\t""engine->LateUpdate();\n"
	                                                                 "\t\t""renderer->ClearColor({0,0,0,1});\n"
	                                                                 "\t\t""renderer->Clear(window->GetWindowSize());\n"
	                                                                 "\t\t""sceneCam.UpdateCamera(window->GetWindowSize());\n"
	                                                                 "\t\t""engine->rendererSystem->Update(engine->renderer,sceneCam);\n"
	                                                                 "\t\t""engine->audioSystem->Update(sceneCam);\n"
	                                                                 "\t\t""Time::Update();\n"
	                                                                 "\t\t""\n"
	                                                                 "\t\t""window->SwapBuffers();\n"
	                                                                 "\t""}\n"
	                                                                 "\t""return 0;\n"
	                                                                 "}\n";
	    if(file.is_open())
	    {
		    file.write(RawCpp.c_str(), RawCpp.size());
		    file.close();
	    }

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
	    if(incmain.is_open())
	    {
		    incmain.write(mainhpp.c_str(), mainhpp.size());
		    incmain.close();
	    }
		auto buildP = srcPath;
	    buildP.append("Build");
	    if(!fs::exists(buildP))
	        fs::create_directory(buildP);
	    std::string t = buildP.string();
        std::string Gen = " @call vcvarsall.bat x64 >" + srcPath.string()+"\\vcvars.log && cmake.exe -DCMAKE_BUILD_TYPE=Debug -G \"CodeBlocks - NMake Makefiles\" -S "+ srcPath.string()+" -B "+buildP.string()+
        		" >" + srcPath.string()+"\\SolidCMakeProjectGen.log\n";

        std::system(Gen.c_str());

	    std::string Gen2 = "\""+vcVarsP.string()+" x64  && cmake.exe -DCMAKE_BUILD_TYPE=Debug -G \"CodeBlocks - NMake Makefiles\" -S "+ srcPath.string()+" -B "+buildP.string()+"\"";
	    std::string output;
#if SOLID_WIN


	    wincmd(Gen2, output, 1024000);


#else
	    std::system(Compile.c_str());

#endif
	    Log::Send(output, Log::ELogSeverity::INFO);


    }

	void GameCompiler::AddLib()
	{
		auto curr = fs::current_path();
		auto SEngine = curr;
		SEngine.append("SolidEngine.dll");

	}

	void GameCompiler::CreateScript(const std::string& _name)
	{
    	std::string copyName = _name;
    	for(int i = 0; i < copyName.size(); ++i)
    	{

    	    if(copyName[i] == ' ' || copyName[i] == '-'|| copyName[i] == '\\' || copyName[i] == '/')
	        {
		        copyName[i] = '_';
	        }
    	}
		fs::path hpp = srcPath.string() +"/Include/" + copyName + ".hpp";
		fs::path cpp = srcPath.string() +"/Src/" + copyName + ".cpp";
		std::string RawCpp = "#include \""+ copyName+".hpp\"\n"
					         "using namespace Solid;\n"
		                     "\n"
					         ""+copyName+"::"+copyName+"()\n"
		                     "{\n"
		                     "\t\n"
		                     "}\n"
		                     "\n"
					         "~"+copyName+"::"+copyName+"()\n"
							 "{\n"
		                     "\t\n"
	                         "}\n"
		                     "\n"
		                     "\n"
		                     "\n";
		/////////////////////////////////////
		std::string RawHpp = "#include \"ECS/types.hpp\"\n"
		                     "#include \"Build/solidAPI.hpp\"\n"
		                     "#include \"GameGenerated/"+copyName+".sld.hpp\"\n"
		                     "\n"
		                     "namespace Solid SLDNamespace()\n"
		                     "{\n"
		                     "\tclass SOLID_API SLDClass() "+copyName+" : public Script\n"
		                     "\t{\n"
		                     "\t""public:\n"
		                     "\t\t""\n"
		                     "\t""private:\n"
		                     "\t\t""\n"
		                     "\t""public:\n"
		                     "\t\t"+copyName+"();\n"
		                     "\t\t~"+copyName+"();\n"
		                     "\t\t""\n"
		                     "\t\t""\n"
		                     "\t\t""\n"
		                     "\t\t""\n"
		                     "\t\t"+copyName+"_GENERATED\n"
							 "\t}\n"
		                     "\t\n"
		                     "\t\n"
		                     "\t\n"
		                     "}\n"
		                     "\n"
					         "File_GENERATED\n";
		std::ofstream file(hpp, std::fstream::trunc);

		if(file.is_open())
		{
			file.write(RawHpp.c_str(), RawHpp.size());
			file.close();
		}
		file.open(cpp, std::fstream::trunc);
		if(file.is_open())
		{
			file.write(RawCpp.c_str(), RawCpp.size());
			file.close();
		}
		ReloadCmake();
	}

	void GameCompiler::ReloadCmake()
	{
		auto buildP = srcPath;
		buildP.append("Build");
		buildP =fs::absolute(buildP);
		if(!fs::exists(buildP))
			fs::create_directory(buildP);
		std::string t = buildP.string();
		std::string Gen = " @call vcvarsall.bat x64 >" + srcPath.string()+"\\vcvars.log && cmake.exe -DCMAKE_BUILD_TYPE=Debug -G \"CodeBlocks - NMake Makefiles\" -S "+ srcPath.string()+" -B "+buildP.string()+
		                  " >" + srcPath.string()+"\\SolidCMakeProjectGen.log\n";
		std::string Gen2 = "\""+vcVarsP.string()+" x64  && cmake.exe -DCMAKE_BUILD_TYPE=Debug -G \"CodeBlocks - NMake Makefiles\" -S "+ srcPath.string()+" -B "+buildP.string()+"\"";
		std::string output;
#if SOLID_WIN


		wincmd(Gen2, output, 102400);

#else
		std::system(Gen.c_str());

#endif
		Log::Send(output, Log::ELogSeverity::INFO);
	}

	void GameCompiler::HotReload()
	{

	}

	void GameCompiler::Build()
	{
    	ReloadCmake();
		LaunchCompile();

		fs::path build= srcPath;
		build.append("Build");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::string Compile = "@call vcvarsall.bat x64 >"+srcPath.string()+"\\vcvars.log && cmake --build "+build.string()+" --target Game >"+srcPath.string()+"\\SolidGameCompile.log";
		std::system(Compile.c_str());

		std::string Gen2 = "\""+vcVarsP.string()+ " x64  && cmake.exe cmake --build "+build.string()+" --target Game \"";
		std::string output;
#if SOLID_WIN


		wincmd(Gen2, output, 1024000);

#else
		std::system(Gen.c_str());

#endif
		Log::Send(output, Log::ELogSeverity::INFO);


	}

	void GameCompiler::updateVCPath()
	{

		fs::path Dlls = fs::current_path();
		Dlls.append("Dlls");
		fs::path vcvars = Dlls;
		std::string batFile = "@echo off\n"
	                          "setlocal enabledelayedexpansion\n"
	                          "\n"
		"for /f \"usebackq tokens=*\" %%i in (`"+Dlls.string()+"\\vswhere.exe -latest -find VC\\Auxiliary\\Build\\vcvarsall.bat`) do (\n"
	                          "  echo %%i\n"
	                          "  exit /b !errorlevel!\n"
	                          ")";

		vcvars.append("vcvars.bat");
		std::ofstream file (vcvars, std::fstream::trunc);
		file.write(batFile.c_str(), batFile.size());
		file.close();
		fs::absolute(vcvars);

		std::string Gen2 = vcvars.string();
		std::string output;
#if SOLID_WIN


		wincmd(Gen2, output, 1024000);

#else
		std::system(Gen.c_str());

#endif
		output.erase(output.begin());
		std::string o2 = output.substr(0, output.size()-2);
		//%ProgramFiles(x86)% C:\Program Files (x86)
		o2 = "\"" + o2 +"\"";

		vcVarsP = o2;

		fs::path sym = fs::current_path();
		sym.append("vcvarsall.bat");
		fs::absolute(vcVarsP);
		//fs::create_symlink(o2, sym);
	}


}