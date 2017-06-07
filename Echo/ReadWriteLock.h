#pragma once

#include "WinInclude.h"
#include "Lock.h"

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

	ReadWriteLock(const ReadWriteLock&)=delete;
	ReadWriteLock(ReadWriteLock&&)=delete;

	ReadWriteLock &operator=(const ReadWriteLock&)=delete;
	ReadWriteLock &operator=(ReadWriteLock&&) = delete;

	/**
	* Enters the lock in exclusive mode
	*/
	void Enter()noexcept
	{
		::AcquireSRWLockExclusive(&m_Lock);
	}

	/**
	* Attempts to enter the lock in exclusive mode
	* @returns true if the lock section was entered, otherwise false
	*/
	bool TryEnter()noexcept
	{
		return ::TryAcquireSRWLockExclusive(&m_Lock)!=FALSE;
	}

	/**
	* Exits a lock that was acquired in exclusive mode
	*/
	void Exit()noexcept
	{
		::ReleaseSRWLockExclusive(&m_Lock);
	}
};

/**
 * Locks a critical section
 */
template<>
class Lock<ReadWriteLock>
{
private:
	ReadWriteLock &m_Lock;

public:
	/**
	 * Exclusively acquires the read write lock
	 */
	explicit Lock(ReadWriteLock &lock) : m_Lock(lock)
	{
		m_Lock.Enter();
	}

	Lock(const Lock &)=delete;
	Lock &operator=(Lock &)=delete;

	/**
	 * Destroys the lock be exitting the critical section
	 */
	~Lock() noexcept
	{
		m_Lock.Exit();
	}
};

} // end of namespace