//
// Created by ryan1 on 27/01/2021.
//
#include "Ressources.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OBJ_Loader.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp\postprocess.h"
#include <sstream>
using namespace Solid;



///
/// ResourceManager 
///

void ResourceManager::AddResource(Resource *r) 
{
    ResourceList.push_back(r);
}
std::vector<Resource *> & ResourceManager::GetList() 
{
    return ResourceList;
}


///
/// ResourcesLoader Base
///


void ResourcesLoader::SetManager(ResourceManager *m) 
{
    Manager = m;
}

void ResourcesLoader::LoadRessource(const fs::path &Rpath) 
{
    if(!fs::exists(Rpath))
        return;

    if(!Rpath.has_extension())
    {
        ///Shader Folder Loading
    }

    std::string extension = Rpath.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c){ return std::tolower(c); });

        ///Solid Asset Loading

    if(extension == ".simage")
        LoadSolidImage(Rpath);
    if(extension == ".stexture")
        ;
    if(extension == ".smesh")
        LoadSolidMesh(Rpath);
    if(extension == ".sshader")
        ;
    if(extension == ".smaterial")
        ;
    if(extension == ".sanim")
        ;

    /// Image Loading
    if(extension == ".bmp")
        LoadImage(Rpath);
    if(extension == ".png")
        LoadImage(Rpath);
    if(extension == ".jpg")
        LoadImage(Rpath);
    if(extension == ".jpeg")
        LoadImage(Rpath);

    ///Mesh Loading
    if(extension == ".obj")
        LoadMesh(Rpath);
    if(extension == ".fbx")
        LoadMesh(Rpath);

}

///
/// Base Asset Loader
///

void ResourcesLoader::LoadImage(const fs::path &Rpath)
{
    ImageResource* Image = new ImageResource();
    Image->_name = Rpath.filename().string();
    unsigned char* img =stbi_load(Rpath.string().c_str(), &Image->x, &Image->y, &Image->ChannelsNum, STBI_rgb_alpha);
    //test convert to const char*
    //or vector<char>
    Image->image.resize(Image->x * Image->y * Image->ChannelsNum);
    std::memcpy(Image->image.data(), img, Image->image.size());

#if 1
    std::vector<char> Data;
    Image->ToDataBuffer(Data);




    fs::path cachePath = Rpath.parent_path();
    cachePath.append(Rpath.filename().string() + ".SImage");
    std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


    if(cacheFile.is_open())
    {
        cacheFile.write(Data.data(), Data.size());
    }
#endif
    stbi_image_free(img);
    Manager->AddResource(Image);
}


/*void ResourcesLoader::LoadMeshOBJ(const fs::path &Rpath)
{
    MeshResource* Mesh = new MeshResource();
    Mesh->_name = Rpath.filename().string();
    objl::Loader MeshLoader;
    MeshLoader.LoadFile(Rpath.string());

   // Mesh->indices.resize(MeshLoader.LoadedIndices.size());
    //std::memcpy(Mesh->indices.data(), MeshLoader.LoadedIndices.data(), sizeof(unsigned int) * MeshLoader.LoadedIndices.size());
    //Mesh->vertices.resize(MeshLoader.LoadedVertices.size());
    //std::memcpy(Mesh->vertices.data(), MeshLoader.LoadedVertices.data(),sizeof(float) * 8 *  MeshLoader.LoadedVertices.size());

#if 0
    std::vector<char> Data;
    Mesh->ToDataBuffer(Data);




    fs::path cachePath = Rpath.parent_path();
    cachePath.append(Rpath.filename().string() + ".SMesh");
    std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


    if(cacheFile.is_open())
    {
        std::cout << "" << "\n";
        cacheFile.write(Data.data(), Data.size());
    }
#endif
    Manager->AddResource(Mesh);
}*/



void ResourcesLoader::LoadMesh(const fs::path &Rpath)
{
    MeshResource* Mesh = new MeshResource;
    Assimp::Importer importer;
    std::string str = Rpath.string();

    const aiScene* scene = importer.ReadFile(str,  aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes| aiProcess_SplitLargeMeshes| aiProcess_Triangulate  | aiProcess_SortByPType | aiProcess_PreTransformVertices );
    if(!scene) {
        printf("Unable to load mesh: %s\n", importer.GetErrorString());
    }

    for(int i = 0; i < scene->mNumMeshes; ++i) {
        int numIndices = 3 * scene->mMeshes[i]->mNumFaces;
        MeshResource::SubMesh& Sub = Mesh->Meshes.emplace_back();
        Sub.indices.resize(numIndices);
        Sub.vertices.resize(scene->mMeshes[i]->mNumVertices);

        std::uint32_t WritePos = 0;
        for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j) {
            std::uint32_t size = scene->mMeshes[i]->mFaces[j].mNumIndices * sizeof(unsigned int);
            std::memcpy(&(Sub.indices[WritePos]), scene->mMeshes[i]->mFaces[j].mIndices, size);
            WritePos+= scene->mMeshes[i]->mFaces[j].mNumIndices;
        }

        for (int j = 0; j < scene->mMeshes[i]->mNumVertices; ++j) {
            if(scene->mMeshes[i]->HasPositions())
                std::memcpy(&(Sub.vertices[j].Pos), &(scene->mMeshes[i]->mVertices[j]), 3 * sizeof(float));
            if(scene->mMeshes[i]->HasNormals())
                std::memcpy(&(Sub.vertices[j].Normal), &(scene->mMeshes[i]->mNormals[j]), 3 * sizeof(float));
            if(scene->mMeshes[i]->HasTextureCoords(j))
                std::memcpy(&(Sub.vertices[j].TexCoords), &(scene->mMeshes[i]->mTextureCoords[j]), 2 * sizeof(float));
        }
    }
#if 1
    std::vector<char> Data;
    Mesh->ToDataBuffer(Data);




    fs::path cachePath = Rpath.parent_path();
    cachePath.append(Rpath.filename().string() + ".SMesh");
    std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


    if(cacheFile.is_open())
    {
        std::cout << "" << "\n";
        cacheFile.write(Data.data(), Data.size());
    }
#endif
    Manager->AddResource(Mesh);

}

///
/// RessourceLoader Memory Func
///


void ResourcesLoader::Append(std::vector<char> &DataBuffer, void *Data, std::uint64_t sizeInByte)
{
    if (Data && (sizeInByte > 0))
    {
        std::size_t start = DataBuffer.size();
        DataBuffer.resize(start + sizeInByte);
        std::memcpy(&DataBuffer[start], Data, sizeInByte);
    }
}


void ResourcesLoader::ReadFromBuffer(char* DataBuffer, void *Data, std::uint64_t sizeInByte, std::uint64_t& ReadPos)
{
    if (Data && (sizeInByte > 0) )
    {
        std::memcpy(Data, &DataBuffer[ReadPos], sizeInByte);
        ReadPos+= sizeInByte;
    }
}




///
/// Solid Assets Loader
///

void ResourcesLoader::LoadSolidImage(const fs::path &Rpath)
{
        ImageResource* Image = new ImageResource();

        std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
        std::ifstream::pos_type pos = ifs.tellg();

        std::vector<char>  buffer(pos);

        ifs.seekg(0, std::ios::beg);
        ifs.read(&buffer[0], pos);
        Image->FromDataBuffer(buffer.data(), buffer.size());
        Manager->AddResource(Image);
}


void ResourcesLoader::LoadSolidMesh(const fs::path &Rpath)
{
    MeshResource* Mesh = new MeshResource();

    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);

    Mesh->FromDataBuffer(buffer.data(), buffer.size());
    Manager->AddResource(Mesh);
}




///
/// Resources Classes
///
ImageResource::~ImageResource()
{

}



///
/// Resources Classes Buffer Reader / Writer
///

// IMAGE
void ImageResource::ToDataBuffer(std::vector<char> &buffer)
{

    std::string pString = this->_path.string();
    std::uint32_t size = pString.size();
    ResourcesLoader::Append(buffer, &(this->_type), sizeof(this->_type));

    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (pString.c_str()),  size * sizeof( std::string::value_type));

    size = this->_name.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->_name.c_str()), size * sizeof(std::string::value_type));

    ResourcesLoader::Append(buffer, &(this->x), sizeof(this->x));
    ResourcesLoader::Append(buffer, &(this->y), sizeof(this->y));
    ResourcesLoader::Append(buffer, &(this->ChannelsNum), sizeof(this->ChannelsNum));

        size = this->image.size();
        ResourcesLoader::Append(buffer, &(size), sizeof(size));
        ResourcesLoader::Append(buffer, (this->image.data()), size * sizeof(unsigned char));



}

void ImageResource::FromDataBuffer(char* buffer , int bSize)
{
    std::uint64_t ReadPos = 0;

    ResourcesLoader::ReadFromBuffer(buffer, &(this->_type), sizeof(this->_type), ReadPos);

    //recup path string
    std::uint32_t size = 0;
    std::string pString;
    ResourcesLoader::ReadFromBuffer(buffer, &size, sizeof(size), ReadPos);
    pString.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (pString.data()),  size * sizeof( std::string::value_type), ReadPos);
    this->_path = pString;

    //recup name
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->_name.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->_name.data()), size * sizeof(std::string::value_type), ReadPos);

    //recup image metadata
    ResourcesLoader::ReadFromBuffer(buffer, &(this->x), sizeof(this->x), ReadPos);
    ResourcesLoader::ReadFromBuffer(buffer, &(this->y), sizeof(this->y), ReadPos);
    ResourcesLoader::ReadFromBuffer(buffer, &(this->ChannelsNum), sizeof(this->ChannelsNum), ReadPos);

    //recup image
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);


    this->image.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (this->image.data()), size * sizeof(unsigned char), ReadPos);

}

// MESH

void MeshResource::ToDataBuffer(std::vector<char> &buffer)
{
    std::string pString = this->_path.string();
    std::uint32_t size = pString.size();
    ResourcesLoader::Append(buffer, &(this->_type), sizeof(this->_type));

    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (pString.c_str()),  size * sizeof( std::string::value_type));

    size = this->_name.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->_name.c_str()), size * sizeof(std::string::value_type));

    size = this->Meshes.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));

    for(MeshResource::SubMesh& Sub : this->Meshes)
    {
        size = Sub.indices.size();
        ResourcesLoader::Append(buffer, &(size), sizeof(size));
        ResourcesLoader::Append(buffer, (Sub.indices.data()), size * sizeof(unsigned int));

        size = Sub.vertices.size();
        ResourcesLoader::Append(buffer, &(size), sizeof(size));
        ResourcesLoader::Append(buffer, (Sub.vertices.data()), size * sizeof(Vertex));
    }

}

void MeshResource::FromDataBuffer(char *buffer, int bSize)
{
    std::uint64_t ReadPos = 0;

    ResourcesLoader::ReadFromBuffer(buffer, &(this->_type), sizeof(this->_type), ReadPos);

    //recup path string
    std::uint32_t size = 0;
    std::string pString;
    ResourcesLoader::ReadFromBuffer(buffer, &size, sizeof(size), ReadPos);
    pString.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (pString.data()),  size * sizeof( std::string::value_type), ReadPos);
    this->_path = pString;

    //recup name
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->_name.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->_name.data()), size * sizeof(std::string::value_type), ReadPos);

    //get SubMeshes Number
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->Meshes.resize(size);
    for(MeshResource::SubMesh& Sub : this->Meshes)
    {
        size = 0;
        ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
        Sub.indices.resize(size);
        ResourcesLoader::ReadFromBuffer(buffer, (Sub.indices.data()), size * sizeof(unsigned int), ReadPos);

        size = 0;
        ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
        Sub.vertices.resize(size);
        ResourcesLoader::ReadFromBuffer(buffer, (Sub.vertices.data()), size * sizeof(Vertex), ReadPos);
    }



}

