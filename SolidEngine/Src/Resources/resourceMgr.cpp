//
// Created by ryan1 on 26/02/2021.
// 
//
// Copyright (c) 2021 Solid Engine. All Rights Reserved.
//

//
//

#include "Resources/ressources.hpp"
#include <sstream>
#include <Resources/resourceMgr.hpp>

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
	switch (r->GetType())
	{
		case EResourceType::Mesh:
			MeshList.List.emplace(r->name, (MeshResource*)r);
			break;
		case EResourceType::Shader:
			ShaderList.List.emplace(r->name, (ShaderResource*)r);
			break;
		case EResourceType::Material:
			MaterialList.List.emplace(r->name, (MaterialResource*)r);
			break;
		case EResourceType::Compute:
			ComputeList.List.emplace(r->name, (ComputeShaderResource*)r);
			break;
		case EResourceType::Image:
			ImageList.List.emplace(r->name, (ImageResource*)r);
			break;
		case EResourceType::Texture:
			TextureList.List.emplace(r->name, (TextureResource*)r);
			break;
		case EResourceType::Anim:
			AnimList.List.emplace(r->name, (AnimResource*)r);
			break;
		default:
			ThrowError("Type Not Stored", ESolidErrorCode::S_INIT_ERROR);
			break;
	}

}

template<class T>
T* ResourceManager::GetResourceByName(const char* _name)
{
    std::string StrName =_name;
    if(EnginePtr == nullptr || !EnginePtr->MultiThreadEnabled())
    {

        auto list = GetResourcesVecByType<T>();
        auto it = list->find(_name);
        if(it == list->end())
	        return nullptr;
	    return it->second;
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
        TaskManager& TaskMgr = EnginePtr->taskManager;
        IDWrapper IDS[ResourceTypeNum];

        {

            int i = 0;
            TaskMgr.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + _name), ETaskType::GENERAL, Lambda, &TextureList, &MtResource[i], _name));
            IDS[i] = {"Find "+ std::to_string(i) + _name, i};
            ++i;
            TaskMgr.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + _name), ETaskType::GENERAL, Lambda, &MeshList, &MtResource[i], _name));
            IDS[i] = {"Find "+ std::to_string(i) + _name, i};
            ++i;
            TaskMgr.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + _name), ETaskType::GENERAL, Lambda, &MaterialList, &MtResource[i], _name));
            IDS[i] = {"Find "+ std::to_string(i) + _name, i};
            ++i;
            TaskMgr.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + _name), ETaskType::GENERAL, Lambda, &ShaderList, &MtResource[i], _name));
            IDS[i] = {"Find "+ std::to_string(i) + _name, i};
            ++i;
            TaskMgr.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + _name), ETaskType::GENERAL, Lambda, &AnimList, &MtResource[i], _name));
            IDS[i] = {"Find "+ std::to_string(i) + _name, i};
            ++i;
            TaskMgr.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + _name), ETaskType::GENERAL, Lambda, &ImageList, &MtResource[i], _name));
            IDS[i] = {"Find "+ std::to_string(i) + _name, i};
            ++i;
            TaskMgr.AddTask(Task(Task::MakeID("Find " + std::to_string(i) + _name), ETaskType::GENERAL, Lambda, &ComputeList, &MtResource[i], _name));
            IDS[i] = {"Find "+ std::to_string(i) + _name, i};
            ++i;
        }

        bool b = true;
        while(b)
        {
            b = false;
            for (int j = 0; j <ResourceTypeNum; ++j) {
                Task* t= TaskMgr.getTaskByID(IDS[j].Name);
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

MeshResource* ResourceManager::GetRawMeshByName(const char* _name)
{
	auto list = MeshList.List;
	auto it = list.find(_name);
	if(it == list.end())
		return nullptr;
	return it->second;
}

ShaderResource* ResourceManager::GetRawShaderByName(const char* _name)
{
	auto list = ShaderList.List;
	auto it = list.find(_name);
	if(it == list.end())
		return nullptr;
	return it->second;
}

ComputeShaderResource* ResourceManager::GetRawComputeByName(const char* _name)
{
	auto list = ComputeList.List;
	auto it = list.find(_name);
	if(it == list.end())
		return nullptr;
	return it->second;
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


