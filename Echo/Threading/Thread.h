#pragma once

#include <Echo/WaitHandle.h>
#include <Echo/HandleTraits.h>
#include <Echo/Exceptions.h>

#include "ThreadException.h"

#include <functional>
#include <process.h>

namespace Echo { namespace Threading {

//
//
//
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
	Thread()
	{
	}

	Thread(std::function<void()> threadFunction) : m_ThreadFunction(threadFunction)
	{

	}

	Thread(Thread &&rhs) : Base(std::move(rhs)), m_ThreadFunction(std::move(rhs.m_ThreadFunction))
	{
	}

	Thread(const Thread&)=delete;

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

	void Start()
	{
		if(UnderlyingHandle()!=Traits::InvalidValue()) throw ThreadException(_T("thread already started"));

		auto handle=::_beginthreadex(nullptr,0,ThreadMain,this,0,nullptr);
		UnderlyingHandle(reinterpret_cast<HANDLE>(handle));
	}

	void Suspend()const
	{
		DWORD success=::SuspendThread(UnderlyingHandle());
		if(success==static_cast<DWORD>(-1)) throw ThreadException(_T("suspend failed"));
	}

	void Resume()const
	{
		DWORD success=::ResumeThread(UnderlyingHandle());
		if(success==static_cast<DWORD>(-1)) throw ThreadException(_T("resume failed"));
	}

	bool Started()const
	{
		return UnderlyingHandle()!=Traits::InvalidValue();
	}
};


}} // end of namespace