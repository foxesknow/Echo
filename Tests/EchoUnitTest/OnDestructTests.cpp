#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo/OnDestruct.h>

namespace EchoUnitTest 
{

TEST_CLASS(OnDestructTests)
{
public:
	TEST_METHOD(IncreaseOnDestruct)
	{
		int count=0;

		{
			Assert::AreEqual(count,0,nullptr,LINE_INFO());
			Echo::OnDestruct destruct([&]{count++;});
		}

		Assert::AreEqual(count,1,nullptr,LINE_INFO());
	}

	TEST_METHOD(MoveInstance)
	{
		int count=0;

		{
			Assert::AreEqual(count,0,nullptr,LINE_INFO());
			Echo::OnDestruct destruct([&]{count++;});
			Echo::OnDestruct copy=std::move(destruct);
		}

		Assert::AreEqual(count,1,nullptr,LINE_INFO());
	}

};

} // end of namespace