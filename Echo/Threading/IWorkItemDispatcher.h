#pragma once

#include <functional>

namespace Echo { namespace Threading {

/**
 * Interface for a class that is able to process work
 */
class IWorkItemDispatcher
{
public:
	/**
	 * Destroys the instance
	 */
	virtual ~IWorkItemDispatcher()
	{

	}

	virtual void Submit(const std::function<void()> &function)=0;
};

}} // end of namespace