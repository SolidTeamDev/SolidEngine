#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OBJ_Loader.h"
#include "Ressources.hpp"
#include <chrono>
struct v3
{
    float x;
    float y;
    float z;
};
int main()
{

    Solid::ResourcesLoader Loader;
    Solid::ResourceManager Manager;
    Loader.SetManager(&Manager);
    fs::path p = fs::current_path();
    fs::path p2 = fs::current_path();

    p.append("scene.obj");
    p2.append("scene.obj.SMesh");

    auto pngb = std::chrono::system_clock::now();
    Loader.LoadRessource(p);
    auto pnga = std::chrono::system_clock::now();
    auto Simageb = std::chrono::system_clock::now();
    Loader.LoadRessource(p2);
    auto Simagea = std::chrono::system_clock::now();

    auto pngTick = (pnga - pngb).count();
    auto SimageTick = (Simagea - Simageb).count();



    std::cout << "OBJ LOAD :       " << pngTick << "\n" << "SMesh LOAD :     " << SimageTick << "\n"
    ;


    std::system("Pause");
    return EXIT_SUCCESS;
}
