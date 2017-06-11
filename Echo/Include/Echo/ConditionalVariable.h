#pragma once

#include <Echo\WinInclude.h>
#include <Echo\Exceptions.h>
#include <Echo\CriticalSection.h>
#include <Echo\ReadWriteLock.h>
#include <Echo\WaitHandle.h>

#include <chrono>

namespace Echo 
{

/**
 * Wraps a Win32 conditional variable
 */
class ConditionalVariable
{
private:
	mutable CONDITION_VARIABLE m_Condition;

	bool DoWaitCS(CRITICAL_SECTION *cs, const std::chrono::milliseconds &milliseconds) const
	{
		DWORD ms = static_cast<DWORD>(milliseconds.count());
		BOOL success = ::SleepConditionVariableCS(&m_Condition, cs, ms);

		if(success) return true;

		// It we fail due to timeout then that's ok
		DWORD lastError = ::GetLastError();
		if(lastError == ERROR_TIMEOUT) return false;

		throw WindowsException(_T("wait on conditional variable failed"));
	}

	bool DoWaitRW(SRWLOCK *rwLock, const std::chrono::milliseconds &milliseconds, ULONG flags) const
	{
		DWORD ms = static_cast<DWORD>(milliseconds.count());
		BOOL success = ::SleepConditionVariableSRW(&m_Condition, rwLock, ms, flags);

		if(success) return true;

		// It we fail due to timeout then that's ok
		DWORD lastError = ::GetLastError();
		if(lastError == ERROR_TIMEOUT) return false;

		throw WindowsException(_T("wait on conditional variable failed"));
	}

public:
	/**
	 * Initializes the instance
	 */
	ConditionalVariable() noexcept
	{
		::InitializeConditionVariable(&m_Condition);
	}

	ConditionalVariable(const ConditionalVariable&) = delete;
	ConditionalVariable(ConditionalVariable&&) = delete;
	
	ConditionalVariable &operator=(const ConditionalVariable &) = delete;
	ConditionalVariable &operator=(ConditionalVariable &&) = delete;

	/**
	 * Destroys the instance
	 */
	~ConditionalVariable() noexcept
	{
	}

	/**
	 * Waits forever on the condition
	 * @param cs  the critical section to unlock whilst we wait
	 */
	void Wait(CriticalSection &cs) const
	{
		DoWaitCS(cs.Underlying(), Infinite);
	}

	/**
	 * Waits on the condition
	 * @param cs  the critical section to unlock whilst we wait
	 * @param duration  how long to wait for
	 * @returns true if condition was notified within the duration, otherwise false
	 */
	bool Wait(CriticalSection &cs, const std::chrono::milliseconds &duration) const
	{
		return DoWaitCS(cs.Underlying(), duration);
	}

	/**
	 * Waits forever on the read write lock
	 * @param rwLock  the read-write to unlock whilst we wait
	 */
	void Wait(ReadWriteLock &rwLock) const
	{
		DoWaitRW(rwLock.Underlying(), Infinite, 0);
	}

	/**
	 * Waits on the condition
	 * @param rwLock  the read-write to unlock whilst we wait
	 * @param duration  how long to wait for
	 * @returns true if condition was notified within the duration, otherwise false
	 */
	bool Wait(ReadWriteLock &rwLock, const std::chrono::milliseconds &duration) const
	{
		return DoWaitRW(rwLock.Underlying(), duration, 0);
	}

	/**
	 * Waits forever on the shared read write lock
	 * @param rwLock  the read-write to unlock whilst we wait
	 */
	void WaitShared(ReadWriteLock &rwLock) const
	{
		DoWaitRW(rwLock.Underlying(), Infinite, CONDITION_VARIABLE_LOCKMODE_SHARED);
	}

	/**
	 * Waits on the condition
	 * @param rwLock  the shared read-write to unlock whilst we wait
	 * @param duration  how long to wait for
	 * @returns true if condition was notified within the duration, otherwise false
	 */
	bool WaitShared(ReadWriteLock &rwLock, const std::chrono::milliseconds &duration) const
	{
		return DoWaitRW(rwLock.Underlying(), duration, CONDITION_VARIABLE_LOCKMODE_SHARED);
	}


	/**
	 * Notifes the condition, releasing a thread that is waiting on the condition
	 */
	void Notify() const noexcept
	{
		::WakeConditionVariable(&m_Condition);
	}

	/**
	 * Notifes the condition, releasing all threads that are waiting on the condition
	 */
	void NotifyAll() const noexcept
	{
		::WakeAllConditionVariable(&m_Condition);
	}
};

} // end of namespace