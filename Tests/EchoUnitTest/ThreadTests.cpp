#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\Thread.h>

namespace EchoUnitTest 
{

TEST_CLASS(ThreadTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		bool flag=false;

		Thread thread([&flag]()
		{
			flag=true;
		});

		Assert::IsFalse(flag);
		Assert::IsFalse(thread.Started());

		thread.Start();
		thread.Wait();
		
		Assert::IsTrue(flag);
		Assert::IsTrue(thread.Started());
	}
};

} // end of namespace