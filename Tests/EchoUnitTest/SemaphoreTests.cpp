#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\Semaphore.h>

namespace EchoUnitTest 
{

TEST_CLASS(SemaphoreTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		Semaphore semaphore(10);
	}

	TEST_METHOD(Release)
	{
		using namespace Echo;

		Semaphore semaphore(1,10);
		semaphore.Release();
		Assert::IsTrue(true);
	}

	TEST_METHOD(UseGuard)
	{
		using namespace Echo;

		Semaphore semaphore(1,10);
		Guard<Semaphore> guard(semaphore);
	}

	TEST_METHOD(NothingAvailable)
	{
		using namespace Echo;

		Semaphore semaphore(0,10);
		Assert::IsFalse(semaphore.Wait(std::chrono::milliseconds(100)));
	}

	TEST_METHOD(SomethingAvailable)
	{
		using namespace Echo;

		Semaphore semaphore(10,10);
		Assert::IsTrue(semaphore.Wait(std::chrono::milliseconds(100)));
	}
};

} // end of namespace