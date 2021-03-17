//
// Created by ryan1 on 26/02/2021.
//

#ifndef SOLIDEDITOR_RESOURCETYPE_HPP
#define SOLIDEDITOR_RESOURCETYPE_HPP
#include <filesystem>

#include "Core/Maths/solidVector.hpp"
#include "json.hpp"




namespace fs = std::filesystem;
namespace Solid {
	typedef nlohmann::json json;

	struct ShaderBinary
	{
		uint size = 0;
		uint format;
		char* b = nullptr;
	};

    enum class SOLID_API EResourceType : char
    {
        Image = 0,
        Mesh,
        Anim,
        Shader,
        Compute,
        Material,
        Texture,
        Scene,
        NONE,
    };

    class SOLID_API Resource {
    public:


        Resource() : type(EResourceType::NONE) {

        }

        virtual ~Resource() {

        }

        static int NoNameNum;
    protected:
        EResourceType type;
        fs::path path;
    public:
        std::string name;

        EResourceType GetType() { return type; }

    };

    class SOLID_API ImageResource : public Resource
    {
    public:
        int x;
        int y;
        int ChannelsNum;
        std::vector<unsigned char> image;

        ImageResource() {
            type = EResourceType::Image;
        }

        ~ImageResource();

        void ToDataBuffer(std::vector<char> &buffer);

//        void FromDataBuffer(std::vector<char>& buffer);
        void FromDataBuffer(char *buffer, int bSize);

    };



    struct SOLID_API Vertex
    {
        Vec3 Pos;
	    Vec3 Normal;
	    Vec2 TexCoords;
    };


    class SOLID_API MeshResource : public Resource
    {


    public:


        class SubMesh
        {
        public:

            std::vector<Vertex> vertices;
            std::vector<uint> indices;
        };

        std::vector<SubMesh> Meshes;


        bool isInit = false;

        MeshResource()
        {
            type = EResourceType::Mesh;
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
            type = EResourceType::Anim;
        }

        ~AnimResource()
        {

        }
    };

    class SOLID_API ShaderResource : public Resource
    {


//programs ID ?
    public:
	    std::string VertexSource;
	    std::string FragSource;
	    ShaderBinary binaries = {.b=nullptr};
        ShaderResource(const char *vs, const char *fs)
        {
            type = EResourceType::Shader;

            VertexSource = vs;
            FragSource = fs;
        }

        ShaderResource()
        {
            type = EResourceType::Shader;
        }

        ~ShaderResource()
        {

        }

        void ToDataBuffer(std::vector<char> &buffer);

        void FromDataBuffer(char *buffer, int bSize);
    };

    class SOLID_API ComputeShaderResource : public Resource
    {



    public:
	    std::string ComputeSource;
	    ShaderBinary binaries = {.b=nullptr};
        ComputeShaderResource(const char *cSource)
        {
            type = EResourceType::Compute;
            ComputeSource = cSource;
        }

        ComputeShaderResource()
        {
            type = EResourceType::Compute;
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
            type = EResourceType::Material;
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
            type = EResourceType::Texture;
        }

        ~TextureResource()
        {

        }
    };
	class SOLID_API SceneResource : public Resource
	{
	public:
		SceneResource()
		{
			type = EResourceType::Scene;
		}

		~SceneResource()
		{

		}
	};
}


#endif //SOLIDEDITOR_RESOURCETYPE_HPP
