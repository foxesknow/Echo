#pragma once

#include <Echo/WaitHandle.h>
#include <Echo/HandleTraits.h>

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

		Handle(handle);
	}

	Event(Event &&rhs) : Base(std::move(rhs))
	{
	}

public:
	void Set()const
	{
		::SetEvent(Handle());
	}

	void Reset()const
	{
		::ResetEvent(Handle());
	}
};

//
// Manual reset event
//
class ManualResetEvent : public Event
{
private:
	typedef Event Base;

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
};

//
// Auto reset event
//
class AutoResetEvent : public Event
{
private:
	typedef Event Base;

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
};

}} // end of namespace