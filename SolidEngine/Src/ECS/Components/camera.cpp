#include "ECS/Components/camera.hpp"

#include "Core/engine.hpp"

namespace Solid
{
    Camera::Camera()
    {
        projection = Mat4<>::Perspective();
        view = Mat4<>::Identity;

        fov  = 45.f;
        _near = 0.01f;
        _far  = 100.f;
    }

    void Camera::Release()
    {
        Components::Release();
        if(isActiveCamera)
            Engine::GetInstance()->activeCamera = nullptr;

        isActiveCamera = false;
    }

    Mat4<float> Camera::GetView() const
    {
        return view;
    }

    Mat4<float> Camera::GetProjection() const
    {
        return projection;
    }

    void Camera::UpdateCamera(const Vec2i _spaceScreen)
    {
        view       = lookAt(position, position + Front, Up);
        projection = Mat4<>::Perspective(fov,(float)_spaceScreen.x / (float)_spaceScreen.y,_near,_far);
    }

     Mat4<float> Camera::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
     {

         //z _front
         Vec3 f = center - eye;
         f.Normalize();


         //x _front
         Vec3 s = Vec3::Cross(f, up);
         s.Normalize();

         //y _front
         Vec3 u = Vec3::Cross(s, f);
         u.Normalize();

         Mat4<float> mat;

         mat.At(0,0) = s.x;
         mat.At(1,0) = s.y;
         mat.At(2,0)= s.z;

         mat.At(0, 1) = u.x;
         mat.At(1, 1) = u.y;
         mat.At(2, 1) = u.z;

         mat.At(0, 2) = -f.x;
         mat.At(1, 2) = -f.y;
         mat.At(2, 2) = -f.z;


         mat.At(3, 0) = -Vec3::Dot(s, eye);
         mat.At(3, 1) = -Vec3::Dot(u, eye);
         mat.At(3, 2) = Vec3::Dot(f, eye);
         mat.At(3, 3) = 1;

         return mat;
     }

    void Camera::SetActiveCamera()
    {
        Engine::GetInstance()->activeCamera = this;
        isActiveCamera = true;
    }
} //!namespace