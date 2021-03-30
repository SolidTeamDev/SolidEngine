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

    public:
        Transform   transform;
        float fov;
        float near;
        float far;

        Camera();

        Mat4<float> GetView();
        Mat4<float> GetProjection() const;

        void UpdateCamera(const Vec2i _spaceScreen);
    };
} //!namespace
