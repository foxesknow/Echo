#pragma once

#include "WinInclude.h"
#include "Guard.h"

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
	* Enters the lock in shared mode
	*/
	void EnterShared()noexcept
	{
		::AcquireSRWLockShared(&m_Lock);
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

	/**
	* Exits a lock that was acquired in shared mode
	*/
	void ExitShared()noexcept
	{
		::ReleaseSRWLockShared(&m_Lock);
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

	Guard(const Guard &)=delete;
	Guard &operator=(Guard &)=delete;

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
class GuardShared
{
private:
	ReadWriteLock &m_Lock;

public:
	/**
	 * Exclusively acquires the read write lock
	 */
	explicit GuardShared(ReadWriteLock &lock) : m_Lock(lock)
	{
		m_Lock.EnterShared();
	}

	GuardShared(const GuardShared &)=delete;
	GuardShared &operator=(GuardShared &)=delete;

	/**
	 * Destroys the lock be exitting the critical section
	 */
	~GuardShared() noexcept
	{
		m_Lock.ExitShared();
	}
};


} // end of namespace