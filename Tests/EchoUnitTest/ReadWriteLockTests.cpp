#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\ReadWriteLock.h>

namespace EchoUnitTest 
{

TEST_CLASS(ReadWriteLockTests)
{
private:
	Echo::UniqueGuard<Echo::ReadWriteLock> CreateGuard(Echo::ReadWriteLock &rwLock)
	{
		using namespace Echo;
		
		UniqueGuard<ReadWriteLock> guard(rwLock);
		return guard;
	}

public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		ReadWriteLock rwLock;
	}

	TEST_METHOD(AcquireExclusive)
	{
		using namespace Echo;

		ReadWriteLock rwLock;
		Guard<ReadWriteLock> lock(rwLock);
	}

	TEST_METHOD(AcquireShared)
	{
		using namespace Echo;

		ReadWriteLock rwLock;
		ReadWriteLockSharedGuard lock(rwLock);
	}

	TEST_METHOD(UniqueLocking)
	{
		using namespace Echo;

		ReadWriteLock rwLock;
		auto uniqueGuard = CreateGuard(rwLock);
	}
};

} // end of namespace