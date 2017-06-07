#pragma once

#include <functional>

namespace Echo 
{


/**
	* Calls a function and throws away any exception that might occur
	* @param function  the function to call
	*/
inline void AllowFail(std::function<void()> function)
{
	try
	{
		function();
	}
	catch(...)
	{
		// Just ignore it
	}
}

} // end of namespace