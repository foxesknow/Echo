#pragma once

#include <Echo/Exceptions.h>

namespace Echo { namespace IO {

class IOException : WindowsException
{
public:
	IOException()
	{
	}

	IOException(const tstd::tstring &reason) : WindowsException(reason)
	{
	}
};

}} // end of namespace