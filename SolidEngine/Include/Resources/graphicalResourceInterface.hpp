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



	protected:
		ComputeShaderResource* source = nullptr;
		bool isInit = false;

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
