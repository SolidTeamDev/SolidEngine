//
// Created by ryan1 on 26/02/2021.
//

#include "Resources/ressources.hpp"

#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"
#include "OBJ_Loader.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp\postprocess.h"
#include "AL/alc.h"
#include "AL/al.h"
#include "AL/alext.h"
#include <vector>
#include <inttypes.h>
#include "sndfile.h"
#include <time.h>
#include <sstream>
#include <Resources/resourceLoader.hpp>

#include "glad/glad.h"
#include "Core/engine.hpp"
#undef LoadImage
using namespace Solid;

__declspec(dllexport) fs::path ResourcesLoader::SolidPath ="";

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
const char* rootFolder = "\\Assets\\";
///
/// ResourcesLoader Base
///


void ResourcesLoader::SetManager(ResourceManager *m)
{
    Manager = m;
}

void  ResourcesLoader::LoadRessourceNoAdd(const fs::path &Rpath, ResourcePtrWrapper &wrapper)
{
    if(!fs::exists(Rpath))
        return;
    Resource* r = nullptr;
    if(fs::is_directory(Rpath))
    {
        std::string name = Rpath.filename().string();
        std::transform(name.begin(), name.end(), name.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if(name.find("shader") != std::string::npos || name.find("compute") != std::string::npos)
            r=LoadShader(Rpath);
        else
        {
            return;
        }
    }

    std::string extension = Rpath.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    ///Solid Asset Loading
    ///material loding is not here because it depends on Graphical Resources and so we cannot multithread it


    if(extension == ".simage")
        r=LoadSolidImage(Rpath);
    else if(extension == ".smesh")
        r=LoadSolidMesh(Rpath);
    else if(extension == ".scompute")
        r=LoadSolidComputeShader(Rpath);
    else if(extension == ".svertfrag")
        r=LoadSolidShader(Rpath);
    else if(extension == ".solidprefab")
	    r=LoadSolidPrefab(Rpath);
    else if(extension == ".solidscene")
	    r=LoadSolidScene(Rpath);
    else if(extension == ".sanim")
        ;
    else if(extension == ".sskel")
	    ;
    /// Image Loading
    else if(extension == ".bmp")
        r=LoadImage(Rpath);
    else if(extension == ".png")
        r=LoadImage(Rpath);
    else if(extension == ".jpg")
        r=LoadImage(Rpath);
    else if(extension == ".jpeg")
        r=LoadImage(Rpath);

    ///Mesh Loading
    else if(extension == ".obj")
        r=LoadMesh(Rpath);
    else if(extension == ".fbx")
    {
	    LoadFBX(Rpath, &wrapper.fbx);
	    wrapper.isFBX = true;
    }


    wrapper.r = r;
}
void ResourcesLoader::LoadRessource(const fs::path &Rpath)
{
    ResourcePtrWrapper RWrapper {nullptr};
    LoadRessourceNoAdd(Rpath, RWrapper);
    if(RWrapper.r != nullptr)
        Manager->AddResource(RWrapper.r);
    if(RWrapper.isFBX)
    {
    	Manager->AddResource(RWrapper.fbx.mesh);
	    Manager->AddResource(RWrapper.fbx.Skeleton);
	    for(AnimResource* elt : RWrapper.fbx.anims)
	    {
		    Manager->AddResource(elt);
	    }
    }
}



void ResourcesLoader::LoadResourcesFromFolder(const fs::path &Rpath)
{
    if(!fs::exists(Rpath) || fs::is_empty(Rpath))
        return;
    ///MultiThread Loading
    if(Manager->GetEngine()->MultiThreadEnabled())
    {
        TaskManager& TaskMan = Manager->GetEngine()->taskManager;
        using fp = bool (*)( const fs::path&);

        /////////////////////////////
	    auto SolidFind = [](const fs::path&item)
	    {
		    if(!fs::is_directory(item))
		    {
			    std::string name = item.extension().string();
			    std::transform(name.begin(), name.end(), name.begin(),
			                   [](unsigned char c){ return std::tolower(c); });

			    return (name.find(".simage") != std::string::npos
			        || name.find(".smesh") != std::string::npos
			        || name.find(".scompute") != std::string::npos
			        || name.find(".svertfrag") != std::string::npos
			        || name.find(".sanim") != std::string::npos
			        || name.find(".sskel") != std::string::npos
			        || name.find(".smaterial") != std::string::npos
			        || name.find(".solidprefab") != std::string::npos
			        || name.find(".solidscene") != std::string::npos
			        || name.find(".saudio") != std::string::npos);

		    }
		    return false;
	    };
	    auto NormalFind = [](const fs::path&item)
	    {
		    if(fs::is_directory(item))
		    {
			    std::string name = item.filename().string();
			    std::transform(name.begin(), name.end(), name.begin(),
			                   [](unsigned char c){ return std::tolower(c); });
			    return (name.find("shader") != std::string::npos || name.find("compute") != std::string::npos);

		    }
		    else
		    {
			    std::string name = item.extension().string();
			    std::transform(name.begin(), name.end(), name.begin(),
			                   [](unsigned char c){ return std::tolower(c); });
			    return (name.find(".bmp") != std::string::npos
			            || name.find(".png") != std::string::npos
			            || name.find(".jpg") != std::string::npos
			            || name.find(".jpeg") != std::string::npos
			            || name.find(".obj") != std::string::npos
			            || name.find(".fbx") != std::string::npos
			            || name.find(".wav") != std::string::npos
			            || name.find(".ogg") != std::string::npos);

		    }
		    return false;
	    };
	    auto FoldersFind = [](const fs::path&item)
	    {
		    if(fs::is_directory(item))
		    {
			    std::string name = item.filename().string();
			    std::transform(name.begin(), name.end(), name.begin(),
			                   [](unsigned char c){ return std::tolower(c); });
			    return (name.find("shader") == std::string::npos && name.find("compute") == std::string::npos);

		    }
		    else
		    {
			    return false;
		    }
		    return false;
	    };
	    std::vector<fs::path> normal;
	    std::vector<fs::path> Solid;
	    std::vector<fs::path> ToLoad;
	    std::vector<fs::path> othersFolders;

        for(auto& item : fs::directory_iterator(Rpath))
        {
			if(NormalFind(item))
			{
				normal.push_back(item);
			}
			else if(SolidFind(item))
			{
				Solid.push_back(item);
			}
			else if(FoldersFind(item))
			{
				othersFolders.push_back(item);
			}
        }
        int counting = 0;
        for(auto it = othersFolders.begin(); it != othersFolders.end(); ++it)
        {
        	bool newFolders = false;
	        for(auto& item : fs::directory_iterator(*it))
	        {
		        if(NormalFind(item))
		        {
			        normal.push_back(item);
		        }
		        else if(SolidFind(item))
		        {
			        Solid.push_back(item);
		        }
		        else if(FoldersFind(item))
		        {
			        othersFolders.push_back(item);
			        newFolders = true;
		        }
	        }
	        ++counting;
	        if(newFolders)
	        	it = othersFolders.begin()+counting;
        }
	    for(auto& elt : normal)
	    {
		    int i = 0;
		    for(auto it = normal.begin(); it!= normal.end() ; ++it)
		    {
			    fs::path& elt2 = *it;
			    if(elt == elt2)
				    continue;
			    if(elt.filename() == elt2.filename())
			    {
				    int count = 0;
				    for(auto& elt3 : Solid)
				    {
					    if(elt3.filename().string().find(elt2.filename().string()) != std::string::npos)
					    {
						    fs::remove(elt3);
						    Solid.erase(Solid.begin()+count);
						    break;
					    }
					    count++;
				    }
				    fs::remove(elt2);
				    Log::Send(elt2.filename().string() + " AT " + elt2.string() + " Was Removed Because a Duplicate File was Found ", Log::ELogSeverity::WARNING);
				    normal.erase(it);
				    it = normal.begin()+i;
				    continue;
			    }
			    i++;
		    }

	    }

	    for(auto& elt : Solid)
	    {
		    int index = 0;
		    for(auto it = Solid.begin(); it!= Solid.end() ; ++it)
		    {
			    fs::path& elt2 = *it;
			    if(elt == elt2)
				    continue;
			    if(elt.filename() == elt2.filename())
			    {
				    fs::remove(elt2);
				    Log::Send(elt2.filename().string() + " AT " + elt2.string() + " Was Removed Because a Duplicate File was Found ", Log::ELogSeverity::WARNING);
				    Solid.erase(it);
				    it = Solid.begin()+index;
				    continue;
			    }
			    index++;
		    }

	    }
	    for(auto& elt : normal)
	    {
		    int index = 0;
		    for(auto it = Solid.begin(); it!= Solid.end() ; ++it)
		    {
			    fs::path& elt2 = *it;
			    if(elt == elt2)
				    continue;
			    if(elt2.filename().string().find(elt.filename().string()) != std::string::npos)
			    {
			    	if(elt.parent_path() == elt2.parent_path())
					    continue;
				    fs::remove(elt2);
				    Log::Send(elt2.filename().string() + " AT " + elt2.string() + " Was Removed Because a Duplicate File was Found ", Log::ELogSeverity::WARNING);
				    Solid.erase(it);
				    it = Solid.begin()+index;
				    continue;
			    }
			    index++;
		    }
	    }

		for(auto& elt : normal)
		{
			bool found = false;
			int count = 0;
			for(auto& elt2 : Solid)
		    {
		        if(elt2.filename().string().find(elt.filename().string()) != std::string::npos)
		        {
		        	ToLoad.push_back(elt2);
		        	Solid.erase(Solid.begin()+count);
		        	found = true;
			        break;
		        }
		        count++;
		    }
			if(!found)
			{
				ToLoad.push_back(elt);
			}
		}

	    for(auto& elt : Solid)
	    {
		    ToLoad.push_back(elt);
	    }



        //////////////////////////////


        auto shaderFind = [](const fs::path&item)
        {
            if(fs::is_directory(item))
            {
                std::string name = item.filename().string();
                std::transform(name.begin(), name.end(), name.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                return (name.find("shader") != std::string::npos || name.find("compute") != std::string::npos);

            }
            return false;
        };
	    auto matFind = [](const fs::path&item)
	    {
	    	std::string name = item.extension().string();
	    	std::transform(name.begin(), name.end(), name.begin(),
					 [](unsigned char c){ return std::tolower(c); });
	    	return (name.find(".smaterial") != std::string::npos);

	    };
	    auto soundFind = [](const fs::path&item)
	    {
		    std::string name = item.extension().string();
		    std::transform(name.begin(), name.end(), name.begin(),
		                   [](unsigned char c){ return std::tolower(c); });
		    return (name.find(".saudio") != std::string::npos
		    || name.find(".wav") != std::string::npos
		    || name.find(".ogg") != std::string::npos);

	    };
        const std::size_t numOffiles = std::count_if(ToLoad.begin(), ToLoad.end(), (fp)fs::is_regular_file);
        const std::size_t numOfShader = std::count_if(ToLoad.begin(), ToLoad.end(), (fp)shaderFind);
	    const std::size_t numOfMat = std::count_if(ToLoad.begin(), ToLoad.end(), (fp)matFind);
	    const std::size_t numOfSound = std::count_if(ToLoad.begin(), ToLoad.end(), (fp)matFind);
        ResourcePtrWrapper* RessourceArray = new ResourcePtrWrapper[numOffiles + numOfShader - numOfMat - numOfSound]();


        auto Lambda = [this](const fs::path *Rpath, ResourcePtrWrapper *wrapper){LoadRessourceNoAdd(*Rpath,*wrapper); delete Rpath;};
        int i =0;
        std::vector<ShaderLoaderWrapper> Shaders;
        std::vector<IDWrapper> IDS;
        IDS.reserve(10);
        Shaders.reserve(5);
	    std::vector<fs::path> matPaths;
	    std::vector<fs::path> soundPaths;
        for (auto& item : ToLoad)
        {
            std::string name = item.filename().string();
            fs::path* newP = new fs::path(item);
            if(fs::is_directory(item))
            {
                std::transform(name.begin(), name.end(), name.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if(name.find("shader") != std::string::npos || name.find("compute") != std::string::npos)
                {
                    Shaders.push_back({item, i});
                    ++i;
                }
                continue; //recursive func
            }
            else
            {
                std::string extension = item.extension().string();
                std::transform(extension.begin(), extension.end(), extension.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if(extension == ".scompute" || extension == ".svertfrag")
                {
                    Shaders.push_back({item, i});
                    ++i;
                    continue;
                }
                else if(matFind(item))
                {
                	matPaths.push_back(item);

                	continue;
                }
                else if(soundFind(item))
                {
	                soundPaths.push_back(item);

	                continue;
                }
                TaskMan.AddTask(Task(Task::MakeID("Load " + name), ETaskType::RESOURCES_LOADER, Lambda, newP, &RessourceArray[i]));
                IDS.push_back( {"Load " + name, i});

                ++i;
            }

        }
        for(auto& s : Shaders) {
            LoadRessourceNoAdd(s.p, RessourceArray[s.i]);
            Manager->AddResource(RessourceArray[s.i].r);
        }
	    for(auto& elt : soundPaths)
	    {
		    std::string name = elt.extension().string();
		    std::transform(name.begin(), name.end(), name.begin(),
		                   [](unsigned char c){ return std::tolower(c); });
		    if(name.find(".saudio") != std::string::npos)
		    {
			    Resource* sound = LoadSolidAudio(elt);
			    Manager->AddResource(sound);
		    }
		    else
		    {
			    Resource* sound = LoadAudio(elt);
			    Manager->AddResource(sound);
		    }

	    }

        bool b = true;
        while(b)
        {
            b = false;
            for (int j = 0; j <IDS.size(); ++j) {
                Task* t= TaskMan.getTaskByID(IDS.at(j).Name);
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
                    if(RessourceArray[k].r!= nullptr) {
                        Manager->AddResource(RessourceArray[k].r);
                    }
	                else if(RessourceArray[k].isFBX) {
	                	Manager->AddResource(RessourceArray[k].fbx.mesh);
	                    Manager->AddResource(RessourceArray[k].fbx.Skeleton);
	                    for(AnimResource* elt : RessourceArray[k].fbx.anims)
	                    {
		                    Manager->AddResource(elt);
	                    }

	                }
                }
            }
        }
		Manager->InitDefaultMat();
	    for(auto& elt : matPaths)
	    {
	        Resource* mat = LoadSolidMaterial(elt);
	        Manager->AddResource(mat);
	    }


    }
        /// MonoThread Loading
    else
    {

        for (auto& item : fs::directory_iterator(Rpath))
        {
            if(fs::is_directory(item))
            {
                std::string name = Rpath.filename().string();
                std::transform(name.begin(), name.end(), name.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if(name.find("shader") != std::string::npos || name.find("compute") != std::string::npos)
                {

                    LoadRessource(item);
                }
                else
                {

                    continue; //recursive func
                }
            }
            else
            {

            	std::string extension = item.path().extension().string();
	            std::transform(extension.begin(), extension.end(), extension.begin(),
	                           [](unsigned char c){ return std::tolower(c); });

	            if(extension == ".smaterial")
	            {
	            	Resource* mat = LoadSolidMaterial(item);
		            Manager->AddResource(mat);
	            }
	            else if(extension == ".wav")
	            {
	            	Resource *r = LoadAudio(Rpath);
		            Manager->AddResource(r);
	            }
	            else if(extension == ".ogg")
	            {
	            	Resource *r = LoadAudio(Rpath);
		            Manager->AddResource(r);
	            }
	            else if(extension == ".saudio")
	            {
		            Resource *r = LoadSolidAudio(Rpath);
		            Manager->AddResource(r);
	            }
	            else
                    LoadRessource(item);

            }
        }

    }
}

///
/// Base Asset Loader
///

void ResourcesLoader::LoadFBX(const fs::path &Rpath, FBXWrapper* fbx)
{
	MeshResource *Mesh = new MeshResource;
	bool hasFoundSkeleton = false;
	SkeletonResource* Skeleton = new SkeletonResource;
	Skeleton->name ="SKELETON_"+Rpath.filename().string();
	Assimp::Importer importer;
	std::string str = Rpath.string();
	Mesh->name = Rpath.filename().string();

	Mesh->path.push_front(rootFolder) ;
	SetPath(Mesh->path, Rpath);
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_ANIMATIONS, true);
	importer.SetPropertyBool(AI_CONFIG_IMPORT_REMOVE_EMPTY_BONES, false);
	const char* name = Mesh->name.data();

	const aiScene *scene = importer.ReadFile(str, aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes |
	                                              aiProcess_SplitLargeMeshes | aiProcess_Triangulate |
	                                              aiProcess_SortByPType | aiProcess_LimitBoneWeights   );



	aiBone* b;


	if (!scene)
	{
		printf("Unable to load FBX: %s\n", importer.GetErrorString());
		delete Mesh;
		return;
	}
	if(!hasFoundSkeleton)
	{
		aiNode* boneNode = nullptr;
		if(scene->HasAnimations())
		{
			aiString randomBoneName =scene->mAnimations[0]->mChannels[0]->mNodeName;
			boneNode = scene->mRootNode->FindNode(randomBoneName);
			aiNode* root = scene->mRootNode;
			while (boneNode->mParent != root)
			{
				boneNode = boneNode->mParent;
			}

		}
		if(boneNode != nullptr)
		{
			hasFoundSkeleton=true;

			auto skelRoot = boneNode;


			std::function<void(SkeletonResource::Bone*, aiNode*)> lambda = [&](SkeletonResource::Bone* _bone, aiNode* _aiNode){

				_bone->name = _aiNode->mName.C_Str();

				_bone->Childrens.reserve(_aiNode->mNumChildren);
				for (int j = 0; j < _aiNode->mNumChildren; ++j)
				{
					SkeletonResource::Bone* cBone = new SkeletonResource::Bone;
					_bone->Childrens.push_back(cBone);
					_bone->transfo =
							Mat4<float>(_aiNode->mTransformation.a1, _aiNode->mTransformation.a2, _aiNode->mTransformation.a3, _aiNode->mTransformation.a4,
							            _aiNode->mTransformation.b1, _aiNode->mTransformation.b2, _aiNode->mTransformation.b3, _aiNode->mTransformation.b4,
							            _aiNode->mTransformation.c1, _aiNode->mTransformation.c2, _aiNode->mTransformation.c3, _aiNode->mTransformation.c4,
							            _aiNode->mTransformation.d1, _aiNode->mTransformation.d2, _aiNode->mTransformation.d3, _aiNode->mTransformation.d4);
					cBone->Parent = _bone;
					lambda(cBone, _aiNode->mChildren[j]);

				}
			};
			std::function<bool(SkeletonResource::Bone*, aiBone*)> setWeights = [&](SkeletonResource::Bone* _bone, aiBone* _aiBone)-> bool{

				if(_bone->name == _aiBone->mName.C_Str())
				{
					_bone->Weights.reserve(_aiBone->mNumWeights);
					Mat4<float> offset =
							Mat4<float>(_aiBone->mOffsetMatrix.a1, _aiBone->mOffsetMatrix.a2, _aiBone->mOffsetMatrix.a3, _aiBone->mOffsetMatrix.a4,
							            _aiBone->mOffsetMatrix.b1, _aiBone->mOffsetMatrix.b2, _aiBone->mOffsetMatrix.b3, _aiBone->mOffsetMatrix.b4,
							            _aiBone->mOffsetMatrix.c1, _aiBone->mOffsetMatrix.c2, _aiBone->mOffsetMatrix.c3, _aiBone->mOffsetMatrix.c4,
							            _aiBone->mOffsetMatrix.d1, _aiBone->mOffsetMatrix.d2, _aiBone->mOffsetMatrix.d3, _aiBone->mOffsetMatrix.d4);
					_bone->offset = offset;
					_bone->WeightInit = true;
					_bone->FinalTrans = offset * _bone->transfo;
					///WARN : trasfo matrix of aiBone and ai node *=-1 ?
					for (int j = 0; j < _aiBone->mNumWeights; ++j)
					{
						_bone->Weights.push_back(_aiBone->mWeights[j].mWeight);
					}
					return true;
				}

				for (auto & Children : _bone->Childrens)
				{

					if(setWeights(Children, _aiBone))
					{
						return true;
					}
				}
				return false;
			};
			lambda(&Skeleton->rootBone, skelRoot);


			if(scene->HasMeshes())
			{
				for (int l = 0; l < scene->mMeshes[0]->mNumBones; ++l)
				{
					aiBone* bone = scene->mMeshes[0]->mBones[l];
					setWeights(&Skeleton->rootBone, bone);
				}
			}
			///load bones
		}
	}


	for (int i = 0; i < scene->mNumAnimations; ++i)
	{
		AnimResource* anim = new AnimResource;
		anim->name = "ANIM_"+Rpath.filename().string();
		anim->numTicks = scene->mAnimations[i]->mDuration;
		anim->ticksPerSeconds = scene->mAnimations[i]->mTicksPerSecond;
		if(hasFoundSkeleton)
			anim->Root = new SkeletonResource::Bone(Skeleton->rootBone);
		fbx->anims.push_back(anim);
		anim->Channels.resize(scene->mAnimations[i]->mNumChannels);
		for(int j = 0; j < anim->Channels.size(); ++j)
		{
			AnimResource::BoneChannel& channel = anim->Channels[j];
		    channel.BoneToMod = anim->Root->FindBoneByName(scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str());
		    std::size_t maxNumFrames =scene->mAnimations[i]->mChannels[j]->mNumPositionKeys ;

		    if(maxNumFrames < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys)
		    	maxNumFrames = scene->mAnimations[i]->mChannels[j]->mNumRotationKeys;

		    if(maxNumFrames < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys)
			    maxNumFrames = scene->mAnimations[i]->mChannels[j]->mNumScalingKeys;

		    channel.Frames.reserve(maxNumFrames);

		    aiVectorKey* vecKeys =scene->mAnimations[i]->mChannels[j]->mPositionKeys;
			int vecKeysNum =scene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
			aiQuatKey* rotKeys =scene->mAnimations[i]->mChannels[j]->mRotationKeys;
			int rotKeysNum =scene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
			aiVectorKey* scaleKeys =scene->mAnimations[i]->mChannels[j]->mScalingKeys;
			int scaleKeysNum =scene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
			int k = 0;
			int l = 0;
			int m = 0;
			for (; k < vecKeysNum && l < rotKeysNum && m < scaleKeysNum;)
			{
				bool timeInit = false;
				bool hasPos = false;
				bool hasRot = false;
				bool hasScale = false;
				double time = 0.0;
				if(k < vecKeysNum)
				{
					if(!timeInit)
					{
						time = vecKeys[k].mTime;
						timeInit = true;
						hasPos = true;
					}
				}
				if(l < rotKeysNum)
				{
					if(!timeInit)
					{
						timeInit= true;
						time = rotKeys[l].mTime;
						hasRot = true;
					}
					else if(rotKeys[l].mTime < time - 0.01)
					{
						time = rotKeys[l].mTime;
						hasRot = true;
						hasPos = false;
					}
					else if(rotKeys[l].mTime >= time - 0.01 && rotKeys[l].mTime <= time + 0.01)
					{
						hasRot = true;
					}
				}
				if(m < scaleKeysNum)
				{
					if(!timeInit)
					{
						timeInit= true;
						time = scaleKeys[m].mTime;
						hasScale = true;
					}
					else if(scaleKeys[m].mTime < time - 0.01)
					{
						time = scaleKeys[m].mTime;
						hasScale = true;
						hasPos = false;
						hasRot = false;
					}
					else if(scaleKeys[m].mTime >= time - 0.01 && scaleKeys[m].mTime <= time + 0.01)
					{
						hasScale = true;
					}
				}
				auto frame =AnimResource::KeyFrame();
				if(channel.Frames.size() != 0)
				{
					frame = AnimResource::KeyFrame(channel.Frames.back());
					frame.time = time;
				}

				if(hasPos)
				{
					frame.usePos = true;
					frame.pos = Vec3(vecKeys[k].mValue.x,vecKeys[k].mValue.y,vecKeys[k].mValue.z);
					k++;
				}
				if (hasRot)
				{
					frame.useRot = true;
					frame.Rot = Quat(rotKeys[k].mValue.x,rotKeys[k].mValue.y,rotKeys[k].mValue.z,rotKeys[k].mValue.w);
					l++;
				}
				if (hasScale)
				{
					frame.useScale = true;
					frame.Scale = Vec3(scaleKeys[k].mValue.x,scaleKeys[k].mValue.y,scaleKeys[k].mValue.z);
					m++;
				}

				channel.Frames.push_back(frame);
			}
		}

		float f = 0;
		float g = f;

	}
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{

		int numIndices = 3 * scene->mMeshes[i]->mNumFaces;
		MeshResource::SubMesh &Sub = Mesh->Meshes.emplace_back();
		Sub.indices.resize(numIndices);
		Sub.vertices.resize(scene->mMeshes[i]->mNumVertices);

		std::uint32_t WritePos = 0;
		for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
		{
			std::uint32_t size = scene->mMeshes[i]->mFaces[j].mNumIndices * sizeof(unsigned int);
			std::memcpy(&(Sub.indices[WritePos]), scene->mMeshes[i]->mFaces[j].mIndices, size);
			WritePos += scene->mMeshes[i]->mFaces[j].mNumIndices;
		}


		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; ++j)
		{
			if (scene->mMeshes[i]->HasPositions())
				std::memcpy(&(Sub.vertices[j].Pos), &(scene->mMeshes[i]->mVertices[j]), 3 * sizeof(float));
			if (scene->mMeshes[i]->HasNormals())
				std::memcpy(&(Sub.vertices[j].Normal), &(scene->mMeshes[i]->mNormals[j]), 3 * sizeof(float));
			if (scene->mMeshes[i]->mTextureCoords[0] != nullptr)
				std::memcpy(&(Sub.vertices[j].TexCoords), &(scene->mMeshes[i]->mTextureCoords[0][j].x),
				            2 * sizeof(float));
		}
		fbx->mesh = Mesh;
		fbx->Skeleton = Skeleton;


	}
}

Resource * ResourcesLoader::LoadImage(const fs::path &Rpath)
{
    ImageResource* Image = new ImageResource();
    Image->name = Rpath.filename().string();
	Image->path.push_front(rootFolder);
	SetPath(Image->path, Rpath);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* img =stbi_load(Rpath.string().c_str(), &Image->x, &Image->y, &Image->ChannelsNum, STBI_rgb_alpha);
    //test convert to const char*
    //or vector<char>
    Image->image.resize(Image->x * Image->y * 4/*Image->ChannelsNum*/);
    std::memcpy(Image->image.data(), img, Image->image.size());

#if SASSET_GEN
    //printf("Generate .SImage\n");
    std::vector<char> Data;
    Image->ToDataBuffer(Data);
    fs::path cachePath = Rpath.parent_path();
    cachePath.append(Rpath.filename().string() + ".SImage");
    std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);
    if(cacheFile.is_open())
    {
        cacheFile.write(Data.data(), Data.size());
    }
#endif
    stbi_image_free(img);
    return Image;
}





Resource * ResourcesLoader::LoadMesh(const fs::path &Rpath)
{
    MeshResource* Mesh = new MeshResource;
    Assimp::Importer importer;
    std::string str = Rpath.string();


    Mesh->name = Rpath.filename().string();
	Mesh->path.push_front(rootFolder) ;
	SetPath(Mesh->path, Rpath);
    const aiScene* scene = importer.ReadFile(str,  aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes| aiProcess_SplitLargeMeshes| aiProcess_Triangulate  | aiProcess_SortByPType | aiProcess_PreTransformVertices );
    if(!scene) {
        printf("Unable to load mesh: %s\n", importer.GetErrorString());
        delete Mesh;
        return nullptr;
    }

    for(int i = 0; i < scene->mNumMeshes; ++i) {
        int numIndices = 3 * scene->mMeshes[i]->mNumFaces;
        MeshResource::SubMesh& Sub = Mesh->Meshes.emplace_back();
        Sub.indices.resize(numIndices);
        Sub.vertices.resize(scene->mMeshes[i]->mNumVertices);

        std::uint32_t WritePos = 0;
        for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j) {
            std::uint32_t size = scene->mMeshes[i]->mFaces[j].mNumIndices * sizeof(unsigned int);
            std::memcpy(&(Sub.indices[WritePos]), scene->mMeshes[i]->mFaces[j].mIndices, size);
            WritePos+= scene->mMeshes[i]->mFaces[j].mNumIndices;
        }

        
        for (int j = 0; j < scene->mMeshes[i]->mNumVertices; ++j) {
            if(scene->mMeshes[i]->HasPositions())
                std::memcpy(&(Sub.vertices[j].Pos), &(scene->mMeshes[i]->mVertices[j]), 3 * sizeof(float));
            if(scene->mMeshes[i]->HasNormals())
                std::memcpy(&(Sub.vertices[j].Normal), &(scene->mMeshes[i]->mNormals[j]), 3 * sizeof(float));
            if(scene->mMeshes[i]->mTextureCoords[0] != nullptr)
                std::memcpy(&(Sub.vertices[j].TexCoords), &(scene->mMeshes[i]->mTextureCoords[0][j].x), 2 * sizeof(float));
        } /// WARN: TEX COORD MAY HAVE MORE DATA LIKE mTextureCoords[1][j] ETC...
    }
#if SASSET_GEN
    printf("generate .Smesh\n");
    std::vector<char> Data;
    Mesh->ToDataBuffer(Data);




    fs::path cachePath = Rpath.parent_path();
    cachePath.append(Rpath.filename().string() + ".SMesh");
    std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


    if(cacheFile.is_open())
    {
        std::cout << "" << "\n";
        cacheFile.write(Data.data(), Data.size());
    }
#endif

    return Mesh;



}




Resource * ResourcesLoader::LoadShader(const fs::path &Rpath)
{
    Renderer* r=Renderer::GetInstance();
    std::string FolderName = Rpath.filename().string();
    std::string FolderNameLower = FolderName;
    std::transform(FolderNameLower.begin(), FolderNameLower.end(), FolderNameLower.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if(FolderNameLower.find("compute") != std::string::npos)
    {

        std::vector<char*> ComputeSources(1);
        bool found = false;
        for(auto& file : fs::directory_iterator(Rpath))
        {

            FolderNameLower = file.path().filename().string();
            std::transform(FolderNameLower.begin(), FolderNameLower.end(), FolderNameLower.begin(),
                           [](unsigned char c){ return std::tolower(c); });
            if(FolderNameLower == "main.compute")
            {
                std::ifstream ifs(file.path(), std::ios::binary|std::ios::ate);
                std::ifstream::pos_type pos = ifs.tellg();

                char* buffer = new char[pos.operator long long() + 1];
                buffer[pos] = '\0';

                ifs.seekg(0, std::ios::beg);
                ifs.read(buffer, pos);
                //std::cout << buffer << "\n";
                ComputeSources[0] = buffer;
                found = true;
                break;
            }


        }
        if(!found)
            return nullptr;

        ComputeShaderResource* Compute = new ComputeShaderResource(ComputeSources[0]);
        Compute->name = Rpath.filename().string();
	    Compute->path.push_front(rootFolder) ;
	    SetPath(Compute->path, Rpath);


#if SASSET_GEN
        printf("generate .SCompute\n");
        std::vector<char> Data;
        Compute->ToDataBuffer(Data);


        fs::path cachePath = Rpath.parent_path();
        cachePath.append(Rpath.filename().string() + ".SCompute");
        std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


        if(cacheFile.is_open())
        {
            std::cout << "" << "\n";
            cacheFile.write(Data.data(), Data.size());
        }
#endif
        delete[] ComputeSources[0];
        return Compute;
		

    }
    else
    {
        std::vector<char*> VertexSources(1);
        std::vector<char*> fragSources(1);
        bool vert  = false; bool frag = false;
        for(auto& file : fs::directory_iterator(Rpath))
        {
            FolderNameLower = file.path().filename().string();
            std::transform(FolderNameLower.begin(), FolderNameLower.end(), FolderNameLower.begin(),
                           [](unsigned char c){ return std::tolower(c); });
            if(FolderNameLower == "main.vert")
            {
                std::ifstream ifs(file.path(), std::ios::binary|std::ios::ate);
                std::ifstream::pos_type pos = ifs.tellg();

                char* buffer = new char[pos.operator long long() + 1];
                buffer[pos] = '\0';

                ifs.seekg(0, std::ios::beg);
                ifs.read(buffer, pos);
                //std::cout << buffer << "\n";
                VertexSources[0] = buffer;
                vert = true;
                if(frag)
                    break;
                continue;
            }
            if(FolderNameLower == "main.frag")
            {
                std::ifstream ifs(file.path(), std::ios::binary|std::ios::ate);
                std::ifstream::pos_type pos = ifs.tellg();

                char* buffer = new char[pos.operator long long() + 1];
                buffer[pos] = '\0';

                ifs.seekg(0, std::ios::beg);
                ifs.read(buffer, pos);
                //std::cout << buffer << "\n";
                fragSources[0] = buffer;
                frag = true;
                if(vert)
                    break;
                continue;
            }
        }
        if(!vert || !frag)
            return nullptr;
        //TODO Cleanup at return

        ShaderResource* Shader = new ShaderResource(VertexSources[0], fragSources[0]);
        Shader->name = Rpath.filename().string();
	    Shader->path.push_front(rootFolder) ;
	    SetPath(Shader->path, Rpath);

#if SASSET_GEN
        printf("generate .SVertFrag\n");
        std::vector<char> Data;
        Shader->ToDataBuffer(Data);




        fs::path cachePath =Rpath.parent_path();
        cachePath.append(Rpath.filename().string() + ".SVertFrag");
        std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


        if(cacheFile.is_open())
        {
            std::cout << "" << "\n";
            cacheFile.write(Data.data(), Data.size());
        }
#endif
        delete[] VertexSources[0];
        delete[] fragSources[0];
        return Shader;

    }
}

Resource *ResourcesLoader::LoadAudio(const fs::path &Rpath)
{
	AudioResource* audio = new AudioResource();
	std::string p = Rpath.string();
	const char* filename = p.c_str();
	ALenum err, format;
	ALuint buff;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf = nullptr;

	ALsizei numBytes;
	audio->name = Rpath.filename().string();
	audio->path.push_front(rootFolder);
	SetPath(audio->path, Rpath);

	sndfile = sf_open(filename, SFM_READ, &sfinfo);
	if (!sndfile)
	{
		std::cout << "Couldn't open the audio " << filename << std::endl;
		return 0;
	}

	format = AL_NONE;
	if (sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	else if (sfinfo.channels == 3)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (sfinfo.channels == 4)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	}

	if(!format)
	{
		std::cout << "Unsupported channel count of " << filename << std::endl;
		sf_close(sndfile);
		return 0;
	}
	audio->audioRawBinary.resize((sfinfo.frames * sfinfo.channels));

	audio->format = format;
	audio->numFrames = sf_readf_short(sndfile, audio->audioRawBinary.data(), sfinfo.frames);
	if (audio->numFrames < 1)
	{
		sf_close(sndfile);
		std::cout << "Failed to read samples of " << filename << std::endl;
		return nullptr;
	}

	numBytes = (ALsizei)(audio->numFrames * sfinfo.channels) * (ALsizei)sizeof(short);

	buff = 0;
	alGenBuffers(1, &buff);
	alBufferData(buff, format, audio->audioRawBinary.data(), numBytes, sfinfo.samplerate);
	audio->buffer = buff;
	audio->info = sfinfo;
	sf_close(sndfile);

	err = alGetError();
	if (err != AL_NO_ERROR)
	{
	    if(alGetString(err))
		    std::cout << "OpenAL Error : " << alGetString(err) << std::endl;
		if (buff && alIsBuffer(buff))
			alDeleteBuffers(1, &buff);
		return nullptr;
	}


#if SASSET_GEN
	printf("generate .SAudio\n");
	std::vector<char> Data;
	audio->ToDataBuffer(Data);




	fs::path cachePath =Rpath.parent_path();
	cachePath.append(Rpath.filename().string() + ".SAudio");
	std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


	if(cacheFile.is_open())
	{
		cacheFile.write(Data.data(), Data.size());
	}
#endif
	audio->audioRawBinary.clear();
	audio->audioRawBinary.reserve(0);

	return audio;
}

///
/// RessourceLoader Memory Func
///


void ResourcesLoader::Append(std::vector<char> &DataBuffer, void *Data, std::uint64_t sizeInByte)
{
    if (Data && (sizeInByte > 0))
    {
        std::size_t start = DataBuffer.size();
        DataBuffer.resize(start + sizeInByte);
        std::memcpy(&DataBuffer[start], Data, sizeInByte);
    }
}


void ResourcesLoader::ReadFromBuffer(char* DataBuffer, void *Data, std::uint64_t sizeInByte, std::uint64_t& ReadPos)
{
    if (Data && (sizeInByte > 0) )
    {
        std::memcpy(Data, &DataBuffer[ReadPos], sizeInByte);
        ReadPos+= sizeInByte;
    }
}




///
/// Solid Assets Loader
///

Resource * ResourcesLoader::LoadSolidImage(const fs::path &Rpath)
{
    ImageResource* Image = new ImageResource();

    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);
    Image->FromDataBuffer(buffer.data(), buffer.size());
	Image->path.clear();
	Image->path.push_front(rootFolder);
	SetPath(Image->path, Rpath);
    if(Image->name == "")
    {
        Image->name = "NoName" + std::to_string(Resource::NoNameNum);
        Resource::NoNameNum++;
    }
    return Image;
}


Resource * ResourcesLoader::LoadSolidMesh(const fs::path &Rpath)
{
    MeshResource* Mesh = new MeshResource();

    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);

    Mesh->FromDataBuffer(buffer.data(), buffer.size());
	Mesh->path.clear();
	Mesh->path.push_front(rootFolder);
	SetPath(Mesh->path, Rpath);
    if(Mesh->name == "")
    {
        Mesh->name = "NoName" + std::to_string(Resource::NoNameNum);
        Resource::NoNameNum++;
    }

    return Mesh;

}

Resource * ResourcesLoader::LoadSolidComputeShader(const fs::path &Rpath)
{
    ComputeShaderResource* cs = new ComputeShaderResource();
    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);

    cs->FromDataBuffer(buffer.data(), buffer.size());
	cs->path.clear();
	cs->path.push_front(rootFolder);
	SetPath(cs->path, Rpath);
    if(cs->name == "")
    {
        cs->name = "NoName" + std::to_string(Resource::NoNameNum);
        Resource::NoNameNum++;
    }
    return cs;


}
Resource * ResourcesLoader::LoadSolidShader(const fs::path &Rpath)
{
    ShaderResource* s = new ShaderResource();
    std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&buffer[0], pos);

    s->FromDataBuffer(buffer.data(), buffer.size());
	s->path.clear();
	s->path.push_front(rootFolder);
	SetPath(s->path, Rpath);
    if(s->name == "")
    {
        s->name = "NoName" + std::to_string(Resource::NoNameNum);
        Resource::NoNameNum++;
    }
    return s;

}

void ResourcesLoader::SaveMaterialToFile(MaterialResource *_mat)
{
	//rintf("generate .SMaterial\n");
	std::vector<char> Data;
	_mat->ToDataBuffer(Data);




	fs::path cachePath =SolidPath.parent_path() ;
	for(std::string& elt : _mat->path)
	{
		if(elt == rootFolder)
		{
			cachePath.append("Assets");
			continue;
		}

		cachePath.append(elt);
	}
	cachePath.append(_mat->name + ".SMaterial");
	std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


	if(cacheFile.is_open())
	{
		std::cout << "" << "\n";
		cacheFile.write(Data.data(), Data.size());
		cacheFile.close();
	}
}

Resource *ResourcesLoader::LoadSolidMaterial(const fs::path &Rpath)
{
	MaterialResource* mat = new MaterialResource();
	std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char>  buffer(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&buffer[0], pos);

	mat->FromDataBuffer(buffer.data(), buffer.size());

	mat->path.clear();
	mat->path.push_front(rootFolder);
	SetPath(mat->path, Rpath);
	if(mat->name == "")
	{
		mat->name = "NoName" + std::to_string(Resource::NoNameNum);
		Resource::NoNameNum++;
	}
	return mat;

}

Resource *ResourcesLoader::LoadSolidAudio(const fs::path &Rpath)
{
	AudioResource* audio = new AudioResource();
	std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char>  buffer(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&buffer[0], pos);


	audio->FromDataBuffer(buffer.data(),buffer.size());


	ALsizei numBytes = (ALsizei)(audio->numFrames * audio->info.channels) * (ALsizei)sizeof(short);


	alGenBuffers(1, &audio->buffer);
	alBufferData(audio->buffer, audio->format, audio->audioRawBinary.data(), numBytes, audio->info.samplerate);
	if(audio->name == "")
	{
		audio->name = "NoName" + std::to_string(Resource::NoNameNum);
		Resource::NoNameNum++;
	}
	audio->path.clear();
	audio->path.push_front(rootFolder);
	SetPath(audio->path, Rpath);
	audio->audioRawBinary.clear();
	audio->audioRawBinary.reserve(0);
	return audio;
}

void ResourcesLoader::SetPath(std::deque<std::string> &resPath, const fs::path& Rpath)
{
	fs::path copy =Rpath;
	copy =copy.parent_path();
	std::deque<std::string> invPaths;

	std::function<bool(fs::path&)> pathFind = [&](fs::path& p)->bool
	{
		bool toRet = false;
		if(p.filename().string() != "Assets")
		{
			invPaths.push_front(p.filename().string());

			p =p.parent_path();
			if(!p.filename().empty())
				return pathFind(p);
			else
			{
				return false;
			}
		}
		else
			return true;
	};
	if(!pathFind(copy))
		return;
	resPath.insert(resPath.end(), invPaths.begin(),invPaths.end());

}

Resource *ResourcesLoader::LoadSolidPrefab(const fs::path &Rpath)
{
	PrefabResource* prefab = new PrefabResource();

	std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char>  buffer(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&buffer[0], pos);

	prefab->FromDataBuffer(buffer.data(),buffer.size());
	prefab->path.clear();
	prefab->path.push_front(rootFolder);
	SetPath(prefab->path, Rpath);
	return prefab;
}

void ResourcesLoader::ReLoadRessource(Resource *_resource)
{
	std::vector<char>b;
	_resource->ToDataBuffer(b);
	fs::path Rpath = SolidPath;
	for (int i = 1; i < _resource->path.size(); ++i)
	{
		Rpath.append(_resource->path[i]);
	}

	switch (_resource->GetType())
	{
		case EResourceType::Mesh:
		{
			Rpath.append(_resource->name+".SMesh");
			break;
		}
		case EResourceType::Shader:
		{
			Rpath.append(_resource->name+".SVertFrag");
			break;
		}
		case EResourceType::Material:
		{
			SaveMaterialToFile((MaterialResource*)_resource);
			return;
		}
		case EResourceType::Compute:
		{
			Rpath.append(_resource->name+".SCompute");
			break;
		}
		case EResourceType::Image:
		{
			Rpath.append(_resource->name+".SImage");
			break;
		}
		case EResourceType::Anim:
		{
			Rpath.append(_resource->name+".SAnim");
			break;
		}
		case EResourceType::Audio:
		{
			Rpath.append(_resource->name);
			Resource* r = LoadAudio(Rpath);
			delete r;
			return;
		}
		case EResourceType::Skeleton:
		{
			Rpath.append(_resource->name);
			break;
		}
		case EResourceType::Scene:
		{
			Rpath.append(_resource->name+ ".SolidScene");
			break;
		}
		case EResourceType::Prefab:
		{
			SavePrefabToFile((PrefabResource *) _resource);
			return;
		}
		default:
			ThrowError("Type Not Stored", ESolidErrorCode::S_INIT_ERROR);
			break;
	}

	std::ofstream cacheFile(Rpath, std::fstream::binary | std::fstream::trunc);
	if(cacheFile.is_open())
	{
		cacheFile.write(b.data(),b.size());
		cacheFile.close();
	}


}

void ResourcesLoader::SavePrefabToFile(PrefabResource *_prefab)
{
	fs::path p = ResourcesLoader::SolidPath.parent_path();
	for(std::string& elt : _prefab->path)
	{
		if(elt == "\\Assets\\")
		{
			p.append("Assets");
		}
		else
			p.append(elt);
	}
	p.append(_prefab->name +".SolidPrefab");
	std::vector<char>buf;
	_prefab->ToDataBuffer(buf);
	std::ofstream file(p, std::ifstream::binary | std::ofstream::trunc);
	file.write(buf.data(), buf.size());
	file.close();
}

Resource *ResourcesLoader::LoadSolidScene(const fs::path &Rpath)
{
	SceneResource* scene = new SceneResource();

	std::ifstream ifs(Rpath, std::ios::binary|std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char>  buffer(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&buffer[0], pos);

	scene->FromDataBuffer(buffer.data(),buffer.size());
	scene->path.clear();
	scene->path.push_front(rootFolder);
	SetPath(scene->path, Rpath);
	return scene;
}


