#include <iostream>
#include "Core/engine.hpp"
#include "Ressources/Ressources.hpp"
#include "editor.hpp"
int main()
{
    Solid::Editor editor;

    Solid::ResourceManager Manager;
    Solid::ResourcesLoader Loader;
    Loader.SetManager(&Manager);
    fs::path zShaderP = fs::current_path();
    fs::path computeShaderP = fs::current_path();
    fs::path SzShaderP = fs::current_path();
    fs::path ScomputeShaderP = fs::current_path();
    zShaderP.append("Resources");
    computeShaderP.append("Resources");
    zShaderP.append("ZShader");
    computeShaderP.append("ComputeShader");
    fs::create_directory(zShaderP);
    fs::create_directory(computeShaderP);

    SzShaderP.append("Solid");
    ScomputeShaderP.append("Solid");
    SzShaderP.append("ZShader.SVertFrag");
    ScomputeShaderP.append("ComputeShader.SCompute");

    Loader.LoadRessource(SzShaderP);
    Loader.LoadRessource(ScomputeShaderP);

    std::system("Pause");
    return EXIT_SUCCESS;
}
