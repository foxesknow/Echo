#pragma once

#include <Echo/WinInclude.h>
#include <Echo/Threading/Lock.h>


namespace Echo { namespace Threading {

class CriticalSection
{
private:
	mutable CRITICAL_SECTION m_Section;

public:
	CriticalSection()
	{
		::InitializeCriticalSection(&m_Section);
	}

	CriticalSection(const CriticalSection &)=delete;

	~CriticalSection()
	{
		::DeleteCriticalSection(&m_Section);
	}

	CriticalSection &operator=(const CriticalSection &) = delete;

	void Enter()const
	{
		::EnterCriticalSection(&m_Section);
	}

	bool TryEnter()const
	{
		return ::TryEnterCriticalSection(&m_Section)!=FALSE;
	}

	void Exit()const
	{
		::LeaveCriticalSection(&m_Section);
	}

	CRITICAL_SECTION *Underlying()const
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

	~Lock()
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