//
// Created by ryan1 on 26/02/2021.
//

#ifndef SOLIDEDITOR_RESOURCEMGR_HPP
#define SOLIDEDITOR_RESOURCEMGR_HPP
#include <mutex>
#include <unordered_map>
#include "Core/Debug/throwError.hpp"
namespace fs = std::filesystem;
namespace Solid
{

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
    };

    class SOLID_API ResourceManager
    {
    private:

        ResourceList<ImageResource>          ImageList;
        ResourceList<MeshResource>           MeshList;
        ResourceList<AnimResource>           AnimList;
        ResourceList<ShaderResource>         ShaderList;
        ResourceList<ComputeShaderResource>  ComputeList;
        ResourceList<MaterialResource>       MaterialList;
        ResourceList<TextureResource>        TextureList;
        class Engine* EnginePtr = nullptr;


    private:
        static std::mutex mutex;
        static ResourceManager* instance;
        explicit ResourceManager(class Engine* _engine)
        {
            EnginePtr =_engine;
        }
        ~ResourceManager()
        {
        }

    public:
        ResourceManager(ResourceManager&) = delete;
        void operator=(const ResourceManager&) = delete;

        static ResourceManager* Initialize(class Engine* e);
        static ResourceManager* GetInstance();

        class Engine* GetEngine();


        void AddResource(Resource* r);

	    template<class T>
	    T* GetResourceByName(const char* name);
	    MeshResource* GetRawMeshByName(const char* name);
	    ShaderResource* GetRawShaderByName(const char* name);
	    ComputeShaderResource* GetRawComputeByName(const char* name);

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

            if(type_value == TextureList.type_value)
                return &(TextureList.List);

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
			    case EResourceType::Texture:
				    return &TextureList.List;
				    break;
			    case EResourceType::Anim:
				    return &AnimList.List;
				    break;
			    default:
				    ThrowError("Type Not Stored", ESolidErrorCode::S_INIT_ERROR);
				    return nullptr;
				    break;
		    }
	    };

    };
}


#endif //SOLIDEDITOR_RESOURCEMGR_HPP
