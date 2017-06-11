#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\ThreadPool.h>
#include <Echo\Events.h>

#include <atomic>

namespace EchoUnitTest 
{

TEST_CLASS(ThreadPoolTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		ThreadPool pool;
	}

	TEST_METHOD(RunMethodInPool)
	{
		using namespace Echo;

		std::atomic<bool> flag=false;

		ManualResetEvent event(InitialState::NonSignalled);

		ThreadPool pool;
		pool.MinimumThreads(4);
		pool.MaximumThreads(16);

		pool.Start();

		pool.Submit([&]
		{
			flag = true;
			event.Set();
		});

		event.Wait();
		Assert::IsTrue(flag);
	}
};

} // end of namespace