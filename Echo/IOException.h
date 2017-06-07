#pragma once

#include "Exceptions.h"

namespace Echo 
{

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