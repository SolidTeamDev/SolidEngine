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

void GL::Mesh::DrawMesh(std::vector<MaterialResource *> _list, Transform& _tr, Camera& _cam)
{
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < Meshes.size(); ++i)
	{
		SubMesh& subMesh = Meshes.at(i);
		const MaterialResource* mat = _list.at(i);

		if(mat == nullptr)
		{
			mat = Engine::GetInstance()->resourceManager->GetDefaultMat();
			mat->defaultshader->SetMVP(_tr, _cam);
		}
		else
		{
			mat->shader->SetMVP(_tr, _cam);
		}

		glBindVertexArray(subMesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,subMesh.EBO);
		glDrawElements(GL_TRIANGLES, subMesh.numOfIndices,GL_UNSIGNED_INT, nullptr);
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



