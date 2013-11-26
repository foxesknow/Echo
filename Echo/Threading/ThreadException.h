#pragma once

#include <Echo/Exceptions.h>

namespace Echo { namespace Threading {

class ThreadException : Exception
{
public:
	ThreadException()
	{
	}

	ThreadException(const tstd::tstring &reason) : Exception(reason)
	{
	}
};

}} // end of namespace