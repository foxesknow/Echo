#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\ConditionalVariable.h>
#include <Echo\CriticalSection.h>
#include <Echo\Thread.h>

namespace EchoUnitTest 
{

TEST_CLASS(ConditionalVariableTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		ConditionalVariable cv;
	}

	TEST_METHOD(WaitOnCriticalSection)
	{
		using namespace Echo;

		CriticalSection section;
		ConditionalVariable cv;

		bool flag = false;

		Thread thread([&]
		{
			::Sleep(3 * 1000);

			CriticalSectionGuard guard(section);
			flag = true;
			cv.Notify();
			
		});

		thread.Start();

		CriticalSectionGuard guard(section);
		while(flag == false)
		{
			cv.Wait(section, std::chrono::milliseconds(1000));
		}

		Assert::AreEqual(true, flag);
	}

	TEST_METHOD(WaitOnReadWriteLock)
	{
		using namespace Echo;

		ReadWriteLock rwLock;
		ConditionalVariable cv;

		bool flag = false;

		Thread thread([&]
		{
			::Sleep(3 * 1000);

			ReadWriteLockGuard guard(rwLock);
			flag = true;
			cv.Notify();
			
		});

		thread.Start();

		ReadWriteLockGuard guard(rwLock);
		while(flag == false)
		{
			cv.Wait(rwLock, std::chrono::milliseconds(1000));
		}

		Assert::AreEqual(true, flag);
	}
};

} // end of namespace