#pragma once

#include <Echo\WinInclude.h>
#include <Echo\Exceptions.h>
#include <Echo\tstring.h>

#include <exception>

namespace Echo{

class Exception : public std::exception
{
private:
	const std::string m_Message;

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
	explicit Exception(const tstd::tstring &message) : m_Message(tstd::to_string(message))
	{
	}

	virtual const char *what() const
	{
		return m_Message.c_str();
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

	explicit ArgumentNullException(const tstd::tstring &message) : Exception(message)
	{
	}
};

class ThreadException : public Exception
{
public:
	ThreadException()
	{
	}

	ThreadException(const tstd::tstring &reason) : Exception(reason)
	{
	}
};


/**
 * Base class for all Win32 api related errors
 */
class WindowsException : public Exception
{
private:
	DWORD m_ErrorCode;
	mutable tstd::tstring m_ErrorCodeAsString;

public:
	/**
	 * Initializes the instance with the value returned from GetLastError
	 */
	WindowsException()
	{
		m_ErrorCode = ::GetLastError();
	}

	/**
	 * Initializes the instance with the value returned from GetLastError and a message 
	 */
	explicit WindowsException(const tstd::tstring &message) : Exception(message)
	{
		m_ErrorCode = ::GetLastError();
	}

	/**
	 * Returns the Win32 error code for the exception
	 */
	DWORD ErrorCode() const
	{
		return m_ErrorCode;
	}

	/**
	 * Creates a HRESULT which wraps the Win32 error
	 */
	HRESULT AsHRESULT() const
	{
		return HRESULT_FROM_WIN32(m_ErrorCode);
	}

	/**
	 * Returns a textual description of the Win32 error code
	 */
	const tstd::tstring &ErrorCodeAsString() const
	{
		if(m_ErrorCode == 0 || m_ErrorCodeAsString.length() != 0) return m_ErrorCodeAsString;

		LPTSTR buffer=nullptr;
		DWORD bufferLength = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
											nullptr,
											m_ErrorCode,
											MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
											(LPTSTR)&buffer,
											0,
											nullptr);
		if(bufferLength)
		{
			LPCSTR lpMsgStr = (LPCSTR)buffer;
			tstd::tstring result(buffer, buffer + bufferLength);
      
			::LocalFree(buffer);

			m_ErrorCodeAsString = result;
		}

		return m_ErrorCodeAsString;
	}
};

class IOException : public WindowsException
{
public:
	IOException()
	{
	}

	IOException(const tstd::tstring &reason) : WindowsException(reason)
	{
	}
};


} // end of namespace