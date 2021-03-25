//
// Created by ryan1 on 09/03/2021.
//
#include <glad/glad.h>
#include "Resources/resourceType.hpp"
#include "Resources/graphicalResource.hpp"

#include <editor.hpp>

using namespace Solid;
GL::Mesh::Mesh(MeshResource *_raw):
IMesh(_raw->Meshes.size())
{
    name = _raw->name;


	Meshes.resize(_raw->Meshes.size());
	for (int i = 0; i < _raw->Meshes.size(); ++i) {
		MeshResource::SubMesh& rawSub = _raw->Meshes.at(i);
		GL::Mesh::SubMesh& sub = Meshes.at(i);
		glGenVertexArrays(1, &sub.VAO);
		glGenBuffers(1, &sub.VBO);
		glGenBuffers(1, &sub.EBO);
		glBindVertexArray(sub.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, sub.VBO);
		glBufferData(GL_ARRAY_BUFFER, rawSub.vertices.size() * 8 *sizeof(GLfloat), rawSub.vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);



		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub.EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  rawSub.indices.size()*sizeof(unsigned int), rawSub.indices.data(), GL_STATIC_DRAW);
		sub.numOfIndices = rawSub.indices.size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
	bIsInit = true;
}

void GL::Mesh::Init(MeshResource *_raw)
{
	if(bIsInit)
		return;

    name = _raw->name;


	Meshes.resize(_raw->Meshes.size());
	for (int i = 0; i < _raw->Meshes.size(); ++i)
	{
		MeshResource::SubMesh& rawSub = _raw->Meshes.at(i);
		GL::Mesh::SubMesh& sub = Meshes.at(i);
		glGenVertexArrays(1, &sub.VAO);
		glGenBuffers(1, &sub.VBO);
		glGenBuffers(1, &sub.EBO);
		glBindVertexArray(sub.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, sub.VBO);
		glBufferData(GL_ARRAY_BUFFER, rawSub.vertices.size() * 8 *sizeof(GLfloat), rawSub.vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 *sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);



		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub.EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  rawSub.indices.size()*sizeof(unsigned int), rawSub.indices.data(), GL_STATIC_DRAW);
		sub.numOfIndices = rawSub.indices.size();

	}
	bIsInit = true;
}

void GL::Mesh::DrawMesh()
{
	glEnable(GL_DEPTH_TEST);

	for (auto& subMesh : Meshes)
	{
		glBindVertexArray(subMesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,subMesh.EBO);
		glDrawElements(GL_TRIANGLES, subMesh.numOfIndices,GL_UNSIGNED_INT, nullptr);
	}
	glBindVertexArray(0);
}

void GL::Mesh::DrawMesh(std::vector<MaterialResource *>& _list, Transform& _tr, Camera& _cam)
{
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < Meshes.size(); ++i)
	{
		SubMesh& subMesh = Meshes.at(i);
		const MaterialResource* mat = _list.at(i);

		if(mat == nullptr)
		{
			mat = Engine::GetInstance()->resourceManager.GetDefaultMat();
			mat->defaultshader->SetMVP(_tr, _cam);
		}
		else
		{
			if(mat->shader == nullptr)
				mat->defaultshader->SetMVP(_tr, _cam);
			else
			{
				for(auto& value : mat->ValuesProperties)
				{
					switch (value.second.type)
					{
						case MaterialResource::EFieldType::BOOL:
							mat->shader->SetBool(value.first.c_str(), value.second.b);
							break;
						case  MaterialResource::EFieldType::INT:
							mat->shader->SetInt(value.first.c_str(), value.second.i);
							break;
						case  MaterialResource::EFieldType::FLOAT:
							mat->shader->SetFloat(value.first.c_str(), value.second.f);
							break;
						case  MaterialResource::EFieldType::VEC2:
							mat->shader->SetVec2(value.first.c_str(), value.second.v2);
							break;
						case  MaterialResource::EFieldType::VEC3:
							mat->shader->SetVec3(value.first.c_str(), value.second.v3);
							break;
						case  MaterialResource::EFieldType::VEC4:
							mat->shader->SetVec4(value.first.c_str(), value.second.v4);
							break;
						default:
							break;
					}
				}

				for(auto& value : mat->TexturesProperties)
				{
					if(value.second == nullptr)
						continue;
					int TexUnit = 0;
					mat->shader->GetInt(value.first.c_str(), &TexUnit);
					value.second->BindTexture(TexUnit);
				}
				mat->shader->SetMVP(_tr, _cam);
			}
		}

		glBindVertexArray(subMesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,subMesh.EBO);
		glDrawElements(GL_TRIANGLES, subMesh.numOfIndices,GL_UNSIGNED_INT, nullptr);

		if(mat->shader != nullptr)
		{
			for(auto& value : mat->TexturesProperties)
			{
				if(value.second == nullptr)
					continue;
				int TexUnit = 0;
				mat->shader->GetInt(value.first.c_str(), &TexUnit);
				value.second->UnBindTexture(TexUnit);

			}
		}

	}
	glBindVertexArray(0);
}


#ifndef ARRAYSIZE
#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

GL::Shader::Shader(ShaderResource *_s) :IShader(EResourceType::Shader)
{
    name = _s->name;

    std::vector<char*> tab;
	tab.push_back(_s->VertexSource.data());
	ShaderWrapper vShader = CreateShader(GL_VERTEX_SHADER, 1, tab);
	tab.clear();
	tab.push_back(_s->FragSource.data());
	ShaderWrapper fShader = CreateShader(GL_FRAGMENT_SHADER, 1, tab);

	if(vShader.error || fShader.error)
		return;

	ProgID = glCreateProgram();
	glAttachShader(ProgID, vShader.id);
	glAttachShader(ProgID, fShader.id);
	glLinkProgram(ProgID);
	GLint linkStatus;
	glGetProgramiv(ProgID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE)
	{
		GLchar infoLog[1024];
		glGetProgramInfoLog(ProgID, ARRAYSIZE(infoLog), nullptr, infoLog);
		printf("Program link error: %s", infoLog);
		//TODO : cleanup at return
	}
}

GL::ComputeShader::ComputeShader(ComputeShaderResource *_cs) :Shader(EResourceType::Compute)
{
    name = _cs->name;

    //use binaries if available
	std::vector<char*> tab;
	tab.push_back(_cs->ComputeSource.data());
	ShaderWrapper compute = CreateShader(GL_COMPUTE_SHADER, 1, tab);
	if(compute.error)
		return;
	ProgID = glCreateProgram();
	glAttachShader(ProgID, compute.id);
	glLinkProgram(ProgID);
	GLint linkStatus;
	glGetProgramiv(ProgID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE)
	{

		GLchar infoLog[1024];
		glGetProgramInfoLog(ProgID, ARRAYSIZE(infoLog), nullptr, infoLog);
		printf("Program link error: %s", infoLog);
		//TODO : Cleanup at return
		compute.error = true;
	}


}



//+Type = GL_VERTEX_SHADER / GL_FRAGMENT_SHADER / GL_COMPUTE_SHADER
GL::Shader::ShaderWrapper GL::Shader::CreateShader(GLenum _type, int _sourceCount, std::vector<char *> &_sources)
{
	ShaderWrapper compute;
	compute.id = glCreateShader(_type);

	glShaderSource(compute.id, _sourceCount, _sources.data(), nullptr);
	glCompileShader(compute.id);
	GLint compileStatus;
	glGetShaderiv(compute.id, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(compute.id, 1024, nullptr, infoLog);
		printf("Shader compilation error: %s", infoLog);
		compute.error = true;
	}

	return compute;
}

void GL::Shader::SetFloat(const char *_name, float _value)
{
	glUseProgram(ProgID);
	glUniform1f(glGetUniformLocation(ProgID,_name), _value);
}

void GL::Shader::SetMVP(Transform& _model, Camera& _camera)const
{

	glUseProgram(ProgID);

	glUniformMatrix4fv(glGetUniformLocation(ProgID,"proj"),1,GL_FALSE,_camera.GetProjection().elements.data());
	glUniformMatrix4fv(glGetUniformLocation(ProgID,"view"),1,GL_FALSE,_camera.GetView().elements.data());
	glUniformMatrix4fv(glGetUniformLocation(ProgID,"model"),1,GL_FALSE,_model.GetMatrix().elements.data());

}

void GL::Shader::SetMaterial(const char *_name)
{

}

void GL::Shader::SetInt(const char *_name, int _value)
{
	glUseProgram(ProgID);
	glUniform1i(glGetUniformLocation(ProgID,_name), _value);
}

void GL::Shader::SetBool(const char *_name, bool _value)
{
	glUseProgram(ProgID);
	glUniform1i(glGetUniformLocation(ProgID,_name), _value);
}

void GL::Shader::SetMatrix(const char *_name, Mat4<float> _value)
{
	glUseProgram(ProgID);
	glUniformMatrix4fv(glGetUniformLocation(ProgID,_name),1,GL_FALSE, _value.elements.data());
}

void GL::Shader::SetFloatArray(const char *_name, int size, float *_value)
{
	glUseProgram(ProgID);
	glUniform1fv(glGetUniformLocation(ProgID,_name),size, _value);
}

void GL::Shader::SetIntArray(const char *_name, int size, int *_value)
{
	glUseProgram(ProgID);
	glUniform1iv(glGetUniformLocation(ProgID,_name),size, _value);
}



void GL::Shader::SetVec3Array(const char *_name, int size, Vec3 *_value)
{
	glUseProgram(ProgID);
	glUniform3fv(glGetUniformLocation(ProgID,_name),size, &(_value->x));
}

void GL::Shader::SetMatrixArray(const char *_name, int size, Mat4<float> *_value)
{
	glUseProgram(ProgID);
	glUniformMatrix4fv(glGetUniformLocation(ProgID,_name),size,GL_FALSE, _value->elements.data());
}

void GL::Shader::SetVec3(const char *_name, Vec3 _value)
{
	glUseProgram(ProgID);
	glUniform3f(glGetUniformLocation(ProgID,_name), _value.x,_value.y,_value.z);
}

void GL::Shader::SetVec2(const char *_name, Vec2 _value)
{
	glUseProgram(ProgID);
	glUniform2f(glGetUniformLocation(ProgID,_name), _value.x,_value.y);
}

void GL::Shader::SetVec4(const char *_name, Vec4 _value)
{
	glUseProgram(ProgID);
	glUniform4f(glGetUniformLocation(ProgID,_name), _value.x,_value.y,_value.z, _value.w);
}


void GL::Shader::GetIntArray(const char *_name, int size, int *_value)
{
	glGetnUniformiv(ProgID, glGetUniformLocation(ProgID,_name), size, _value);
}

void GL::Shader::GetInt(const char *_name, int *_value)
{
	int loc = glGetUniformLocation(ProgID,_name);
	if(loc == -1)
		return;
	glGetUniformiv(ProgID, loc, _value);
}

GL::Texture::Texture(ImageResource *_image)
{
	Log::Send("IMAGE CHAN NUM = " + std::to_string(_image->ChannelsNum));
	name = _image->name;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//if(_image->ChannelsNum == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _image->x, _image->y, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image->image.data());
	//else if(_image->ChannelsNum == 3)
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _image->x, _image->y, 0, GL_RGB, GL_UNSIGNED_BYTE, _image->image.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GL::Texture::BindTexture(uint _texUnit)
{
	glActiveTexture(GL_TEXTURE0+ _texUnit);
	glBindTexture(GL_TEXTURE_2D, texId);
}

void GL::Texture::UnBindTexture(uint _texUnit)
{
	glActiveTexture(GL_TEXTURE0+ _texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

