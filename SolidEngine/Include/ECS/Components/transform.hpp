#pragma once

#include "Build/solidAPI.hpp"

#include "Core/Maths/Quaternion/quaternion.hpp"
#include "Core/Maths/Vector/vector3.hpp"
#include "Core/Maths/Matrix/matrix4.hpp"

namespace Solid
{
    class SOLID_API Transform
    {
    private:
        Vec3 position {0,0,0};
        Vec3 scale {1,1,1};
        Quat rotation {};
        Vec3 euler{};
        Mat4<float> transMat;

        bool hasToUpdateMat = true;

        void UpdateTransformMatrix();

    public:
        Transform();
        Transform(const Vec3& _position, const Quat& _rotation, const Vec3& _scale);

        void SetPosition(const Vec3& _vec);
        void SetRotation(const Quat& _quat);
        void SetScale(const Vec3& _vec);
        void SetEuler(const Vec3& _euler);
        void Translate(const Vec3& _vec);
        void Rotate(const Quat& _quat);
        void Scale(const Vec3& _vec);

        Vec3 GetPosition() const;
        Vec3 GetEuler()const;
        Quat GetRotation() const;
        Vec3 GetScale() const;

        Mat4<float> GetMatrix();
    };
} //!namespace

