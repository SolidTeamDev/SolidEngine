#pragma once

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
				uint animVBO;
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

			virtual void DrawMesh(const std::vector<MaterialResource *>& _list, Transform &_tr, Camera &_cam, Animation* _anim = nullptr) override;

		};
		class SOLID_API Shader : public IShader
		{
		public:
		//public members

			uint ProgID;
			struct ShaderWrapper{uint id; bool error = false;};
		protected:
			//protected members
			uint vert;
            uint frag;

            int LastShader = 0;
		public:
		//public func

			Shader():IShader(EResourceType::Shader){}
			Shader(ShaderResource* _s);
			Shader(EResourceType _type):IShader(_type){}
			ShaderWrapper CreateShader(uint _type, int _sourceCount, std::vector<char*>& _sources);



			void BindShader()override;
			void UnbindShader() override;
			void ReloadShader() override;

			void SetFloat(const char *_name, float _value) override;

			void SetMVP(Transform& _model, Camera& _camera) const override;

			void SetLights(Camera& _camera) const override;

            void SetAnim(Animation* _anim) const override;

            void SetMaterial(const char *_name) override;

			void SetVec2(const char *_name, Vec2 _value) override;

			void SetVec3(const char *_name, Vec3 _value) override;

			void SetVec4(const char *_name, Vec4 _value) override;

			void SetInt(const char *_name, int _value) override;

			void SetBool(const char *_name, bool _value) override;

			void SetMatrix(const char *_name, Mat4<float> _value) override;

			void SetFloatArray(const char *_name, int size, float *_value) override;

			void SetIntArray(const char *_name, int size, int *_value) override;

			void SetVec3Array(const char *_name, int size, Vec3 *_value) override;

			void SetMatrixArray(const char *_name, int size, Mat4<float> *_value) override;

			void GetIntArray(const char *_name, int size, int *_value) override;

			void GetInt(const char *_name, int *_value) override;

            void LoadShaderFields() override;

            std::vector<ShaderUniform>& GetUniformList() override;

            std::string& GetFragSource() override;
            std::string& GetVertSource() override;
            void SetFragSource(const std::string& _src) override;
            void SetVertSource(const std::string& _src) override;


            ~Shader() = default;

		};
		class SOLID_API ComputeShader : public ICompute
		{
		public:
		//public members
		Shader shader;
		protected:
			//protected members
			Vec2i TexSize = {128,128};
			uint OutTexId;
			uint compute;

		public:
		//public func
		    ComputeShader(): ICompute(EResourceType::Compute){}
			 void ReloadShader() override;

			 void BindShader() override;

			 void UnbindShader() override;

			 void SetFloat(const char *_name, float _value) override;

			 void SetInt(const char *_name, int _value) override;

			 void SetBool(const char *_name, bool _value) override;

			 void SetVec2(const char *_name, Vec2 _value) override;

			 void SetVec3(const char *_name, Vec3 _value) override;

			 void SetVec4(const char *_name, Vec4 _value) override;

			 void SetMatrix(const char *_name, Mat4<float> _value) override;

			 void SetFloatArray(const char *_name, int size, float *_value) override;

			 void SetIntArray(const char *_name, int size, int *_value) override;

			 void SetVec3Array(const char *_name, int size, Vec3 *_value) override;

			 void SetMatrixArray(const char *_name, int size, Mat4<float> *_value) override;

			 void GetIntArray(const char *_name, int size, int *_value) override;

			 void GetInt(const char *_name, int *_value) override;

			 void LoadShaderFields() override;

			 std::vector<ShaderUniform> &GetUniformList() override;

			 void SetMVP(Transform &_model, Camera &_camera) const override;

			 void SetLights(Camera &_camera) const override;

			 void SetAnim(Animation *_anim) const override;

			 void SetMaterial(const char *_name) override;

			 std::string &GetFragSource() override;

			 std::string &GetVertSource() override;

			 void SetFragSource(const std::string &_src) override;

			 void SetVertSource(const std::string &_src) override;

			ComputeShader(ComputeShaderResource* _cs);
			void InitTex(Vec2i size) override;
			Vec2i GetTexSize() override;
			uint Dispatch()override;

			virtual void SetComputeSource(const std::string &_src) override;

			virtual std::string &GetComputeSource() override;



			~ComputeShader() = default;

			virtual uint ForceDispatch() override;

		};

		class SOLID_API Texture : public ITexture
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

			virtual void BindImageTexture(uint _texUnit) override;

			virtual void UnBindImageTexture(uint _texUnit) override;

			virtual void UnBindTexture(uint _texUnit) override;
		};

		class SOLID_API Cubemap : public ICubemap
		{
		public:
			//public members
			uint cubemapID = 0;
		protected:
			//protected members


		public:
			//public func
			Cubemap() = default;
			Cubemap(CubemapResource* _cubemap);
			~Cubemap() = default;

			virtual void BindCubemap() override;
			virtual void UnBindCubemap() override;
		};

	}

	//namespace VK {}
	//namespace DX11 {}
	//namespace DX12 {}


}
