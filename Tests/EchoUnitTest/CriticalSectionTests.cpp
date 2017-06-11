#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo/CriticalSection.h>

#include <utility>

namespace EchoUnitTest 
{

TEST_CLASS(CriticalSectionTests)
{
private:
	Echo::UniqueGuard<Echo::CriticalSection> CreateGuard(Echo::CriticalSection &section)
	{
		using namespace Echo;
		
		UniqueGuard<CriticalSection> guard(section);
		return guard;
	}

public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		CriticalSection section;
	}

	TEST_METHOD(TryEnter)
	{
		using namespace Echo;

		CriticalSection section;
		Assert::AreEqual(true,section.TryEnter());
		section.Exit();
	}

	TEST_METHOD(Locking)
	{
		using namespace Echo;

		CriticalSection section;
		Guard<CriticalSection> lock(section);
	}

	TEST_METHOD(TryLocking)
	{
		using namespace Echo;

		CriticalSection section;
		
		TryGuard<CriticalSection> lock(section);
		Assert::IsTrue(lock.IsLocked());
	}

	TEST_METHOD(Unlocking)
	{
		using namespace Echo;

		CriticalSection section;
		Guard<CriticalSection> lock(section);
		Unguard<CriticalSection> unlock(section);
	}

	TEST_METHOD(UniqueLocking)
	{
		using namespace Echo;

		CriticalSection section;
		auto uniqueGuard = CreateGuard(section);
	}
};

} // end of namespace