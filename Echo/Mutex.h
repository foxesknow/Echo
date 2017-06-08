#pragma once

#include "WaitHandle.h"
#include "HandleTraits.h"
#include "tstring.h"

#include "Guard.h"
#include "ThreadException.h"

#include <utility>

namespace Echo 
{

enum class Ownership
{
	Owned,
	NotOwned
};

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
	Mutex(Ownership ownership)
	{
		BOOL isOwned=(ownership==Ownership::Owned ? TRUE : FALSE);
		auto handle=::CreateMutex(nullptr,isOwned,nullptr);
		if(handle==Traits::InvalidValue()) throw ThreadException(_T("could not create mutex"));

		UnderlyingHandle(handle);
	}

	Mutex(Ownership ownership, const tstd::tstring &mutexName)
	{
		BOOL isOwned=(ownership==Ownership::Owned ? TRUE : FALSE);
		auto handle=::CreateMutex(nullptr,isOwned,mutexName.c_str());
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
class Guard<Mutex>
{
private:
	Mutex &m_Mutex;

public:
	/**
	 * Initializes the instance my waiting on the mutex
	 */
	explicit Guard(Mutex &mutex) : m_Mutex(mutex)
	{
		m_Mutex.Wait();
	}

	Guard(const Guard &)=delete;
	Guard &operator=(Guard &)=delete;

	/**
	 * Destroys the instance by releasing the mutex
	 */
	~Guard()
	{
		m_Mutex.Release();
	}
};


} // end of namespace