#include <iostream>
#include <Core/Maths/Utils/numerics.hpp>
#include <Core/Maths/unitTestMaths.hpp>
#include <Core/Maths/Quaternion/quaternion.hpp>
#include <Core/Maths/Vector/vector3.hpp>
#define SA_UTH_DFLT_CSL_LOG 1
#include <SA-UnitTestHelper.hpp>

using namespace Sa;

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

        //Default constructor
        {
            Quat q1;
            SA_UTH_EQ(q1.x, 0.f);
            SA_UTH_EQ(q1.y, 0.f);
            SA_UTH_EQ(q1.z, 0.f);
            SA_UTH_EQ(q1.w, 1.f);
        }

        //Value constructor
        {
           float x = UTH::Rand(-50,50);
           float y = UTH::Rand(-50,50);
           float z = UTH::Rand(-50,50);
           float w = UTH::Rand(-50,50);

           Quat q1 (x);

           SA_UTH_EQ(q1.x, x);
           SA_UTH_EQ(q1.y, x);
           SA_UTH_EQ(q1.z, x);
           SA_UTH_EQ(q1.w, x);

           Quat q2 (x,y,z,w);

           SA_UTH_EQ(q2.x, x);
           SA_UTH_EQ(q2.y, y);
           SA_UTH_EQ(q2.z, z);
           SA_UTH_EQ(q2.w, w);
        }

        //constructor angle Axis
        {
            const float deg = UTH::Rand(-180,180);
            const float rad = Solid::Maths::DegToRad(deg);

            const Vec3 v1 = Vec3(UTH::Rand(-100.0f, 100.0f), UTH::Rand(-100.0f, 100.0f), UTH::Rand(-100.0f, 100.0f)).Normalize();
            const Vec3 res_v1 = v1 * std::sin(static_cast<float>(rad));
            const Quat q1(rad, v1);

            SA_UTH_EQ(q1.w, std::cos(static_cast<float>(rad)));
            SA_UTH_EQ(q1.x, res_v1.x);
            SA_UTH_EQ(q1.y, res_v1.y);
            SA_UTH_EQ(q1.z, res_v1.z);
        }

        //Constructor with Vec3
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
    }
    void Main()
    {
        SA_UTH_GP(TestConstructors());
    }
}