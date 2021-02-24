#include "ECS/Components/camera.hpp"

namespace Solid
{
    Camera::Camera() :
    transform()
    {
        projection = Mat4<>::Perspective();
        view = Mat4<>::Identity;

        fov  = 45.f;
        near = 0.01f;
        far  = 100.f;
    }

    Mat4<float> Camera::GetView()
    {
        return view;
    }

    Mat4<float> Camera::GetProjection() const
    {
        return projection;
    }

    void Camera::UpdateCamera(const Int2 _spaceScreen)
    {
        view       = transform.GetMatrix();
        projection = Mat4<>::Perspective(fov,(float)_spaceScreen.x / (float)_spaceScreen.y,near,far);
    }

    void Camera::UpdateFreeFly()
    {

    }
} //!namespace