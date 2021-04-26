#include "ECS/Components/camera.hpp"
#include "editor.hpp"

#include "Inputs/editorInputs.hpp"
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

    void Camera::UpdateCamera(const Vec2i _spaceScreen)
    {

        transform.SetTransformMatrix(lookAt(position, position + Front, Up).GetInversed());
        view       = transform.GetMatrix();
        projection = Mat4<>::Perspective(fov,(float)_spaceScreen.x / (float)_spaceScreen.y,near,far);
    }

    void Camera::MoveCamera(EDIRECTION dir, float cameraSpeed)
    {
        if (dir == EDIRECTION::LEFT)
            position -= Right * cameraSpeed;

        if (dir == EDIRECTION::RIGHT)
            position += Right * cameraSpeed;

        if (dir == EDIRECTION::FOWARD)
            position += cameraSpeed * Front;

        if (dir == EDIRECTION::BACK)
            position -= cameraSpeed * Front;

        if(dir == EDIRECTION::UP)
            position -= cameraSpeed * Vec3::Up;

        if(dir == EDIRECTION::DOWN)
            position += cameraSpeed * Vec3::Up;
    }
    void Camera::RotateCamera(float xpos, float ypos)
    {

        if(MouseInCenterScreen)
        {
            lastMousePos.x = xpos;
            lastMousePos.y = ypos;
            MouseInCenterScreen = true;
        }

        float xoffset = xpos ;
        float yoffset = ypos ;


        lastMousePos.x = xpos;
        lastMousePos.y = ypos;

        float sensitivity = 10.f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;



        Euler.x   += xoffset;
        Euler.y += yoffset;

        if(Euler.y > 89.0f)
            Euler.y = 89.0f;
        if(Euler.y < -89.0f)
            Euler.y = -89.0f;

        Vec3 _front;
        _front.z = Maths::Cos(Maths::DegToRad(Euler.x)) * Maths::Cos(Maths::DegToRad(Euler.y));
        _front.y = Maths::Sin(Maths::DegToRad(Euler.y));
        _front.x = Maths::Sin(Maths::DegToRad(Euler.x)) * Maths::Cos(Maths::DegToRad(Euler.y));
        Front = _front.GetNormalized();
        Right = Vec3::Cross(Front, Vec3::Up).GetNormalized();
        Up = Vec3::Cross(Right,Front).GetNormalized();

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
} //!namespace