//
// Created by ryan1 on 26/02/2021.
// 
// RC: Use official copyright mark:
// Copyright (c) 2021 Solid Engine. All Rights Reserved.
//

// RC: folder named ReSSource.
// RC: ResourceMan? Should be ResourceMgr / ResourceManager.

#include "Ressources/ressources.hpp"
#include <sstream>
#include "glad/glad.h"
#include "Core/engine.hpp"
#include "Core/Debug/throwError.hpp"
using namespace Solid;

#define SASSET_GEN 1

ResourceManager* ResourceManager::instance = nullptr;
std::mutex ResourceManager::mutex = std::mutex();
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
    if(r->GetType() == EResourceType::Mesh) // RC: Please use ELSE IF or Switch case with default log.
    {
        EnginePtr->renderer->InitMesh((MeshResource*)r);
        MeshList.List.push_back(r);
    }
    if(r->GetType() == EResourceType::Shader)
        ShaderList.List.push_back(r);
    if(r->GetType() == EResourceType::Material)
        MaterialList.List.push_back(r);
    if(r->GetType() == EResourceType::Compute)
        ComputeList.List.push_back(r);
    if(r->GetType() == EResourceType::Image)
        ImageList.List.push_back(r);
    if(r->GetType() == EResourceType::Texture)
        TextureList.List.push_back(r);
    if(r->GetType() == EResourceType::Anim)
        AnimList.List.push_back(r);

}


Resource * ResourceManager::GetResourceByName(const char* name)
{
    std::string StrName =name;
    if(EnginePtr == nullptr || !EnginePtr->MultiThreadEnabled())
    {
        // RC: Bad code.
        // Use 1 switch case to get list from EResourceType.
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
        TaskManager& TaskMan = EnginePtr->taskManager;
        IDWrapper IDS[ResourceTypeNum];

        {
            // RC: TaskMgr
            int i = 0;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name), ETaskType::GENERAL, Lambda, &TextureList, &MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name), ETaskType::GENERAL, Lambda, &MeshList, &MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name), ETaskType::GENERAL, Lambda, &MaterialList, &MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name), ETaskType::GENERAL, Lambda, &ShaderList, &MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name), ETaskType::GENERAL, Lambda, &AnimList, &MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name), ETaskType::GENERAL, Lambda, &ImageList, &MtResource[i], name));
            IDS[i] = {"Find "+ std::to_string(i) + name, i};
            ++i;
            TaskMan.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + name), ETaskType::GENERAL, Lambda, &ComputeList, &MtResource[i], name));
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

ResourceManager * ResourceManager::Initialize(Engine* e)
{
    std::lock_guard<std::mutex>lck(mutex);
    if(instance != nullptr)
        ThrowError("ResourceManager is already Initialized", ESolidErrorCode::S_INIT_ERROR);

    return new ResourceManager(e);
}

ResourceManager * ResourceManager::GetInstance()
{
    std::lock_guard<std::mutex>lck(mutex);
    if(instance == nullptr)
        ThrowError("ResourceManager is not Initialized", ESolidErrorCode::S_INIT_ERROR);
    return instance;
}