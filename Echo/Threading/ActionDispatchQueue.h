#pragma once

#include "ThreadPool.h"
#include "WorkDispatchQueue.h"

#include <functional>

namespace Echo { namespace Threading {

class ActionDispatchQueue : public WorkDispatchQueue<std::function<void()>>
{
private:
	typedef WorkDispatchQueue<std::function<void()>> Base;

protected:
	void ProcessItem(std::function<void()> &function) override
	{
		function();
	}

public:
	ActionDispatchQueue(ThreadPool &pool) : Base(pool)
	{
	}

	~ActionDispatchQueue()
	{
		ProcessRemainingItems(true);
		Shutdown();
	}
};

}} // end of namespace