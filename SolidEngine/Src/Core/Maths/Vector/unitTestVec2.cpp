#include <iostream>
#include <Core/Maths/Utils/numerics.hpp>
#include <Core/Maths/Vector/vector2.hpp>
#include <Core/Maths/Vector/vector3.hpp>
#include <Core/Maths/Vector/vector4.hpp>
#include <Core/Maths/unitTestMaths.hpp>
#define SA_UTH_DFLT_CSL_LOG 1
#include <SA-UnitTestHelper.hpp>


using namespace Sa;

namespace Solid::Vec2UnitTest
{
    Vec2 GenerateRandomVec()
    {
        return Vec2(UTH::Rand(-50.f,50.f), UTH::Rand(-50.f,50.f));
    }
    void TestConstructors()
    {
        //Test default contructor
        {
            Vec2 v0;
            SA_UTH_EQ(v0.x,0.f);
            SA_UTH_EQ(v0.y,0.f);
        }

        {
            Vec2 v0 (1,5);
            SA_UTH_EQ(v0.x,1.f);
            SA_UTH_EQ(v0.y,5.f);
        }

        {
            Vec2 v1(5,2);
            Vec2 v0(v1);
            SA_UTH_EQ(v0.x,5.f);
            SA_UTH_EQ(v0.y,2.f);
        }

        {
            Vec3 v1(5.f,2.f,6.f);

            Vec2 v0(v1);
            SA_UTH_EQ(v0.x,5.f);
            SA_UTH_EQ(v0.y,2.f);
        }

        {
            Vec4 v1(5.f,2.f,6.f,1.f);

            Vec2 v0(v1);
            SA_UTH_EQ(v0.x,5.f);
            SA_UTH_EQ(v0.y,2.f);
        }
    }


    void TestStatics()
    {
        SA_UTH_EQ(Vec2::Zero.x,0.f);
        SA_UTH_EQ(Vec2::Zero.y,0.f);

        SA_UTH_EQ(Vec2::Up.x,0.f);
        SA_UTH_EQ(Vec2::Up.y,1.f);

        SA_UTH_EQ(Vec2::Down.x,0.f);
        SA_UTH_EQ(Vec2::Down.y,-1.f);

        SA_UTH_EQ(Vec2::Right.x,1.f);
        SA_UTH_EQ(Vec2::Right.y,0.f);

        SA_UTH_EQ(Vec2::Left.x,-1.f);
        SA_UTH_EQ(Vec2::Left.y,0.f);

        {
            const Vec2 v1 = GenerateRandomVec();
            const Vec2 v2 = GenerateRandomVec();

            float dot = v1.x * v2.x + v1.y * v2.y;
            SA_UTH_RSF(dot, Vec2::Dot, v1, v2);

            float cross = v1.x * v2.y + v1.y * v2.x;
            SA_UTH_RSF(cross, Vec2::Cross, v1, v2);
        }

        {
            const Vec2 v1(2.f,2.f);
            const Vec2 v2 (-2.f,4.f);
            const Vec2 slerp_v1(2.0f, 2.0f);
            const Vec2 slerp_v2(-2.0f, 2.0f);

            const Vec2 lerp_res =  Vec2(0.0f, 3.0f);
            const Vec2 Nlerp_res =  lerp_res.GetNormalized();
            const Vec2 Slerp_res =  Vec2(0.0f, slerp_v1.Length());

            SA_UTH_RSF(lerp_res, Vec2::Lerp, v1, v2,0.5f);
            SA_UTH_RSF(Nlerp_res, Vec2::Nlerp, v1, v2,0.5f);
            SA_UTH_RSF(Slerp_res, Vec2::Slerp, slerp_v1, slerp_v2,0.5f);

        }
    }

    void TestMethods()
    {
        {
            Vec2 v1 = GenerateRandomVec();
            const float lenSqrt = v1.x * v1.x + v1.y * v1.y;
            const float len = Solid::Maths::Sqrt(lenSqrt);
            SA_UTH_RMF(len, v1, Length);
            SA_UTH_RMF(lenSqrt, v1, SqrtLength);
        }

    }

    void TestOperators()
    {

    }

    void Main()
    {
        SA_UTH_GP(TestConstructors());
        SA_UTH_GP(TestStatics());

    }

}



