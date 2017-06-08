#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\WorkDispatchQueue.h>
#include <Echo\ImmediateWorkItemDispatcher.h>
#include <Echo\ThreadPool.h>
#include <Echo\Events.h>

#include <atomic>

namespace EchoUnitTest 
{

TEST_CLASS(WorkDispatchQueueTests)
{
public:
	TEST_METHOD(Immediate)
	{
		using namespace Echo;

		ImmediateWorkItemDispatcher dispatcher;
		FunctionWorkDispatchQueue queue(dispatcher);

		bool flag=false;

		queue.Enqueue([&flag]{flag=true;});
		Assert::IsTrue(flag);
	}

	TEST_METHOD(OnThread)
	{
		using namespace Echo;

		ThreadPool pool;
		pool.Start();

		ManualResetEvent event(InitialState::NonSignalled);
		FunctionWorkDispatchQueue queue(pool);

		std::atomic<bool> flag=false;

		queue.Enqueue([&]
		{
			flag=true;
			event.Set();
		});

		event.Wait();
		Assert::IsTrue(flag);
	}
};

} // end of namespace