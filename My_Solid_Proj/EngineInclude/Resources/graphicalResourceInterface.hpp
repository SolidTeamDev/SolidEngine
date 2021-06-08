#pragma once

#include "Build/solidAPI.hpp"
#include "ECS/Components/transform.hpp"
#include "ECS/Components/camera.hpp"
#include "Resources/resourceType.hpp"
#include "ECS/Components/animation.hpp"

namespace Solid
{

	class SOLID_API IMesh
	{
	public:
		//public members
        std::string name;
		const unsigned int subMeshCount = 0;
	protected:
		//protected members


	public:
		//public func
		IMesh() = default;
		IMesh(uint _count):subMeshCount(_count){}
		~IMesh() = default;
		virtual void DrawMesh() = 0;
		virtual void DrawMesh(const std::vector<MaterialResource *>& _list, Transform& _tr, Camera& _cam,Animation* _anim = nullptr ) = 0;
	};

    struct ShaderUniform
    {
        std::string name;
        MaterialResource::EShaderFieldType type;
    };

	class SOLID_API IShader
	{
	public:

		//public members
		std::string name;
		const EResourceType type;

	protected:
		//protected members
        std::vector<ShaderUniform> uniforms;
		ShaderResource* source = nullptr;

	public:
		//public func
		IShader() = default;
		IShader(EResourceType _type):type(_type){}
		~IShader() = default;

        virtual void ReloadShader() = 0;
		virtual void BindShader() = 0;
		virtual void UnbindShader() = 0;

        virtual void SetFloat(const char *_name, float _value) = 0;
		virtual void SetInt(const char *_name, int _value) = 0;
		virtual void SetBool(const char *_name, bool _value) = 0;
		virtual void SetVec2(const char *_name, Vec2 _value) = 0;
		virtual void SetVec3(const char *_name, Vec3 _value) = 0;
		virtual void SetVec4(const char *_name, Vec4 _value) = 0;
		virtual void SetMatrix(const char *_name, Mat4<float> _value) = 0;
		virtual void SetFloatArray(const char *_name, int size, float* _value) = 0;
		virtual void SetIntArray(const char *_name, int size, int* _value) = 0;
		virtual void SetVec3Array(const char *_name, int size, Vec3* _value) = 0;
		virtual void SetMatrixArray(const char *_name, int size, Mat4<float>* _value) = 0;
		virtual void GetIntArray(const char *_name, int size, int* _value) = 0;
		virtual void GetInt(const char *_name, int* _value) = 0;

		virtual void LoadShaderFields() = 0;

		virtual std::vector<ShaderUniform>& GetUniformList() = 0;

		virtual void SetMVP(Transform& _model, Camera& _camera) const = 0;
		virtual void SetLights(Camera& _camera) const = 0;
		virtual void SetAnim(Animation* _anim) const = 0;
		virtual void SetMaterial(const char* _name) = 0;

        virtual std::string& GetFragSource() = 0;
        virtual std::string& GetVertSource() = 0;
        virtual void SetFragSource(const std::string& _src) = 0;
        virtual void SetVertSource(const std::string& _src) = 0;
	};

	class SOLID_API ICompute : public IShader
	{
	public:
		enum class EShaderFieldType : int
		{
			BOOL,
			INT,
			FLOAT,
			VEC2,
			VEC3,
			VEC4,
			TEXT,
			NONE
		};
		class SOLID_API ShaderField
		{
		public:
			std::string name;
			EShaderFieldType type;
			union
			{
				bool   b;
				int    i;
				float  f;
				Vec2   v2;
				Vec3   v3;
				Vec4   v4;

			};
			MatText text;
			ShaderField();

			ShaderField(EShaderFieldType _type);
			ShaderField(const struct ShaderUniform& _uniform);
			ShaderField(const ShaderField& _copy);
			~ShaderField(){};
			ShaderField& operator=(const ShaderField& _copy);
		};

		std::vector<ShaderField> ComputeFields;
	protected:
		ComputeShaderResource* source = nullptr;
		bool isInit = false;
		bool Dispatched = false;

	public:
		//public func
		ICompute() = default;
		ICompute(EResourceType _type):IShader(_type){}
		~ICompute() = default;

		virtual void InitTex(Vec2i size) = 0;
		virtual Vec2i GetTexSize() = 0;
		virtual void SetComputeSource(const std::string& _src) = 0;
		virtual std::string& GetComputeSource() = 0;
		virtual uint Dispatch() = 0;
		void SaveFields(std::vector<char> &buffer);
		size_t GetSkipSize();
		void LoadFields(char *buffer, size_t bSize, uint64_t &readPos);
		void ReloadFields()
		{
			std::vector<ShaderField> temp = ComputeFields;
			ComputeFields.clear();

			auto uniforms = GetUniformList();

			for(const auto& uniform: uniforms)
				ComputeFields.emplace_back(uniform);
			for (int i = 0; i < ComputeFields.size(); ++i)
			{
				for (int j = 0; j < temp.size(); ++j)
				{
					if(ComputeFields[i].name == temp[j].name && ComputeFields[i].type == temp[j].type)
					{
						switch (ComputeFields[i].type)
						{
							case EShaderFieldType::BOOL:
								ComputeFields[i].b = temp[j].b;
								break;
							case EShaderFieldType::INT:
								ComputeFields[i].i = temp[j].i;
								break;
							case EShaderFieldType::FLOAT:
								ComputeFields[i].f = temp[j].f;
								break;
							case EShaderFieldType::VEC2:
								ComputeFields[i].v2 = temp[j].v2;
								break;
							case EShaderFieldType::VEC3:
								ComputeFields[i].v3 = temp[j].v3;
								break;
							case EShaderFieldType::VEC4:
								ComputeFields[i].v4 = temp[j].v4;
								break;
							case EShaderFieldType::TEXT:
								ComputeFields[i].text = temp[j].text;
								break;
							default:
								break;

						}
					}
				}
			}
		}

		bool WasDispatched()
		{
			return Dispatched;
		}
		void ResetDispatchState()
		{
			Dispatched = false;
		}
		virtual uint ForceDispatch() = 0;
	};



	class SOLID_API ITexture
	{
	public:
	//public members
		std::string name;
	protected:
	//protected members


	public:
	//public func
	    ITexture() = default;
	    ~ITexture() = default;
	    virtual void BindTexture(uint _texUnit) = 0;
	    virtual void UnBindTexture(uint _texUnit) = 0;
		virtual void BindImageTexture(uint _texUnit) = 0;
		virtual void UnBindImageTexture(uint _texUnit) = 0;
	};

	class SOLID_API ICubemap
    {
    public:
        //public members
        std::string name;
    protected:
        //protected members


    public:
        //public func
        ICubemap() = default;
        ~ICubemap() = default;
        virtual void BindCubemap() = 0;
        virtual void UnBindCubemap() = 0;
    };
}
