#pragma once

#include <Echo\WaitHandle.h>
#include <Echo\HandleTraits.h>

#include <Echo\Guard.h>
#include <Echo\Exceptions.h>

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
		auto handle = ::CreateSemaphore(nullptr, initialCount, maximumCount, nullptr);
		if(handle == Traits::InvalidValue()) throw ThreadException(_T("could not create semaphore"));

		UnderlyingHandle(handle);
	}

	/**
	 * Initializes the instance via a move
	 */
	Semaphore(Semaphore &&rhs) : Base(std::move(rhs))
	{
	}

	Semaphore(const Semaphore&) = delete;	
	Semaphore &operator=(const Semaphore&) = delete;

	/**
	 * Assigns the semaphore via a move
	 */
	Semaphore &operator=(Semaphore &&rhs)
	{
		if(this != &rhs)
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
		
		auto success = ::ReleaseSemaphore(UnderlyingHandle(), releaseCount, &previousCount);
		if(!success) throw ThreadException(_T("could not release semaphore"));

		return previousCount;
	}

	/**
	 * Creates a semaphore from an existing handle
	 */
	static Semaphore Attach(HANDLE handle)
	{
		if(handle == Traits::InvalidValue()) throw ThreadException(_T("invalid handle"));

		return Semaphore(handle);
	}
};

/**
 * Locks a semaphore
 */
template<>
class Guard<Semaphore>
{
private:
	Semaphore &m_Semaphore;

public:
	/**
	 * Initializes the instance my waiting on the semaphore
	 */
	explicit Guard(Semaphore &semaphore) : m_Semaphore(semaphore)
	{
		m_Semaphore.Wait();
	}

	Guard(const Guard &) = delete;
	Guard &operator=(const Guard &) = delete;
	Guard &operator=(Guard &&) = delete;

	/**
	 * Destroys the instance by releasing the mutex
	 */
	~Guard()
	{
		m_Semaphore.Release();
	}
};

template<>
class UniqueGuard<Semaphore>
{
private:
	Semaphore *m_Semaphore;

public:
	/**
	 * Exclusively acquires the read write lock
	 */
	explicit UniqueGuard(Semaphore &mutex) : m_Semaphore(&mutex)
	{
		m_Semaphore->Wait();
	}

	UniqueGuard(UniqueGuard &&rhs) : m_Semaphore(nullptr)
	{
		std::swap(m_Semaphore, rhs.m_Semaphore);
	}

	UniqueGuard(const UniqueGuard &) = delete;
	
	UniqueGuard &operator=(UniqueGuard &&rhs) noexcept
	{
		if(this != &rhs)
		{
			std::swap(m_Semaphore, rhs.m_Semaphore);
		}

		return *this;
	}

	/**
	 * Destroys the lock be exitting the critical section
	 */
	~UniqueGuard() noexcept
	{
		if(m_Semaphore) m_Semaphore->Release();
	}
};


using SemaphoreGuard=Guard<Semaphore>;
using SemaphoreUniqueGuard=UniqueGuard<Semaphore>;

} // end of namespace