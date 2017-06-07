#pragma once

#include <Echo/WaitHandle.h>
#include <Echo/HandleTraits.h>

#include "Lock.h"
#include "ThreadException.h"

#include <utility>

namespace Echo 
{

class Semaphore : public WaitHandleImpl<HandleNull>
{
private:
	typedef WaitHandleImpl<HandleNull> Base;

protected:
	Semaphore(HANDLE handle) : 	Base(handle)
	{
	}

public:
	/**
	 * Initializes the instance
	 * @param count  the initial and maximum count for the semaphore
	 */
	Semaphore(LONG count) : Semaphore(count,count)
	{
	}

	/**
	 * Initializes the instance
	 * @param initialCount  the initial count of the semaphore. Must be less than or equal to maximumCount
	 * @param maximumCount  the maximum count for the semaphore. Must be greater than zero
	 */
	Semaphore(LONG initialCount, LONG maximumCount)
	{
		auto handle=::CreateSemaphore(nullptr,initialCount,maximumCount,nullptr);
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("could not create semaphore"));

		UnderlyingHandle(handle);
	}

	/**
	 * Initializes the instance via a move
	 */
	Semaphore(Semaphore &&rhs) : Base(std::move(rhs))
	{
	}

	Semaphore(const Semaphore&)=delete;	
	Semaphore &operator=(const Semaphore&)=delete;

	/**
	 * Assigns the semaphore via a move
	 */
	Semaphore &operator=(Semaphore &&rhs)
	{
		if(this!=&rhs)
		{
			Swap(rhs);
			rhs.Close();
		}

		return *this;
	}

	/**
	 * Increases the count on the semaphore
	 * @param releaseCount  the amount to increase the semaphore count by
	 * @returns the previous count of the semaphore
	 */
	LONG Release(LONG releaseCount=1) const
	{
		LONG previousCount=0;
		
		auto success=::ReleaseSemaphore(UnderlyingHandle(),releaseCount,&previousCount);
		if(!success) throw ThreadException(_T("could not release semaphore"));

		return previousCount;
	}

	/**
	 * Creates a semaphore from an existing handle
	 */
	static Semaphore Attach(HANDLE handle)
	{
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("invalid handle"));

		return Semaphore(handle);
	}
};


} // end of namespace