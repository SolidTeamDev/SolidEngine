#include "pch.h"
#include "CppUnitTest.h"
#include <Core/Maths/solidMaths.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Solid;
namespace TestMaths
{
	void AreEqual(Vec2 expected, Vec2 actual, double tolerance, const wchar_t* message = NULL, const __LineInfo * pLineInfo = NULL)
	{
		//FailOnCondition(expected.IsEquals(actual), "", pLineInfo);
	}


	TEST_CLASS(TestMaths)
	{
	public:


		TEST_METHOD(TestMethod1)
		{
			Solid::Vec2 a = {};
			Assert::AreEqual(0.f, a.x);
		}
	};
}
