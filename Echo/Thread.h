#pragma once

#include <Echo/WaitHandle.h>
#include <Echo/HandleTraits.h>
#include <Echo/Exceptions.h>

#include "ThreadException.h"

#include <functional>
#include <process.h>

namespace Echo 
{

/**
 * Thread
 */
class Thread : public WaitHandleImpl<HandleNull>
{
private:
	typedef WaitHandleImpl<HandleNull> Base;

	std::function<void()> m_ThreadFunction;

	static unsigned __stdcall ThreadMain(void *data)
	{
		auto thread=static_cast<Thread*>(data);
		auto function=thread->m_ThreadFunction;
		function();
		return 0;
	}

public:
	/**
	 * Initializes the instance
	 */
	Thread() noexcept
	{
	}

	/**
	 * Initializes the instance
	 * @param threadFunction  the function to execute on the thread
	 */
	Thread(const std::function<void()> &threadFunction) : m_ThreadFunction(threadFunction)
	{

	}

	/**
	 * Initializes the instance
	 * @param rhs  the thread to move into this instance
	 */
	Thread(Thread &&rhs) : Base(std::move(rhs)), m_ThreadFunction(std::move(rhs.m_ThreadFunction))
	{
	}

	Thread(const Thread&)=delete;

	/**
	 * Moves the thread into the instance
	 * If the instance is already running then the instance is closed.
	 * @param rhs  the instance to move
	 */
	Thread &operator=(Thread &&rhs)
	{
		if(this!=&rhs)
		{
			std::swap(m_ThreadFunction,rhs.m_ThreadFunction);
			Swap(rhs);
			rhs.Close();
		}

		return *this;
	}

	/**
	 * Starts the thread
	 */
	void Start()
	{
		if(UnderlyingHandle()!=Traits::InvalidValue()) throw ThreadException(_T("thread already started"));
		if(!m_ThreadFunction) throw ThreadException(_T("no function specified"));

		auto handle=::_beginthreadex(nullptr,0,ThreadMain,this,0,nullptr);
		UnderlyingHandle(reinterpret_cast<HANDLE>(handle));
	}

	/**
	 *Suspends the thread
	 */
	void Suspend()const
	{
		if(UnderlyingHandle()==Traits::InvalidValue()) throw ThreadException(_T("thread not started"));

		DWORD success=::SuspendThread(UnderlyingHandle());
		if(success==static_cast<DWORD>(-1)) throw ThreadException(_T("Suspend failed"));
	}

	/**
	 * Resumes a previously suspended thread
	 */
	void Resume()const
	{
		if(UnderlyingHandle()==Traits::InvalidValue()) throw ThreadException(_T("thread not started"));

		DWORD success=::ResumeThread(UnderlyingHandle());
		if(success==static_cast<DWORD>(-1)) throw ThreadException(_T("Resume failed"));
	}

	/**
	 * Indicates if the thread has been started
	 * @returns true if the thread has been started, otherwise false
	 */
	bool Started()const
	{
		return UnderlyingHandle()!=Traits::InvalidValue();
	}
};


} // end of namespace