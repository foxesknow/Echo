#pragma once

#include "IWorkItemDispatcher.h"

#include <functional>

namespace Echo { namespace Threading {

//
// Immediately executes a function on the callers thread
//
class ImmediateWorkItemDispatcher : IWorkItemDispatcher
{
public:
	virtual void Submit(const std::function<void()> &function) override
	{
		function();
	}
};

}} // end of namespace