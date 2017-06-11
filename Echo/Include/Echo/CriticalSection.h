#pragma once

#include <Echo\WinInclude.h>
#include <Echo\Guard.h>

#include <utility>

namespace Echo 
{

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
	CriticalSection() noexcept
	{
		::InitializeCriticalSection(&m_Section);
	}

	CriticalSection(const CriticalSection &) = delete;

	/**
	 * Destroys the instance
	 */
	~CriticalSection() noexcept
	{
		::DeleteCriticalSection(&m_Section);
	}

	CriticalSection &operator=(const CriticalSection &) = delete;
	CriticalSection &operator=(CriticalSection &&) = delete;

	/**
	 * Enters the critical section
	 */
	void Enter() noexcept
	{
		::EnterCriticalSection(&m_Section);
	}

	/**
	 * Attempts to enter the critical section
	 * @returns true if the critical section was entered, otherwise false
	 */
	bool TryEnter() noexcept
	{
		return ::TryEnterCriticalSection(&m_Section)!=FALSE;
	}

	/**
	 * Exits the critical section
	 */
	void Exit() noexcept
	{
		::LeaveCriticalSection(&m_Section);
	}

	/**
	 * Returns the underlying critical section
	 */
	CRITICAL_SECTION *Underlying() noexcept
	{
		return &m_Section;
	}
};

/**
 * Locks a critical section
 */
template<>
class Guard<CriticalSection>
{
private:
	CriticalSection &m_Section;

public:
	/**
	 * Initializes the lock by entering the critical section
	 */
	explicit Guard(CriticalSection &section) : m_Section(section)
	{
		m_Section.Enter();
	}

	Guard(const Guard &) = delete;
	Guard &operator=(const Guard &) = delete;
	Guard &operator=(Guard &&) = delete;

	/**
	 * Destroys the lock be exitting the critical section
	 */
	~Guard() noexcept
	{
		m_Section.Exit();
	}
};


/**
 * Unlocks a critical section
 */
template<>
class Unguard<CriticalSection>
{
private:
	CriticalSection &m_Section;

public:
	/**
	 * Initializes the instance by exitting the critical section
	 */
	Unguard(CriticalSection &section) : m_Section(section)
	{
		m_Section.Exit();
	}

	Unguard(const Unguard &) = delete;
	Unguard &operator=(const Unguard &) = delete;
	Unguard &operator=(Unguard &&) = delete;

	/**
	 * Destroys the instance by entering the critical section
	 */
	~Unguard()
	{
		m_Section.Enter();
	}
};

template<>
class TryGuard<CriticalSection>
{
private:
	CriticalSection &m_Section;
	const bool m_Locked;

public:
	/**
	 * Initializes the instance by attempting to enter a critical section
	 */
	TryGuard(CriticalSection &section) : m_Section(section), m_Locked(section.TryEnter())
	{
	}

	TryGuard(const TryGuard &) = delete;
	TryGuard &operator=(const TryGuard &) = delete;
	TryGuard &operator=(TryGuard &&) = delete;

	/**
	 * Inidicates if the critical section was locked
	 */
	bool IsLocked() const
	{
		return m_Locked;
	}

	/**
	 * Destroys the instance exitting the section if it was entered
	 */
	~TryGuard()
	{
		if(m_Locked)
		{
			m_Section.Exit();
		}
	}
};

template<>
class UniqueGuard<CriticalSection>
{
private:
	CriticalSection *m_Section;

public:
	/**
	 * Initializes the lock by entering the critical section
	 */
	explicit UniqueGuard(CriticalSection &section) : m_Section(&section)
	{
		m_Section->Enter();
	}

	UniqueGuard(UniqueGuard &&rhs) : m_Section(nullptr)
	{
		std::swap(m_Section, rhs.m_Section);
	}

	UniqueGuard(const UniqueGuard &) = delete;
	
	UniqueGuard &operator=(UniqueGuard &&rhs) noexcept
	{
		if(this != &rhs)
		{
			std::swap(m_Section, rhs.m_Section);
		}

		return *this;
	}

	/**
	 * Destroys the lock be exitting the critical section
	 */
	~UniqueGuard() noexcept
	{
		if(m_Section) m_Section->Exit();
	}
};

using CriticalSectionGuard=Guard<CriticalSection>;
using CriticalSectionUniqueGuard=UniqueGuard<CriticalSection>;

} // end of namespace