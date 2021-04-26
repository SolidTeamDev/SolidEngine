#pragma once

#include "Core/Maths/Vector/vector2.hpp"
#include "Core/Maths/Matrix/matrix4.hpp"
#include "Core/Maths/Quaternion/quaternion.hpp"
#include "transform.hpp"
#include "EngineGenerated/camera.sld.hpp"

namespace Solid
{


    class SOLID_API Camera
    {
    private:
        Mat4<float> projection;
        Mat4<float> view;



        Vec3 lastMousePos = Vec3::Zero;
        Vec3 Right = Vec3::Zero;
        Vec3 Front = Vec3::Back;
        Vec3 Up =  Vec3::Up;

    public:
        Transform transform;
        float fov;
        float near;
        float far;
        float MouseSensitivity = 1;
        Vec3 position = {0,0,3};
        Vec3 Euler = Vec3::Zero;
        bool MouseInCenterScreen = false;

        Camera();

        Mat4<float> GetView();

        Mat4<float> GetProjection() const;

        void UpdateCamera(const Vec2i _spaceScreen);

        Mat4<float> lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);

        void RotateCamera(float xoffset, float yoffset);


        enum class EDIRECTION
        {
            UP,
            DOWN,
            LEFT,
            RIGHT,
            FOWARD,
            BACK
        };

        void MoveCamera(EDIRECTION dir, float cameraSpeed);
    };
} //!namespace
