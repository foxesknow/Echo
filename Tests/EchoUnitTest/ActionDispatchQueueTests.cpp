#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\ActionDispatchQueue.h>

namespace EchoUnitTest 
{

TEST_CLASS(ActionDispatchQueueTests)
{
public:
	TEST_METHOD(NoActions)
	{
		using namespace Echo;

		ThreadPool pool;
		pool.Start();

		ActionDispatchQueue queue(pool);
	}

	TEST_METHOD(OneAction)
	{
		using namespace Echo;

		long count=0;

		{
			ThreadPool pool;
			pool.Start();

			ActionDispatchQueue queue(pool);

			auto function=[&]{::InterlockedIncrement(&count);};
			queue.Enqueue(function);
		}

		Assert::AreEqual((long)1,count,nullptr,LINE_INFO());
	}

	TEST_METHOD(TwoAction)
	{
		using namespace Echo;

		long count=0;

		{
			ThreadPool pool;
			pool.Start();

			ActionDispatchQueue queue(pool);

			auto function1=[&]{::InterlockedIncrement(&count);};
			queue.Enqueue(function1);
			
			auto function2=[&]{::InterlockedAdd(&count,2);};
			queue.Enqueue(function2);
		}

		Assert::AreEqual((long)3,count,nullptr,LINE_INFO());
	}

	TEST_METHOD(TryEnqueue)
	{
		using namespace Echo;

		long count=0;

		ThreadPool pool;
		pool.Start();

		ActionDispatchQueue queue(pool);

		auto function=[&]{::InterlockedIncrement(&count);};
		queue.Enqueue(function);
			
		queue.Shutdown();
		bool enqueued=queue.TryEnqueue(function);

		long finalCount=::InterlockedCompareExchange(&count,0,0);

		Assert::AreEqual((long)1,finalCount,nullptr,LINE_INFO());
		Assert::AreEqual(false,enqueued,nullptr,LINE_INFO());
	}

};

} // end of namespace