#pragma once

#include <functional>

namespace Echo { namespace Threading {

/**
 * Interface for a class that is able execute a function
 */
class IFunctionDispatcher
{
public:
	/**
	 * Destroys the instance
	 */
	virtual ~IFunctionDispatcher()
	{

	}

	virtual void Submit(const std::function<void()> &function)=0;
};

}} // end of namespace