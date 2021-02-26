//
// Created by ryan1 on 26/02/2021.
//

#include "Ressources/Ressources.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OBJ_Loader.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp\postprocess.h"
#include <sstream>
#include "glad/glad.h"
#include "Core/engine.hpp"
using namespace Solid;

#define SASSET_GEN 1
struct ShaderLoaderWrapper
{
    fs::path p;
    int i;
};

struct IDWrapper
{
    std::string Name;
    int i;
};


///
/// ResourceManager
///


Engine* ResourceManager::GetEngine()
{
    return EnginePtr;
}

void ResourceManager::AddResource(Resource *r)
{
    if(r == nullptr)
        return;
    if(r->GetType() == ResourceType::Mesh)
    {
        EnginePtr->renderer->InitMesh((MeshResource*)r);
        MeshList.List.push_back(r);
    }
    if(r->GetType() == ResourceType::Shader)
        ShaderList.List.push_back(r);
    if(r->GetType() == ResourceType::Material)
        MaterialList.List.push_back(r);
    if(r->GetType() == ResourceType::Compute)
        ComputeList.List.push_back(r);
    if(r->GetType() == ResourceType::Image)
        ImageList.List.push_back(r);
    if(r->GetType() == ResourceType::Texture)
        TextureList.List.push_back(r);
    if(r->GetType() == ResourceType::Anim)
        AnimList.List.push_back(r);

}


Resource * ResourceManager::GetResourceByName(const char* name)
{
    std::string StrName =name;
    if(EnginePtr == nullptr || !EnginePtr->MultiThreadEnabled())
    {
        Resource* r = TextureList.Find(name);
        if(r != nullptr)
            return r;
        r = MeshList.Find(name);
        if(r != nullptr)
            return r;
        r = MaterialList.Find(name);
        if(r != nullptr)
            return r;
        r = ShaderList.Find(name);
        if(r != nullptr)
            return r;
        r = AnimList.Find(name);
        if(r != nullptr)
            return r;
        r = ImageList.Find(name);
        if(r != nullptr)
            return r;
        r = ComputeList.Find(name);
        if(r != nullptr)
            return r;
        return nullptr;
    }
    else
    {
        const int ResourceTypeNum = 7;
        Resource* MtResource[ResourceTypeNum] {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        // for templated lambda you can use l= [](auto& var) {}; or l= []<typename T>(t& var) {};
        auto Lambda =[]<typename  T>(ResourceList<T>* List, Resource** PtrWrapper, const char* _name)
        {
            *PtrWrapper = List->Find(_name);
        };
        //Lambda(MeshList, &MtResource[0], name);
        TaskManager& TaskMan = EnginePtr->MultiTask;
        IDWrapper IDS[ResourceTypeNum];

        {
            int i = 0;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name),TaskType::GENERAL, Lambda,&TextureList,&MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name),TaskType::GENERAL, Lambda,&MeshList,&MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name),TaskType::GENERAL, Lambda,&MaterialList,&MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name),TaskType::GENERAL, Lambda,&ShaderList,&MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name),TaskType::GENERAL, Lambda,&AnimList,&MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name),TaskType::GENERAL, Lambda,&ImageList,&MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name),TaskType::GENERAL, Lambda,&ComputeList,&MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
        }

        bool b = true;
        while(b)
        {
            b = false;
            for (int j = 0; j <ResourceTypeNum; ++j) {
                Task* t= TaskMan.getTaskByID(IDS[j].Name);
                if(t == nullptr)
                    continue;
                if(!t->IsFinished())
                {
                    b = true;
                }
                else
                {
                    IDS[j].Name = "";
                    int k = IDS[j].i;
                    if(MtResource[k] != nullptr) {
                        return MtResource[k];
                    }
                }
            }
        }

    }
    return nullptr;
}