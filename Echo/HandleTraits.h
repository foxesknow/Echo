#pragma once

#include <Echo/WinInclude.h>

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


} // end of namespace