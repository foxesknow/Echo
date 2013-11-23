#pragma once

#include "WinInclude.h"

class Handle
{
private:
	HANDLE m_Handle;

protected:
	Handle(HANDLE handle) ECHO_NOEXCEPT : m_Handle(handle)
	{
	}

	Handle(Handle &&rhs)=delete;
	Handle(const Handle &)=delete;
	Handle &operator=(const Handle &)=delete;

	void Swap(Handle &rhs) ECHO_NOEXCEPT
	{
		std::swap(m_Handle,rhs.m_Handle);
	}

	void UnderlyingHandle(HANDLE value) ECHO_NOEXCEPT
	{
		m_Handle=value;
	}

public:
	virtual ~Handle()=0
	{
		// It's down to the derived class to know how to release the handle
	}

	HANDLE UnderlyingHandle() const ECHO_NOEXCEPT
	{
		return m_Handle;
	}
};
