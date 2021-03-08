//
// Created by ryan1 on 26/02/2021.
//

#ifndef SOLIDEDITOR_RESOURCETYPE_HPP
#define SOLIDEDITOR_RESOURCETYPE_HPP
#include <filesystem>
namespace fs = std::filesystem;
namespace Solid {

    enum class SOLID_API EResourceType : char
    {
        Image = 0,
        Mesh,
        Anim,
        Shader,
        Compute,
        Material,
        Texture,
        NONE,
    };

    class SOLID_API Resource {
    public:


        Resource() : _type(EResourceType::NONE) {

        }

        virtual ~Resource() {

        }

        static int NoNameNum;
    protected:
        EResourceType _type;
        fs::path _path;
    public:
        std::string _name;

        EResourceType GetType() { return _type; }

    };

    class SOLID_API ImageResource : public Resource
    {
    public:
        int x;
        int y;
        int ChannelsNum;
        std::vector<unsigned char> image;

        ImageResource() {
            _type = EResourceType::Image;
        }

        ~ImageResource();

        void ToDataBuffer(std::vector<char> &buffer);

//        void FromDataBuffer(std::vector<char>& buffer);
        void FromDataBuffer(char *buffer, int bSize);

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
        uint VAO = 0;

        class SubMesh
        {
        public:
            uint VBO = 0;
            uint EBO = 0;
            std::vector<Vertex> vertices;
            std::vector<uint> indices;
        };

        std::vector<SubMesh> Meshes;


        bool isInit = false;

        MeshResource()
        {
            _type = EResourceType::Mesh;
        }

        ~MeshResource()
        {

        }

        void ToDataBuffer(std::vector<char> &buffer);

        void FromDataBuffer(char *buffer, int bSize);
    };

    class SOLID_API AnimResource : public Resource
    {
    public:
        AnimResource()
        {
            _type = EResourceType::Anim;
        }

        ~AnimResource()
        {

        }
    };

    class SOLID_API ShaderResource : public Resource
    {
        uint VertShaderID;
        uint FragShaderID;
        uint ProgramID;
        std::string VertexSource;
        std::string FragSource;
//programs ID ?
    public:
        ShaderResource(uint vid, uint fid, uint pid, const char *vs, const char *fs)
        {
            _type = EResourceType::Shader;
            VertShaderID = vid;
            FragShaderID = fid;
            ProgramID = pid;
            VertexSource = vs;
            FragSource = fs;
        }

        ShaderResource()
        {
            _type = EResourceType::Shader;
        }

        ~ShaderResource()
        {

        }
        uint GetProgram() const;
        void ToDataBuffer(std::vector<char> &buffer);

        void FromDataBuffer(char *buffer, int bSize);
    };

    class SOLID_API ComputeShaderResource : public Resource
    {
        uint ShaderID;
        uint ProgramID;
        std::string ComputeSource;
//programs ID ?
    public:

        ComputeShaderResource(uint sid, uint pid, const char *cSource)
        {
            _type = EResourceType::Compute;
            ShaderID = sid;
            ProgramID = pid;
            ComputeSource = cSource;
        }

        ComputeShaderResource()
        {
            _type = EResourceType::Compute;
        }

        ~ComputeShaderResource()
        {

        }

        void ToDataBuffer(std::vector<char> &buffer);

        void FromDataBuffer(char *buffer, int bSize);
    };

    class SOLID_API MaterialResource : public Resource
    {
    public:
        MaterialResource()
        {
            _type = EResourceType::Material;
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
            _type = EResourceType::Texture;
        }

        ~TextureResource()
        {

        }
    };
}
#endif //SOLIDEDITOR_RESOURCETYPE_HPP