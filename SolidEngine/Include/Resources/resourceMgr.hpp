#pragma once

#include <mutex>
#include <unordered_map>
#include "Core/Debug/throwError.hpp"

namespace fs = std::filesystem;

namespace Solid
{

	struct ResourcesPathData
	{
		std::string RName;
		EResourceType RType;
		Resource* RPtr= nullptr;
		std::deque<std::string> rPath;
	};

    template<class T>
    struct SOLID_API ResourceList
    {
        std::unordered_map<std::string,Resource*> List;
        const char* type_value = typeid(T*).name();
        ~ResourceList()
        {
            for(auto& r : List)
            {
                if(r.second != nullptr)
                    delete r.second;
            }
        }
        T* Find(const char* name)
        {
            auto it = List.find(name);
            if(it == List.end())
                return nullptr;
            else
                return (T*)it->second;

        }
        void addPathToVec(std::vector<ResourcesPathData>& vec)
        {
        	for(auto& elt : List)
        	{
        	    vec.push_back({.RName=elt.second->name, .RType=elt.second->GetType(), .RPtr=elt.second, .rPath=elt.second->path});
        	}
        }
    };

    class SOLID_API ResourceManager
    {
    private:

        ResourceList<ImageResource>          ImageList;
        ResourceList<MeshResource>           MeshList;
	    ResourceList<AnimResource>           AnimList;
	    ResourceList<SkeletonResource>       SkeletonList;
        ResourceList<ShaderResource>         ShaderList;
        ResourceList<ComputeShaderResource>  ComputeList;
	    ResourceList<MaterialResource>       MaterialList;
	    ResourceList<AudioResource>          AudioList;
	    ResourceList<SceneResource>          SceneList;
	    ResourceList<PrefabResource>         PrefabList;
        const MaterialResource* defaultMat = nullptr;
        class Engine* EnginePtr = nullptr;
        bool defaultMatInit = false;


    private:



    public:
	    explicit ResourceManager(class Engine* _engine);
	    ~ResourceManager()
	    {

	    }
        ResourceManager(ResourceManager&) = delete;
        void operator=(const ResourceManager&) = delete;

	    void InitDefaultMat();
	    const MaterialResource* GetDefaultMat();
        class Engine* GetEngine();


        void AddResource(Resource* r);

	    template<class T>
	    T* GetResourceByName(const char* name);
	    bool IsResourceExist(Resource *r);
	    std::vector<ResourcesPathData> GetAllResourcesPath();
	    MeshResource* GetRawMeshByName(const char* name);
	    ShaderResource* GetRawShaderByName(const char* name);
	    ComputeShaderResource* GetRawComputeByName(const char* name);
	    MaterialResource* GetRawMaterialByName(const char* name);
	    ImageResource* GetRawImageByName(const char* name);
	    AudioResource* GetRawAudioByName(const char* name);
	    MaterialResource* CreateMaterial(const char* name);
	    PrefabResource* GetPrefabByName(const char* name);
	    void CreatePrefab(GameObject* _gameObject);

        template<typename T>
        std::unordered_map<std::string,Resource*> * GetResourcesVecByType()
        {

            std::string_view type_value = typeid(T*).name();
            if(type_value == ImageList.type_value)
                return &(ImageList.List);

            if(type_value == MeshList.type_value)
                return &(MeshList.List);

            if(type_value == AnimList.type_value)
                return &(AnimList.List);

            if(type_value == ShaderList.type_value)
                return &(ShaderList.List);

            if(type_value == ComputeList.type_value)
                return &(ComputeList.List);

            if(type_value == MaterialList.type_value)
                return &(MaterialList.List);

            if(type_value == AudioList.type_value)
                return  &(AudioList.List);

	        if(type_value == PrefabList.type_value)
		        return  &(PrefabList.List);
            if(type_value == SkeletonList.type_value)
                return  &(SkeletonList.List);

            return nullptr;
        }
	    template<typename T>
	    std::unordered_map<std::string,Resource*> * GetResourcesVecByType(EResourceType _type)
	    {
		    switch (_type)
		    {
			    case EResourceType::Mesh:
				    return &MeshList.List;
				    break;
			    case EResourceType::Shader:
				    return &ShaderList.List;
				    break;
			    case EResourceType::Material:
				    return &MaterialList.List;
				    break;
			    case EResourceType::Compute:
				    return &ComputeList.List;
				    break;
			    case EResourceType::Image:
				    return &ImageList.List;
				    break;
			    case EResourceType::Anim:
				    return &AnimList.List;
				    break;
		        case EResourceType::Audio:
                    return &AudioList.List;
                    break;
			    case EResourceType::Skeleton:
				    return &SkeletonList.List;
				    break;
			    case EResourceType::Scene:
				    return &SceneList.List;
				    break;
			    case EResourceType::Prefab:
				    return &PrefabList.List;
				    break;
			    default:
				    ThrowError("Type Not Stored", ESolidErrorCode::S_INIT_ERROR);
				    return nullptr;
				    break;
		    }
	    };

    };
}
