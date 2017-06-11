// TestConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Echo\ThreadPool.h>
#include <Echo\Events.h>
#include <Echo\tstring.h>

#include <atomic>

int main()
{
	using namespace Echo;

	const char *greeting = "Hello, world!";
	auto converted = tstd::to_wstring(greeting);

	std::atomic<int> counter(0);

	ManualResetEvent event(InitialState::NonSignalled);

	{
		ThreadPool pool;
		pool.MinimumThreads(1);
		pool.MaximumThreads(1);

		pool.Start();

		for(int i = 0; i < 1000; i++)
		{
			pool.Submit([&]
			{
				counter.fetch_add(1);
				if(counter == 1000) event.Set();
			});
		}

		pool.CancelOutstanding(false);
	}

	event.Wait();

    return 0;   
}

