#pragma once

#include "WinInclude.h"
#include "Exceptions.h"
#include "CriticalSection.h"

namespace Echo 
{

/**
 * Wraps a Win32 conditional variable
 */
class ConditionalVariable
{
private:
	mutable CONDITION_VARIABLE m_Condition;

	bool DoWait(CRITICAL_SECTION *cs, const std::chrono::milliseconds &milliseconds)const
	{
		DWORD ms=static_cast<DWORD>(milliseconds.count());
		BOOL success=::SleepConditionVariableCS(&m_Condition,cs,ms);

		if(success) return true;

		// It we fail due to timeout then that's ok
		DWORD lastError=::GetLastError();
		if(lastError==ERROR_TIMEOUT) return false;

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

	ConditionalVariable(const ConditionalVariable&)=delete;
	ConditionalVariable &operator=(const ConditionalVariable &)=delete;

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
	void Wait(const CriticalSection &cs)const
	{
		DoWait(cs.Underlying(),Infinite);
	}

	/**
	 * Waits on the condition
	 * @param cs  the critical section to unlock whilst we wait
	 * @param milliseconds  how long to wait for
	 * @returns true if condition was notified within the duration, otherwise false
	 */
	bool Wait(const CriticalSection &cs, const std::chrono::milliseconds &milliseconds)const
	{
		return DoWait(cs.Underlying(),milliseconds);
	}

	/**
	 * Waits on the condition
	 * @param cs  the critical section to unlock whilst we wait
	 * @param duration  how long to wait for
	 * @returns true if condition was notified within the duration, otherwise false
	 */
	template<typename REP, typename PERIOD>
	bool Wait(const CriticalSection &cs, const std::chrono::duration<REP,PERIOD> &duration)const
	{
		auto ms=std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		return DoWait(cs.Underlying(),ms);
	}

	/**
	 * Notifes the condition, releasing a thread that is waiting on the condition
	 */
	void Notify()const noexcept
	{
		::WakeConditionVariable(&m_Condition);
	}

	/**
	 * Notifes the condition, releasing all threads that are waiting on the condition
	 */
	void NotifyAll()const noexcept
	{
		::WakeAllConditionVariable(&m_Condition);
	}
};

} // end of namespace