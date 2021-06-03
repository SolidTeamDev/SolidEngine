#pragma once

namespace fs = std::filesystem;

namespace Solid
{
	struct SOLID_API FBXWrapper
	{
		MeshResource* mesh = nullptr;
		SkeletonResource* Skeleton = nullptr;
		std::vector<AnimResource*> anims;

	};
    struct SOLID_API ResourcePtrWrapper
    {
        Resource* r = nullptr;
        bool isFBX = false;
	    FBXWrapper fbx;
    };

    class SOLID_API ResourcesLoader
    {
        ResourceManager* Manager;
        Resource * LoadImage(const fs::path& Rpath);
        Resource * LoadCubemap(const fs::path& Rpath);
	    ///void LoadMeshOBJ(const fs::path& Rpath);
        Resource * LoadMesh(const fs::path& Rpath);
	    Resource * LoadAnim(const fs::path& Rpath);
	    Resource * LoadShader(const fs::path& Rpath);
	    Resource * LoadTexture(const fs::path& Rpath);
	    Resource * LoadAudio(const fs::path& Rpath);
	    Resource * LoadSolidImage(const fs::path& Rpath);
	    Resource * LoadSolidTex(const fs::path& Rpath);
	    Resource * LoadSolidMesh(const fs::path& Rpath);
	    Resource * LoadSolidShader(const fs::path& Rpath);
	    Resource * LoadSolidComputeShader(const fs::path& Rpath);
	    Resource * LoadSolidMaterial(const fs::path& Rpath);
	    Resource * LoadSolidAnim(const fs::path& Rpath);
	    Resource * LoadSolidAudio(const fs::path& Rpath);
	    Resource * LoadSolidPrefab(const fs::path& Rpath);
	    Resource * LoadSolidScene(const fs::path& Rpath);
	    void LoadFBX(const fs::path& Rpath, FBXWrapper* fbx);
    public:
	    void SetPath(std::deque<std::string> &resPath, const fs::path& Rpath);




	    static fs::path SolidPath ;


        ResourcesLoader()
        {

        };
	    ~ResourcesLoader() = default;
	    void SetManager(ResourceManager* m);
	    void LoadRessource(const fs::path& Rpath);
	    void ReLoadRessource(Resource* _resource);
        void LoadRessourceNoAdd(const fs::path &Rpath, ResourcePtrWrapper &wrapper);
        void LoadResourcesFromFolder(const fs::path& Rpath);
        static void Append(std::vector<char>& DataBuffer, void* Data, std::uint64_t sizeInByte);
        void SaveMaterialToFile(MaterialResource* _mat);
	    void SavePrefabToFile(PrefabResource* _prefab);
        //inline static void ReadFromBuffer(std::vector<char>& DataBuffer, void* Data, std::uint64_t sizeInByte, std::uint64_t& ReadPos);
        static void ReadFromBuffer(char *DataBuffer, void *Data, std::uint64_t sizeInByte, std::uint64_t &ReadPos,
                                   std::size_t DataBufferSize);
    };
}
