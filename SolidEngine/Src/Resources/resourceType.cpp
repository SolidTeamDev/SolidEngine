#include "Resources/ressources.hpp"
#include <sstream>
#include <Resources/resourceType.hpp>

#include "glad/glad.h"
#include "Core/engine.hpp"

using namespace Solid;

int Resource::NoNameNum = 0;

void Resource::ToDataBuffer(std::vector<char> &buffer)
{
	std::string pString ;
	std::uint32_t size = this->path.size();
	//asset type
	ResourcesLoader::Append(buffer, &(this->type), sizeof(this->type));

	ResourcesLoader::Append(buffer, &(size), sizeof(size));

	for(std::string& elt : this->path)
	{
		size = elt.size();

		//asset path
		ResourcesLoader::Append(buffer, &(size), sizeof(size));
		ResourcesLoader::Append(buffer, (void *) (elt.c_str()),  size * sizeof( std::string::value_type));

	}




	//asset name
	size = this->name.size();
	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	ResourcesLoader::Append(buffer, (void *) (this->name.c_str()), size * sizeof(std::string::value_type));

}

int Resource::FromDataBuffer(char *buffer, int bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;

	//asset type
	ResourcesLoader::ReadFromBuffer(buffer, &(this->type), sizeof(this->type), ReadPos);

	//recup path string
	std::uint32_t size = 0;
	std::string pString;
	ResourcesLoader::ReadFromBuffer(buffer, &size, sizeof(size), ReadPos);
	this->path.resize(size);
	for(std::string& elt : this->path)
	{
		ResourcesLoader::ReadFromBuffer(buffer, &size, sizeof(size), ReadPos);
		elt.resize(size);
		ResourcesLoader::ReadFromBuffer(buffer, (void *) (elt.data()),  size * sizeof( std::string::value_type), ReadPos);

	}


	//recup name
	size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
	this->name.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->name.data()), size * sizeof(std::string::value_type), ReadPos);
	return ReadPos;
}

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

    std::string pString ;
    std::uint32_t size = pString.size();
    Resource::ToDataBuffer(buffer);

    ResourcesLoader::Append(buffer, &(this->x), sizeof(this->x));
    ResourcesLoader::Append(buffer, &(this->y), sizeof(this->y));
    ResourcesLoader::Append(buffer, &(this->ChannelsNum), sizeof(this->ChannelsNum));

    size = this->image.size();
    ResourcesLoader::Append(buffer, &(size), sizeof(size));
    ResourcesLoader::Append(buffer, (this->image.data()), size * sizeof(unsigned char));



}

int ImageResource::FromDataBuffer(char* buffer , int bSize)
{
    //WARNING : No test for read overflow
    std::uint64_t ReadPos = 0;
    std::uint32_t size = 0;
    std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);

    //recup image metadata
    ResourcesLoader::ReadFromBuffer(buffer, &(this->x), sizeof(this->x), ReadPos);
    ResourcesLoader::ReadFromBuffer(buffer, &(this->y), sizeof(this->y), ReadPos);
    ResourcesLoader::ReadFromBuffer(buffer, &(this->ChannelsNum), sizeof(this->ChannelsNum), ReadPos);

    //recup image
    size = 0;
    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);


    this->image.resize(size);
    ResourcesLoader::ReadFromBuffer(buffer, (this->image.data()), size * sizeof(unsigned char), ReadPos);
	return ReadPos;
}

// MESH

void MeshResource::ToDataBuffer(std::vector<char> &buffer)
{
    std::string pString ;
    std::uint32_t size = pString.size();
	Resource::ToDataBuffer(buffer);

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

int MeshResource::FromDataBuffer(char *buffer, int bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;
	std::uint32_t size = 0;
	std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);
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


	return ReadPos;
}

// Shaders


//Compute

void ComputeShaderResource::ToDataBuffer(std::vector<char> &buffer)
{
    std::string pString ;
    std::uint32_t size = pString.size();

	Resource::ToDataBuffer(buffer);

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
int ComputeShaderResource::FromDataBuffer(char *buffer, int bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;
	std::uint32_t size = 0;
	std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);

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
	return ReadPos;
}

//Vertex / Frag

void ShaderResource::ToDataBuffer(std::vector<char> &buffer)
{
    std::string pString ;
    std::uint32_t size = pString.size();

	Resource::ToDataBuffer(buffer);

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
int ShaderResource::FromDataBuffer(char *buffer, int bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;
	std::uint32_t size = 0;
	std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);

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
	return ReadPos;
}

void MaterialResource::ToDataBuffer(std::vector<char> &buffer)
{
	std::string pString ;
	std::uint32_t size = pString.size();

	Resource::ToDataBuffer(buffer);

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

	size = this->fields.size();
	ResourcesLoader::Append(buffer, &(size), sizeof(size));

	for(auto& field : this->fields)
	{
		//store field name
		size = field.name.size();
		ResourcesLoader::Append(buffer, &(size), sizeof(size));
		ResourcesLoader::Append(buffer, (void *) (field.name.c_str()), size * sizeof(std::string::value_type));
		//store field type
		ResourcesLoader::Append(buffer, &(field.type), sizeof(EShaderFieldType));
		switch (field.type)
		{
			case EShaderFieldType::BOOL:
			{
				ResourcesLoader::Append(buffer, &(field.b), sizeof(bool));
				break;
			}
			case EShaderFieldType::INT:
			{
				ResourcesLoader::Append(buffer, &(field.i), sizeof(int));

				break;
			}
			case EShaderFieldType::FLOAT:
			{
				ResourcesLoader::Append(buffer, &(field.f), sizeof(float));

				break;
			}
			case EShaderFieldType::VEC2:
			{
				ResourcesLoader::Append(buffer, &(field.v2), sizeof(Vec2));

				break;
			}
			case EShaderFieldType::VEC3:
			{
				ResourcesLoader::Append(buffer, &(field.v3), sizeof(Vec3));

				break;
			}
			case EShaderFieldType::VEC4:
			{
				ResourcesLoader::Append(buffer, &(field.v4), sizeof(Vec4));

				break;
			}
			case EShaderFieldType::TEXT:
			{
				size =  field.text == nullptr ? 256 : 128;
				ResourcesLoader::Append(buffer, &(size), sizeof(size));
				if(field.text != nullptr)
				{
					size = field.text->name.size();
					ResourcesLoader::Append(buffer, &(size), sizeof(size));
					ResourcesLoader::Append(buffer, (void *) (field.text->name.c_str()), size * sizeof(std::string::value_type));

				}

				break;
			}
			default:
				break;
		}
	}

}

int MaterialResource::FromDataBuffer(char *buffer, int bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;
	std::uint32_t size = 0;
	std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);

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
	this->fields.resize(size);

	for(auto& field : this->fields)
	{
	    //load field name
	    size = 0;
		ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
		field.name.resize(size);
		ResourcesLoader::ReadFromBuffer(buffer, (void *) (field.name.data()), size * sizeof(std::string::value_type), ReadPos);
		//load field type
		ResourcesLoader::ReadFromBuffer(buffer, &(field.type), sizeof(EShaderFieldType), ReadPos);
		switch (field.type)
		{
			case EShaderFieldType::BOOL:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.b), sizeof(bool), ReadPos);
				break;
			}
			case EShaderFieldType::INT:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.i), sizeof(int), ReadPos);

				break;
			}
			case EShaderFieldType::FLOAT:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.f), sizeof(float), ReadPos);

				break;
			}
			case EShaderFieldType::VEC2:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.v2), sizeof(Vec2), ReadPos);

				break;
			}
			case EShaderFieldType::VEC3:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.v3), sizeof(Vec3), ReadPos);

				break;
			}
			case EShaderFieldType::VEC4:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.v4), sizeof(Vec4), ReadPos);

				break;
			}
			case EShaderFieldType::TEXT:
			{
				size =  256;//field.text == nullptr ? 256 : 128;
				ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
				if(size == 128)
				{
					size = 0;
					std::string tStr ;

					ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
					tStr.resize(size);

					ResourcesLoader::ReadFromBuffer(buffer, (void *) (tStr.data()), size * sizeof(std::string::value_type),ReadPos);
					field.text = Engine::GetInstance()->graphicsResourceMgr.GetTexture(tStr.c_str());
				}

				break;
			}
			default:
				break;
		}
	}


	return ReadPos;

}

void AudioResource::ToDataBuffer(std::vector<char> &buffer)
{
	std::string pString ;
	std::uint32_t size = pString.size();

	Resource::ToDataBuffer(buffer);

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

int AudioResource::FromDataBuffer(char *buffer, int bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;
	std::uint32_t size = 0;
	std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);

	//recup RawAudio
	size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos);
	this->audioRawBinary.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->audioRawBinary.data()), size * sizeof(short), ReadPos);

	ResourcesLoader::ReadFromBuffer(buffer, (void *) &(this->info), sizeof(SF_INFO), ReadPos);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) &(this->format), sizeof(ALenum), ReadPos);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) &(this->numFrames), sizeof(sf_count_t), ReadPos);
	return ReadPos;
}


MaterialResource::MaterialResource(const char *_name, bool _genfile)
{
	type = EResourceType::Material;
	shader = nullptr;
	defaultshader = Engine::GetInstance()->graphicsResourceMgr.GetDefaultShader();
	name = _name;
	shouldGenerateFileAtDestroy = _genfile;
	path.push_front("\\Assets\\");
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

MaterialResource::ShaderField::ShaderField()
{
	name = "ERROR 404";
	type = EShaderFieldType::NONE;
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
