//
// Created by ryan1 on 27/01/2021.
//

#ifndef SOLIDEDITOR_RESSOURCES_HPP
#define SOLIDEDITOR_RESSOURCES_HPP


#ifndef SOLID_API
#define SOLID_API __declspec(dllexport)
#endif

#include <vector>
#include <string>
//#include <string_view>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "SolidMath.hpp"

namespace fs = std::filesystem;

namespace Solid
{
    enum class SOLID_API ResourceType : char
    {
        Image = 0,
        Mesh,
        Anim,
        Shader,
        Material,
        Texture,
        NONE,

    };
    class SOLID_API Resource
    {
    public:

        Resource() : _type(ResourceType::NONE)
        {

        }
        virtual ~Resource()
        {

        }
    protected:
        ResourceType _type;
        fs::path _path;
    public:
        std::string _name;

    };

    class SOLID_API ImageResource : public Resource
    {
    public:
        int x;
        int y;
        int ChannelsNum;
        std::vector<unsigned char> image;
        ImageResource()
        {
            _type = ResourceType::Image;
        }
        ~ImageResource();
        void ToDataBuffer(std::vector<char>& buffer);
//        void FromDataBuffer(std::vector<char>& buffer);
        void FromDataBuffer(char* buffer, int bSize);

    };


    /// TEMPORARY STRUCTS
    struct SOLID_API v2
    {
        union
        {
            struct
            {
                float x;
                float y;
            };
            float elt[2];
        };
    };
    struct SOLID_API v3
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
            };
            float elt[3];
        };
    };
    struct SOLID_API Vertex
    {
        v3 Pos;
        v3 Normal;
        v2 TexCoords;
    };

    class SOLID_API MeshResource : public Resource
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        MeshResource()
        {
            _type = ResourceType::Mesh;
        }
        ~MeshResource()
        {

        }
        void ToDataBuffer(std::vector<char>& buffer);
        void FromDataBuffer(char* buffer, int bSize);
    };
    class SOLID_API AnimResource : public Resource
    {
    public:
        AnimResource()
        {
            _type = ResourceType::Anim;
        }
        ~AnimResource()
        {

        }
    };
    class SOLID_API ShaderResource : public Resource
    {
        unsigned int ShaderID;
        std::string VertexSource;
        std::string FragSource;
        //programs ID ?
    public:

        ShaderResource()
        {
            _type = ResourceType::Shader;
        }
        ~ShaderResource()
        {

        }
    };
    class SOLID_API ComputeShaderResource : public Resource
    {
        unsigned int ShaderID;
        std::string ComputeSource;
        //programs ID ?
    public:

        ComputeShaderResource()
        {
            _type = ResourceType::Shader;
        }
        ~ComputeShaderResource()
        {

        }
    };

    class SOLID_API MaterialResource : public Resource
    {
    public:
        MaterialResource()
        {
            _type = ResourceType::Material;
        }
        ~MaterialResource()
        {

        }
    };
    class SOLID_API TextureResource : public Resource
    {
    public:
        TextureResource()
        {
            _type = ResourceType::Texture;
        }
        ~TextureResource()
        {

        }
    };

    class SOLID_API ResourceManager
    {
        std::vector<Resource*> ResourceList;
    public:
        ResourceManager() = default;
        ~ResourceManager()
        {
            for(Resource* r : ResourceList)
            {
                if(r != nullptr)
                    delete r;
            }
        }

        void AddResource(Resource* r);
        std::vector<Resource*>& GetList();
    };

    class SOLID_API ResourcesLoader
    {
        ResourceManager* Manager;
        void LoadImage(const fs::path& Rpath);
        void LoadMesh(const fs::path& Rpath);
        void LoadAnim(const fs::path& Rpath);
        void LoadShader(const fs::path& Rpath);
        void LoadMaterial(const fs::path& Rpath);
        void LoadTexture(const fs::path& Rpath);

        void LoadSolidImage(const fs::path& Rpath);
        void LoadSolidTex(const fs::path& Rpath);
        void LoadSolidMesh(const fs::path& Rpath);
        void LoadSolidShader(const fs::path& Rpath);
        void LoadSolidMaterial(const fs::path& Rpath);
        void LoadSolidAnim(const fs::path& Rpath);




    public:

        ResourcesLoader() = default;
        ~ResourcesLoader() = default;
        void SetManager(ResourceManager* m);
        void LoadRessource(const fs::path& Rpath);
        inline static void Append(std::vector<char>& DataBuffer, void* Data, std::uint64_t sizeInByte);
        //inline static void ReadFromBuffer(std::vector<char>& DataBuffer, void* Data, std::uint64_t sizeInByte, std::uint64_t& ReadPos);
        inline static void ReadFromBuffer(char* DataBuffer, void* Data, std::uint64_t sizeInByte, std::uint64_t& ReadPos);
    };



}


#endif //SOLIDEDITOR_RESSOURCES_HPP
