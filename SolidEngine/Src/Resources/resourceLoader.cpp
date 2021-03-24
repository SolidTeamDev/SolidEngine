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
    else if(extension == ".sanim")
        ;
    else if(extension == ".saudio")
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
        r=LoadMesh(Rpath);

    ///Audio Loading
    else if(extension == ".wav")
	    r=LoadAudio(Rpath);
    else if(extension == ".ogg")
	    r=LoadAudio(Rpath);
    wrapper.r = r;
}
void ResourcesLoader::LoadRessource(const fs::path &Rpath)
{
    ResourcePtrWrapper RWrapper {nullptr};
    LoadRessourceNoAdd(Rpath, RWrapper);
    if(RWrapper.r != nullptr)
        Manager->AddResource(RWrapper.r);
}



void ResourcesLoader::LoadResourcesFromFolder(const fs::path &Rpath)
{
    if(!fs::exists(Rpath))
        return;
    ///MultiThread Loading
    if(Manager->GetEngine()->MultiThreadEnabled())
    {
        TaskManager& TaskMan = Manager->GetEngine()->taskManager;
        using fp = bool (*)( const fs::path&);
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
        const std::size_t numOffiles = std::count_if(fs::directory_iterator(Rpath), fs::directory_iterator{}, (fp)fs::is_regular_file);
        const std::size_t numOfShader = std::count_if(fs::directory_iterator(Rpath), fs::directory_iterator{}, (fp)shaderFind);
	    const std::size_t numOfMat = std::count_if(fs::directory_iterator(Rpath), fs::directory_iterator{}, (fp)matFind);
        ResourcePtrWrapper* RessourceArray = new ResourcePtrWrapper[numOffiles + numOfShader - numOfMat] {nullptr};

        auto Lambda = [this](const fs::path *Rpath, ResourcePtrWrapper *wrapper){LoadRessourceNoAdd(*Rpath,*wrapper); delete Rpath;};
        int i =0;
        std::vector<ShaderLoaderWrapper> Shaders;
        std::vector<IDWrapper> IDS;
        IDS.reserve(10);
        Shaders.reserve(5);
        std::vector<fs::path> matPaths;
        for (auto& item : fs::directory_iterator(Rpath))
        {
            std::string name = item.path().filename().string();
            fs::path* newP = new fs::path(item.path());
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
                std::string extension = item.path().extension().string();
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
                TaskMan.AddTask(Task(Task::MakeID("Load " + name), ETaskType::RESOURCES_LOADER, Lambda, newP, &RessourceArray[i]));
                IDS.push_back( {"Load " + name, i});

                ++i;
            }

        }
        for(auto& s : Shaders) {
            LoadRessourceNoAdd(s.p, RessourceArray[s.i]);
            Manager->AddResource(RessourceArray[s.i].r);
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
	            else
                    LoadRessource(item);

            }
        }

    }
}

///
/// Base Asset Loader
///

Resource * ResourcesLoader::LoadImage(const fs::path &Rpath)
{
    ImageResource* Image = new ImageResource();
    Image->name = Rpath.filename().string();
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
    fs::path cachePath = SolidPath;
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




    fs::path cachePath = SolidPath;
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

#if SASSET_GEN
        printf("generate .SCompute\n");
        std::vector<char> Data;
        Compute->ToDataBuffer(Data);


        fs::path cachePath = SolidPath;
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

#if SASSET_GEN
        printf("generate .SVertFrag\n");
        std::vector<char> Data;
        Shader->ToDataBuffer(Data);




        fs::path cachePath =SolidPath;
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
	sf_count_t numFrames;
	ALsizei numBytes;
	audio->name = Rpath.filename().string();
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


	numFrames = sf_readf_short(sndfile, audio->audioRawBinary.data(), sfinfo.frames);
	if (numFrames < 1)
	{
		sf_close(sndfile);
		std::cout << "Failed to read samples of " << filename << std::endl;
		return nullptr;
	}

	numBytes = (ALsizei)(numFrames * sfinfo.channels) * (ALsizei)sizeof(short);

	buff = 0;
	alGenBuffers(1, &buff);
	alBufferData(buff, format, audio->audioRawBinary.data(), numBytes, sfinfo.samplerate);
	audio->buffer = buff;
	audio->info = sfinfo;
	sf_close(sndfile);

	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		std::cout << "OpenAL Error : " << alGetString(err) << std::endl;
		if (buff && alIsBuffer(buff))
			alDeleteBuffers(1, &buff);
		return nullptr;
	}
	///TODO
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
    if(s->name == "")
    {
        s->name = "NoName" + std::to_string(Resource::NoNameNum);
        Resource::NoNameNum++;
    }
    return s;

}

void ResourcesLoader::SaveMaterialToFile(MaterialResource *_mat)
{
	printf("generate .SMaterial\n");
	std::vector<char> Data;
	_mat->ToDataBuffer(Data);




	fs::path cachePath =SolidPath;
	cachePath.append(_mat->name + ".SMaterial");
	std::ofstream cacheFile(cachePath, std::fstream::binary | std::fstream::trunc);


	if(cacheFile.is_open())
	{
		std::cout << "" << "\n";
		cacheFile.write(Data.data(), Data.size());
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
	if(mat->name == "")
	{
		mat->name = "NoName" + std::to_string(Resource::NoNameNum);
		Resource::NoNameNum++;
	}
	return mat;

}
