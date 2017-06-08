#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\Mutex.h>

namespace EchoUnitTest 
{

TEST_CLASS(MutexTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		Mutex mutex(Ownership::Owned);
	}

	TEST_METHOD(Release)
	{
		using namespace Echo;

		Mutex mutex(Ownership::Owned);
		mutex.Release();
		Assert::IsTrue(true);
	}

	TEST_METHOD(AcquireAndRelease)
	{
		using namespace Echo;

		Mutex mutex(Ownership::NotOwned);
		mutex.Wait();
		mutex.Release();
	}
};

} // end of namespace