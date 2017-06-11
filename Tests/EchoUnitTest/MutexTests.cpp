#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\Mutex.h>

namespace EchoUnitTest 
{

TEST_CLASS(MutexTests)
{
private:
	Echo::UniqueGuard<Echo::Mutex> CreateGuard(Echo::Mutex &mutex)
	{
		using namespace Echo;
		
		UniqueGuard<Mutex> guard(mutex);
		return guard;
	}

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

	TEST_METHOD(UniqueLocking)
	{
		using namespace Echo;

		Mutex mutex(Ownership::NotOwned);
		auto uniqueGuard = CreateGuard(mutex);
	}
};

} // end of namespace