#pragma once

#include <filesystem>
#include <sndfile.h>
#include <deque>

#include "Core/Maths/solidVector.hpp"
#include "Core/Maths/Matrix/matrix4.hpp"
#include "json.hpp"
#include "AL/alc.h"
#include "AL/al.h"
#include "AL/alext.h"


namespace fs = std::filesystem;
namespace Solid {
	typedef nlohmann::ordered_json json ;
	class ITexture;
	class IShader;
	class ICompute;

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
        Prefab,
        Cubemap,
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
    public:
	    std::deque<std::string> path;
	    std::string name;

        EResourceType GetType() { return type; }
	    virtual void ToDataBuffer(std::vector<char> &buffer);

	    virtual int FromDataBuffer(char *buffer, size_t bSize);

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

	    virtual void ToDataBuffer(std::vector<char> &buffer) override;

	    virtual int FromDataBuffer(char *buffer, size_t bSize) override;

    };

    class SOLID_API CubemapResource : public Resource
    {
    public:
        struct CubemapImg
        {
            int x;
            int y;
            int ChannelsNum;
            std::vector<unsigned char> image;
        };

        std::array<CubemapImg,6> imageMap;

        CubemapResource() {
            type = EResourceType::Cubemap;
        }

        ~CubemapResource() = default;

        virtual void ToDataBuffer(std::vector<char> &buffer) override;

        virtual int FromDataBuffer(char *buffer, size_t bSize) override;

    };

    struct SOLID_API Vertex
    {
        Vec3 Pos;
        Vec3 Normal;
        Vec2 TexCoords;
    };

    struct SOLID_API AnimData
    {
        int boneIds[4] {-1,-1,-1,-1};
        float  weights[4] {0.0f,0.0f,0.0f,0.0f};
    };


    class SOLID_API MeshResource : public Resource
    {
    public:


        class SubMesh
        {
        public:

            std::vector<Vertex> vertices;
            std::vector<uint> indices;
            std::vector<AnimData> animData;
        };

        bool hadAnim = false;


        std::vector<SubMesh> Meshes;


        bool isInit = false;

        MeshResource()
        {
            type = EResourceType::Mesh;
        }

        ~MeshResource()
        {

        }

        virtual void ToDataBuffer(std::vector<char> &buffer) override;

        virtual int FromDataBuffer(char *buffer, size_t bSize) override;
    };

	class SOLID_API SkeletonResource : public Resource
	{
	public:
		class Bone
		{
			bool isPtr = false;
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
		    BoneChannel channel;
			std::string name;
			Bone* Parent = nullptr;
			std::vector<Bone*> Childrens;
			std::vector<float> Weights;
			bool WeightInit = false;

            Mat4f LocalTrans;
            Mat4f offset;
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

	    //std::vector<BoneChannel> Channels;
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
		void operator delete(void* _ptr);
	    virtual void ToDataBuffer(std::vector<char> &buffer) override;

	    virtual int FromDataBuffer(char *buffer, size_t bSize) override;
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

	    virtual void ToDataBuffer(std::vector<char> &buffer) override;

	    virtual int FromDataBuffer(char *buffer, size_t bSize) override;
    };

    struct MatText
    {
    	bool isUsingComputeGeneratedTex = false;
	    std::shared_ptr<ITexture> text;
	    std::shared_ptr<ICompute> Compute;
    };


    class SOLID_API MaterialResource : public Resource
    {
    private:
        std::shared_ptr<IShader> shader;
        std::shared_ptr<const IShader> defaultshader;
    public:
        enum class EShaderFieldType : int
        {
            BOOL,
            INT,
            FLOAT,
            VEC2,
            VEC3,
            VEC4,
            TEXT,
            NONE
        };
        class SOLID_API ShaderField
        {
        public:
            std::string name;
            EShaderFieldType type;
            union
            {
                bool   b;
                int    i;
                float  f;
                Vec2   v2;
                Vec3   v3;
                Vec4   v4;

            };
	        MatText text;
	        ShaderField();

            ShaderField(EShaderFieldType _type);
            ShaderField(const struct ShaderUniform& _uniform);
            ShaderField(const ShaderField& _copy);
            ~ShaderField(){};
            ShaderField& operator=(const ShaderField& _copy);
        };

	    bool shouldGenerateFileAtDestroy = true;

	    std::vector<ShaderField> fields;

        MaterialResource();
	    MaterialResource(const char* _name, bool _genfile = true);


        ~MaterialResource();

	    virtual void ToDataBuffer(std::vector<char> &buffer) override;
	    virtual int FromDataBuffer(char *buffer, size_t bSize) override;

        const std::shared_ptr<IShader> GetShader() const;
        const std::shared_ptr<const IShader> GetDefaultshader() const;

        void SetShader(const std::shared_ptr<IShader> _shader);
        void LoadShaderFields();

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
		virtual void ToDataBuffer(std::vector<char> &buffer) override;

		virtual int FromDataBuffer(char *buffer, size_t bSize) override;
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
		virtual void ToDataBuffer(std::vector<char> &buffer) override;

		virtual int FromDataBuffer(char *buffer, size_t bSize) override;

	};
	class SOLID_API PrefabResource : public Resource
	{
	public:
	//public members
	std::vector<char> PrefabBinary;

	protected:
		//protected members

	public:
	//public func
	    PrefabResource()
		{
			type =EResourceType::Prefab;
		}
	    ~PrefabResource() = default;

		void UpdatePrefab(GameObject* _gameObject);


		virtual void ToDataBuffer(std::vector<char> &buffer) override;

		virtual int FromDataBuffer(char *buffer, size_t bSize) override;

	};
}
