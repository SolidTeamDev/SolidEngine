#include "Resources/ressources.hpp"
#include <sstream>
#include <Resources/resourceType.hpp>

#include "glad/glad.h"
#include "Core/engine.hpp"

using namespace Solid;

int Resource::NoNameNum = 0;

#define SASSET_GEN 1

ImageResource::~ImageResource() = default;



MaterialResource::MaterialResource()
{
	type = EResourceType::Material;
	shader = nullptr;
	defaultshader = Engine::GetInstance()->graphicsResourceMgr.GetDefaultShader();
}

MaterialResource::~MaterialResource()
{

	if(shouldGenerateFileAtDestroy)
	{
		ResourcesLoader loader;
		loader.SaveMaterialToFile(this);
	}


}

AudioResource::~AudioResource()
{
	alDeleteBuffers(1, &(this->buffer));
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
	/*std::string pString = this->path.string();
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
	}*/
}

void MaterialResource::FromDataBuffer(char *buffer, int bSize)
{
	//WARNING : No test for read overflow
	/*std::uint64_t ReadPos = 0;


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

	size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
	for (int i = 0; i < size; ++i)
	{
		std::uint32_t pSize = 0;
		std::string pName;
		FieldValue Value(EShaderFieldType::NONE);
		//property name
		ResourcesLoader::ReadFromBuffer(buffer, &(pSize), sizeof(pSize), ReadPos);
		pName.resize(pSize);
		ResourcesLoader::ReadFromBuffer(buffer, (void *) (pName.data()), pSize * sizeof(std::string::value_type), ReadPos);
		//property value
		ResourcesLoader::ReadFromBuffer(buffer, (void *) &(Value), sizeof(FieldValue), ReadPos);
		// append  FieldValue
		this->ValuesProperties.emplace(pName, Value);
	}*/


}

void AudioResource::ToDataBuffer(std::vector<char> &buffer)
{
	std::string pString = this->path.string();
	std::uint32_t size = pString.size();
	ResourcesLoader::Append(buffer, &(this->type), sizeof(this->type));

	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	ResourcesLoader::Append(buffer, (void *) (pString.c_str()),  size * sizeof( std::string::value_type));

	size = this->name.size();
	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	ResourcesLoader::Append(buffer, (void *) (this->name.c_str()), size * sizeof(std::string::value_type));

	//RawAudio save
	size = this->audioRawBinary.size();
	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	ResourcesLoader::Append(buffer, (void *) (this->audioRawBinary.data()), size * sizeof(short));

	//SFINFO Save
	ResourcesLoader::Append(buffer, (void *) &(this->info), sizeof(SF_INFO));

	//format
	ResourcesLoader::Append(buffer, (void *) &(this->format), sizeof(ALenum));

	ResourcesLoader::Append(buffer, (void *) &(this->numFrames), sizeof(sf_count_t));


}

void AudioResource::FromDataBuffer(char *buffer, int bSize)
{
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

	//recup RawAudio
	size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
	this->audioRawBinary.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->audioRawBinary.data()), size * sizeof(short), ReadPos);

	ResourcesLoader::ReadFromBuffer(buffer, (void *) &(this->info), sizeof(SF_INFO), ReadPos);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) &(this->format), sizeof(ALenum), ReadPos);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) &(this->numFrames), sizeof(sf_count_t), ReadPos);

}


MaterialResource::MaterialResource(const char *_name, bool _genfile)
{
	type = EResourceType::Material;
	shader = nullptr;
	defaultshader = Engine::GetInstance()->graphicsResourceMgr.GetDefaultShader();
	name = _name;
	shouldGenerateFileAtDestroy = _genfile;
}

const std::shared_ptr<IShader> MaterialResource::GetShader() const
{
    return shader;
}

const std::shared_ptr<const IShader> MaterialResource::GetDefaultshader() const
{
    return defaultshader;
}

void MaterialResource::SetShader(const std::shared_ptr<IShader> _shader)
{
    shader = _shader;

    fields.clear();

    auto uniforms = _shader->GetUniformList();

    for(const auto& uniform: uniforms)
        fields.emplace_back(uniform);
}

MaterialResource::ShaderField::ShaderField(MaterialResource::EShaderFieldType _type)
{
	type = _type;
	switch (type)
	{
		case EShaderFieldType::BOOL:
			b = false;
			break;
		case EShaderFieldType::INT:
			i = 0;
			break;
		case EShaderFieldType::FLOAT:
			f = 0.0f;
			break;
		case EShaderFieldType::VEC2:
			v2 = (0,0,0);
			break;
		case EShaderFieldType::VEC3:
			v3 = (0,0,0);
			break;
		case EShaderFieldType::VEC4:
			v4 = (0,0,0);
			break;
	    case EShaderFieldType::TEXT:
	        text = nullptr;
            break;
		default:
			type = EShaderFieldType::NONE;
			break;
	}
}

MaterialResource::ShaderField::ShaderField(const MaterialResource::ShaderField& _copy)
{
    type = _copy.type;
    name = _copy.name;
    switch (type)
    {
        case EShaderFieldType::BOOL:
            b = _copy.b;
            break;
        case EShaderFieldType::INT:
            i = _copy.i;
            break;
        case EShaderFieldType::FLOAT:
            f = _copy.f;
            break;
        case EShaderFieldType::VEC2:
            v2 = _copy.v2;
            break;
        case EShaderFieldType::VEC3:
            v3 = _copy.v3;
            break;
        case EShaderFieldType::VEC4:
            v4 = _copy.v4;
            break;
        case EShaderFieldType::TEXT:
            {

               text = _copy.text;
                break;
            }
        default:
            type = EShaderFieldType::NONE;
            break;
    }
}

MaterialResource::ShaderField::ShaderField(const ShaderUniform &_uniform)
{
    name = _uniform.name;
    type = _uniform.type;

    switch (type)
    {
        case EShaderFieldType::BOOL:
            b = false;
            break;
        case EShaderFieldType::INT:
            i = 0;
            break;
        case EShaderFieldType::FLOAT:
            f = 0.0f;
            break;
        case EShaderFieldType::VEC2:
            v2 = (0,0,0);
            break;
        case EShaderFieldType::VEC3:
            v3 = (0,0,0);
            break;
        case EShaderFieldType::VEC4:
            v4 = (0,0,0);
            break;
        case EShaderFieldType::TEXT:
            text = nullptr;
            break;
        default:
            type = EShaderFieldType::NONE;
            break;
    }
}
MaterialResource::ShaderField &
MaterialResource::ShaderField::operator=(const MaterialResource::ShaderField &_copy)
{
    type = _copy.type;
    switch (type)
    {
        case EShaderFieldType::BOOL:
            b = _copy.b;
            break;
        case EShaderFieldType::INT:
            i = _copy.i;
            break;
        case EShaderFieldType::FLOAT:
            f = _copy.f;
            break;
        case EShaderFieldType::VEC2:
            v2 = _copy.v2;
            break;
        case EShaderFieldType::VEC3:
            v3 = _copy.v3;
            break;
        case EShaderFieldType::VEC4:
            v4 = _copy.v4;
            break;
        case EShaderFieldType::TEXT:
            text = _copy.text;
            break;
        default:
            type = EShaderFieldType::NONE;
            break;
    }

    return *this;
}

SkeletonResource::Bone::~Bone()
{
	for(Bone* child : Childrens)
	{
	    delete child;
	}
}

SkeletonResource::Bone &SkeletonResource::Bone::operator=(const SkeletonResource::Bone &b)
{
	{
		for(Bone* child : Childrens)
		{
			delete child;
		}
		Childrens.clear();
		Childrens.reserve(b.Childrens.size());
		name = b.name;
		Parent = nullptr;
		Weights = b.Weights;
		transfo = b.transfo;
		offset =b.offset;
		FinalTrans= b.FinalTrans;
		std::function<void(Bone*, Bone*,Bone*)> lambda = [&](Bone* child,Bone* Parent, Bone* childToCopy){
			child->Childrens.reserve(childToCopy->Childrens.size());
			child->name = childToCopy->name;
			child->Parent = Parent;
			child->Weights = childToCopy->Weights;
			child->transfo = childToCopy->transfo;
			child->offset =childToCopy->offset;
			child->FinalTrans= childToCopy->FinalTrans;

			for (int i = 0; i < childToCopy->Childrens.size(); ++i)
			{
				Bone* childc = new Bone();
				Bone* childToCopyc =childToCopy->Childrens[i];
				lambda(childc, child, childToCopyc);
			}
		};
		for (int i = 0; i < b.Childrens.size(); ++i)
		{
			Bone* child = new Bone();
			Bone* childToCopy =b.Childrens[i];
			lambda(child, this, childToCopy);
		}
	}
	if(b.Parent != nullptr)
	{


		Bone* BoneRoot = b.Parent;
		while (BoneRoot->Parent != nullptr)
		{
			BoneRoot = BoneRoot->Parent;
		}
		new Bone(*BoneRoot, this);
		return *this;
	}

	return *this;
}

SkeletonResource::Bone::Bone(const SkeletonResource::Bone &b)
{
	{
		Childrens.reserve(b.Childrens.size());
		name = b.name;
		Parent = nullptr;
		Weights = b.Weights;
		transfo = b.transfo;
		offset =b.offset;
		FinalTrans= b.FinalTrans;
		std::function<void(Bone*, Bone*,Bone*)> lambda = [&](Bone* child,Bone* Parent, Bone* childToCopy){
			Parent->Childrens.push_back(child);
			child->Childrens.reserve(childToCopy->Childrens.size());
			child->name = childToCopy->name;
			child->Parent = Parent;
			child->Weights = childToCopy->Weights;
			child->transfo = childToCopy->transfo;
			child->offset =childToCopy->offset;
			child->FinalTrans= childToCopy->FinalTrans;

			for (int i = 0; i < childToCopy->Childrens.size(); ++i)
			{
				Bone* childc = new Bone();
				Bone* childToCopyc =childToCopy->Childrens[i];
				lambda(childc, child, childToCopyc);
			}
		};
		for (int i = 0; i < b.Childrens.size(); ++i)
		{
			Bone* child = new Bone();
			Bone* childToCopy =b.Childrens[i];
			lambda(child, this, childToCopy);
		}
	}
	if(b.Parent != nullptr)
	{
		Bone* BoneRoot = b.Parent;
		while (BoneRoot->Parent != nullptr)
		{
			BoneRoot = BoneRoot->Parent;
		}
		new Bone(*BoneRoot, this);
		return;
	}
}

SkeletonResource::Bone::Bone(const SkeletonResource::Bone &b, SkeletonResource::Bone *toSet)
{
	if(!toSet->isPtr)
	{
		return;
	}
	/// b is root
	Childrens.reserve(b.Childrens.size());
	name = b.name;
	Parent = nullptr;
	Weights = b.Weights;
	transfo = b.transfo;
	offset =b.offset;
	FinalTrans= b.FinalTrans;
	std::function<void(Bone*, Bone*,Bone*)> lambda = [&](Bone* child,Bone* Parent, Bone* childToCopy){
		child->Childrens.reserve(childToCopy->Childrens.size());
		child->name = childToCopy->name;
		child->Parent = Parent;
		child->Weights = childToCopy->Weights;
		child->transfo = childToCopy->transfo;
		child->offset =childToCopy->offset;
		child->FinalTrans= childToCopy->FinalTrans;

		for (int i = 0; i < childToCopy->Childrens.size(); ++i)
		{
			Bone* childToCopyc =childToCopy->Childrens[i];
			if(childToCopy->name == toSet->name)
			{
				child->Childrens.push_back(toSet);
				break;
			}
			Bone* childc = new Bone();
			lambda(childc, child, childToCopyc);
		}
	};
	for (int i = 0; i < b.Childrens.size(); ++i)
	{
		Bone* childToCopy =b.Childrens[i];
		if(childToCopy->name == toSet->name)
		{
			this->Childrens.push_back(toSet);
			break;
		}
		Bone* child = new Bone();

		lambda(child, this, childToCopy);
	}
}

void *SkeletonResource::Bone::operator new(std::size_t size)
{
	void * p = ::operator new(size);
	Bone* b = (Bone*)p;
	b->isPtr = true;
	return p;
}

void SkeletonResource::Bone::operator delete(void* p)
{
	free(p);
}

SkeletonResource::Bone *SkeletonResource::Bone::FindBoneByName(const char *_name)
{
	if(name == _name)
	{
		return this;
	}
	for(Bone* child : Childrens)
	{
	    if(child->name == _name)
	    {
		    return child;
	    }
	    Bone* Search =child->FindBoneByName(_name);
	    if(Search != nullptr)
		    return Search;
	}

	return nullptr;
}
