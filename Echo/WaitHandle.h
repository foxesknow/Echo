#pragma once

#include <Echo/WinInclude.h>
#include <Echo/Handle.h>
#include <Echo/HandleTraits.h>
#include <Echo/Exceptions.h>

#include <chrono>
#include <utility>

namespace Echo{

const std::chrono::milliseconds Infinite(INFINITE);

class WaitHandle : public Handle
{
private:
	bool DoWait(const std::chrono::milliseconds &milliseconds) const
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
	WaitHandle(HANDLE handle) : Handle(handle) ECHO_NOEXCEPT
	{
	}

	WaitHandle(WaitHandle &&rhs)=delete;
	WaitHandle(const WaitHandle &)=delete;
	WaitHandle &operator=(const WaitHandle &)=delete;

public:

	bool Wait()const
	{
		return DoWait(Infinite);
	}

	bool Wait(const std::chrono::milliseconds &milliseconds) const
	{
		return DoWait(milliseconds);
	}

	template<typename REP, typename PERIOD>
	bool Wait(const std::chrono::duration<REP,PERIOD> &duration) const
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
	WaitHandleImpl() : WaitHandle(TRAITS::InvalidValue()) ECHO_NOEXCEPT
	{
	}

	WaitHandleImpl(HANDLE handle) : WaitHandle(handle) ECHO_NOEXCEPT
	{
	}

	WaitHandleImpl(WaitHandleImpl &&rhs) : WaitHandle(TRAITS::InvalidValue()) ECHO_NOEXCEPT
	{
		Swap(rhs);
	}

	WaitHandleImpl(const WaitHandleImpl &)=delete;
	WaitHandleImpl operator=(const WaitHandleImpl &)=delete;	

	void Close() ECHO_NOEXCEPT
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

	HANDLE Detach() ECHO_NOEXCEPT
	{
		HANDLE handle=UnderlyingHandle();
		UnderlyingHandle(Traits::InvalidValue());

		return handle;
	}
};


} // end of namespace