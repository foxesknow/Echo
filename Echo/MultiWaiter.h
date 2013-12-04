#pragma once

#include "WinInclude.h"
#include "WaitHandle.h"
#include "Exceptions.h"

#include <vector>

namespace Echo {

/**
 * Allows you to wait on multiple wait handles
 */
class MultiWaiter
{
private:
	std::vector<HANDLE> m_Handles;

public:
	/**
	 * Initializes the instance
	 */
	MultiWaiter()
	{
	}

	/**
	 * Initializes the instance via a move
	 * @param rhs  the instance to move into this instance
	 */
	MultiWaiter(MultiWaiter &&rhs)
	{
		m_Handles.swap(rhs.m_Handles);
	}

	/**
	 * Initializes the instance via a move assignment
	 * @param rhs  the instance to move into this instance
	 */
	MultiWaiter &operator=(MultiWaiter &&rhs)
	{
		if(this!=&rhs)
		{
			m_Handles.swap(rhs.m_Handles);
			rhs.m_Handles.clear();
		}

		return *this;
	}

	MultiWaiter(const MultiWaiter&)=delete;
	MultiWaiter &operator=(const MultiWaiter&)=delete;

	/**
	 * Add a handle to the waiter
	 * @param handle  the handle to add
	 * @returns a reference to this instance
	 */
	MultiWaiter &operator<<(const WaitHandle &handle)
	{
		m_Handles.push_back(handle.UnderlyingHandle());
		return *this;
	}

	/**
	 * Waits forever for all of the handles to be signalled
	 * @returns true if all handles were signalled, otherwise false
	 */
	bool WaitAll() const
	{
		WaitAll(Infinite);
	}

	/**
	 * Waits forever for all of the handles to be signalled
	 * @param duration  how long to wait for
	 * @returns true if all handles were signalled, otherwise false
	 */
	template<typename REP, typename PERIOD>
	bool WaitAll(const std::chrono::duration<REP,PERIOD> &duration) const
	{
		auto asMilliseconds=std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		auto ms=static_cast<DWORD>(asMilliseconds.count());

		const HANDLE *handles=m_Handles.data();
		auto size=m_Handles.size();

		auto outcome=::WaitForMultipleObjects(size,handles,TRUE,ms);
		if(outcome>=WAIT_OBJECT_0 && outcome<(WAIT_OBJECT_0+size))
		{
			// They were all signalled
			return true;
		}
		else if(outcome>=WAIT_ABANDONED_0 && outcome<(WAIT_ABANDONED_0+size))
		{
			throw WindowsException(_T("wait abandoned"));
		}
		else if(outcome==WAIT_FAILED)
		{
			throw WindowsException(_T("failed"));
		}
		else
		{
			return false;
		}
	}

	/**
	 * Waits forever for any of the handles to be signalled
	 * @returns the 0 based index of the lowest signalled handle, of -1 if nothing was signalled
	 */
	int WaitAny() const
	{
		return WaitAny(Infinite);
	}

	/**
	 * Waits forever for any of the handles to be signalled
	 * @param duration  how long to wait for
	 * @returns the 0 based index of the lowest signalled handle, of -1 if nothing was signalled
	 */
	template<typename REP, typename PERIOD>
	int WaitAny(const std::chrono::duration<REP,PERIOD> &duration) const
	{
		auto asMilliseconds=std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		auto ms=static_cast<DWORD>(asMilliseconds.count());

		const HANDLE *handles=m_Handles.data();
		auto size=m_Handles.size();

		auto outcome=::WaitForMultipleObjects(size,handles,FALSE,ms);
		if(outcome>=WAIT_OBJECT_0 && outcome<(WAIT_OBJECT_0+size))
		{
			// They were all signalled
			return static_cast<int>(outcome);
		}
		else if(outcome>=WAIT_ABANDONED_0 && outcome<(WAIT_ABANDONED_0+size))
		{
			throw WindowsException(_T("wait abandoned"));
		}
		else if(outcome==WAIT_FAILED)
		{
			throw WindowsException(_T("failed"));
		}
		else
		{
			return -1;
		}
	}

};

} // end of namespace