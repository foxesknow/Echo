#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo/MethodCall.h>
#include <Echo/Exceptions.h>

namespace EchoUnitTest 
{

TEST_CLASS(MethodCallTests)
{
public:
	TEST_METHOD(Call)
	{
		using namespace Echo;

		AllowFail([]{throw Exception(_T("hello"));});
		Assert::IsTrue(true);
	}
};

} // end of namespace