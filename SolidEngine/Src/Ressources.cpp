//
// Created by ryan1 on 27/01/2021.
//
#include "Ressources.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OBJ_Loader.h"
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
        ;

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


void ResourcesLoader::LoadMesh(const fs::path &Rpath)
{
    MeshResource* Mesh = new MeshResource();
    Mesh->_name = Rpath.filename().string();
    objl::Loader MeshLoader;
    MeshLoader.LoadFile(Rpath.string());

    Mesh->indices.resize(MeshLoader.LoadedIndices.size());
    std::memcpy(Mesh->indices.data(), MeshLoader.LoadedIndices.data(), sizeof(unsigned int) * MeshLoader.LoadedIndices.size());
    Mesh->vertices.resize(MeshLoader.LoadedVertices.size());
    std::memcpy(Mesh->vertices.data(), MeshLoader.LoadedVertices.data(),sizeof(float) * 8 *  MeshLoader.LoadedVertices.size());

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


    size = this->indices.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (this->indices.data()), size * sizeof(unsigned int));

    size = this->vertices.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (this->vertices.data()), size * sizeof(Vertex));
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


    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->indices.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (this->indices.data()), size * sizeof(unsigned int), ReadPos);

    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->vertices.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (this->vertices.data()), size * sizeof(Vertex), ReadPos);

}

