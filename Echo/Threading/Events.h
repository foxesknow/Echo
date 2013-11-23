#pragma once

#include <Echo/WaitHandle.h>
#include <Echo/HandleTraits.h>

#include "ThreadException.h"

#include <utility>

namespace Echo { namespace Threading {

/**
 * Specifies the initial state for an event
 */
enum class InitialState
{
	NonSignalled,
	Signalled
};

/**
 * Base class for all events
 */
class Event : public WaitHandleImpl<HandleNull>
{
private:
	typedef WaitHandleImpl<HandleNull> Base;

protected:
	/**
	 * Initializes the instance
	 * @param isManual  true for a manual reset event, false for an auto reset event
	 * @param initialState  the initial state for the event
	 */
	Event(bool isManual, InitialState initialState)
	{
		BOOL signalled=(initialState==InitialState::Signalled ? TRUE : FALSE);
		auto handle=::CreateEvent(nullptr,isManual,signalled,nullptr);

		if(handle==Traits::InvalidValue())
		{
			throw WindowsException(_T("could not create event"));
		}

		UnderlyingHandle(handle);
	}

	/**
	 * Initializes the instance
	 */
	Event(Event &&rhs) : Base(std::move(rhs))
	{
	}

	/**
	 * Initializes the instance
	 * @param handle  an existing handle to attach to the event
	 */
	Event(HANDLE handle) : Base(handle)
	{
	}

public:
	/**
	 * Sets the event
	 */
	void Set()const
	{
		::SetEvent(UnderlyingHandle());
	}

	/**
	 * Resets the event
	 */
	void Reset()const
	{
		::ResetEvent(UnderlyingHandle());
	}

	static Event Attach(HANDLE handle)
	{
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("invalid handle"));

		return Event(handle);
	}
};

/**
 * Manual reset event
 */
class ManualResetEvent final : public Event
{
private:
	typedef Event Base;

protected:
	/**
	 * Initializes the instance
	 * @param handle  an existing handle to attach
	 */
	ManualResetEvent(HANDLE handle) : Event(handle)
	{
	}

public:
	/**
	 * Initializes the instance
	 * @param initialState  the initial state of the event
	 */
	explicit ManualResetEvent(InitialState initialState) : Event(true,initialState)
	{
	}

	/**
	 * Initializes the instance
	 */
	ManualResetEvent(ManualResetEvent &&rhs) : Base(std::move(rhs))
	{
	}

	ManualResetEvent(const ManualResetEvent&)=delete;

	/**
	 * Moves an event into the instance
	 */
	ManualResetEvent &operator=(ManualResetEvent &&rhs)
	{
		if(this!=&rhs)
		{
			Swap(rhs);
			rhs.Close();
		}

		return *this;
	}

	/**
	 * Creates an event from an existing handle
	 */
	static ManualResetEvent Attach(HANDLE handle)
	{
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("invalid handle"));

		return ManualResetEvent(handle);
	}
};

/**
 * Auto reset event
 */
class AutoResetEvent final : public Event
{
private:
	typedef Event Base;

protected:
	/**
	 * Initializes the instance
	 * @param handle  an existing handle to attach
	 */
	AutoResetEvent(HANDLE handle) : Event(handle)
	{
	}

public:
	/**
	 * Initializes the instance
	 * @param initialState  the initial state of the event
	 */
	explicit AutoResetEvent(InitialState initialState) : Event(false,initialState)
	{
	}

	/**
	 * Initializes the instance
	 */
	AutoResetEvent(AutoResetEvent &&rhs) : Base(std::move(rhs))
	{
	}
	
	AutoResetEvent(const AutoResetEvent&)=delete;

	/**
	 * Moves an event into the instance
	 */
	AutoResetEvent &operator=(AutoResetEvent &&rhs)
	{
		if(this!=&rhs)
		{
			Swap(rhs);
			rhs.Close();
		}

		return *this;
	}

	/**
	 * Creates an event from an existing handle
	 */
	static AutoResetEvent Attach(HANDLE handle)
	{
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("invalid handle"));

		return AutoResetEvent(handle);
	}
};

}} // end of namespace