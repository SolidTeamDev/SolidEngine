//
// Created by ryan1 on 06/06/2021.
//
#include "Core/engine.hpp"
#include "Resources/graphicalResourceInterface.hpp"
using namespace Solid;

ICompute::ShaderField::ShaderField(ICompute::EShaderFieldType _type)
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

ICompute::ShaderField::ShaderField(const ICompute::ShaderField& _copy)
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

ICompute::ShaderField::ShaderField(const ShaderUniform &_uniform)
{
	name = _uniform.name;
	type = (EShaderFieldType)_uniform.type;

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
ICompute::ShaderField &
ICompute::ShaderField::operator=(const ICompute::ShaderField &_copy)
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

ICompute::ShaderField::ShaderField()
{
	name = "ERROR 404";
	type = EShaderFieldType::NONE;
}

void ICompute::SaveFields(std::vector<char> &buffer)
{
	size_t skipSize = GetSkipSize();
	ResourcesLoader::Append(buffer, &(skipSize), sizeof(skipSize));




	size_t size = ComputeFields.size();

	ResourcesLoader::Append(buffer, &(size), sizeof(size));

	for(auto& field : ComputeFields)
	{
		//store field name
		size = field.name.size();
		ResourcesLoader::Append(buffer, &(size), sizeof(size));
		ResourcesLoader::Append(buffer, (void *) (field.name.c_str()), size * sizeof(std::string::value_type));
		//store field type
		ResourcesLoader::Append(buffer, &(field.type), sizeof(EShaderFieldType));
		switch (field.type)
		{
			case ICompute::EShaderFieldType::BOOL:
			{
				ResourcesLoader::Append(buffer, &(field.b), sizeof(bool));
				break;
			}
			case ICompute::EShaderFieldType::INT:
			{
				ResourcesLoader::Append(buffer, &(field.i), sizeof(int));

				break;
			}
			case ICompute::EShaderFieldType::FLOAT:
			{
				ResourcesLoader::Append(buffer, &(field.f), sizeof(float));

				break;
			}
			case ICompute::EShaderFieldType::VEC2:
			{
				ResourcesLoader::Append(buffer, &(field.v2), sizeof(Vec2));

				break;
			}
			case ICompute::EShaderFieldType::VEC3:
			{
				ResourcesLoader::Append(buffer, &(field.v3), sizeof(Vec3));

				break;
			}
			case ICompute::EShaderFieldType::VEC4:
			{
				ResourcesLoader::Append(buffer, &(field.v4), sizeof(Vec4));

				break;
			}
			case ICompute::EShaderFieldType::TEXT:
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

void ICompute::LoadFields(char *buffer, size_t bSize, uint64_t &ReadPos)
{
	std::size_t size = 0;
	ResourcesLoader::ReadFromBuffer(buffer, &(size), sizeof(size), ReadPos, bSize);
	ComputeFields.resize(size);

	for(auto& field : ComputeFields)
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
						//skipSize
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

}

size_t ICompute::GetSkipSize()
{
	size_t size;
	size_t skipSize = 0;
	skipSize +=( sizeof(size));

	for(auto& field : ComputeFields)
	{
		//store field name
		size = field.name.size();
		skipSize +=( sizeof(size));
		skipSize +=( size * sizeof(std::string::value_type));
		//store field type
		skipSize +=(  sizeof(EShaderFieldType));
		switch (field.type)
		{
			case ICompute::EShaderFieldType::BOOL:
			{
				skipSize +=(  sizeof(bool));
				break;
			}
			case ICompute::EShaderFieldType::INT:
			{
				skipSize +=(  sizeof(int));

				break;
			}
			case ICompute::EShaderFieldType::FLOAT:
			{
				skipSize +=(  sizeof(float));

				break;
			}
			case ICompute::EShaderFieldType::VEC2:
			{
				skipSize +=(  sizeof(Vec2));

				break;
			}
			case ICompute::EShaderFieldType::VEC3:
			{
				skipSize +=(  sizeof(Vec3));

				break;
			}
			case ICompute::EShaderFieldType::VEC4:
			{
				skipSize +=(  sizeof(Vec4));

				break;
			}
			case ICompute::EShaderFieldType::TEXT:
			{
				size =  field.text.isUsingComputeGeneratedTex ? 256 : 128;
				skipSize +=( sizeof(size));
				if(field.text.isUsingComputeGeneratedTex)
				{
					size =  field.text.Compute == nullptr ? 256 : 128;
					skipSize +=( sizeof(size));
					if(field.text.Compute != nullptr)
					{
						Vec2i TS = field.text.Compute->GetTexSize();
						size = field.text.Compute->name.size();
						skipSize +=( sizeof(size));
						skipSize +=( size * sizeof(std::string::value_type));
						skipSize +=(  sizeof(Vec2i));
						skipSize += field.text.Compute->GetSkipSize();

					}
				}
				else
				{
					size =  field.text.text == nullptr ? 256 : 128;
					skipSize +=( sizeof(size));
					if(field.text.text != nullptr)
					{
						size = field.text.text->name.size();
						skipSize +=( sizeof(size));
						skipSize +=( size * sizeof(std::string::value_type));

					}
				}


				break;
			}
			default:
				break;
		}
	}
	return skipSize;
}
