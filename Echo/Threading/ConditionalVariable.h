#pragma once

#include <Echo/WinInclude.h>
#include <Echo/Exceptions.h>
#include <Echo/Threading/CriticalSection.h>

namespace Echo { namespace Threading {

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
	ConditionalVariable()
	{
		::InitializeConditionVariable(&m_Condition);
	}

	ConditionalVariable(const ConditionalVariable&)=delete;
	ConditionalVariable &operator=(const ConditionalVariable &)=delete;

	~ConditionalVariable()
	{
	}

	bool Wait(const CriticalSection &cs)const
	{
		return DoWait(cs.Underlying(),Infinite);
	}

	bool Wait(const CriticalSection &cs, const std::chrono::milliseconds &milliseconds)const
	{
		return DoWait(cs.Underlying(),milliseconds);
	}

	template<typename REP, typename PERIOD>
	bool Wait(const CriticalSection &cs, const std::chrono::duration<REP,PERIOD> &duration)const
	{
		auto ms=std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		return DoWait(cs.Underlying(),ms);
	}

	void Notify()const
	{
		::WakeConditionVariable(&m_Condition);
	}

	void NotifyAll()const
	{
		::WakeAllConditionVariable(&m_Condition);
	}

};

}} // end of namespace