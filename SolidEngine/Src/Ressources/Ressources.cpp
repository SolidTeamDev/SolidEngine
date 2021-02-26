//
// Created by ryan1 on 27/01/2021.
//
#include "Ressources/Ressources.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OBJ_Loader.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp\postprocess.h"
#include <sstream>
#include "glad/glad.h"
#include "Core/engine.hpp"
using namespace Solid;

#define SASSET_GEN 1
int Resource::NoNameNum = 0;

struct ShaderLoaderWrapper
{
    fs::path p;
    int i;
};

struct IDWrapper
{
    std::string Name;
    int i;
};






