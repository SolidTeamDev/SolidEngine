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

    if(fs::is_directory(Rpath))
    {
        LoadShader(Rpath);
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
    if(extension == ".scompute")
        LoadSolidComputeShader(Rpath);
    if(extension == ".svertfrag")
        LoadSolidShader(Rpath);
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

// Type = GL_VERTEX_SHADER / GL_FRAGMENT_SHADER / GL_COMPUTE_SHADER
GLuint CreateShader(GLenum type, int sourceCount, std::vector<char*>& sources)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, sourceCount, sources.data(), nullptr);
    glCompileShader(shader);
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        printf("Shader compilation error: %s", infoLog);
    }

    return shader;
}

void ResourcesLoader::LoadShader(const fs::path &Rpath)
{
    std::string FolderName = Rpath.filename().string();
    std::string FolderNameLower = FolderName;
    std::transform(FolderNameLower.begin(), FolderNameLower.end(), FolderNameLower.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if(FolderNameLower.find("compute") != std::string::npos)
    {
        std::cout << "Compute Shader Loading" << "\n";

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
            return;
        GLuint cShader = CreateShader(GL_COMPUTE_SHADER, 1, ComputeSources);
        GLuint program = glCreateProgram();
        glAttachShader(program, cShader);
        glLinkProgram(program);
        GLint linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE)
        {
            #ifndef ARRAYSIZE
            #define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
            #endif
            GLchar infoLog[1024];
            glGetProgramInfoLog(program, ARRAYSIZE(infoLog), nullptr, infoLog);
            printf("Program link error: %s", infoLog);
            return;
        }
        ComputeShaderResource* Compute = new ComputeShaderResource(cShader, program, ComputeSources[0]);
        Compute->_name = Rpath.filename().string();
        Manager->AddResource(Compute);
#if 1
        std::vector<char> Data;
        Compute->ToDataBuffer(Data);




        fs::path cachePath = Rpath.parent_path();
        cachePath.append(Rpath.filename().string() + ".SCompute");
        std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


        if(cacheFile.is_open())
        {
            std::cout << "" << "\n";
            cacheFile.write(Data.data(), Data.size());
        }
#endif
        delete[] ComputeSources[0];


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
            return;
        std::cout << "Vertex Shader Loading" << "\n";

        GLuint vShader = CreateShader(GL_VERTEX_SHADER, 1, VertexSources);
        GLuint fShader = CreateShader(GL_FRAGMENT_SHADER, 1, fragSources);

        GLuint program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);
        glLinkProgram(program);
        GLint linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE)
        {
            #ifndef ARRAYSIZE
            #define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
            #endif
            GLchar infoLog[1024];
            glGetProgramInfoLog(program, ARRAYSIZE(infoLog), nullptr, infoLog);
            printf("Program link error: %s", infoLog);
            return;
        }
        ShaderResource* Shader = new ShaderResource(vShader,fShader, program, VertexSources[0], fragSources[0]);
        Shader->_name = Rpath.filename().string();
        Manager->AddResource(Shader);
#if 1
        std::vector<char> Data;
        Shader->ToDataBuffer(Data);




        fs::path cachePath = Rpath.parent_path();
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

void ResourcesLoader::LoadSolidComputeShader(const fs::path &Rpath)
{
    std::cout << "Solid Compute Shader Loading" << "\n";
    ComputeShaderResource* cs = new ComputeShaderResource();
    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);

    cs->FromDataBuffer(buffer.data(), buffer.size());
    Manager->AddResource(cs);

}
void ResourcesLoader::LoadSolidShader(const fs::path &Rpath)
{
    std::cout << "Solid Vertex Shader Loading" << "\n";
    ShaderResource* s = new ShaderResource();
    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);

    s->FromDataBuffer(buffer.data(), buffer.size());
    Manager->AddResource(s);
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

// Shaders


//Compute

void ComputeShaderResource::ToDataBuffer(std::vector<char> &buffer)
{
    std::string pString = this->_path.string();
    std::uint32_t size = pString.size();
    ResourcesLoader::Append(buffer, &(this->_type), sizeof(this->_type));

    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (pString.c_str()),  size * sizeof( std::string::value_type));

    size = this->_name.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->_name.c_str()), size * sizeof(std::string::value_type));

    size = this->ComputeSource.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->ComputeSource.c_str()), size * sizeof(std::string::value_type));
    // Write Program Binary
    int pSize = 0;
    glGetProgramiv(ProgramID, GL_PROGRAM_BINARY_LENGTH, &pSize);
    char* binary = new char[pSize];
    GLenum BFormat = 0;
    glGetProgramBinary(ProgramID, pSize, nullptr, &BFormat, (void*)binary);
    std::uint32_t PSize = pSize;
    ResourcesLoader::Append(buffer, &PSize, sizeof  (PSize));
    ResourcesLoader::Append(buffer, &BFormat, sizeof  (BFormat));
    ResourcesLoader::Append(buffer, binary, sizeof  (char) * pSize);
}
void ComputeShaderResource::FromDataBuffer(char *buffer, int bSize)
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
    this->ComputeSource.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->ComputeSource.data()), size * sizeof(std::string::value_type), ReadPos);

    //Shader Binary
    size = 0;
    GLenum bFormat;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    ResourcesLoader::ReadFromBuffer(buffer, &(bFormat), sizeof(bFormat), ReadPos);
    char* binary = new char[size];
    ResourcesLoader::ReadFromBuffer(buffer, binary, sizeof(char) * size, ReadPos);
    this->ProgramID = glCreateProgram();
    glProgramBinary(this->ProgramID, bFormat, (void*)binary, size);
    GLenum ErrorCode = glGetError();
    while (ErrorCode != GL_NO_ERROR)
    {
        if(ErrorCode == GL_INVALID_ENUM)
        {
            printf("Binary Format = %u is not a value recognized by the implementation.\n", bFormat);
        }
        if(ErrorCode == GL_INVALID_OPERATION )
        {
            printf("programID = %u is not the name of an existing program object .\n", this->ProgramID);
        }
    }


}

//Vertex / Frag

void ShaderResource::ToDataBuffer(std::vector<char> &buffer)
{
    std::string pString = this->_path.string();
    std::uint32_t size = pString.size();
    ResourcesLoader::Append(buffer, &(this->_type), sizeof(this->_type));

    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (pString.c_str()),  size * sizeof( std::string::value_type));

    size = this->_name.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->_name.c_str()), size * sizeof(std::string::value_type));

    size = this->VertexSource.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->VertexSource.c_str()), size * sizeof(std::string::value_type));
    size = this->FragSource.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->FragSource.c_str()), size * sizeof(std::string::value_type));

    // Write Program Binary
    int pSize = 0;
    glGetProgramiv(ProgramID, GL_PROGRAM_BINARY_LENGTH, &pSize);
    char* binary = new char[pSize];
    GLenum BFormat = 0;
    glGetProgramBinary(ProgramID, pSize, nullptr, &BFormat, (void*)binary);
    std::uint32_t PSize = pSize;
    ResourcesLoader::Append(buffer, &PSize, sizeof  (PSize));
    ResourcesLoader::Append(buffer, &BFormat, sizeof  (BFormat));
    ResourcesLoader::Append(buffer, binary, sizeof  (char) * pSize);
    delete[] binary;
}
void ShaderResource::FromDataBuffer(char *buffer, int bSize)
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

    //shaders sources
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->VertexSource.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->VertexSource.data()), size * sizeof(std::string::value_type), ReadPos);

    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->FragSource.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->FragSource.data()), size * sizeof(std::string::value_type), ReadPos);

    //Shader Binary
    size = 0;
    GLenum bFormat;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    ResourcesLoader::ReadFromBuffer(buffer, &(bFormat), sizeof(bFormat), ReadPos);
    char* binary = new char[size];
    ResourcesLoader::ReadFromBuffer(buffer, binary, sizeof(char) * size, ReadPos);
    this->ProgramID = glCreateProgram();
    glProgramBinary(this->ProgramID, bFormat, (void*)binary, size);
    GLenum ErrorCode = glGetError();
    while (ErrorCode != GL_NO_ERROR)
    {
        if(ErrorCode == GL_INVALID_ENUM)
        {
            printf("Binary Format = %u is not a value recognized by the implementation.\n", bFormat);
        }
        if(ErrorCode == GL_INVALID_OPERATION )
        {
            printf("programID = %u is not the name of an existing program object .\n", this->ProgramID);
        }
    }
}