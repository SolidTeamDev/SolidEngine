//
// Created by ryan1 on 09/03/2021.
//

#ifndef SOLIDEDITOR_GRAPHICALRESOURCE_HPP
#define SOLIDEDITOR_GRAPHICALRESOURCE_HPP

#include "graphicalResourceInterface.hpp"

namespace Solid
{
	namespace GL
	{
		class SOLID_API Mesh : public IMesh
		{
		public:
			//public members

			bool bIsInit = false;
			class SubMesh
			{
			public:
				uint VAO;
				uint VBO;
				uint EBO;
				std::size_t numOfIndices;
			};

			std::vector<SubMesh> Meshes;
		protected:
			//protected members


		public:
			//public func
			Mesh() = default;
			Mesh(MeshResource* _raw);
			void Init(MeshResource* _raw);
			~Mesh() = default;

			virtual void DrawMesh() override;

			virtual void DrawMesh(const std::vector<MaterialResource *>& _list, Transform &_tr, Camera &_cam) override;

		};
		class SOLID_API Shader : public IShader
		{
		public:
		//public members

			uint ProgID;
		protected:
		//protected members
			struct ShaderWrapper{uint id; bool error = false;};

		public:
		//public func

			Shader():IShader(EResourceType::Shader){}
			Shader(ShaderResource* _s);
			Shader(EResourceType _type):IShader(_type){}
			ShaderWrapper CreateShader(uint _type, int _sourceCount, std::vector<char*>& _sources);
			virtual void SetFloat(const char *_name, float _value) override;

			virtual void SetMVP(Transform& _model, Camera& _camera) const override;

			virtual void SetMaterial(const char *_name) override;

			virtual void SetVec2(const char *_name, Vec2 _value) override;

			virtual void SetVec3(const char *_name, Vec3 _value) override;

			virtual void SetVec4(const char *_name, Vec4 _value) override;

			virtual void SetInt(const char *_name, int _value) override;

			virtual void SetBool(const char *_name, bool _value) override;

			virtual void SetMatrix(const char *_name, Mat4<float> _value) override;

			virtual void SetFloatArray(const char *_name, int size, float *_value) override;

			virtual void SetIntArray(const char *_name, int size, int *_value) override;

			virtual void SetVec3Array(const char *_name, int size, Vec3 *_value) override;

			virtual void SetMatrixArray(const char *_name, int size, Mat4<float> *_value) override;

			virtual void GetIntArray(const char *_name, int size, int *_value) override;

			virtual void GetInt(const char *_name, int *_value) override;


			~Shader() = default;

		};
		class SOLID_API ComputeShader : public Shader
		{
		public:
		//public members

		protected:
		//protected members


		public:
		//public func
		    ComputeShader():Shader(EResourceType::Compute){}
			ComputeShader(ComputeShaderResource* _cs);

			~ComputeShader() = default;

		};

		class Texture : public ITexture
		{
		public:
		//public members
			uint texId = 0;
		protected:
		//protected members


		public:
		//public func
		    Texture() = default;
			Texture(ImageResource* _image);
		    ~Texture() = default;

			virtual void BindTexture(uint _texUnit) override;

			virtual void UnBindTexture(uint _texUnit) override;
		};

	}

	//namespace VK {}
	//namespace DX11 {}
	//namespace DX12 {}


}
#endif //SOLIDEDITOR_GRAPHICALRESOURCE_HPP
