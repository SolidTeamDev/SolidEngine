#include "Resources/ressources.hpp"
#include <sstream>
#include "Resources/resourceType.hpp"
#include "ECS/Components/scriptList.hpp"

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

	/*ResourcesLoader::Append(buffer, &(size), sizeof(size));

	for(std::string& elt : this->path)
	{
		size = elt.size();

		//asset path
		ResourcesLoader::Append(buffer, &(size), sizeof(size));
		ResourcesLoader::Append(buffer, (void *) (elt.c_str()),  size * sizeof( std::string::value_type));

	}*/




	//asset name
	size = this->name.size();
	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	ResourcesLoader::Append(buffer, (void *) (this->name.c_str()), size * sizeof(std::string::value_type));

}

int Resource::FromDataBuffer(char *buffer, size_t bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;

	//asset type
	ResourcesLoader::ReadFromBuffer(buffer, &(this->type), sizeof(this->type), ReadPos, bSize);

	//recup path string
	std::uint32_t size = 0;
	std::string pString;


	//recup name
	size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
	this->name.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->name.data()), size * sizeof(std::string::value_type),
	                                ReadPos, bSize);
	return ReadPos;
}

#define SASSET_GEN 1

ImageResource::~ImageResource() 
{

}



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

int ImageResource::FromDataBuffer(char* buffer , size_t bSize)
{
    //WARNING : No test for read overflow
    std::uint64_t ReadPos = 0;
    std::uint32_t size = 0;
    std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);

    //recup image metadata
	ResourcesLoader::ReadFromBuffer(buffer, &(this->x), sizeof(this->x), ReadPos, bSize);
	ResourcesLoader::ReadFromBuffer(buffer, &(this->y), sizeof(this->y), ReadPos, bSize);
	ResourcesLoader::ReadFromBuffer(buffer, &(this->ChannelsNum), sizeof(this->ChannelsNum), ReadPos, bSize);

    //recup image
    size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);


    this->image.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (this->image.data()), size * sizeof(unsigned char), ReadPos, bSize);
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

int MeshResource::FromDataBuffer(char *buffer, size_t bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;
	std::uint32_t size = 0;
	std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);
    //get SubMeshes Number
    size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
    this->Meshes.resize(size);
    for(MeshResource::SubMesh& Sub : this->Meshes)
    {
        size = 0;
	    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
        Sub.indices.resize(size);
	    ResourcesLoader::ReadFromBuffer(buffer, (Sub.indices.data()), size * sizeof(unsigned int), ReadPos, bSize);

        size = 0;
	    ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
        Sub.vertices.resize(size);
	    ResourcesLoader::ReadFromBuffer(buffer, (Sub.vertices.data()), size * sizeof(Vertex), ReadPos, bSize);
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

	binaries =Renderer::GetInstance()->GetShaderBinary(TempShader.shader.ProgID);
    std::uint32_t PSize = binaries.size;
    ResourcesLoader::Append(buffer, &PSize, sizeof  (PSize));
    ResourcesLoader::Append(buffer, &binaries.format, sizeof  (binaries.format));
    ResourcesLoader::Append(buffer, binaries.b, sizeof  (char) * binaries.size);

    delete[] binaries.b;
}
int ComputeShaderResource::FromDataBuffer(char *buffer, size_t bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;
	std::uint32_t size = 0;
	std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);

    size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
    this->ComputeSource.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->ComputeSource.data()),
	                                size * sizeof(std::string::value_type), ReadPos, bSize);

    //Shader Binary
    size = 0;
    GLenum bFormat;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
	ResourcesLoader::ReadFromBuffer(buffer, &(bFormat), sizeof(bFormat), ReadPos, bSize);
    char* binary = new char[size];
	ResourcesLoader::ReadFromBuffer(buffer, binary, sizeof(char) * size, ReadPos, bSize);
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
int ShaderResource::FromDataBuffer(char *buffer, size_t bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;
	std::uint32_t size = 0;
	std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);

    //shaders sources
    size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
    this->VertexSource.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->VertexSource.data()),
	                                size * sizeof(std::string::value_type), ReadPos, bSize);

    size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
    this->FragSource.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->FragSource.data()), size * sizeof(std::string::value_type),
	                                ReadPos, bSize);

    //Shader Binary
    size = 0;
    GLenum bFormat;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
	ResourcesLoader::ReadFromBuffer(buffer, &(bFormat), sizeof(bFormat), ReadPos, bSize);
    char* binary = new char[size];
	ResourcesLoader::ReadFromBuffer(buffer, binary, sizeof(char) * size, ReadPos, bSize);
    binaries ={.size =size ,.format=bFormat,.b=binary};
    delete[] binary;
	return ReadPos;
}

void ShaderResource::operator delete(void* _ptr)
{
	::operator delete(_ptr);
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
				size =  field.text.isUsingComputeGeneratedTex ? 256 : 128;
				ResourcesLoader::Append(buffer, &(size), sizeof(size));
				if(field.text.isUsingComputeGeneratedTex)
				{
					size =  field.text.Compute == nullptr ? 256 : 128;
					ResourcesLoader::Append(buffer, &(size), sizeof(size));
					if(field.text.Compute != nullptr)
					{
						Vec2i TS = field.text.Compute->GetTexSize();
						size = field.text.Compute->name.size();
						ResourcesLoader::Append(buffer, &(size), sizeof(size));
						ResourcesLoader::Append(buffer, (void *) (field.text.Compute->name.c_str()), size * sizeof(std::string::value_type));
						ResourcesLoader::Append(buffer, &TS, sizeof(Vec2i));
						field.text.Compute->SaveFields(buffer);

					}
				}
				else
				{
					size =  field.text.text == nullptr ? 256 : 128;
					ResourcesLoader::Append(buffer, &(size), sizeof(size));
					if(field.text.text != nullptr)
					{
						size = field.text.text->name.size();
						ResourcesLoader::Append(buffer, &(size), sizeof(size));
						ResourcesLoader::Append(buffer, (void *) (field.text.text->name.c_str()), size * sizeof(std::string::value_type));

					}
				}


				break;
			}
			default:
				break;
		}
	}

}

int MaterialResource::FromDataBuffer(char *buffer, size_t bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;
	std::uint32_t size = 0;
	std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);

	///shader loading
	int j = 0;
	Engine* engine = Engine::GetInstance();
	ResourcesLoader::ReadFromBuffer(buffer, &(j), sizeof(j), ReadPos, bSize);
	if(j == 128)
	{
		this->shader = nullptr;
		this->defaultshader = engine->graphicsResourceMgr.GetDefaultShader();
	}
	else
	{
		std::string pName;
		size = 0;
		ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
		pName.resize(size);
		ResourcesLoader::ReadFromBuffer(buffer, (void *) (pName.data()), size * sizeof(std::string::value_type),
		                                ReadPos, bSize);
		this->shader = engine->graphicsResourceMgr.GetShader(pName.c_str());

	}

	size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
	this->fields.resize(size);

	for(auto& field : this->fields)
	{
	    //load field name
	    size = 0;
		ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
		field.name.resize(size);
		ResourcesLoader::ReadFromBuffer(buffer, (void *) (field.name.data()), size * sizeof(std::string::value_type),
		                                ReadPos, bSize);
		//load field type
		ResourcesLoader::ReadFromBuffer(buffer, &(field.type), sizeof(EShaderFieldType), ReadPos, bSize);
		switch (field.type)
		{
			case EShaderFieldType::BOOL:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.b), sizeof(bool), ReadPos, bSize);
				break;
			}
			case EShaderFieldType::INT:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.i), sizeof(int), ReadPos, bSize);

				break;
			}
			case EShaderFieldType::FLOAT:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.f), sizeof(float), ReadPos, bSize);

				break;
			}
			case EShaderFieldType::VEC2:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.v2), sizeof(Vec2), ReadPos, bSize);

				break;
			}
			case EShaderFieldType::VEC3:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.v3), sizeof(Vec3), ReadPos, bSize);

				break;
			}
			case EShaderFieldType::VEC4:
			{
				ResourcesLoader::ReadFromBuffer(buffer, &(field.v4), sizeof(Vec4), ReadPos, bSize);

				break;
			}
			case EShaderFieldType::TEXT:
			{
				size =  0;//field.text.isUsing == nullptr ? 256 : 128;
				ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
				if(size == 256)
				{
					field.text.isUsingComputeGeneratedTex = true;
				}
				ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
				if(size == 128)
				{
					size = 0;
					std::string tStr ;
					Vec2i TS = {128,128};

					ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);

					tStr.resize(size);

					ResourcesLoader::ReadFromBuffer(buffer, (void *) (tStr.data()),
					                                size * sizeof(std::string::value_type), ReadPos, bSize);

					if(field.text.isUsingComputeGeneratedTex)
					{
						ResourcesLoader::ReadFromBuffer(buffer, &(TS), sizeof(Vec2i), ReadPos, bSize);
						field.text.Compute = Engine::GetInstance()->graphicsResourceMgr.GetCompute(tStr.c_str());
						std::size_t skipSize = 0;
						ResourcesLoader::ReadFromBuffer(buffer, &(skipSize), sizeof(size_t), ReadPos, bSize);

						if(field.text.Compute)
						{
							field.text.Compute->InitTex(TS);
							field.text.Compute->LoadFields(buffer, bSize, ReadPos);
						}
						else
							ReadPos += skipSize;
					}
					else
						field.text.text = Engine::GetInstance()->graphicsResourceMgr.GetTexture(tStr.c_str());
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

int AudioResource::FromDataBuffer(char *buffer, size_t bSize)
{
	//WARNING : No test for read overflow
	std::uint64_t ReadPos = 0;
	std::uint32_t size = 0;
	std::string pString;
	ReadPos +=Resource::FromDataBuffer(buffer, bSize);

	//recup RawAudio
	size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
	this->audioRawBinary.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) (this->audioRawBinary.data()), size * sizeof(short), ReadPos, bSize);

	ResourcesLoader::ReadFromBuffer(buffer, (void *) &(this->info), sizeof(SF_INFO), ReadPos, bSize);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) &(this->format), sizeof(ALenum), ReadPos, bSize);
	ResourcesLoader::ReadFromBuffer(buffer, (void *) &(this->numFrames), sizeof(sf_count_t), ReadPos, bSize);
	return ReadPos;
}


MaterialResource::MaterialResource(const char *_name, bool _genfile)
{
	type = EResourceType::Material;
	shader = nullptr;
	defaultshader = Engine::GetInstance()->graphicsResourceMgr.GetDefaultShader();
	name = _name;
	shouldGenerateFileAtDestroy = _genfile;
	//path.push_front("Assets\\");
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

    LoadShaderFields();
}

void MaterialResource::LoadShaderFields()
{
	std::vector<MaterialResource::ShaderField> temp = fields;
    fields.clear();

    auto uniforms = shader->GetUniformList();

    for(const auto& uniform: uniforms)
        fields.emplace_back(uniform);
	for (int i = 0; i < fields.size(); ++i)
	{
		for (int j = 0; j < temp.size(); ++j)
		{
			if(fields[i].name == temp[j].name && fields[i].type == temp[j].type)
			{
				switch (fields[i].type)
				{
					case EShaderFieldType::BOOL:
						fields[i].b = temp[j].b;
						break;
					case EShaderFieldType::INT:
						fields[i].i = temp[j].i;
						break;
					case EShaderFieldType::FLOAT:
						fields[i].f = temp[j].f;
						break;
					case EShaderFieldType::VEC2:
						fields[i].v2 = temp[j].v2;
						break;
					case EShaderFieldType::VEC3:
						fields[i].v3 = temp[j].v3;
						break;
					case EShaderFieldType::VEC4:
						fields[i].v4 = temp[j].v4;
						break;
					case EShaderFieldType::TEXT:
						fields[i].text = temp[j].text;
						break;
					default:
						break;

				}
			}
		}
	}
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
	        text.text = nullptr;
	        text.Compute = nullptr;
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
            text.text = nullptr;
            text.Compute = nullptr;
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
        LocalTrans = b.LocalTrans;
		offset =b.offset;
        id = b.id;
        isAnimated = b.isAnimated;
		std::function<void(Bone*, Bone*,Bone*)> lambda = [&](Bone* child,Bone* Parent, Bone* childToCopy){
			child->Childrens.reserve(childToCopy->Childrens.size());
			child->name = childToCopy->name;
			child->Parent = Parent;
			child->Weights = childToCopy->Weights;
			child->LocalTrans = childToCopy->LocalTrans;
			child->offset =childToCopy->offset;
            child->id = childToCopy->id;
            child->isAnimated = childToCopy->isAnimated;

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
        LocalTrans = b.LocalTrans;
		offset =b.offset;
		id = b.id;
		isAnimated = b.isAnimated;
		std::function<void(Bone*, Bone*,Bone*)> lambda = [&](Bone* child,Bone* Parent, Bone* childToCopy){
			Parent->Childrens.push_back(child);
			child->Childrens.reserve(childToCopy->Childrens.size());
			child->name = childToCopy->name;
			child->Parent = Parent;
			child->Weights = childToCopy->Weights;
			child->LocalTrans = childToCopy->LocalTrans;
			child->offset =childToCopy->offset;
            child->id = childToCopy->id;
            child->isAnimated = childToCopy->isAnimated;

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
    LocalTrans = b.LocalTrans;
	offset =b.offset;
    id = b.id;
    isAnimated = b.isAnimated;
	std::function<void(Bone*, Bone*,Bone*)> lambda = [&](Bone* child,Bone* Parent, Bone* childToCopy){
		child->Childrens.reserve(childToCopy->Childrens.size());
		child->name = childToCopy->name;
		child->Parent = Parent;
		child->Weights = childToCopy->Weights;
		child->LocalTrans = childToCopy->LocalTrans;
		child->offset =childToCopy->offset;
        child->id = childToCopy->id;
        child->isAnimated = childToCopy->isAnimated;

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
	::operator delete (p);
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

void PrefabResource::UpdatePrefab(GameObject *_gameObject)
{

	json j;
	j["Scene"].array();
	j = j.flatten();
	std::string elt = "/Scene";
	GameObject* world = _gameObject;
	std::function<void(json&, GameObject*, std::string&)> Lambda = [&](json& j, GameObject* elt, std::string& path){

		std::string subP = path + "/{GameObject_"+ std::to_string(elt->GetEntity()) + "}" ;
		j[subP + "/Name"] = elt->name;
		j[subP + "/TAG"] = elt->tag;

		for(GameObject* sub : elt->childs)
		{
			Lambda(std::ref(j), sub, std::ref(subP));
		}

	};
	Lambda(std::ref(j), world, std::ref(elt));
	j = j.unflatten();

	std::vector<char> buffer;
	std::stringstream sstr;
	sstr << std::setw(4) << j << std::endl;
	std::size_t sstrSize = sstr.str().size() * sizeof(std::string::value_type);
	ResourcesLoader::Append(buffer, &sstrSize , sizeof(std::size_t));

	ResourcesLoader::Append(buffer, sstr.str().data(), sstrSize);


	std::function<void(GameObject*)> LambdaCmp = [&](GameObject* elt){


		//store num of comps
		std::size_t cmpNum = elt->compsList.size();
		ResourcesLoader::Append(buffer, &cmpNum, sizeof(std::size_t));

		for(Components* cmp : elt->compsList)
		{
			if(cmp->getArchetype().name == "ScriptList")
			{
				for(Script* script : ((ScriptList*)cmp)->GetAllScripts())
				{

					std::size_t offset =0;

					std::size_t scriptNameSize = 0;

					//store comp name / string
					scriptNameSize = script->getArchetype().name.size()*sizeof(std::string::value_type);
					ResourcesLoader::Append(buffer, &scriptNameSize, sizeof(std::size_t));
					ResourcesLoader::Append(buffer, (void*)script->getArchetype().name.data(),  scriptNameSize);

					//store num of fields
					std::size_t numFields = script->getArchetype().fields.size();
					ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
					//calc skip size
					std::size_t skipSize = 0;
					for(auto& cField : script->getArchetype().fields)//2 cField var WARN
					{
						std::size_t size = 0;
						size = cField.name.size()*sizeof(std::string::value_type);
						//store field name / string
						skipSize += sizeof(std::size_t);
						skipSize +=  size;
						short isNull = 128;
						if(cField.type.archetype == nullptr)
						{
							isNull = 256;
							std::string str = cField.getData<std::string>(script);
							std::size_t strS =  str.size()*sizeof(std::string::value_type);
							//store isNull
							skipSize += sizeof(short);
							//store field data
							skipSize +=  sizeof(std::size_t);
							skipSize +=  strS;
						}
						else
						{
							skipSize +=  sizeof(short);
							if(cField.type.archetype->name == "String")
							{
								String* str = (String*)cField.getDataAddress(script);
								size = str->size()*sizeof(std::string::value_type);
								skipSize +=  sizeof(std::size_t);
								skipSize +=  size;
							}
							else if(cField.type.archetype->name == "vectorStr")
							{
								vectorStr* vstr = (vectorStr*)cField.getDataAddress(cmp);
								size = vstr->size();
								std::size_t fieldsize = sizeof(std::size_t);
								for(auto& str : *vstr)
								{
									fieldsize += str.size()*sizeof(std::string::value_type);
									fieldsize += sizeof(std::size_t);
								}

								skipSize +=  sizeof(std::size_t);

								skipSize +=  sizeof(std::size_t);
								for(auto& str : *vstr)
								{
									std::size_t strSize = str.size()*sizeof(std::string::value_type);
									skipSize += sizeof(std::size_t);
									skipSize +=  strSize;
								}
							}
							else
							{
								size = cField.type.archetype->memorySize;
								//store isNull
								//store field data
								skipSize += sizeof(std::size_t);
								skipSize += size;

							}
						}

					}

					ResourcesLoader::Append(buffer, &skipSize, sizeof(std::size_t));


					for(auto& cField : script->getArchetype().fields)//2 cField var WARN
					{
						std::size_t size = 0;
						size = cField.name.size()*sizeof(std::string::value_type);
						//store field name / string
						ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
						ResourcesLoader::Append(buffer, (void*)cField.name.data(),  size);
						short isNull = 128;
						if(cField.type.archetype == nullptr)
						{
							isNull = 256;
							std::string str = cField.getData<std::string>(script);
							std::size_t strS =  str.size()*sizeof(std::string::value_type);
							//store isNull
							ResourcesLoader::Append(buffer, &isNull, sizeof(short));
							//store field data
							ResourcesLoader::Append(buffer, &strS, sizeof(std::size_t));
							ResourcesLoader::Append(buffer, str.data(), strS);
						}
						else
						{
							ResourcesLoader::Append(buffer, &isNull, sizeof(short));
							if(cField.type.archetype->name == "String")
							{
								String* str = (String*)cField.getDataAddress(script);
								size = str->size()*sizeof(std::string::value_type);
								ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
								ResourcesLoader::Append(buffer, str->data(), size);
							}
							else if(cField.type.archetype->name == "vectorStr")
							{
								vectorStr* vstr = (vectorStr*)cField.getDataAddress(cmp);
								size = vstr->size();
								std::size_t fieldsize = sizeof(std::size_t);
								for(auto& str : *vstr)
								{
									fieldsize += str.size()*sizeof(std::string::value_type);
									fieldsize += sizeof(std::size_t);
								}

								ResourcesLoader::Append(buffer, &fieldsize, sizeof(std::size_t));

								ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
								for(auto& str : *vstr)
								{
									std::size_t strSize = str.size()*sizeof(std::string::value_type);
									ResourcesLoader::Append(buffer, &strSize, sizeof(std::size_t));
									ResourcesLoader::Append(buffer, str.data(), strSize);
								}
							}
							else
							{
								size = cField.type.archetype->memorySize;
								//store isNull
								//store field data
								ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
								ResourcesLoader::Append(buffer, cField.getDataAddress(script), size);

							}
						}

					}

				}
			}
			else if(cmp->getArchetype().name == "ParticleEffect")
			{
				std::size_t offset =0;

				std::size_t cmpNameSize = 0;

				//store comp name / string
				cmpNameSize = cmp->getArchetype().name.size()*sizeof(std::string::value_type);
				ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
				ResourcesLoader::Append(buffer, (void*)cmp->getArchetype().name.data(),  cmpNameSize);
				//store num of fields
				std::size_t numFields = 0;
				ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
				ParticleEffect* effect = (ParticleEffect*)cmp;
				std::size_t skipSize = 0;


				std::size_t isNull = (effect->GetSystem() != nullptr) ? 128 : 256;
				skipSize += sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->GetSystem();
					skipSize +=  sizeof(std::size_t);

				}
				isNull = (effect->GetEmitter() != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->GetEmitter();
					skipSize +=  sizeof(float);

				}
				isNull = (effect->GetParticleTex() != nullptr) ? 128 : 256;
				skipSize += sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->GetParticleTex();
					cmpNameSize = ptr->name.size()*sizeof(std::string::value_type);
					skipSize += sizeof(std::size_t);
					skipSize += ( cmpNameSize);

				}
				isNull = (effect->GetRenderer() != nullptr) ? 128 : 256;
				skipSize += sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->GetRenderer();
				}
				isNull = (effect->velFromPosGen != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->velFromPosGen;
					skipSize +=  sizeof(Vec4);
					skipSize +=  sizeof(float);
					skipSize += sizeof(float);
				}
				isNull = (effect->colGen != nullptr) ? 128 : 256;
				skipSize += sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->colGen;
					skipSize +=  sizeof(Vec4);
					skipSize +=  sizeof(Vec4);
					skipSize +=  sizeof(Vec4);
					skipSize +=  sizeof(Vec4);
				}
				isNull = (effect->sphereVelGen != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->sphereVelGen;
					skipSize += sizeof(float);
					skipSize +=  sizeof(float);

				}
				isNull = (effect->spherePosGen != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->spherePosGen;
					skipSize +=  sizeof(Vec4);
					skipSize +=  sizeof(float);
					skipSize +=  sizeof(float);
					skipSize += sizeof(float);

				}
				isNull = (effect->timeGen != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->timeGen;
					skipSize +=  sizeof(float);
					skipSize +=  sizeof(float);


				}
				isNull = (effect->velGen != nullptr) ? 128 : 256;
				skipSize += sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->velGen;
					skipSize +=  sizeof(Vec4);
					skipSize +=  sizeof(Vec4);

				}
				isNull = (effect->boxPosGen != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->boxPosGen;
					skipSize +=  sizeof(Vec4);
					skipSize +=  sizeof(Vec4);

				}
				isNull = (effect->killerZoneUpdater != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->killerZoneUpdater;
					skipSize += sizeof(Vec3);
					skipSize +=  sizeof(Vec3);

				}
				isNull = (effect->attractorUpdater != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->attractorUpdater;
					cmpNameSize = ptr->attractors.size();
					skipSize +=  sizeof(std::size_t);
					skipSize += ( sizeof(Vec4) * cmpNameSize);

				}
				isNull = (effect->velColUpdater != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->velColUpdater;
					skipSize +=  sizeof(Vec4);
					skipSize +=  sizeof(Vec4);

				}
				isNull = (effect->posColUpdater != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->posColUpdater;
					skipSize +=  sizeof(Vec4);
					skipSize += sizeof(Vec4);

				}
				isNull = (effect->colorUpdater != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->colorUpdater;

				}
				isNull = (effect->eulerUpdater != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->eulerUpdater;
					skipSize +=  sizeof(Vec4);

				}
				isNull = (effect->floorUpdater != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->floorUpdater;
					skipSize +=  sizeof(float);
					skipSize +=  sizeof(float);


				}
				isNull = (effect->timeUpdater != nullptr) ? 128 : 256;
				skipSize +=  sizeof(std::size_t);
				if(isNull == 128)
				{
					auto ptr = effect->timeUpdater;

				}


				ResourcesLoader::Append(buffer, &skipSize, sizeof(std::size_t));




				std::size_t numP = effect->particlesSize;
				ResourcesLoader::Append(buffer, &numP, sizeof(std::size_t));

				isNull = (effect->GetSystem() != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->GetSystem();
					ResourcesLoader::Append(buffer, &ptr->count, sizeof(std::size_t));

				}
				isNull = (effect->GetEmitter() != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->GetEmitter();
					ResourcesLoader::Append(buffer, &ptr->emitRate, sizeof(float));

				}
				isNull = (effect->GetParticleTex() != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->GetParticleTex();
					cmpNameSize = ptr->name.size()*sizeof(std::string::value_type);
					ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
					ResourcesLoader::Append(buffer, (void*)ptr->name.data(),  cmpNameSize);

				}
				isNull = (effect->GetRenderer() != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->GetRenderer();
				}
				isNull = (effect->velFromPosGen != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->velFromPosGen;
					ResourcesLoader::Append(buffer, &ptr->offset, sizeof(Vec4));
					ResourcesLoader::Append(buffer, &ptr->minScale, sizeof(float));
					ResourcesLoader::Append(buffer, &ptr->maxScale, sizeof(float));
				}
				isNull = (effect->colGen != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->colGen;
					ResourcesLoader::Append(buffer, &ptr->minStartCol, sizeof(Vec4));
					ResourcesLoader::Append(buffer, &ptr->maxStartCol, sizeof(Vec4));
					ResourcesLoader::Append(buffer, &ptr->minEndCol, sizeof(Vec4));
					ResourcesLoader::Append(buffer, &ptr->maxEndCol, sizeof(Vec4));
				}
				isNull = (effect->sphereVelGen != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->sphereVelGen;
					ResourcesLoader::Append(buffer, &ptr->minVel, sizeof(float));
					ResourcesLoader::Append(buffer, &ptr->maxVel, sizeof(float));

				}
				isNull = (effect->spherePosGen != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->spherePosGen;
					ResourcesLoader::Append(buffer, &ptr->center, sizeof(Vec4));
					ResourcesLoader::Append(buffer, &ptr->radX, sizeof(float));
					ResourcesLoader::Append(buffer, &ptr->radY, sizeof(float));
					ResourcesLoader::Append(buffer, &ptr->radZ, sizeof(float));

				}
				isNull = (effect->timeGen != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->timeGen;
					ResourcesLoader::Append(buffer, &ptr->minTime, sizeof(float));
					ResourcesLoader::Append(buffer, &ptr->maxTime, sizeof(float));


				}
				isNull = (effect->velGen != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->velGen;
					ResourcesLoader::Append(buffer, &ptr->minStartVel, sizeof(Vec4));
					ResourcesLoader::Append(buffer, &ptr->maxStartVel, sizeof(Vec4));

				}
				isNull = (effect->boxPosGen != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->boxPosGen;
					ResourcesLoader::Append(buffer, &ptr->pos, sizeof(Vec4));
					ResourcesLoader::Append(buffer, &ptr->maxStartPosOffset, sizeof(Vec4));

				}
				isNull = (effect->killerZoneUpdater != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->killerZoneUpdater;
					ResourcesLoader::Append(buffer, &ptr->pos, sizeof(Vec3));
					ResourcesLoader::Append(buffer, &ptr->offset, sizeof(Vec3));

				}
				isNull = (effect->attractorUpdater != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->attractorUpdater;
					cmpNameSize = ptr->attractors.size();
					ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
					ResourcesLoader::Append(buffer, ptr->attractors.data(), sizeof(Vec4) * cmpNameSize);

				}
				isNull = (effect->velColUpdater != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->velColUpdater;
					ResourcesLoader::Append(buffer, &ptr->minVel, sizeof(Vec4));
					ResourcesLoader::Append(buffer, &ptr->maxVel, sizeof(Vec4));

				}
				isNull = (effect->posColUpdater != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->posColUpdater;
					ResourcesLoader::Append(buffer, &ptr->minPos, sizeof(Vec4));
					ResourcesLoader::Append(buffer, &ptr->maxPos, sizeof(Vec4));

				}
				isNull = (effect->colorUpdater != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->colorUpdater;

				}
				isNull = (effect->eulerUpdater != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->eulerUpdater;
					ResourcesLoader::Append(buffer, &ptr->globalAcceleration, sizeof(Vec4));

				}
				isNull = (effect->floorUpdater != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->floorUpdater;
					ResourcesLoader::Append(buffer, &ptr->bounceFactor, sizeof(float));
					ResourcesLoader::Append(buffer, &ptr->floorY, sizeof(float));


				}
				isNull = (effect->timeUpdater != nullptr) ? 128 : 256;
				ResourcesLoader::Append(buffer, &isNull, sizeof(std::size_t));
				if(isNull == 128)
				{
					auto ptr = effect->timeUpdater;

				}

			}
			else
			{

				std::size_t offset =0;

				std::size_t cmpNameSize = 0;

				//store comp name / string
				cmpNameSize = cmp->getArchetype().name.size()*sizeof(std::string::value_type);
				ResourcesLoader::Append(buffer, &cmpNameSize, sizeof(std::size_t));
				ResourcesLoader::Append(buffer, (void*)cmp->getArchetype().name.data(),  cmpNameSize);

				//store num of fields
				std::size_t numFields = cmp->getArchetype().fields.size();
				ResourcesLoader::Append(buffer, &numFields, sizeof(std::size_t));
				std::size_t skipSize = 0;
				for(auto& cField : cmp->getArchetype().fields)//2 cField var WARN
				{
					std::size_t size = 0;
					size = cField.name.size()*sizeof(std::string::value_type);
					//store field name / string
					skipSize += sizeof(std::size_t);
					skipSize +=  size;
					short isNull = 128;
					if(cField.type.archetype == nullptr)
					{
						isNull = 256;
						std::string str = cField.getData<std::string>(cmp);
						std::size_t strS =  str.size()*sizeof(std::string::value_type);
						//store isNull
						skipSize += sizeof(short);
						//store field data
						skipSize +=  sizeof(std::size_t);
						skipSize +=  strS;
					}
					else
					{
						skipSize +=  sizeof(short);
						if(cField.type.archetype->name == "String")
						{
							String* str = (String*)cField.getDataAddress(cmp);
							size = str->size()*sizeof(std::string::value_type);
							skipSize +=  sizeof(std::size_t);
							skipSize +=  size;
						}
						else if(cField.type.archetype->name == "vectorStr")
						{
							vectorStr* vstr = (vectorStr*)cField.getDataAddress(cmp);
							size = vstr->size();
							std::size_t fieldsize = sizeof(std::size_t);
							for(auto& str : *vstr)
							{
								fieldsize += str.size()*sizeof(std::string::value_type);
								fieldsize += sizeof(std::size_t);
							}

							skipSize +=  sizeof(std::size_t);

							skipSize +=  sizeof(std::size_t);
							for(auto& str : *vstr)
							{
								std::size_t strSize = str.size()*sizeof(std::string::value_type);
								skipSize += sizeof(std::size_t);
								skipSize +=  strSize;
							}
						}
						else
						{
							size = cField.type.archetype->memorySize;
							//store isNull
							//store field data
							skipSize += sizeof(std::size_t);
							skipSize += size;

						}
					}

				}

				ResourcesLoader::Append(buffer, &skipSize, sizeof(std::size_t));


				for(auto& cField : cmp->getArchetype().fields)//2 cField var WARN
				{
					std::size_t size = 0;
					size = cField.name.size()*sizeof(std::string::value_type);
					//store field name / string
					ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
					ResourcesLoader::Append(buffer, (void*)cField.name.data(),  size);
					short isNull = 128;
					if(cField.type.archetype == nullptr)
					{
						isNull = 256;
						std::string str = cField.getData<std::string>(cmp);
						std::size_t strS =  str.size()*sizeof(std::string::value_type);
						//store isNull
						ResourcesLoader::Append(buffer, &isNull, sizeof(short));
						//store field data
						ResourcesLoader::Append(buffer, &strS, sizeof(std::size_t));
						ResourcesLoader::Append(buffer, str.data(), strS);
					}
					else
					{
						ResourcesLoader::Append(buffer, &isNull, sizeof(short));
						if(cField.type.archetype->name == "String")
						{
							String* str = (String*)cField.getDataAddress(cmp);
							size = str->size()*sizeof(std::string::value_type);
							ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
							ResourcesLoader::Append(buffer, str->data(), size);
						}
						else if(cField.type.archetype->name == "vectorStr")
						{
							vectorStr* vstr = (vectorStr*)cField.getDataAddress(cmp);
							size = vstr->size();
							std::size_t fieldsize = sizeof(std::size_t);
							for(auto& str : *vstr)
							{
								fieldsize += str.size()*sizeof(std::string::value_type);
								fieldsize += sizeof(std::size_t);
							}

							ResourcesLoader::Append(buffer, &fieldsize, sizeof(std::size_t));

							ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
							for(auto& str : *vstr)
							{
								std::size_t strSize = str.size()*sizeof(std::string::value_type);
								ResourcesLoader::Append(buffer, &strSize, sizeof(std::size_t));
								ResourcesLoader::Append(buffer, str.data(), strSize);
							}
						}
						else
						{
							size = cField.type.archetype->memorySize;
							//store isNull
							//store field data
							ResourcesLoader::Append(buffer, &size, sizeof(std::size_t));
							ResourcesLoader::Append(buffer, cField.getDataAddress(cmp), size);

						}
					}

				}

			}

		}


		//store Num of Childs
		std::size_t ChildNum = elt->childs.size();
		ResourcesLoader::Append(buffer, &ChildNum, sizeof(std::size_t));
		for(GameObject* sub : elt->childs)
		{
			LambdaCmp(sub);
		}

	};
	LambdaCmp(world);
	PrefabBinary = buffer;

	ResourcesLoader loader;
	loader.SavePrefabToFile(this);


}

void PrefabResource::ToDataBuffer(std::vector<char> &buffer)
{
	Resource::ToDataBuffer(buffer);
	std::uint32_t size = this->PrefabBinary.size();

	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	ResourcesLoader::Append(buffer, this->PrefabBinary.data(), size *sizeof(char));

}

int PrefabResource::FromDataBuffer(char *buffer, size_t bSize)
{
	std::uint64_t ReadPos =  Resource::FromDataBuffer(buffer, bSize);
	std::uint32_t size =0;
	//asset type
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
	PrefabBinary.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, PrefabBinary.data(), size * sizeof(char), ReadPos, bSize);
	return ReadPos;
}

void SceneResource::ToDataBuffer(std::vector<char> &buffer)
{
	Resource::ToDataBuffer(buffer);
	std::uint32_t size = this->rawScene.size();

	ResourcesLoader::Append(buffer, &(size), sizeof(size));
	ResourcesLoader::Append(buffer, this->rawScene.data(), size *sizeof(char));
}

int SceneResource::FromDataBuffer(char *buffer, size_t bSize)
{
	std::uint64_t ReadPos =  Resource::FromDataBuffer(buffer, bSize);
	std::uint32_t size =0;
	//asset type
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
	rawScene.resize(size);
	ResourcesLoader::ReadFromBuffer(buffer, rawScene.data(), size * sizeof(char), ReadPos, bSize);
	return ReadPos;
}

void CubemapResource::ToDataBuffer(std::vector<char> &buffer)
{

}

int CubemapResource::FromDataBuffer(char *buffer, size_t bSize)
{
    return 0;
}
