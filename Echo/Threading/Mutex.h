#pragma once

#include <Echo/WaitHandle.h>
#include <Echo/HandleTraits.h>
#include <Echo/tstring.h>

#include "Lock.h"
#include "ThreadException.h"

#include <utility>

namespace Echo { namespace Threading {

/**
 * Wraps a mutex
 */
class Mutex : public WaitHandleImpl<HandleNull>
{
private:
	typedef WaitHandleImpl<HandleNull> Base;

protected:
	Mutex(HANDLE handle) : 	Base(handle)
	{
	}

public:
	/**
	 * Initializes the instance
	 */
	Mutex()
	{
		auto handle=::CreateMutex(nullptr,FALSE,nullptr);
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("could not create mutex"));

		UnderlyingHandle(handle);
	}

	Mutex(const tstd::tstring &mutexName)
	{
		auto handle=::CreateMutex(nullptr,FALSE,mutexName.c_str());
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("could not create mutex"));

		UnderlyingHandle(handle);
	}

	/**
	 * Initializes the instance via a move
	 */
	Mutex(Mutex &&rhs) : Base(std::move(rhs))
	{
	}

	Mutex(const Mutex&)=delete;
	Mutex &operator=(const Mutex&)=delete;

	/**
	 * Assigns the mutex via a move
	 */
	Mutex &operator=(Mutex &&rhs)
	{
		if(this!=&rhs)
		{
			Swap(rhs);
			rhs.Close();
		}

		return *this;
	}

	/**
	 * Releases a mutex acquired with one of the wait methods
	 */
	void Release() const
	{
		auto handle=UnderlyingHandle();
		
		auto success=::ReleaseMutex(handle);
		if(!success) throw ThreadException(_T("Release failed"));
	}

	/**
	 * Creates a mutex from an existing handle
	 */
	static Mutex Attach(HANDLE handle)
	{
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("invalid handle"));

		return Mutex(handle);
	}
};

/**
 * Locks a mutex
 */
template<>
class Lock<Mutex>
{
private:
	const Mutex &m_Mutex;

public:
	/**
	 * Initializes the instance my waiting on the mutex
	 */
	explicit Lock(const Mutex &mutex) : m_Mutex(mutex)
	{
		m_Mutex.Wait();
	}

	Lock(const Lock &)=delete;
	Lock &operator=(Lock &)=delete;

	/**
	 * Destroys the instance by releasing the mutex
	 */
	~Lock()
	{
		m_Mutex.Release();
	}
};


}} // end of namespace