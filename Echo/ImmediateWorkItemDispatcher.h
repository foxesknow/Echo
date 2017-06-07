#pragma once

#include "IFunctionDispatcher.h"

#include <functional>

namespace Echo 
{

/**
 * Immediately executes a function on the callers thread
 */
class ImmediateWorkItemDispatcher : public IFunctionDispatcher
{
public:
	/**
	 * Accepts a function and executes it immediately on the current thread
	 */
	virtual void Submit(const std::function<void()> &function) override
	{
		function();
	}
};

} // end of namespace