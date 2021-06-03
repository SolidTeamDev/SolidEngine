#pragma once

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
        typedef std::unordered_map<std::string, std::shared_ptr<ICubemap>>   CubemapMap;
	protected:
		//protected members
		std::unordered_map<std::string, std::shared_ptr<IMesh>> Meshes;
		std::unordered_map<std::string, std::shared_ptr<IShader>> Shaders;
		std::unordered_map<std::string, std::shared_ptr<ICompute>> Computes;
        std::unordered_map<std::string, std::shared_ptr<ITexture>> textures;
        std::unordered_map<std::string, std::shared_ptr<ICubemap>> cubemaps;
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
		std::shared_ptr<ICompute> StoreShader(ComputeShaderResource* _shader)
		{
			switch (renderer->GetRenderType())
			{
				case ERendererType::OpenGl45:
				{
					auto it =Computes.emplace(_shader->name,std::make_shared<GL::ComputeShader>());
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
        std::shared_ptr<ICubemap> StoreCubemap(CubemapResource* _cubemap)
        {
            switch (renderer->GetRenderType())
            {
                case ERendererType::OpenGl45:
                {
                    auto it =cubemaps.emplace(_cubemap->name,std::make_shared<GL::Cubemap>());
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
        std::shared_ptr<ICubemap> GetCubemap(const char* _name)
        {
            auto it = cubemaps.find(_name);
            if(it == cubemaps.end())
            {
                CubemapResource* map = resourceMgr->GetRawCubemapByName(_name);
                if(map == nullptr)
                    return nullptr;
                return cubemaps.emplace(map->name, std::make_shared<GL::Cubemap>(map)).first->second;
            }
            return it->second;
        }
		std::shared_ptr<IShader> GetShader(const char* _name)
		{
			auto it = Shaders.find(_name);
			if(it == Shaders.end())
			{
				ShaderResource* s = resourceMgr->GetRawShaderByName(_name);
				if(s == nullptr)
				{
					return nullptr;
				}
				return Shaders.emplace(s->name, std::make_shared<GL::Shader>(s)).first->second;
			}
			return it->second;
		}
		std::shared_ptr<ICompute> GetCompute(const char* _name)
		{
			auto it = Computes.find(_name);
			if(it == Computes.end())
			{
				ComputeShaderResource* cs = resourceMgr->GetRawComputeByName(_name);
				if(cs == nullptr)
				{
					return nullptr;
				}
				return Computes.emplace(cs->name, std::make_shared<GL::ComputeShader>(cs)).first->second;
			}
			return it->second;
		}
		std::shared_ptr<const IShader> GetDefaultShader()
		{
			if(defaultShaderInit)
				return defaultShader;
			ShaderResource* s=resourceMgr->GetRawShaderByName("DefaultShader");
			if(s != nullptr)
			{
				defaultShader = std::make_shared<const GL::Shader>(s);
				defaultShaderInit = true;
			}

			return defaultShader;
		}
		ShaderMap GetShaders()
		{
			return Shaders;
		}
		void Init(ResourceManager* _mgr, Renderer* _renderer) {resourceMgr= _mgr; renderer = _renderer;}

	};
}
