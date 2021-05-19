//
// Created by ryan1 on 26/02/2021.
//

#ifndef SOLIDEDITOR_RESOURCETYPE_HPP
#define SOLIDEDITOR_RESOURCETYPE_HPP
#include <filesystem>
#include <sndfile.h>

#include "Core/Maths/solidVector.hpp"
#include "Core/Maths/Matrix/matrix4.hpp"
#include "json.hpp"
#include "AL/alc.h"
#include "AL/al.h"
#include "AL/alext.h"


namespace fs = std::filesystem;
namespace Solid {
	typedef nlohmann::json json;
	class ITexture;
	class IShader;

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
        Scene,
        Audio,
        Skeleton,
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
        fs::path assetProjectPath;
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

	class SOLID_API SkeletonResource : public Resource
	{
	public:
		class Bone
		{
			bool isPtr = false;
		public:
			std::string name;
			Bone* Parent = nullptr;
			std::vector<Bone*> Childrens;
			std::vector<float> Weights;
			bool WeightInit = false;

            Mat4f T_pos;
            Mat4f LocalTrans;
            Mat4f GlobalTrans;
            Mat4f offset; // inverse bind pose
			Mat4f FinalTrans;
			int id = -1;
			bool isAnimated = false;
			Bone() = default;
			~Bone();
			Bone& operator=(const Bone& b);
			Bone(const Bone& b);
			Bone(const Bone& b, Bone* toSet);
			void* operator new(std::size_t size);
			void operator delete(void*);
			//find Bone Anywhere on the tree below used node
			Bone* FindBoneByName(const char* name);
		}rootBone;

		uint numOfBone = 0;
		SkeletonResource()
		{
			type = EResourceType::Skeleton;
		}

		~SkeletonResource()
		{

		}
	};

    class SOLID_API AnimResource : public Resource
    {
    public:
	    struct KeyFrame
	    {

			double time;
			bool usePos= false;
	    	Vec3 pos;
		    bool useRot= false;
	    	Quat Rot;
		    bool useScale = false;
	    	Vec3 Scale;
	    };
	    struct BoneChannel
	    {
	    	SkeletonResource::Bone* BoneToMod = nullptr;
	    	std::vector<KeyFrame> Frames;
	    	std::size_t currentFrame = 0;
	    };
	    std::vector<BoneChannel> Channels;
	    SkeletonResource::Bone* Root = nullptr;
	    uint numOfBones = 0;
    	double numTicks;
	    double ticksPerSeconds;


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
	    enum class EFieldType : int
	    {
	    	BOOL = 1,
	    	INT,
	    	FLOAT,
	    	VEC2,
	    	VEC3,
	    	VEC4,
	    	NONE
	    };
		class SOLID_API FieldValue
		{
		public:
			EFieldType type;
			union
			{
				bool   b;
				int    i;
				float  f;
				Vec2   v2;
				Vec3   v3;
				Vec4   v4;
			};

			FieldValue(EFieldType _type);
			FieldValue(const FieldValue& copy);
		};

    	std::shared_ptr<IShader> shader;
	    bool shouldGenerateFileAtDestroy = true;

	    std::shared_ptr<const IShader> defaultshader;
	    std::unordered_map<std::string, std::shared_ptr<ITexture>> TexturesProperties;
	    std::unordered_map<std::string, FieldValue> ValuesProperties;


        MaterialResource();
	    MaterialResource(const char* _name, bool _genfile = true);


        ~MaterialResource();

	    void ToDataBuffer(std::vector<char> &buffer);

	    void FromDataBuffer(char *buffer, int bSize);

    };

	class SOLID_API SceneResource : public Resource
	{
	public:
		std::vector<char> rawScene;
		SceneResource()
		{
			type = EResourceType::Scene;
		}

		~SceneResource()
		{

		}
	};
	class SOLID_API AudioResource : public Resource
	{
	public:
		std::vector<short> audioRawBinary;
		SF_INFO info;
		ALuint buffer;
		sf_count_t numFrames;
		ALenum format;
		AudioResource()
		{
			type = EResourceType::Audio;
		}

		~AudioResource();
		void ToDataBuffer(std::vector<char> &buffer);

		void FromDataBuffer(char *buffer, int bSize);

	};
}


#endif //SOLIDEDITOR_RESOURCETYPE_HPP
