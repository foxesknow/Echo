#pragma once

#include "WinInclude.h"

namespace Echo{

class HandleNull
{
public:
	static HANDLE InvalidValue()
	{
		return nullptr;
	}

	static void Destroy(HANDLE handle)
	{
		if(handle!=InvalidValue())
		{
			::CloseHandle(handle);
		}
	}
};

class HandleInvalidValue
{
public:
	static HANDLE InvalidValue()
	{
		return INVALID_HANDLE_VALUE;
	}

	static void Destroy(HANDLE handle)
	{
		if(handle!=InvalidValue())
		{
			::CloseHandle(handle);
		}
	}
};


} // end of namespace