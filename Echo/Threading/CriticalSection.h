#pragma once

#include <Echo/WinInclude.h>
#include <Echo/Threading/Lock.h>


namespace Echo { namespace Threading {

class CriticalSection
{
private:
	mutable CRITICAL_SECTION m_Section;

public:
	CriticalSection() ECHO_NOEXCEPT
	{
		::InitializeCriticalSection(&m_Section);
	}

	CriticalSection(const CriticalSection &)=delete;

	~CriticalSection() ECHO_NOEXCEPT
	{
		::DeleteCriticalSection(&m_Section);
	}

	CriticalSection &operator=(const CriticalSection &) = delete;

	void Enter()const ECHO_NOEXCEPT
	{
		::EnterCriticalSection(&m_Section);
	}

	bool TryEnter()const ECHO_NOEXCEPT
	{
		return ::TryEnterCriticalSection(&m_Section)!=FALSE;
	}

	void Exit()const ECHO_NOEXCEPT
	{
		::LeaveCriticalSection(&m_Section);
	}

	CRITICAL_SECTION *Underlying()const ECHO_NOEXCEPT
	{
		return &m_Section;
	}
};

template<>
class Lock<CriticalSection>
{
private:
	const CriticalSection &m_Section;

public:
	Lock(const CriticalSection &section) : m_Section(section)
	{
		m_Section.Enter();
	}

	Lock(const Lock &)=delete;

	Lock &operator=(Lock &)=delete;

	~Lock() ECHO_NOEXCEPT
	{
		m_Section.Exit();
	}
};



template<>
class Unlock<CriticalSection>
{
private:
	const CriticalSection &m_Section;

public:
	Unlock(const CriticalSection &section) : m_Section(section)
	{
		m_Section.Exit();
	}

	Unlock(const Unlock &)=delete;

	Unlock &operator=(Unlock &)=delete;

	~Unlock()
	{
		m_Section.Enter();
	}
};

}} // end of namespace