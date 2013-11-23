#include "stdafx.h"
#include "CppUnitTest.h"

#include <chrono>
#include <utility>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo/Threading/Events.h>

namespace EchoUnitTest { namespace Threading {		

TEST_CLASS(EventsTests)
{
private:
	std::chrono::milliseconds m_Zero;

public:
	EventsTests()
	{
		m_Zero=std::chrono::milliseconds(0);
	}

	TEST_METHOD(ManualReset)
	{
		using namespace Echo::Threading;

		ManualResetEvent event(InitialState::NonSignalled);

		// It's not signalled, so wait will be false
		bool signalled=event.Wait(m_Zero);
		Assert::AreEqual(false,signalled,nullptr,LINE_INFO());

		event.Set();

		// It should stay set now
		signalled=event.Wait();
		Assert::AreEqual(true,signalled,nullptr,LINE_INFO());

		signalled=event.Wait(m_Zero);
		Assert::AreEqual(true,signalled,nullptr,LINE_INFO());

		// Set it back to non-signalled
		event.Reset();
		signalled=event.Wait(m_Zero);
		Assert::AreEqual(false,signalled,nullptr,LINE_INFO());
	}

	TEST_METHOD(AutoReset)
	{
		using namespace Echo::Threading;

		AutoResetEvent event(InitialState::NonSignalled);

		// It's not signalled, so wait will be false
		bool signalled=event.Wait(m_Zero);
		Assert::AreEqual(false,signalled,nullptr,LINE_INFO());

		event.Set();

		// It'll be set for onw wait
		signalled=event.Wait();
		Assert::AreEqual(true,signalled,nullptr,LINE_INFO());

		signalled=event.Wait(m_Zero);
		Assert::AreEqual(false,signalled,nullptr,LINE_INFO());
	}

	TEST_METHOD(ManualEvent_MoveCtor)
	{
		using namespace Echo::Threading;

		auto event=CreateManualResetEvent();
		Assert::AreNotEqual(Event::Traits::InvalidValue(),event.UnderlyingHandle(),nullptr,LINE_INFO());

		ManualResetEvent event2=std::move(event);
		Assert::AreNotEqual(Event::Traits::InvalidValue(),event2.UnderlyingHandle(),nullptr,LINE_INFO());
		Assert::AreEqual(Event::Traits::InvalidValue(),event.UnderlyingHandle(),nullptr,LINE_INFO());
	}

	Echo::Threading::ManualResetEvent CreateManualResetEvent()
	{
		using namespace Echo::Threading;

		return ManualResetEvent(InitialState::NonSignalled);
	}
};

}} // end of namespace