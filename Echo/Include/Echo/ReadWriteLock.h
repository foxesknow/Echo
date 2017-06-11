#pragma once

#include <Echo\WinInclude.h>
#include <Echo\Guard.h>

#include <utility>

namespace Echo
{

/**
 * Wraps a read write lock
 */
class ReadWriteLock
{
private:
	SRWLOCK m_Lock=SRWLOCK_INIT;

public:
	ReadWriteLock()
	{
	}

	ReadWriteLock(const ReadWriteLock&) = delete;
	ReadWriteLock(ReadWriteLock&&) = delete;

	ReadWriteLock &operator=(const ReadWriteLock&) = delete;
	ReadWriteLock &operator=(ReadWriteLock&&) = delete;

	/**
	* Enters the lock in exclusive mode
	*/
	void Enter() noexcept
	{
		::AcquireSRWLockExclusive(&m_Lock);
	}

	/**
	* Enters the lock in shared mode
	*/
	void EnterShared() noexcept
	{
		::AcquireSRWLockShared(&m_Lock);
	}

	/**
	* Attempts to enter the lock in exclusive mode
	* @returns true if the lock section was entered, otherwise false
	*/
	bool TryEnter() noexcept
	{
		return ::TryAcquireSRWLockExclusive(&m_Lock)!=FALSE;
	}

	/**
	* Exits a lock that was acquired in exclusive mode
	*/
	void Exit() noexcept
	{
		::ReleaseSRWLockExclusive(&m_Lock);
	}

	/**
	* Exits a lock that was acquired in shared mode
	*/
	void ExitShared() noexcept
	{
		::ReleaseSRWLockShared(&m_Lock);
	}

	/**
	 * Returns the underlying lock
	 */
	SRWLOCK *Underlying() noexcept
	{
		return &m_Lock;
	}
};

/**
 * Locks a read write lock in exclusive mode
 */
template<>
class Guard<ReadWriteLock>
{
private:
	ReadWriteLock &m_Lock;

public:
	/**
	 * Exclusively acquires the read write lock
	 */
	explicit Guard(ReadWriteLock &lock) : m_Lock(lock)
	{
		m_Lock.Enter();
	}

	Guard(const Guard &) = delete;
	Guard(Guard &&) = delete;
	
	Guard &operator=(Guard &&) = delete;
	Guard &operator=(const Guard &) = delete;

	/**
	 * Destroys the lock be exitting the critical section
	 */
	~Guard() noexcept
	{
		m_Lock.Exit();
	}
};


/**
 * Locks a read write lock in shared mode
 */
class ReadWriteLockSharedGuard
{
private:
	ReadWriteLock &m_Lock;

public:
	/**
	 * Exclusively acquires the read write lock
	 */
	explicit ReadWriteLockSharedGuard(ReadWriteLock &lock) : m_Lock(lock)
	{
		m_Lock.EnterShared();
	}

	ReadWriteLockSharedGuard(const ReadWriteLockSharedGuard &) = delete;
	ReadWriteLockSharedGuard(ReadWriteLockSharedGuard &&) = delete;
	ReadWriteLockSharedGuard &operator=(const ReadWriteLockSharedGuard &) = delete;
	ReadWriteLockSharedGuard &operator=(ReadWriteLockSharedGuard &&) = delete;

	/**
	 * Destroys the lock be exitting the critical section
	 */
	~ReadWriteLockSharedGuard() noexcept
	{
		m_Lock.ExitShared();
	}
};

template<>
class UniqueGuard<ReadWriteLock>
{
private:
	ReadWriteLock *m_Lock;

public:
	/**
	 * Exclusively acquires the read write lock
	 */
	explicit UniqueGuard(ReadWriteLock &lock) : m_Lock(&lock)
	{
		m_Lock->Enter();
	}

	UniqueGuard(UniqueGuard &&rhs) : m_Lock(nullptr)
	{
		std::swap(m_Lock, rhs.m_Lock);
	}

	UniqueGuard(const UniqueGuard &) = delete;
	
	UniqueGuard &operator=(UniqueGuard &&rhs) noexcept
	{
		if(this != &rhs)
		{
			std::swap(m_Lock, rhs.m_Lock);
		}

		return *this;
	}

	/**
	 * Destroys the lock be exitting the critical section
	 */
	~UniqueGuard() noexcept
	{
		if(m_Lock) m_Lock->Exit();
	}
};

// An exclusive lock
using ReadWriteLockGuard = Guard<ReadWriteLock>;
using ReadWriteLockUniqueGuard = UniqueGuard<ReadWriteLock>;

} // end of namespace