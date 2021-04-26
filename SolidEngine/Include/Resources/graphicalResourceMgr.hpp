//
// Created by ryan1 on 09/03/2021.
//

#ifndef SOLIDEDITOR_GRAPHICALRESOURCEMGR_HPP
#define SOLIDEDITOR_GRAPHICALRESOURCEMGR_HPP
#include "resourceMgr.hpp"
#include "graphicalResource.hpp"
#include <unordered_map>
namespace Solid
{


	class SOLID_API GraphicalResourceMgr
	{
	public:
		//public members
		typedef std::unordered_map<std::string, std::shared_ptr<IMesh>>   MeshMap;
		typedef std::unordered_map<std::string, std::shared_ptr<IShader>>   ShaderMap;

	protected:
		//protected members
		std::unordered_map<std::string, std::shared_ptr<IMesh>> Meshes;
		std::unordered_map<std::string, std::shared_ptr<IShader>> Shaders;
		std::unordered_map<std::string, std::shared_ptr<ITexture>> textures;
		ResourceManager* resourceMgr = nullptr;
		Renderer* renderer = nullptr;
		std::shared_ptr<const IShader> defaultShader = nullptr;
		bool defaultShaderInit = false;
	public:
		//public func
		GraphicalResourceMgr() = default;

		~GraphicalResourceMgr() = default;

		std::shared_ptr<IMesh> StoreMesh(MeshResource* _meshResource)
		{
			switch (renderer->GetRenderType())
			{
				case ERendererType::OpenGl45:
				{
					auto it =Meshes.emplace(_meshResource->name, std::make_shared<GL::Mesh>());
					return it.first->second;
					break;
				}
				default:
					return nullptr;
					break;
			}
		}
		std::shared_ptr<IShader> StoreShader(ShaderResource* _shader)
		{
			switch (renderer->GetRenderType())
			{
				case ERendererType::OpenGl45:
				{
					auto it = Shaders.emplace(_shader->name, std::make_shared<GL::Shader>());
					return it.first->second;
					break;
				}
				default:
					return nullptr;
					break;
			}
		}
		std::shared_ptr<IShader> StoreShader(ComputeShaderResource* _shader)
		{
			switch (renderer->GetRenderType())
			{
				case ERendererType::OpenGl45:
				{
					auto it =Shaders.emplace(_shader->name,std::make_shared<GL::ComputeShader>());
					return it.first->second;
					break;
				}
				default:
					return nullptr;
					break;
			}
		}
		std::shared_ptr<ITexture> StoreTexture(ImageResource* _image)
		{
			switch (renderer->GetRenderType())
			{
				case ERendererType::OpenGl45:
				{
					auto it =textures.emplace(_image->name,std::make_shared<GL::Texture>());
					return it.first->second;
					break;
				}
				default:
					return nullptr;
					break;
			}
		}
		MeshMap GetMeshes()
		{
			return Meshes;
		}
		std::shared_ptr<IMesh> GetMesh(const char* _name)
		{
			auto it = Meshes.find(_name);
			if(it == Meshes.end())
			{
				MeshResource* mesh = resourceMgr->GetRawMeshByName(_name);
				if(mesh == nullptr)
					return nullptr;
				return Meshes.emplace(mesh->name, std::make_shared<GL::Mesh>(mesh)).first->second;
			}
			return it->second;
		}
		std::shared_ptr<ITexture> GetTexture(const char* _name)
		{
			auto it = textures.find(_name);
			if(it == textures.end())
			{
				ImageResource* image = resourceMgr->GetRawImageByName(_name);
				if(image == nullptr)
					return nullptr;
				return textures.emplace(image->name, std::make_shared<GL::Texture>(image)).first->second;
			}
			return it->second;
		}
		std::shared_ptr<IShader> GetShader(const char* _name)
		{
			auto it = Shaders.find(_name);
			if(it == Shaders.end())
			{
				ShaderResource* s = resourceMgr->GetRawShaderByName(_name);
				ComputeShaderResource* cs = nullptr;
				if(s == nullptr)
				{
					cs = resourceMgr->GetRawComputeByName(_name);
					if(cs== nullptr)
						return nullptr;
					return Shaders.emplace(cs->name, std::make_shared<GL::ComputeShader>(cs)).first->second;
				}
				return Shaders.emplace(s->name, std::make_shared<GL::Shader>(s)).first->second;
			}
			return it->second;
		}
		std::shared_ptr<const IShader> GetDefaultShader()
		{
			if(defaultShaderInit)
				return defaultShader;
			ShaderResource* s=resourceMgr->GetRawShaderByName("DefaultShader");
			defaultShader = std::make_shared<const GL::Shader>(s);
			defaultShaderInit = true;
			return defaultShader;
		}
		ShaderMap GetShaders()
		{
			return Shaders;
		}
		void Init(ResourceManager* _mgr, Renderer* _renderer) {resourceMgr= _mgr; renderer = _renderer;}

	};
}
#endif //SOLIDEDITOR_GRAPHICALRESOURCEMGR_HPP
