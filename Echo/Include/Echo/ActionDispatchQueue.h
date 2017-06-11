#pragma once

#include "WinInclude.h"

#include <Echo\ThreadPool.h>
#include <Echo\WorkDispatchQueue.h>

#include <functional>

namespace Echo
{

/**
 * An implementation of the WorkDispatchQueue that allows functions to be scheduled
 */
class ActionDispatchQueue : public WorkDispatchQueue<std::function<void()>>
{
private:
	typedef WorkDispatchQueue<std::function<void()>> Base;

protected:
	/**
	 * Executes the function
	 */
	void ProcessItem(std::function<void()> &function) override
	{
		function();
	}

public:
	/**
	 * Initializes the instance
	 * @param dispatcher  an object that is able to dispatch function invocations
	 */
	ActionDispatchQueue(IFunctionDispatcher &dispatcher) : Base(dispatcher)
	{
		ProcessRemainingItems(true);
	}

	/**
	 * Destroys the instance
	 */
	~ActionDispatchQueue() override
	{
		// NOTE: We call shutdown here so that the correct implementation of ProcessItem is called.
		// If we leave it to the base constructor to shut down the queue then any remaining items
		// that may be processed will call the base ProcessItem, which is pure virtual!
		Shutdown();
	}
};

} // end of namespace