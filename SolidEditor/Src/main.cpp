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



    std::cout << "FBX LOAD :       " << pngTick << "\n" << "SMesh LOAD :     " << SimageTick << "\n"
    ;
    auto fbx = ((Solid::MeshResource*)(Manager.GetList()[0]));
    auto Smesh = ((Solid::MeshResource*)(Manager.GetList()[1]));

    std::cout << "FBX HAS : " << fbx->Meshes.size() << "subMeshes \n";
    std::cout << "Smesh HAS : " << Smesh->Meshes.size() << "subMeshes \n";
    std::cout <<  "_____________________________________________________ \n";

    std::cout << "FBX MESH 0 HAS : " << fbx->Meshes[0].vertices.size() << "vertices \n";
    std::cout << "FBX MESH 0 HAS : " << fbx->Meshes[0].indices.size() << "indices \n";
    std::cout << "Smesh MESH 0 HAS : " << Smesh->Meshes[0].vertices.size() << "vertices \n";
    std::cout << "Smesh MESH 0 HAS : " << Smesh->Meshes[0].indices.size() << "indices \n";


    std::system("Pause");
    return EXIT_SUCCESS;
}
