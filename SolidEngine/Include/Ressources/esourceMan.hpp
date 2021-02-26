//
// Created by ryan1 on 26/02/2021.
//

#ifndef SOLIDEDITOR_ESOURCEMAN_HPP
#define SOLIDEDITOR_ESOURCEMAN_HPP
#include <mutex>
namespace fs = std::filesystem;
namespace Solid
{
    class SOLID_API ResourceManager
    {
        template<class T>
        struct ResourceList
        {
            std::vector<Resource*> List;
            const char* type_value = typeid(T*).name();
            ~ResourceList()
            {
                for(Resource* r : List)
                {
                    if(r != nullptr)
                        delete r;
                }
            }
            Resource* Find(const char* name)
            {
                for(Resource* r : List)
                {
                    if(r == nullptr)
                        continue;
                    if(r->name == name)
                        return (r);
                }
                return nullptr;
            }
        };

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


        Resource* GetResourceByName(const char* name);

        template<typename T>
        std::vector<Resource*> * GetResourcesVecByType()
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

    };
}


#endif //SOLIDEDITOR_ESOURCEMAN_HPP
