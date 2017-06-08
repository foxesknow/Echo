#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\MultiWaiter.h>
#include <Echo\Events.h>

namespace EchoUnitTest 
{

TEST_CLASS(MultiWaiterTests)
{
public:
	TEST_METHOD(WaitAll_AllSet)
	{
		using namespace Echo;

		ManualResetEvent event1(InitialState::Signalled);
		ManualResetEvent event2(InitialState::Signalled);

		MultiWaiter waiter;
		waiter << event1 << event2;

		bool anySignalled=waiter.WaitAll();
		Assert::IsTrue(anySignalled);
	}

	TEST_METHOD(WaitAll_OneSet)
	{
		using namespace Echo;

		ManualResetEvent event1(InitialState::NonSignalled);
		ManualResetEvent event2(InitialState::Signalled);

		MultiWaiter waiter;
		waiter << event1 << event2;

		bool anySignalled=waiter.WaitAll(std::chrono::milliseconds(100));
		Assert::IsFalse(anySignalled);
	}

	TEST_METHOD(WaitAny_AllSet)
	{
		using namespace Echo;

		ManualResetEvent event1(InitialState::Signalled);
		ManualResetEvent event2(InitialState::Signalled);

		MultiWaiter waiter;
		waiter << event1 << event2;

		int signalledIndex=waiter.WaitAny(std::chrono::milliseconds(100));
		Assert::AreEqual(0,signalledIndex);
	}

	TEST_METHOD(WaitAny_OneSet)
	{
		using namespace Echo;

		ManualResetEvent event1(InitialState::NonSignalled);
		ManualResetEvent event2(InitialState::Signalled);

		MultiWaiter waiter;
		waiter << event1 << event2;

		int signalledIndex=waiter.WaitAny(std::chrono::milliseconds(100));
		Assert::AreEqual(1,signalledIndex);
	}

	TEST_METHOD(WaitAny_NoneSet)
	{
		using namespace Echo;

		ManualResetEvent event1(InitialState::NonSignalled);
		ManualResetEvent event2(InitialState::NonSignalled);

		MultiWaiter waiter;
		waiter << event1 << event2;

		int signalledIndex=waiter.WaitAny(std::chrono::milliseconds(100));
		Assert::AreEqual(-1,signalledIndex);
	}
};

} // end of namespace