#include <iostream>
#include "Core/engine.hpp"
#include "Ressources/Ressources.hpp"
#include "editor.hpp"
#include <chrono>
int main()
{
    Solid::Editor editor;


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
    std::system("Pause");
    return EXIT_SUCCESS;
}
