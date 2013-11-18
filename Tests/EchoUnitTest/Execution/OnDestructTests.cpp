#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo/Execution/OnDestruct.h>

namespace EchoUnitTest { namespace Execution {		

TEST_CLASS(OnDestructTests)
{
public:
	TEST_METHOD(IncreaseOnDestruct)
	{
		int count=0;

		{
			Assert::AreEqual(count,0,nullptr,LINE_INFO());
			Echo::Execution::OnDestruct destruct([&]{count++;});
		}

		Assert::AreEqual(count,1,nullptr,LINE_INFO());
	}

	TEST_METHOD(MoveInstance)
	{
		int count=0;

		{
			Assert::AreEqual(count,0,nullptr,LINE_INFO());
			Echo::Execution::OnDestruct destruct([&]{count++;});
			Echo::Execution::OnDestruct copy=std::move(destruct);
		}

		Assert::AreEqual(count,1,nullptr,LINE_INFO());
	}

};

}} // end of namespace