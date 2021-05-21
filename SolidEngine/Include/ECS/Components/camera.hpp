//TODO: Remove field only for camera editor
#pragma once


#include "Core/Maths/Vector/vector2.hpp"
#include "Core/Maths/Matrix/matrix4.hpp"
#include "Core/Maths/Quaternion/quaternion.hpp"
#include "transform.hpp"

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/camera.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() Camera : public Components
    {
    private:
        Mat4<float> projection;
        Mat4<float> view;

        bool isActiveCamera = false;
    public:
	    SLDField()
	    float fov;
	    SLDField()
	    float _near;
	    SLDField()
	    float _far;
	    SLDField()
 		Vec3 Right = Vec3::Zero;
        SLDField()
        Vec3 Front = Vec3::Back;
        SLDField()
        Vec3 Up =  Vec3::Up;
        SLDField()
        float MouseSensitivity = 50.f;
        SLDField()
        Vec3 position = {0,0,0};
        SLDField()
        Vec3 Euler = Vec3::Zero;
        SLDField()
        bool MouseInCenterScreen = false;
        Camera();

        void Release() override;

        Mat4<float> GetView() const;
        Mat4<float> GetProjection() const;

        void UpdateCamera(const Vec2i _spaceScreen);
        Mat4<float> lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

        void SetActiveCamera();

        Camera_GENERATED
    };
} //!namespace

File_GENERATED
