#include "matrix4.hpp"
#include "Core/Maths/Utils/numerics.hpp"

namespace Solid
{
    template <typename T>
    Mat4::Mat4(T e0 , T e1 , T e2  , T e3,
         T e4 , T e5 , T e6  , T e7,
         T e8 , T e9 , T e10 , T e11
         T e12, T e13, T e14 , T e15)
         {
            elements[0] = e0;
            elements[1] = e1;
            elements[2] = e2;
            elements[3] = e3;

            elements[4] = e4;
            elements[5] = e5;
            elements[6] = e6;
            elements[7] = e7;

            elements[8] = e8;
            elements[9] = e9;
            elements[10] = e10;
            elements[11] = e11;

            elements[12] = e12;
            elements[13] = e13;
            elements[14] = e14;
            elements[15] = e15;

         }

    template <typename T>
    Mat4::Mat4(std::array<T, 16> _elements)
    {
        elements = _elements;
    }

#pragma region Static Methods

    Mat4 Mat4::Zero = {0,0,0,0,
                       0,0,0,0,
                       0,0,0,0,
                       0,0,0,0};

    Mat4 Mat4::Identity = {1,0,0,0,
                           0,1,0,0,
                           0,0,1,0,
                           0,0,0,1};

    Mat4 Mat4::Translate(const Vec3& _pos)
    {

    }
    Mat4 Mat4::Rotation(const Quat& _rot)
    {

    }

    Mat4 Mat4::Scale(const Vec3& _scale)
    {

    }

    Mat4 Mat4::Transform(const Vec3& _pos,const Quat& _rot, const Vec3& _scale)
    {

    }

    Mat4 Mat4::Perspective(T _fov = T(90.f), T _aspect, T  _near, T _far))
    {

    }

#pragma endregion

}