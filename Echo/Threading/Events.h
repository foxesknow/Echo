#pragma once

#include <Echo/WaitHandle.h>
#include <Echo/HandleTraits.h>

#include "ThreadException.h"

#include <utility>

namespace Echo { namespace Threading {

//
// Specifies the initial state for an event
//
enum class InitialState
{
	NonSignalled,
	Signalled
};

//
// Base class for all events
//
class Event : public WaitHandleImpl<HandleNull>
{
private:
	typedef WaitHandleImpl<HandleNull> Base;

protected:
	Event(BOOL isManual, InitialState initialState)
	{
		BOOL signalled=(initialState==InitialState::Signalled ? TRUE : FALSE);
		auto handle=::CreateEvent(nullptr,isManual,signalled,nullptr);

		if(handle==Traits::InvalidValue())
		{
			throw WindowsException(_T("could not create event"));
		}

		UnderlyingHandle(handle);
	}

	Event(Event &&rhs) : Base(std::move(rhs))
	{
	}

	Event(HANDLE handle) : Base(handle)
	{
	}

public:
	void Set()const
	{
		::SetEvent(UnderlyingHandle());
	}

	void Reset()const
	{
		::ResetEvent(UnderlyingHandle());
	}
};

//
// Manual reset event
//
class ManualResetEvent final : public Event
{
private:
	typedef Event Base;

protected:
	ManualResetEvent(HANDLE handle) : Event(handle)
	{
	}

public:
	explicit ManualResetEvent(InitialState initialState) : Event(TRUE,initialState)
	{
	}

	ManualResetEvent(ManualResetEvent &&rhs) : Base(std::move(rhs))
	{
	}

	ManualResetEvent(const ManualResetEvent&)=delete;

	ManualResetEvent &operator=(ManualResetEvent &&rhs)
	{
		if(this!=&rhs)
		{
			Swap(rhs);
			rhs.Close();
		}

		return *this;
	}

	static ManualResetEvent Attach(HANDLE handle)
	{
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("invalid handle"));

		return ManualResetEvent(handle);
	}
};

//
// Auto reset event
//
class AutoResetEvent final : public Event
{
private:
	typedef Event Base;

protected:
	AutoResetEvent(HANDLE handle) : Event(handle)
	{
	}

public:
	explicit AutoResetEvent(InitialState initialState) : Event(FALSE,initialState)
	{
	}

	AutoResetEvent(AutoResetEvent &&rhs) : Base(std::move(rhs))
	{
	}

	AutoResetEvent(const AutoResetEvent&)=delete;

	AutoResetEvent &operator=(AutoResetEvent &&rhs)
	{
		if(this!=&rhs)
		{
			Swap(rhs);
			rhs.Close();
		}

		return *this;
	}

	static AutoResetEvent Attach(HANDLE handle)
	{
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("invalid handle"));

		return AutoResetEvent(handle);
	}
};

}} // end of namespace