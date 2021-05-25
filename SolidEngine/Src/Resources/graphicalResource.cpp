//
// Created by ryan1 on 09/03/2021.
//
#include <glad/glad.h>
#include "Resources/resourceType.hpp"
#include "Resources/graphicalResource.hpp"
#include "ECS/Components/light.hpp"

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

        //ANIM
        if(_raw->hadAnim)
        {
            size_t bufferSize = 4*sizeof(GLint) + 4*sizeof(GLfloat);
            glBindBuffer(GL_ARRAY_BUFFER, sub.animVBO);
            glBufferData(GL_ARRAY_BUFFER, _raw->animData.size() * bufferSize, _raw->animData.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(3,4,GL_INT,GL_FALSE, bufferSize, (void*)0);
            glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE, bufferSize, (void*)(4*sizeof(GLint)));
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
        }
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

        //ANIM
        if(_raw->hadAnim)
        {
            size_t bufferSize = 4*sizeof(GLint) + 4*sizeof(GLfloat);
            glBindBuffer(GL_ARRAY_BUFFER, sub.animVBO);
            glBufferData(GL_ARRAY_BUFFER, _raw->animData.size() * bufferSize, _raw->animData.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(3,4,GL_INT,GL_FALSE, bufferSize, (void*)0);
            glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE, bufferSize, (void*)(4*sizeof(GLint)));
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
        }
	}
    glBindVertexArray(0);
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

void GL::Mesh::DrawMesh(const std::vector<MaterialResource *>& _list, Transform& _tr, Camera& _cam)
{
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < Meshes.size(); ++i)
	{
	    uint textID = 0;
		SubMesh& subMesh = Meshes.at(i);
		const MaterialResource* mat = _list.at(i);
        std::shared_ptr<IShader> shader = nullptr;
		if(mat == nullptr)
		{
			mat = Engine::GetInstance()->resourceManager.GetDefaultMat();
			mat->GetDefaultshader()->SetMVP(_tr, _cam);
		}
		else
		{
            shader = mat->GetShader();
            if(shader == nullptr)
				mat->GetDefaultshader()->SetMVP(_tr, _cam);
			else
			{
				for(auto& value : mat->fields)
				{
					switch (value.type)
					{
						case MaterialResource::EShaderFieldType::BOOL:
							shader->SetBool(value.name.c_str(), value.b);
							break;
						case  MaterialResource::EShaderFieldType::INT:
							shader->SetInt(value.name.c_str(), value.i);
							break;
						case  MaterialResource::EShaderFieldType::FLOAT:
							shader->SetFloat(value.name.c_str(), value.f);
							break;
						case  MaterialResource::EShaderFieldType::VEC2:
							shader->SetVec2(value.name.c_str(), value.v2);
							break;
						case  MaterialResource::EShaderFieldType::VEC3:
							shader->SetVec3(value.name.c_str(), value.v3);
							break;
						case  MaterialResource::EShaderFieldType::VEC4:
							shader->SetVec4(value.name.c_str(), value.v4);
							break;
                        case MaterialResource::EShaderFieldType::TEXT:
                            {
                                if(value.text == nullptr)
                                    continue;

                                shader->SetInt(value.name.c_str(), textID);
                                value.text->BindTexture(textID);
                                ++textID;
                                break;
                            }
						default:
							break;
					}
				}

				shader->SetMVP(_tr, _cam);
				shader->SetLights(_cam);
			}
		}

		glBindVertexArray(subMesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,subMesh.EBO);
		glDrawElements(GL_TRIANGLES, subMesh.numOfIndices,GL_UNSIGNED_INT, nullptr);

		if(shader != nullptr)
		{
            for(auto& value : mat->fields)
            {
                if(value.type == MaterialResource::EShaderFieldType::TEXT)
                {
                    if(value.text == nullptr)
                        continue;

                    int TexUnit = 0;
                    shader->GetInt(value.name.c_str(), &TexUnit);
                    value.text->UnBindTexture(TexUnit);
                }
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
    source = _s;
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
    vert = vShader.id;
    frag = fShader.id;
	if (linkStatus == GL_FALSE)
	{
		GLchar infoLog[1024];
		glGetProgramInfoLog(ProgID, ARRAYSIZE(infoLog), nullptr, infoLog);
		printf("Program link error: %s", infoLog);
		//TODO : cleanup at return
	}

	LoadShaderFields();
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

void GL::Shader::ReloadShader()
{
    std::vector<char*> fragS;
    std::vector<char*> vertS;
    fragS.push_back(source->FragSource.data());
    vertS.push_back(source->VertexSource.data());
    glShaderSource(frag,1,fragS.data(), nullptr);
    glShaderSource(vert,1,vertS.data(), nullptr);
    glCompileShader(frag);
    glCompileShader(vert);
    GLint success = 0;
    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(frag, 1024, nullptr, infoLog);
        printf("Shader compilation error: %s", infoLog);
    }
    success = 0;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(vert, 1024, nullptr, infoLog);
        printf("Shader compilation error: %s", infoLog);
    }
    if (success > 0)
    {
        glLinkProgram(ProgID);
        LoadShaderFields();
    }
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
	Mat4<float> modelM =  (_model.GetMatrix()*_model.GetParentMatrix()) ;
	glUniformMatrix4fv(glGetUniformLocation(ProgID,"model"),1,GL_FALSE,modelM.elements.data());

}

void GL::Shader::SetLights(Camera& _camera) const
{
    glUseProgram(ProgID);

    std::vector<Light*> lights = Light::GetLightList();
    int i = 0;
    for(const auto& light : lights)
    {
        std::string id = std::to_string(i);
        Vec3 pos = light->gameObject->transform->GetPosition();
        glUniform3fv(glGetUniformLocation(ProgID,std::string("_lights[" + id + "].pos").c_str()),1,&pos.x);
        glUniform3fv(glGetUniformLocation(ProgID,std::string("_lights[" + id + "].color").c_str()),1,&light->color.x);
        glUniform1f(glGetUniformLocation(ProgID,std::string("_lights[" + id + "].intensity").c_str()),light->intensity);

        ++i;
    }
    glUniform1i(glGetUniformLocation(ProgID,"_nbLights"),lights.size());
    glUniform3fv(glGetUniformLocation(ProgID,"_camPos"),1,&_camera.position.x);
}

void GL::Shader::SetAnim(SkeletonResource::Bone* _root) const
{
    glUseProgram(ProgID);

    _root.
    //for(int)

    glUniformMatrix4fv(glGetUniformLocation(ProgID,"finalBonesMatrices["+  +"]"),1,GL_FALSE,);
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
	GLenum error = glGetError();
    while (error != GL_NO_ERROR)
    {
        std::cout << glGetString(error) << "\n";
        error = glGetError();
    }
}

void GL::Shader::LoadShaderFields()
{
    uniforms.clear();
    int count = -1;
    glGetProgramiv(ProgID, GL_ACTIVE_UNIFORMS,&count);

    for (int i = 0; i < count; ++i)
    {
        ShaderUniform shaderUniform;
        int name_len=-1, num=-1;
        GLenum type = GL_ZERO;
        char name[100];
        glGetActiveUniform(ProgID, GLuint(i), sizeof(name)-1,
                            &name_len, &num, &type, name );
        name[name_len] = 0;
        //GLuint location = glGetUniformLocation(ProgID, name );
        if(name[0] == '_')
            continue;

        shaderUniform.name = name;
        switch (type)
        {
            case GL_BOOL:
                shaderUniform.type = MaterialResource::EShaderFieldType::BOOL;
                break;
            case GL_INT:
                shaderUniform.type = MaterialResource::EShaderFieldType::INT;
                break;
            case GL_FLOAT:
                shaderUniform.type = MaterialResource::EShaderFieldType::FLOAT;
                break;
            case GL_FLOAT_VEC2:
                shaderUniform.type = MaterialResource::EShaderFieldType::VEC2;
                break;
            case GL_FLOAT_VEC3:
                shaderUniform.type = MaterialResource::EShaderFieldType::VEC3;
                break;
            case GL_FLOAT_VEC4:
                shaderUniform.type = MaterialResource::EShaderFieldType::VEC4;
                break;
            case GL_SAMPLER_2D:
                shaderUniform.type = MaterialResource::EShaderFieldType::TEXT;
                break;
            default:
                shaderUniform.type = MaterialResource::EShaderFieldType::NONE;
                break;
        }

        uniforms.push_back(shaderUniform);
    }
}

std::vector<ShaderUniform> &GL::Shader::GetUniformList()
{
    return uniforms;
}

GL::Texture::Texture(ImageResource *_image)
{
	//Log::Send("IMAGE CHAN NUM = " + std::to_string(_image->ChannelsNum));
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

std::string& GL::Shader::GetFragSource()
{
    return source->FragSource;
}

std::string& GL::Shader::GetVertSource()
{
    return source->VertexSource;
}

void GL::Shader::SetFragSource(const std::string& _src)
{
    source->FragSource = _src;
}

void GL::Shader::SetVertSource(const std::string& _src)
{
    source->VertexSource = _src;
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

