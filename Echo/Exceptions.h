#pragma once

#include "WinInclude.h"
#include "Exceptions.h"
#include "tstring.h"

namespace Echo{

class Exception
{
private:
	const tstd::tstring m_Message;

public:
	/**
	 * Initializes the instance
	 */
	Exception()
	{
	}

	/**
	 * Initializes the instance
	 */
	Exception(const tstd::tstring &message) : m_Message(message)
	{
	}

	/**
	 * The reason for the exception
	 */
	const tstd::tstring &Message()const
	{
		return m_Message;
	}
};

class ArgumentException : public Exception
{
public:
	ArgumentException()
	{
	}

	ArgumentException(const tstd::tstring &message) : Exception(message)
	{
	}
};

class ArgumentNullException : public Exception
{
public:
	ArgumentNullException()
	{
	}

	ArgumentNullException(const tstd::tstring &message) : Exception(message)
	{
	}
};


/**
 * Base class for all Win32 api related errors
 */
class WindowsException : public Exception
{
private:
	const tstd::tstring m_Reason;
	DWORD m_ErrorCode;
	mutable tstd::tstring m_ErrorCodeAsString;

public:
	/**
	 * Initializes the instance with the value returned from GetLastError
	 */
	WindowsException()
	{
		m_ErrorCode=::GetLastError();
	}

	/**
	 * Initializes the instance with the value returned from GetLastError and a message 
	 */
	WindowsException(const tstd::tstring &message) : Exception(message)
	{
		m_ErrorCode=::GetLastError();
	}

	/**
	 * Returns the Win32 error code for the exception
	 */
	DWORD ErrorCode()const
	{
		return m_ErrorCode;
	}

	/**
	 * Creates a HRESULT which wraps the Win32 error
	 */
	HRESULT AsHRESULT()const
	{
		return HRESULT_FROM_WIN32(m_ErrorCode);
	}

	/**
	 * Returns a textual description of the Win32 error code
	 */
	const tstd::tstring &ErrorCodeAsString() const
	{
		if(m_ErrorCode==0 || m_ErrorCodeAsString.length()!=0) return m_ErrorCodeAsString;

		LPTSTR buffer=nullptr;
		DWORD bufferLength=::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
							nullptr,m_ErrorCode,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
							(LPTSTR)&buffer,0,nullptr);
		if(bufferLength)
		{
			LPCSTR lpMsgStr = (LPCSTR)buffer;
			tstd::tstring result(buffer,buffer+bufferLength);
      
			::LocalFree(buffer);

			swap(m_ErrorCodeAsString,result);
		}

		return m_ErrorCodeAsString;
	}
};


} // end of namespace