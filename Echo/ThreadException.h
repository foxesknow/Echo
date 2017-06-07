#pragma once

#include "Exceptions.h"

namespace Echo 
{

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

} // end of namespace