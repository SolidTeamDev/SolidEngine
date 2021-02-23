#include "ECS/Components/camera.hpp"

namespace Solid
{
    Camera::Camera() :
    transform()
    {
        projection = Mat4<float>::Perspective();
        view       = Mat4<float>::Identity;

        fov  = 45.f;
        near = 0.01f;
        far  = 100.f;
    }

    Mat4<float> Camera::GetView()
    {
        return view;
    }

    Mat4<float> Camera::GetProjection()
    {
        return projection;
    }

    void Camera::UpdateFreeFly(const Int2 _spaceScreen)
    {
        projection = Mat4<float>::Perspective(fov,(float)_spaceScreen.x / (float)_spaceScreen.y,near,far);

    }
} //!namespace