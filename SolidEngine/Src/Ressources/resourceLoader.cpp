//
// Created by ryan1 on 26/02/2021.
//

#include "Ressources/ressources.hpp"

#define STB_IMAGE_IMPLEMENTATION 1
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

///
/// ResourcesLoader Base
///


void ResourcesLoader::SetManager(ResourceManager *m)
{
    Manager = m;
}

void  ResourcesLoader::LoadRessourceNoAdd(const fs::path &Rpath, ResourcePtrWrapper &wrapper)
{
    if(!fs::exists(Rpath))
        return;
    Resource* r = nullptr;
    if(fs::is_directory(Rpath))
    {
        std::string name = Rpath.filename().string();
        std::transform(name.begin(), name.end(), name.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if(name.find("shader") != std::string::npos || name.find("compute") != std::string::npos)
            r=LoadShader(Rpath);
        else
        {
            return;
        }
    }

    std::string extension = Rpath.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    ///Solid Asset Loading

    if(extension == ".simage")
        r=LoadSolidImage(Rpath);
    if(extension == ".stexture")
        ;
    if(extension == ".smesh")
        r=LoadSolidMesh(Rpath);
    if(extension == ".scompute")
        r=LoadSolidComputeShader(Rpath);
    if(extension == ".svertfrag")
        r=LoadSolidShader(Rpath);
    if(extension == ".smaterial")
        ;
    if(extension == ".sanim")
        ;

    /// Image Loading
    if(extension == ".bmp")
        r=LoadImage(Rpath);
    if(extension == ".png")
        r=LoadImage(Rpath);
    if(extension == ".jpg")
        r=LoadImage(Rpath);
    if(extension == ".jpeg")
        r=LoadImage(Rpath);

    ///Mesh Loading
    if(extension == ".obj")
        r=LoadMesh(Rpath);
    if(extension == ".fbx")
        r=LoadMesh(Rpath);

    wrapper.r = r;
}
void ResourcesLoader::LoadRessource(const fs::path &Rpath)
{
    ResourcePtrWrapper RWrapper {nullptr};
    LoadRessourceNoAdd(Rpath, RWrapper);
    if(RWrapper.r != nullptr)
        Manager->AddResource(RWrapper.r);
}



void ResourcesLoader::LoadResourcesFromFolder(const fs::path &Rpath)
{
    if(!fs::exists(Rpath))
        return;
    ///MultiThread Loading
    if(Manager->GetEngine()->MultiThreadEnabled())
    {
        TaskManager& TaskMan = Manager->GetEngine()->taskManager;
        using fp = bool (*)( const fs::path&);
        auto shaderFind = [](const fs::path&item)
        {
            if(fs::is_directory(item))
            {
                std::string name = item.filename().string();
                std::transform(name.begin(), name.end(), name.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                return (name.find("shader") != std::string::npos || name.find("compute") != std::string::npos);

            }
            return false;
        };
        const std::size_t numOffiles = std::count_if(fs::directory_iterator(Rpath), fs::directory_iterator{}, (fp)fs::is_regular_file);
        const std::size_t numOfShader = std::count_if(fs::directory_iterator(Rpath), fs::directory_iterator{}, (fp)shaderFind);
        ResourcePtrWrapper* RessourceArray = new ResourcePtrWrapper[numOffiles + numOfShader] {nullptr};

        auto Lambda = [this](const fs::path *Rpath, ResourcePtrWrapper *wrapper){LoadRessourceNoAdd(*Rpath,*wrapper); delete Rpath;};
        int i =0;
        std::vector<ShaderLoaderWrapper> Shaders;
        std::vector<IDWrapper> IDS;
        IDS.reserve(10);
        Shaders.reserve(5);
        for (auto& item : fs::directory_iterator(Rpath))
        {
            std::string name = item.path().filename().string();
            fs::path* newP = new fs::path(item.path());
            if(fs::is_directory(item))
            {
                std::transform(name.begin(), name.end(), name.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if(name.find("shader") != std::string::npos || name.find("compute") != std::string::npos)
                {
                    Shaders.push_back({item, i});
                    ++i;
                }
                continue; //recursive func
            }
            else
            {
                std::string extension = item.path().extension().string();
                std::transform(extension.begin(), extension.end(), extension.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if(extension == ".scompute" || extension == ".svertfrag")
                {
                    Shaders.push_back({item, i});
                    ++i;
                    continue;
                }
                TaskMan.AddTask(Task(Task::MakeID("Load " + name), ETaskType::RESOURCES_LOADER, Lambda, newP, &RessourceArray[i]));
                IDS.push_back( {"Load " + name, i});

                ++i;
            }

        }
        for(auto& s : Shaders) {
            LoadRessourceNoAdd(s.p, RessourceArray[s.i]);
            Manager->AddResource(RessourceArray[s.i].r);
        }

        bool b = true;
        while(b)
        {
            b = false;
            for (int j = 0; j <IDS.size(); ++j) {
                Task* t= TaskMan.getTaskByID(IDS.at(j).Name);
                if(t == nullptr)
                    continue;
                if(!t->IsFinished())
                {
                    b = true;
                }
                else
                {
                    IDS[j].Name = "";
                    int k = IDS[j].i;
                    if(RessourceArray[k].r!= nullptr) {
                        Manager->AddResource(RessourceArray[k].r);
                    }
                }
            }
        }


    }
        /// MonoThread Loading
    else
    {

        for (auto& item : fs::directory_iterator(Rpath))
        {
            if(fs::is_directory(item))
            {
                std::string name = Rpath.filename().string();
                std::transform(name.begin(), name.end(), name.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if(name.find("shader") != std::string::npos || name.find("compute") != std::string::npos)
                {

                    LoadRessource(item);
                }
                else
                {

                    continue; //recursive func
                }
            }
            else
            {
                LoadRessource(item);

            }
        }

    }
}

///
/// Base Asset Loader
///

Resource * ResourcesLoader::LoadImage(const fs::path &Rpath)
{
    ImageResource* Image = new ImageResource();
    Image->name = Rpath.filename().string();
    unsigned char* img =stbi_load(Rpath.string().c_str(), &Image->x, &Image->y, &Image->ChannelsNum, STBI_rgb_alpha);
    //test convert to const char*
    //or vector<char>
    Image->image.resize(Image->x * Image->y * Image->ChannelsNum);
    std::memcpy(Image->image.data(), img, Image->image.size());

#if SASSET_GEN
    //printf("Generate .SImage\n");
    std::vector<char> Data;
    Image->ToDataBuffer(Data);
    fs::path cachePath = SolidPath;
    cachePath.append(Rpath.filename().string() + ".SImage");
    std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);
    if(cacheFile.is_open())
    {
        cacheFile.write(Data.data(), Data.size());
    }
#endif
    stbi_image_free(img);
    return Image;
}





Resource * ResourcesLoader::LoadMesh(const fs::path &Rpath)
{
    MeshResource* Mesh = new MeshResource;
    Assimp::Importer importer;
    std::string str = Rpath.string();
    Mesh->name = Rpath.filename().string();

    const aiScene* scene = importer.ReadFile(str,  aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes| aiProcess_SplitLargeMeshes| aiProcess_Triangulate  | aiProcess_SortByPType | aiProcess_PreTransformVertices );
    if(!scene) {
        printf("Unable to load mesh: %s\n", importer.GetErrorString());
        delete Mesh;
        return nullptr;
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
#if SASSET_GEN
    printf("generate .Smesh\n");
    std::vector<char> Data;
    Mesh->ToDataBuffer(Data);




    fs::path cachePath = SolidPath;
    cachePath.append(Rpath.filename().string() + ".SMesh");
    std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


    if(cacheFile.is_open())
    {
        std::cout << "" << "\n";
        cacheFile.write(Data.data(), Data.size());
    }
#endif

    return Mesh;



}




Resource * ResourcesLoader::LoadShader(const fs::path &Rpath)
{
    Renderer* r=Renderer::GetInstance();
    std::string FolderName = Rpath.filename().string();
    std::string FolderNameLower = FolderName;
    std::transform(FolderNameLower.begin(), FolderNameLower.end(), FolderNameLower.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if(FolderNameLower.find("compute") != std::string::npos)
    {

        std::vector<char*> ComputeSources(1);
        bool found = false;
        for(auto& file : fs::directory_iterator(Rpath))
        {

            FolderNameLower = file.path().filename().string();
            std::transform(FolderNameLower.begin(), FolderNameLower.end(), FolderNameLower.begin(),
                           [](unsigned char c){ return std::tolower(c); });
            if(FolderNameLower == "main.compute")
            {
                std::ifstream ifs(file.path(), std::ios::binary|std::ios::ate);
                std::ifstream::pos_type pos = ifs.tellg();

                char* buffer = new char[pos.operator long long() + 1];
                buffer[pos] = '\0';

                ifs.seekg(0, std::ios::beg);
                ifs.read(buffer, pos);
                //std::cout << buffer << "\n";
                ComputeSources[0] = buffer;
                found = true;
                break;
            }


        }
        if(!found)
            return nullptr;
        Renderer::CShader program = r->CreateComputeProgram(ComputeSources);
        if(program.error)
        {
            //CLEAN
            return nullptr;
        }
        ComputeShaderResource* Compute = new ComputeShaderResource(program.sID, program.pID, ComputeSources[0]);
        Compute->name = Rpath.filename().string();

#if SASSET_GEN
        printf("generate .SCompute\n");
        std::vector<char> Data;
        Compute->ToDataBuffer(Data);


        fs::path cachePath = SolidPath;
        cachePath.append(Rpath.filename().string() + ".SCompute");
        std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


        if(cacheFile.is_open())
        {
            std::cout << "" << "\n";
            cacheFile.write(Data.data(), Data.size());
        }
#endif
        delete[] ComputeSources[0];
        return Compute;


    }
    else
    {
        std::vector<char*> VertexSources(1);
        std::vector<char*> fragSources(1);
        bool vert  = false; bool frag = false;
        for(auto& file : fs::directory_iterator(Rpath))
        {
            FolderNameLower = file.path().filename().string();
            std::transform(FolderNameLower.begin(), FolderNameLower.end(), FolderNameLower.begin(),
                           [](unsigned char c){ return std::tolower(c); });
            if(FolderNameLower == "main.vert")
            {
                std::ifstream ifs(file.path(), std::ios::binary|std::ios::ate);
                std::ifstream::pos_type pos = ifs.tellg();

                char* buffer = new char[pos.operator long long() + 1];
                buffer[pos] = '\0';

                ifs.seekg(0, std::ios::beg);
                ifs.read(buffer, pos);
                //std::cout << buffer << "\n";
                VertexSources[0] = buffer;
                vert = true;
                if(frag)
                    break;
                continue;
            }
            if(FolderNameLower == "main.frag")
            {
                std::ifstream ifs(file.path(), std::ios::binary|std::ios::ate);
                std::ifstream::pos_type pos = ifs.tellg();

                char* buffer = new char[pos.operator long long() + 1];
                buffer[pos] = '\0';

                ifs.seekg(0, std::ios::beg);
                ifs.read(buffer, pos);
                //std::cout << buffer << "\n";
                fragSources[0] = buffer;
                frag = true;
                if(vert)
                    break;
                continue;
            }
        }
        if(!vert || !frag)
            return nullptr;
        //TODO Cleanup at return
        Renderer::VFShader shader = r->CreateVertFragProgram(VertexSources, fragSources);
        if(shader.error)
            return nullptr;
        ShaderResource* Shader = new ShaderResource(shader.vID,shader.fID, shader.pID, VertexSources[0], fragSources[0]);
        Shader->name = Rpath.filename().string();

#if SASSET_GEN
        printf("generate .SVertFrag\n");
        std::vector<char> Data;
        Shader->ToDataBuffer(Data);




        fs::path cachePath =SolidPath;
        cachePath.append(Rpath.filename().string() + ".SVertFrag");
        std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


        if(cacheFile.is_open())
        {
            std::cout << "" << "\n";
            cacheFile.write(Data.data(), Data.size());
        }
#endif
        delete[] VertexSources[0];
        delete[] fragSources[0];
        return Shader;

    }
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

Resource * ResourcesLoader::LoadSolidImage(const fs::path &Rpath)
{
    ImageResource* Image = new ImageResource();

    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);
    Image->FromDataBuffer(buffer.data(), buffer.size());
    if(Image->name == "")
    {
        Image->name = "NoName" + std::to_string(Resource::NoNameNum);
        Resource::NoNameNum++;
    }
    return Image;
}


Resource * ResourcesLoader::LoadSolidMesh(const fs::path &Rpath)
{
    MeshResource* Mesh = new MeshResource();

    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);

    Mesh->FromDataBuffer(buffer.data(), buffer.size());
    if(Mesh->name == "")
    {
        Mesh->name = "NoName" + std::to_string(Resource::NoNameNum);
        Resource::NoNameNum++;
    }

    return Mesh;

}

Resource * ResourcesLoader::LoadSolidComputeShader(const fs::path &Rpath)
{
    ComputeShaderResource* cs = new ComputeShaderResource();
    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);

    cs->FromDataBuffer(buffer.data(), buffer.size());
    if(cs->name == "")
    {
        cs->name = "NoName" + std::to_string(Resource::NoNameNum);
        Resource::NoNameNum++;
    }
    return cs;


}
Resource * ResourcesLoader::LoadSolidShader(const fs::path &Rpath)
{
    ShaderResource* s = new ShaderResource();
    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);

    s->FromDataBuffer(buffer.data(), buffer.size());
    if(s->name == "")
    {
        s->name = "NoName" + std::to_string(Resource::NoNameNum);
        Resource::NoNameNum++;
    }
    return s;

}
