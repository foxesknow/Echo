#pragma once

#include <Echo/WinInclude.h>
#include <Echo/Handle.h>
#include <Echo/HandleTraits.h>
#include <Echo/Exceptions.h>

#include <chrono>
#include <utility>

namespace Echo{

const std::chrono::milliseconds Infinite(INFINITE);

/**
 * Base class for waitable handles
 */
class WaitHandle : public Handle
{
private:
	/**
	 * Waits on a handle
	 * @param milliseconds  how long to wait
	 * @returns true if the handle was signalled in the time specified, otherwise false
	 */
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
	/// Initializes the instance
	/// @param handle  the handle to attach to the instance
	WaitHandle(HANDLE handle) : Handle(handle) ECHO_NOEXCEPT
	{
	}

	WaitHandle(WaitHandle &&rhs)=delete;
	WaitHandle(const WaitHandle &)=delete;
	WaitHandle &operator=(const WaitHandle &)=delete;

public:

	/**
	 * Waits forever for a handle to be signalled
	 */
	void Wait()const
	{
		DoWait(Infinite);
	}

	/**
	 * Waits for the handle to be signalled
	 * @param milliseconds  how long to wait
	 * @returns true if the handle is signalled, false otherwise
	 */
	bool Wait(const std::chrono::milliseconds &milliseconds) const
	{
		return DoWait(milliseconds);
	}

	/**
	 * Waits for the handle to be signalled
	 * @param duration  how long to wait
	 * @returns true if the handle is signalled, false otherwise
	 */
	template<typename REP, typename PERIOD>
	bool Wait(const std::chrono::duration<REP,PERIOD> &duration) const
	{
		auto asMilliseconds=std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		auto ms=static_cast<DWORD>(asMilliseconds.count());
		return DoWait(ms);
	}
};

/**
 * Templated wait handle that allows you to specify what an invalid
 * handle is and how the handle should be closed
 */
template<class TRAITS>
class WaitHandleImpl : public WaitHandle
{
public:
	typedef TRAITS Traits;

protected:
	/**
	 * Initializes the instance with an invalid handle
	 */
	WaitHandleImpl() : WaitHandle(TRAITS::InvalidValue()) ECHO_NOEXCEPT
	{
	}

	
	/**
	 * Initializes the instace
	 * @param handle  the handle to attach to the instance
	 */
	WaitHandleImpl(HANDLE handle) : WaitHandle(handle) ECHO_NOEXCEPT
	{
	}

	/**
	 * Moves an instance
	 * @param rhs  the instance to move into our instance
	 */
	WaitHandleImpl(WaitHandleImpl &&rhs) : WaitHandle(TRAITS::InvalidValue()) ECHO_NOEXCEPT
	{
		Swap(rhs);
	}

	WaitHandleImpl(const WaitHandleImpl &)=delete;
	WaitHandleImpl operator=(const WaitHandleImpl &)=delete;	

	/**
	 * Closes the handle, if open
	 */
	void Close() ECHO_NOEXCEPT
	{
		Traits::Destroy(UnderlyingHandle());
		UnderlyingHandle(Traits::InvalidValue());
	}

public:
	/**
	 * Closes the handle, if open
	 */
	virtual ~WaitHandleImpl()override
	{
		auto handle=UnderlyingHandle();
		Traits::Destroy(handle);
	}

	/**
	 * Sets the handle to an invalid value and returns the OS value
	 * @returns the OS handle
	 */
	virtual HANDLE Detach() override ECHO_NOEXCEPT
	{
		HANDLE handle=UnderlyingHandle();
		UnderlyingHandle(Traits::InvalidValue());

		return handle;
	}
};


} // end of namespace