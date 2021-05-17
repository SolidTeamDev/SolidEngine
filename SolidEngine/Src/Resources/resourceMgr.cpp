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
			MeshList.List.emplace(r->name, r);
			break;
		case EResourceType::Shader:
			ShaderList.List.emplace(r->name, r);
            break;
		case EResourceType::Material:
			MaterialList.List.emplace(r->name, r);
            break;
		case EResourceType::Compute:
			ComputeList.List.emplace(r->name, r);
            break;
		case EResourceType::Image:
			ImageList.List.emplace(r->name, r);
            break;
		case EResourceType::Anim:
			AnimList.List.emplace(r->name, r);
            break;
		case EResourceType::Audio:
			AudioList.List.emplace(r->name, r);
			break;
		case EResourceType::Skeleton:
			SkeletonList.List.emplace(r->name, r);
			break;
		case EResourceType::Scene:
			SceneList.List.emplace(r->name, r);
			break;
		case EResourceType::Prefab:
			PrefabList.List.emplace(r->name, r);
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
	return (MeshResource*)it->second;
}

ShaderResource* ResourceManager::GetRawShaderByName(const char* _name)
{
	auto list = ShaderList.List;
	auto it = list.find(_name);
	if(it == list.end())
		return nullptr;
	return (ShaderResource*)it->second;
}

ComputeShaderResource* ResourceManager::GetRawComputeByName(const char* _name)
{
	auto list = ComputeList.List;
	auto it = list.find(_name);
	if(it == list.end())
		return nullptr;
	return (ComputeShaderResource*)it->second;
}

AudioResource *ResourceManager::GetRawAudioByName(const char *_name)
{
	auto list = AudioList.List;
	auto it = list.find(_name);
	if(it == list.end())
		return nullptr;
	return (AudioResource*)it->second;
}


void ResourceManager::InitDefaultMat()
{
	if(defaultMatInit)
		return;
	defaultMat = new MaterialResource("DEFAULT MATERIAL", false);
	defaultMatInit = true;
}

const MaterialResource* ResourceManager::GetDefaultMat()
{
	return defaultMat;
}

MaterialResource *ResourceManager::GetRawMaterialByName(const char *_name)
{
	auto list = MaterialList.List;
	auto it = list.find(_name);
	if(it == list.end())
		return nullptr;
	return (MaterialResource*)it->second;
	return nullptr;
}

ImageResource *ResourceManager::GetRawImageByName(const char *_name)
{
	auto list = ImageList.List;
	auto it = list.find(_name);
	if(it == list.end())
		return nullptr;
	return (ImageResource*)it->second;
	return nullptr;
}

MaterialResource *ResourceManager::CreateMaterial(const char *name)
{
	MaterialResource* mat = new MaterialResource();
	mat->name = name;
	mat->path.emplace_back("\\Assets\\");

	AddResource(mat);
	ResourcesLoader loader;
	loader.SaveMaterialToFile(mat);
	Log::Send("Material {" + mat->name + "} Has been Created");
	return mat;
}

ResourceManager::ResourceManager(struct Engine *_engine)
{
	EnginePtr =_engine;
}

std::vector<ResourcesPathData> ResourceManager::GetAllResourcesPath()
{
	std::vector<ResourcesPathData> vec;
	ImageList.addPathToVec(vec);
	MeshList.addPathToVec(vec);
	AnimList.addPathToVec(vec);
	SkeletonList.addPathToVec(vec);
	ShaderList.addPathToVec(vec);
	ComputeList.addPathToVec(vec);
	MaterialList.addPathToVec(vec);
	AudioList.addPathToVec(vec);
	SceneList.addPathToVec(vec);
	PrefabList.addPathToVec(vec);
	return vec;
}

void ResourceManager::CreatePrefab(GameObject* _gameObject)
{

}


