#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\ReadWriteLock.h>

namespace EchoUnitTest 
{

TEST_CLASS(ReadWriteLockTests)
{
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
};

} // end of namespace