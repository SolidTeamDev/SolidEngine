//
// Created by ryan1 on 26/02/2021.
//

#include "Resources/ressources.hpp"
#include <sstream>
#include <Resources/resourceType.hpp>

#include "glad/glad.h"
#include "Core/engine.hpp"

using namespace Solid;

int Resource::NoNameNum = 0;

#define SASSET_GEN 1





///
/// Resources Classes
///
ImageResource::~ImageResource() = default;



MaterialResource::MaterialResource()
{
	type = EResourceType::Material;
	shader = nullptr;
	defaultshader = Engine::GetInstance()->graphicsResourceMgr.GetDefaultShader();
}

MaterialResource::~MaterialResource()
{
	//TODO: save to .SMaterial
	if(shouldGenerateFileAtDestroy)
	{
		ResourcesLoader loader;
		loader.SaveMaterialToFile(this);
	}


}


/*

MaterialResource::~MaterialResource(bool shouldGenerateFile)
{

}*/
///
/// Resources Classes Buffer Reader / Writer
///

// IMAGE
void ImageResource::ToDataBuffer(std::vector<char> &buffer)
{

    std::string pString = this->path.string();
    std::uint32_t size = pString.size();
    ResourcesLoader::Append(buffer, &(this->type), sizeof(this->type));

    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (pString.c_str()),  size * sizeof( std::string::value_type));

    size = this->name.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->name.c_str()), size * sizeof(std::string::value_type));

    ResourcesLoader::Append(buffer, &(this->x), sizeof(this->x));
    ResourcesLoader::Append(buffer, &(this->y), sizeof(this->y));
    ResourcesLoader::Append(buffer, &(this->ChannelsNum), sizeof(this->ChannelsNum));

    size = this->image.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (this->image.data()), size * sizeof(unsigned char));



}

void ImageResource::FromDataBuffer(char* buffer , int bSize)
{
    //WARNING : No test for read overflow
    std::uint64_t ReadPos = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(this->type), sizeof(this->type), ReadPos);

    //recup path string
    std::uint32_t size = 0;
    std::string pString;
    ResourcesLoader::ReadFromBuffer(buffer, &size, sizeof(size), ReadPos);
    pString.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (pString.data()),  size * sizeof( std::string::value_type), ReadPos);
    this->path = pString;

    //recup name
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->name.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->name.data()), size * sizeof(std::string::value_type), ReadPos);

    //recup image metadata
    ResourcesLoader::ReadFromBuffer(buffer, &(this->x), sizeof(this->x), ReadPos);
    ResourcesLoader::ReadFromBuffer(buffer, &(this->y), sizeof(this->y), ReadPos);
    ResourcesLoader::ReadFromBuffer(buffer, &(this->ChannelsNum), sizeof(this->ChannelsNum), ReadPos);

    //recup image
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);


    this->image.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (this->image.data()), size * sizeof(unsigned char), ReadPos);

}

// MESH

void MeshResource::ToDataBuffer(std::vector<char> &buffer)
{
    std::string pString = this->path.string();
    std::uint32_t size = pString.size();
    ResourcesLoader::Append(buffer, &(this->type), sizeof(this->type));

    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (pString.c_str()),  size * sizeof( std::string::value_type));

    size = this->name.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->name.c_str()), size * sizeof(std::string::value_type));

    size = this->Meshes.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));

    for(MeshResource::SubMesh& Sub : this->Meshes)
    {
        size = Sub.indices.size();
        ResourcesLoader::Append(buffer, &(size), sizeof(size));
        ResourcesLoader::Append(buffer, (Sub.indices.data()), size * sizeof(unsigned int));

        size = Sub.vertices.size();
        ResourcesLoader::Append(buffer, &(size), sizeof(size));
        ResourcesLoader::Append(buffer, (Sub.vertices.data()), size * sizeof(Vertex));
    }

}

void MeshResource::FromDataBuffer(char *buffer, int bSize)
{
    //WARNING : No test for read overflow
    std::uint64_t ReadPos = 0;

    ResourcesLoader::ReadFromBuffer(buffer, &(this->type), sizeof(this->type), ReadPos);

    //recup path string
    std::uint32_t size = 0;
    std::string pString;
    ResourcesLoader::ReadFromBuffer(buffer, &size, sizeof(size), ReadPos);
    pString.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (pString.data()),  size * sizeof( std::string::value_type), ReadPos);
    this->path = pString;

    //recup name
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->name.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->name.data()), size * sizeof(std::string::value_type), ReadPos);

    //get SubMeshes Number
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->Meshes.resize(size);
    for(MeshResource::SubMesh& Sub : this->Meshes)
    {
        size = 0;
        ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
        Sub.indices.resize(size);
        ResourcesLoader::ReadFromBuffer(buffer, (Sub.indices.data()), size * sizeof(unsigned int), ReadPos);

        size = 0;
        ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
        Sub.vertices.resize(size);
        ResourcesLoader::ReadFromBuffer(buffer, (Sub.vertices.data()), size * sizeof(Vertex), ReadPos);
    }



}

// Shaders


//Compute

void ComputeShaderResource::ToDataBuffer(std::vector<char> &buffer)
{
    std::string pString = this->path.string();
    std::uint32_t size = pString.size();
    ResourcesLoader::Append(buffer, &(this->type), sizeof(this->type));

    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (pString.c_str()),  size * sizeof( std::string::value_type));

    size = this->name.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->name.c_str()), size * sizeof(std::string::value_type));

    size = this->ComputeSource.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->ComputeSource.c_str()), size * sizeof(std::string::value_type));
    // Write Program Binary
    GL::ComputeShader TempShader = GL::ComputeShader(this);

    binaries =Renderer::GetInstance()->GetShaderBinary(TempShader.ProgID);
    std::uint32_t PSize = binaries.size;
    ResourcesLoader::Append(buffer, &PSize, sizeof  (PSize));
    ResourcesLoader::Append(buffer, &binaries.format, sizeof  (binaries.format));
    ResourcesLoader::Append(buffer, binaries.b, sizeof  (char) * binaries.size);

    delete[] binaries.b;
}
void ComputeShaderResource::FromDataBuffer(char *buffer, int bSize)
{
    //WARNING : No test for read overflow
    std::uint64_t ReadPos = 0;

    ResourcesLoader::ReadFromBuffer(buffer, &(this->type), sizeof(this->type), ReadPos);

    //recup path string
    std::uint32_t size = 0;
    std::string pString;
    ResourcesLoader::ReadFromBuffer(buffer, &size, sizeof(size), ReadPos);
    pString.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (pString.data()),  size * sizeof( std::string::value_type), ReadPos);
    this->path = pString;

    //recup name
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->name.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->name.data()), size * sizeof(std::string::value_type), ReadPos);

    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->ComputeSource.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->ComputeSource.data()), size * sizeof(std::string::value_type), ReadPos);

    //Shader Binary
    size = 0;
    GLenum bFormat;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    ResourcesLoader::ReadFromBuffer(buffer, &(bFormat), sizeof(bFormat), ReadPos);
    char* binary = new char[size];
    ResourcesLoader::ReadFromBuffer(buffer, binary, sizeof(char) * size, ReadPos);
    binaries = {.size =size ,.format=bFormat,.b=binary};

    delete[] binary;

}

//Vertex / Frag

void ShaderResource::ToDataBuffer(std::vector<char> &buffer)
{
    std::string pString = this->path.string();
    std::uint32_t size = pString.size();
    ResourcesLoader::Append(buffer, &(this->type), sizeof(this->type));

    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (pString.c_str()),  size * sizeof( std::string::value_type));

    size = this->name.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->name.c_str()), size * sizeof(std::string::value_type));

    size = this->VertexSource.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->VertexSource.c_str()), size * sizeof(std::string::value_type));
    size = this->FragSource.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (void *) (this->FragSource.c_str()), size * sizeof(std::string::value_type));

    // Write Program Binary
	GL::Shader TempShader = GL::Shader(this);
    binaries =Renderer::GetInstance()->GetShaderBinary(TempShader.ProgID);
    std::uint32_t PSize = binaries.size;
    ResourcesLoader::Append(buffer, &PSize, sizeof  (PSize));
    ResourcesLoader::Append(buffer, &binaries.format, sizeof  (binaries.format));
    ResourcesLoader::Append(buffer, binaries.b, sizeof  (char) * binaries.size);
    delete[] binaries.b;
}
void ShaderResource::FromDataBuffer(char *buffer, int bSize)
{
    //WARNING : No test for read overflow
    std::uint64_t ReadPos = 0;

    ResourcesLoader::ReadFromBuffer(buffer, &(this->type), sizeof(this->type), ReadPos);

    //recup path string
    std::uint32_t size = 0;
    std::string pString;
    ResourcesLoader::ReadFromBuffer(buffer, &size, sizeof(size), ReadPos);
    pString.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (pString.data()),  size * sizeof( std::string::value_type), ReadPos);
    this->path = pString;

    //recup name
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->name.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->name.data()), size * sizeof(std::string::value_type), ReadPos);

    //shaders sources
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->VertexSource.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->VertexSource.data()), size * sizeof(std::string::value_type), ReadPos);

    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    this->FragSource.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->FragSource.data()), size * sizeof(std::string::value_type), ReadPos);

    //Shader Binary
    size = 0;
    GLenum bFormat;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
    ResourcesLoader::ReadFromBuffer(buffer, &(bFormat), sizeof(bFormat), ReadPos);
    char* binary = new char[size];
    ResourcesLoader::ReadFromBuffer(buffer, binary, sizeof(char) * size, ReadPos);
    binaries ={.size =size ,.format=bFormat,.b=binary};
    delete[] binary;
}

void MaterialResource::ToDataBuffer(std::vector<char> &buffer)
{
	std::string pString = this->path.string();
	std::uint32_t size = pString.size();
	ResourcesLoader::Append(buffer, &(this->type), sizeof(this->type));

	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	ResourcesLoader::Append(buffer, (void *) (pString.c_str()),  size * sizeof( std::string::value_type));

	size = this->name.size();
	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	ResourcesLoader::Append(buffer, (void *) (this->name.c_str()), size * sizeof(std::string::value_type));

	///shader saving
	if(this->shader == nullptr)
	{
		int j = 128;
		ResourcesLoader::Append(buffer, &(j), sizeof(j));
	}
	else
	{
		int j = 256;
		ResourcesLoader::Append(buffer, &(j), sizeof(j));
		size = this->shader->name.size();
		ResourcesLoader::Append(buffer, &(size), sizeof(size));
		ResourcesLoader::Append(buffer, (void *) (this->shader->name.c_str()), size * sizeof(std::string::value_type));

	}


	size = this->TexturesProperties.size();
	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	for (auto& tex : this->TexturesProperties)
	{
		//property name
		size = tex.first.size();
		ResourcesLoader::Append(buffer, &(size), sizeof(size));
		ResourcesLoader::Append(buffer, (void *) (tex.first.c_str()), size * sizeof(std::string::value_type));

		//is texture empty
		size = (tex.second == nullptr) ? 128 : 256;
		ResourcesLoader::Append(buffer, &(size), sizeof(size));
		if(size == 128)
		{

		}
		else
		{
			//property value
			size = tex.second->name.size();
			ResourcesLoader::Append(buffer, &(size), sizeof(size));
			ResourcesLoader::Append(buffer, (void *) (tex.second->name.c_str()), size * sizeof(std::string::value_type));
			// append  property name and value (tex name)
		}

	}

	////HERE
	size = this->ValuesProperties.size();
	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	for (auto& tex : this->ValuesProperties)
	{
		//property name
		size = tex.first.size();
		ResourcesLoader::Append(buffer, &(size), sizeof(size));
		ResourcesLoader::Append(buffer, (void *) (tex.first.c_str()), size * sizeof(std::string::value_type));
		//property value
		ResourcesLoader::Append(buffer, (void *) &(tex.second),  sizeof(FieldValue));
		// append  property name and value (float value)
	}
}

void MaterialResource::FromDataBuffer(char *buffer, int bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;


	ResourcesLoader::ReadFromBuffer(buffer, &(this->type), sizeof(this->type), ReadPos);

	//recup path string
	std::uint32_t size = 0;
	std::string pString;
	ResourcesLoader::ReadFromBuffer(buffer, &size, sizeof(size), ReadPos);
	pString.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) (pString.data()),  size * sizeof( std::string::value_type), ReadPos);
	this->path = pString;

	//recup name
	size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
	this->name.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->name.data()), size * sizeof(std::string::value_type), ReadPos);

	///shader loading
	int j = 0;
	Engine* engine = Engine::GetInstance();
	ResourcesLoader::ReadFromBuffer(buffer, &(j), sizeof(j), ReadPos);
	if(j == 128)
	{
		this->shader = nullptr;
		this->defaultshader = engine->graphicsResourceMgr.GetDefaultShader();
	}
	else
	{
		std::string pName;
		size = 0;
		ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
		pName.resize(size);
		ResourcesLoader::ReadFromBuffer(buffer, (void *) (pName.data()), size * sizeof(std::string::value_type), ReadPos);
		this->shader = engine->graphicsResourceMgr.GetShader(pName.c_str());

	}

	size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
	for (int i = 0; i < size; ++i)
	{
		std::uint32_t pSize = 0;
		std::string pName;
		std::uint32_t vSize = 0;
		std::string vName;
		//property name
		ResourcesLoader::ReadFromBuffer(buffer, &(pSize), sizeof(pSize), ReadPos);
		pName.resize(pSize);
		ResourcesLoader::ReadFromBuffer(buffer, (void *) (pName.data()), pSize * sizeof(std::string::value_type), ReadPos);

		//is texture empty
		ResourcesLoader::ReadFromBuffer(buffer, &(pSize), sizeof(pSize), ReadPos);
		if(pSize == 128)
		{
			this->TexturesProperties.emplace(pName, nullptr);
		}
		else
		{
			//property value
			ResourcesLoader::ReadFromBuffer(buffer, &(vSize), sizeof(vSize), ReadPos);
			vName.resize(vSize);
			ResourcesLoader::ReadFromBuffer(buffer, (void *) (vName.data()), vSize * sizeof(std::string::value_type), ReadPos);
			// append  Texture
			this->TexturesProperties.emplace(pName, engine->graphicsResourceMgr.GetTexture(vName.c_str()));
		}
	}


	////HERE
	size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
	for (int i = 0; i < size; ++i)
	{
		std::uint32_t pSize = 0;
		std::string pName;
		FieldValue Value(EFieldType::NONE);
		//property name
		ResourcesLoader::ReadFromBuffer(buffer, &(pSize), sizeof(pSize), ReadPos);
		pName.resize(pSize);
		ResourcesLoader::ReadFromBuffer(buffer, (void *) (pName.data()), pSize * sizeof(std::string::value_type), ReadPos);
		//property value
		ResourcesLoader::ReadFromBuffer(buffer, (void *) &(Value), sizeof(FieldValue), ReadPos);
		// append  FieldValue
		this->ValuesProperties.emplace(pName, Value);
	}


}

MaterialResource::MaterialResource(const char *_name, bool _genfile)
{
	type = EResourceType::Material;
	shader = nullptr;
	defaultshader = Engine::GetInstance()->graphicsResourceMgr.GetDefaultShader();
	name = _name;
	shouldGenerateFileAtDestroy = _genfile;
}

MaterialResource::FieldValue::FieldValue(MaterialResource::EFieldType _type)
{
	type = _type;
	switch (type)
	{
		case EFieldType::BOOL:
			b = false;
			break;
		case EFieldType::INT:
			i = 0;
			break;
		case EFieldType::FLOAT:
			f = 0.0f;
			break;
		case EFieldType::VEC2:
			v2 = (0,0,0);
			break;
		case EFieldType::VEC3:
			v3 = (0,0,0);
			break;
		case EFieldType::VEC4:
			v4 = (0,0,0);
			break;
		default:
			b = false;
			type = EFieldType::BOOL;
			break;
	}
}

MaterialResource::FieldValue::FieldValue(const MaterialResource::FieldValue &copy)
{
	type = copy.type;
	switch (type)
	{
		case EFieldType::BOOL:
			b = copy.b;
			break;
		case EFieldType::INT:
			i = copy.i;
			break;
		case EFieldType::FLOAT:
			f = copy.f;
			break;
		case EFieldType::VEC2:
			v2 = copy.v2;
			break;
		case EFieldType::VEC3:
			v3 = copy.v3;
			break;
		case EFieldType::VEC4:
			v4 = copy.v4;
			break;
		default:
			b = copy.b;
			type = EFieldType::BOOL;
			break;
	}
}
