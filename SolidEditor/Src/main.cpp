#include <iostream>
#include "Core/engine.hpp"
#include "Ressources/Ressources.hpp"
#include "editor.hpp"
#include <chrono>
int main()
{
    /*Solid::Editor editor;


    Solid::ResourcesLoader Loader;
    Loader.SetManager(editor.RManager);
    editor.EnableMultiThread(true);
    fs::path ResourcesPath = fs::current_path().append("SolidResources");
    auto before = std::chrono::high_resolution_clock::now();
    Loader.LoadResourcesFromFolder(ResourcesPath);
    auto after = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::ratio<1,1000>> fp_s = after - before;
    std::cout << "Loading has Taken " << fp_s.count() << " milliseconds\n";

    editor.EnableMultiThread(false);
    before = std::chrono::high_resolution_clock::now();
    Loader.LoadResourcesFromFolder(ResourcesPath);
    after = std::chrono::high_resolution_clock::now();
    fp_s = after - before;
    std::cout << "Loading Mono has Taken " << fp_s.count() << " milliseconds\n";
    std::system("Pause");*/
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
