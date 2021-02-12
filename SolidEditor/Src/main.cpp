#include <iostream>
#include "Core/engine.hpp"
#include "Ressources/Ressources.hpp"
#include "editor.hpp"
int main()
{
    try
    {
        Solid::Editor editor;
        editor.Loop();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
    Solid::ResourceManager Manager;
    Solid::ResourcesLoader Loader;
    Loader.SetManager(&Manager);
    fs::path zShaderP = fs::current_path();
    fs::path computeShaderP = fs::current_path();
    zShaderP.append("ZShader");
    computeShaderP.append("ComputeShader");
    fs::create_directory(zShaderP);
    fs::create_directory(computeShaderP);

    Loader.LoadRessource(computeShaderP);
    
    return EXIT_SUCCESS;
}
