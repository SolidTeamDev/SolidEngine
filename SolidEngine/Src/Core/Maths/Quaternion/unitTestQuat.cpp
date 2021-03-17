#include <iostream>
#include <Core/Maths/Utils/numerics.hpp>
#include <Core/Maths/unitTestMaths.hpp>
#include <Core/Maths/Quaternion/quaternion.hpp>
#include <Core/Maths/Vector/vector3.hpp>
#define SA_UTH_DFLT_CSL_LOG 1
#include <SA-UnitTestHelper.hpp>

namespace Solid::QuatUnitTest
{
    Quat GenerateRandomQuat()
    {
        return Quat(Sa::UTH::Rand(-50.f,50.f),
                    Sa::UTH::Rand(-50.f,50.f),
                    Sa::UTH::Rand(-50.f,50.f),
                    Sa::UTH::Rand(-50.f,50.f)).Normalize();
    }

    void TestConstructors()
    {
        // Pi/2 angle on X Axis.
        {
            const Quat q1(90.f, Vec3(1.f, 0.f, 0.f));
            const Vec3 result_euler1(Maths::DegToRad(90.f), 0.f, 0.f);
            const Vec3 euler1 = q1.ToEuler();
            SA_UTH_MF(euler1, IsEquals, result_euler1);
            Quat quat_result(euler1);
            SA_UTH_MF(quat_result, IsEquals, q1);
        }

        // Pi/2 angle on Y Axis.
        {
            const Quat q1(90.f, Vec3(0.f, 1.f, 0.f));
            const Vec3 result_euler1(0.f, Maths::DegToRad(90.f), 0.f);
            const Vec3 euler1 = q1.ToEuler();
            SA_UTH_MF(euler1, IsEquals, result_euler1);
            Quat quat_result(euler1);
            SA_UTH_MF(quat_result, IsEquals, q1);
        }


        // Pi/2 angle on Z Axis.
        {
            const Quat q1(90.f, Vec3(0.f, 0.f, 1.f));
            const Vec3 result_euler1(0.f, 0.f, Maths::DegToRad(90.f));
            const Vec3 euler1 = q1.ToEuler();
            SA_UTH_MF(euler1, IsEquals, result_euler1);
            Quat quat_result(euler1);
            SA_UTH_MF(quat_result, IsEquals, q1);
        }

        // Pi/2 angle on XY Axis.
        {
            const Quat q1(90.f, Vec3(1.f, 1.f, 0.f));
            const Vec3 result_euler1(Maths::DegToRad(90.f),
                                     Maths::DegToRad(45.f),
                                     Maths::DegToRad(45.f));

            const Vec3 euler1 = q1.ToEuler();
            SA_UTH_MF(euler1, IsEquals, result_euler1);
            Quat quat_result(euler1);
            SA_UTH_MF(quat_result, IsEquals, q1);
        }

        // Pi/2 angle on XZ Axis.
        {
            const Quat q1(90.f, Vec3(1.f, 0.f, 1.f));
            const Vec3 result_euler1(Maths::DegToRad(54.74f),
                                     Maths::DegToRad(-30.f),
                                     Maths::DegToRad(54.74f));

            const Vec3 euler1 = q1.ToEuler();
            SA_UTH_MF(euler1, IsEquals, result_euler1);
            Quat quat_result(euler1);
            SA_UTH_MF(quat_result, IsEquals, q1);
        }

        // Pi/2 angle on YZ Axis.
        {
            const Quat q1(90.f, Vec3(0.f, 1.f, 1.f));
            const Vec3 result_euler1(Maths::DegToRad(45.f),
                                     Maths::DegToRad(45.f),
                                     Maths::DegToRad(90.f));

            const Vec3 euler1 = q1.ToEuler();
            SA_UTH_MF(euler1, IsEquals, result_euler1);
            Quat quat_result(euler1);
            SA_UTH_MF(quat_result, IsEquals, q1);
        }

        // Pi/2 angle on XYZ Axis.
        {
            const Quat q1(90.f, Vec3(1.f, 1.f, 1.f).Normalize());
            const Vec3 result_euler1(Maths::DegToRad(70.f),
                                     Maths::DegToRad(14.f),
                                     Maths::DegToRad(70.f));

            const Vec3 euler1 = q1.ToEuler();
            SA_UTH_MF(euler1, IsEquals, result_euler1);
            Quat quat_result(euler1);
            SA_UTH_MF(quat_result, IsEquals, q1);
        }

        // Pi/2 angle on random Axis.
        {
            const Quat q1(90.f, Vec3(3.656f, 1.15f, 7.36f).Normalize());
            const Vec3 result_euler1(Maths::DegToRad(35.61f),
                                     Maths::DegToRad(-14.6f),
                                     Maths::DegToRad(78.42f));

            const Vec3 euler1 = q1.ToEuler();
            SA_UTH_MF(euler1, IsEquals, result_euler1);
            Quat quat_result(euler1);
            SA_UTH_MF(quat_result, IsEquals, q1);
        }
    }

    void Main()
    {
        SA_UTH_GP(TestConstructors());
    }
}