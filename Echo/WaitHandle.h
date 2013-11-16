#pragma once

#include <Echo/WinInclude.h>
#include <Echo/HandleTraits.h>
#include <Echo/Exceptions.h>

#include <chrono>

namespace Echo{

const std::chrono::milliseconds Infinite(INFINITE);

class WaitHandle
{

private:
	HANDLE m_Handle;

	bool DoWait(const std::chrono::milliseconds &milliseconds)const
	{
		DWORD ms=static_cast<DWORD>(milliseconds.count());
		auto outcome=::WaitForSingleObject(m_Handle,ms);

		if(outcome==WAIT_OBJECT_0) return true;
		if(outcome==WAIT_TIMEOUT) return false;

		if(outcome==WAIT_ABANDONED)
		{
			throw WindowsException(_T("handle abandoned"));
		}
		else
		{
			throw WindowsException(_T("failed"));
		}
	}

protected:
	WaitHandle(HANDLE handle) : m_Handle(handle)
	{
	}

	WaitHandle(WaitHandle &&rhs)
	{
		Swap(rhs);
	}

	WaitHandle(const WaitHandle &)=delete;
	WaitHandle &operator=(const WaitHandle &)=delete;

	HANDLE Handle()const
	{
		return m_Handle;
	}

	void Handle(HANDLE value)
	{
		m_Handle=value;
	}

	void Swap(WaitHandle &rhs)
	{
		std::swap(m_Handle,rhs.m_Handle);
	}

public:
	virtual ~WaitHandle()=0
	{
		// It's down to the derived class to know how to release the handle
	}

	bool Wait()const
	{
		return DoWait(Infinite);
	}

	bool Wait(const std::chrono::milliseconds &milliseconds)const
	{
		return DoWait(milliseconds);
	}

	template<typename REP, typename PERIOD>
	bool Wait(const std::chrono::duration<REP,PERIOD> &duration)const
	{
		auto asMilliseconds=std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		auto ms=static_cast<DWORD>(asMilliseconds.count());
		return DoWait(ms);
	}
};

template<class TRAITS>
class WaitHandleImpl : public WaitHandle
{
public:
	typedef TRAITS Traits;

protected:
	WaitHandleImpl() : WaitHandle(TRAITS::InvalidValue())
	{
	}

	WaitHandleImpl(HANDLE handle) : WaitHandle(handle)
	{
	}

	WaitHandleImpl(WaitHandleImpl &&rhs) : WaitHandle(std::move(rhs))
	{
	}

	WaitHandleImpl(const WaitHandleImpl &)=delete;
	WaitHandleImpl operator=(const WaitHandleImpl &)=delete;	

	void Close()
	{
		Traits::Destroy(Handle());
		Handle(Traits::InvalidValue());
	}

public:
	virtual ~WaitHandleImpl()override
	{
		auto handle=Handle();
		Traits::Destroy(handle);
	}
};


} // end of namespace