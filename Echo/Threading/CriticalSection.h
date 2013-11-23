#pragma once

#include <Echo/WinInclude.h>
#include <Echo/Threading/Lock.h>


namespace Echo { namespace Threading {

/**
 * Wraps a critical section
 */
class CriticalSection
{
private:
	mutable CRITICAL_SECTION m_Section;

public:
	/**
	 * Initializes the instance
	 */
	CriticalSection() ECHO_NOEXCEPT
	{
		::InitializeCriticalSection(&m_Section);
	}

	CriticalSection(const CriticalSection &)=delete;

	/**
	 * Destroys the instance
	 */
	~CriticalSection() ECHO_NOEXCEPT
	{
		::DeleteCriticalSection(&m_Section);
	}

	CriticalSection &operator=(const CriticalSection &) = delete;

	/**
	 * Enters the critical section
	 */
	void Enter()const ECHO_NOEXCEPT
	{
		::EnterCriticalSection(&m_Section);
	}

	/**
	 * Attempts to enter the critical section
	 * @returns true if the critical section was entered, otherwise false
	 */
	bool TryEnter()const ECHO_NOEXCEPT
	{
		return ::TryEnterCriticalSection(&m_Section)!=FALSE;
	}

	/**
	 * Exits the critical section
	 */
	void Exit()const ECHO_NOEXCEPT
	{
		::LeaveCriticalSection(&m_Section);
	}

	/**
	 * Returns the underlying critical section
	 */
	CRITICAL_SECTION *Underlying()const ECHO_NOEXCEPT
	{
		return &m_Section;
	}
};

/**
 * Locks a critical section
 */
template<>
class Lock<CriticalSection>
{
private:
	const CriticalSection &m_Section;

public:
	/**
	 * Initializes the lock by entering the critical section
	 */
	Lock(const CriticalSection &section) : m_Section(section)
	{
		m_Section.Enter();
	}

	Lock(const Lock &)=delete;

	Lock &operator=(Lock &)=delete;

	/**
	 * Destroys the lock be exitting the critical section
	 */
	~Lock() ECHO_NOEXCEPT
	{
		m_Section.Exit();
	}
};


/**
 * Unlocks a critical section
 */
template<>
class Unlock<CriticalSection>
{
private:
	const CriticalSection &m_Section;

public:
	/**
	 * Initializes the instance by exitting the critical section
	 */
	Unlock(const CriticalSection &section) : m_Section(section)
	{
		m_Section.Exit();
	}

	Unlock(const Unlock &)=delete;

	Unlock &operator=(Unlock &)=delete;

	/**
	 * Destroys the instance by entering the critical section
	 */
	~Unlock()
	{
		m_Section.Enter();
	}
};

template<>
class TryLock<CriticalSection>
{
private:
	const CriticalSection &m_Section;
	const bool m_Locked;

public:
	/**
	 * Initializes the instance by attempting to enter a critical section
	 */
	TryLock(const CriticalSection &section) : m_Section(section), m_Locked(section.TryEnter())
	{
	}

	TryLock(const TryLock &)=delete;
	TryLock &operator=(TryLock &)=delete;

	/**
	 * Inidicates if the critical section was locked
	 */
	bool IsLocked()const
	{
		return m_Locked;
	}

	/**
	 * Destroys the instance exitting the section if it was entered
	 */
	~TryLock()
	{
		if(m_Locked)
		{
			m_Section.Exit();
		}
	}
};

}} // end of namespace