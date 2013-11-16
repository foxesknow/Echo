#pragma once

#include <Echo/Exceptions.h>
#include <Echo/tstring.h>

namespace Echo{

class Exception
{
private:
	const std::tstring m_Reason;

public:
	Exception()
	{
	}

	Exception(const std::tstring &reason) : m_Reason(reason)
	{
	}

	const std::tstring &Reason()const
	{
		return m_Reason;
	}
};

class WindowsException : Exception
{
private:
	const std::tstring m_Reason;
	DWORD m_ErrorCode;

public:
	WindowsException()
	{
		m_ErrorCode=::GetLastError();
	}

	WindowsException(const std::tstring &reason) : Exception(reason)
	{
		m_ErrorCode=::GetLastError();
	}

	DWORD ErrorCode()const
	{
		return m_ErrorCode;
	}

	HRESULT AsHRESULT()const
	{
		return HRESULT_FROM_WIN32(m_ErrorCode);
	}
};


} // end of namespace