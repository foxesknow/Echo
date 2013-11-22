#pragma once

#include <Echo/WinInclude.h>
#include <Echo/Handle.h>
#include <Echo/HandleTraits.h>
#include <Echo/Exceptions.h>

#include <chrono>

namespace Echo{

const std::chrono::milliseconds Infinite(INFINITE);

class WaitHandle : public Handle
{
private:
	bool DoWait(const std::chrono::milliseconds &milliseconds)const
	{
		DWORD ms=static_cast<DWORD>(milliseconds.count());
		auto outcome=::WaitForSingleObject(UnderlyingHandle(),ms);

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
	WaitHandle(HANDLE handle) : Handle(handle)
	{
	}

	WaitHandle(WaitHandle &&rhs) : Handle(std::move(rhs))
	{
	}

	WaitHandle(const WaitHandle &)=delete;
	WaitHandle &operator=(const WaitHandle &)=delete;

public:

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
		Traits::Destroy(UnderlyingHandle());
		UnderlyingHandle(Traits::InvalidValue());
	}

public:
	virtual ~WaitHandleImpl()override
	{
		auto handle=UnderlyingHandle();
		Traits::Destroy(handle);
	}
};


} // end of namespace