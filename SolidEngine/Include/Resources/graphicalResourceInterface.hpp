//
// Created by ryan1 on 09/03/2021.
//

#ifndef SOLIDEDITOR_GRAPHICALRESOURCEINTERFACE_HPP
#define SOLIDEDITOR_GRAPHICALRESOURCEINTERFACE_HPP
#include "Build/solidAPI.hpp"
#include "ECS/Components/transform.hpp"
#include "ECS/Components/camera.hpp"
namespace Solid
{

	class SOLID_API IMesh
	{
	public:
		//public members

	protected:
		//protected members


	public:
		//public func
		IMesh() = default;
		~IMesh() = default;
		virtual void DrawMesh() = 0;

	};

	class SOLID_API IShader
	{
	public:
		//public members
		const EResourceType type;
	protected:
		//protected members


	public:
		//public func
		IShader() = default;
		IShader(EResourceType _type):type(_type){}
		~IShader() = default;
		virtual void SetFloat(const char *_name, float _value) = 0;
		virtual void SetInt(const char *_name, int _value) = 0;
		virtual void SetBool(const char *_name, bool _value) = 0;
		virtual void SetVec3(const char *_name, Vec3 _value) = 0;
		virtual void SetMatrix(const char *_name, Mat4<float> _value) = 0;
		virtual void SetFloatArray(const char *_name, int size, float* _value) = 0;
		virtual void SetIntArray(const char *_name, int size, int* _value) = 0;
		virtual void SetVec3Array(const char *_name, int size, Vec3* _value) = 0;
		virtual void SetMatrixArray(const char *_name, int size, Mat4<float>* _value) = 0;

		virtual void SetMVP(Transform& _model, Camera& _camera) = 0;
		virtual void SetMaterial(const char* _name) = 0;


	};
	class SOLID_API ITexture
	{
	public:
	//public members

	protected:
	//protected members


	public:
	//public func
	    ITexture() = default;
	    ~ITexture() = default;

	};
}
#endif //SOLIDEDITOR_GRAPHICALRESOURCEINTERFACE_HPP