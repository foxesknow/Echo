#include "stdafx.h"
#include "CppUnitTest.h"

#include <chrono>
#include <utility>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo/Events.h>

namespace EchoUnitTest 
{

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
		using namespace Echo;

		ManualResetEvent event(InitialState::NonSignalled);

		// It's not signalled, so wait will be false
		bool signalled=event.Wait(m_Zero);
		Assert::AreEqual(false,signalled,nullptr,LINE_INFO());

		event.Set();

		// It should stay set now
		event.Wait();

		signalled=event.Wait(m_Zero);
		Assert::AreEqual(true,signalled,nullptr,LINE_INFO());

		// Set it back to non-signalled
		event.Reset();
		signalled=event.Wait(m_Zero);
		Assert::AreEqual(false,signalled,nullptr,LINE_INFO());
	}

	TEST_METHOD(AutoReset)
	{
		using namespace Echo;

		AutoResetEvent event(InitialState::NonSignalled);

		// It's not signalled, so wait will be false
		bool signalled=event.Wait(m_Zero);
		Assert::AreEqual(false,signalled,nullptr,LINE_INFO());

		event.Set();

		// It'll be set for onw wait
		event.Wait();

		signalled=event.Wait(m_Zero);
		Assert::AreEqual(false,signalled,nullptr,LINE_INFO());
	}

	TEST_METHOD(ManualEvent_MoveCtor)
	{
		using namespace Echo;

		auto event=CreateManualResetEvent();
		Assert::AreNotEqual(Event::Traits::InvalidValue(),event.UnderlyingHandle(),nullptr,LINE_INFO());

		ManualResetEvent event2=std::move(event);
		Assert::AreNotEqual(Event::Traits::InvalidValue(),event2.UnderlyingHandle(),nullptr,LINE_INFO());
		Assert::AreEqual(Event::Traits::InvalidValue(),event.UnderlyingHandle(),nullptr,LINE_INFO());
	}

	Echo::ManualResetEvent CreateManualResetEvent()
	{
		using namespace Echo;

		return ManualResetEvent(InitialState::NonSignalled);
	}
};

} // end of namespace