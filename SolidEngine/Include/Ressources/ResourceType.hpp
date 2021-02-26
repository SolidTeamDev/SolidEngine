//
// Created by ryan1 on 26/02/2021.
//

#ifndef SOLIDEDITOR_RESOURCETYPE_HPP
#define SOLIDEDITOR_RESOURCETYPE_HPP
namespace fs = std::filesystem;
namespace Solid {
    enum class SOLID_API ResourceType : char {
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


        Resource() : _type(ResourceType::NONE) {

        }

        virtual ~Resource() {

        }

        static int NoNameNum;
    protected:
        ResourceType _type;
        fs::path _path;
    public:
        std::string _name;

        ResourceType GetType() { return _type; }

    };

    class SOLID_API ImageResource : public Resource
    {
    public:
        int x;
        int y;
        int ChannelsNum;
        std::vector<unsigned char> image;

        ImageResource() {
            _type = ResourceType::Image;
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
        unsigned int VAO = 0;

        class SubMesh
        {
        public:
            unsigned int VBO = 0;
            unsigned int EBO = 0;
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
        };

        std::vector<SubMesh> Meshes;


        bool isInit = false;

        MeshResource()
        {
            _type = ResourceType::Mesh;
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
            _type = ResourceType::Anim;
        }

        ~AnimResource()
        {

        }
    };

    class SOLID_API ShaderResource : public Resource
    {
        unsigned int VertShaderID;
        unsigned int FragShaderID;
        unsigned int ProgramID;
        std::string VertexSource;
        std::string FragSource;
//programs ID ?
    public:

        ShaderResource(unsigned int vid, unsigned int fid, unsigned int pid, const char *vs, const char *fs)
        {
            _type = ResourceType::Shader;
            VertShaderID = vid;
            FragShaderID = fid;
            ProgramID = pid;
            VertexSource = vs;
            FragSource = fs;
        }

        ShaderResource()
        {
            _type = ResourceType::Shader;
        }

        ~ShaderResource()
        {

        }

        void ToDataBuffer(std::vector<char> &buffer);

        void FromDataBuffer(char *buffer, int bSize);
    };

    class SOLID_API ComputeShaderResource : public Resource
    {
        unsigned int ShaderID;
        unsigned int ProgramID;
        std::string ComputeSource;
//programs ID ?
    public:

        ComputeShaderResource(unsigned int sid, unsigned int pid, const char *cSource)
        {
            _type = ResourceType::Compute;
            ShaderID = sid;
            ProgramID = pid;
            ComputeSource = cSource;
        }

        ComputeShaderResource()
        {
            _type = ResourceType::Compute;
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
}
#endif //SOLIDEDITOR_RESOURCETYPE_HPP
