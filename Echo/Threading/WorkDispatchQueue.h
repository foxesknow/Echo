#pragma once

#include <Echo/Execution/OnDestruct.h>


#include "CriticalSection.h"
#include "Events.h"
#include "ThreadException.h"
#include "ThreadPool.h"

#include <deque>

namespace Echo { namespace Threading {

/**
 * A work queue that allows work to be farmed off onto another thread
 */
template<typename T>
class WorkDispatchQueue
{
private:
	std::deque<T> m_Data;
	std::deque<T> m_SwapData;

	std::deque<T> *m_ActiveData;

	ThreadPool &m_Pool;
	
	const CriticalSection m_SyncRoot;
	const AutoResetEvent m_StopEvent;

	bool m_ThreadActive=false;
	bool m_StopProcessing=false;
	bool m_Shutdown=false;
	bool m_ProcessRemainingItems=false;

	/**
	 * Switches the active queue with the swap deque
	 * @returns the previous active deque
	 */
	std::deque<T> &SwitchActive()
	{
		auto previous=m_ActiveData;

		if(m_ActiveData==&m_Data)
		{
			m_ActiveData=&m_SwapData;
		}
		else
		{
			m_ActiveData=&m_Data;
		}

		return *previous;
	}

	/**
	 * Enqueues data to be worked on
	 */
	void DoEnqueue(const T &data)
	{
		if(m_Shutdown) throw ThreadException(_T("dispatch queue has been shut down"));

		m_ActiveData->push_back(data);

		if(m_ThreadActive==false)
		{
			m_ThreadActive=true;
			auto function=[this]{ProcessQueue();};
			m_Pool.Submit(function);
		}
	}

	/**
	 * Processes the queued up data on a thread
	 */
	void ProcessQueue()
	{
		Lock<CriticalSection> lock(m_SyncRoot);

		while(m_ActiveData->size()!=0 && m_StopProcessing==false)
		{
			std::deque<T> &data=SwitchActive();

			// We can get exit the lock now
			Unlock<CriticalSection> unlock(m_SyncRoot);

			// Make sure we clear out the data regardless of what happens
			Echo::Execution::OnDestruct onDestruct([&]{data.clear();});

			ProcessItems(data);
		}

		// We're back in the lock here
		m_ThreadActive=false;

		if(m_StopProcessing)
		{
			// Process anything that's left
			if(m_ProcessRemainingItems) ProcessItems(*m_ActiveData);
			m_StopEvent.Set();
		}
	}

	/**
	 * Processes all the items in a deque
	 */
	void ProcessItems(std::deque<T> &data)
	{
		for(T &item : data)
		{
			ProcessItem(item);
		}
	}

protected:
	/**
	 * Carries out the processing an an individual item of work
	 */
	virtual void ProcessItem(T &item)=0;

	/**
	 * Indicates if any remaining items should be processed when the work queue is shut down
	 * @param value  true to process remaining item, false to ignore them
	 */
	void ProcessRemainingItems(bool value)
	{
		m_ProcessRemainingItems=value;
	}

	/**
	 * Indicates if any remaining items should be processed when the work queue is shut down
	 */
	bool ProcessRemainingItems()const
	{
		return m_ProcessRemainingItems;
	}

public:
	/**
	 * Initializes the instance
	 * @param pool  the thread pool to execute the work on
	 */
	WorkDispatchQueue(ThreadPool &pool) : m_Pool(pool), m_StopEvent(InitialState::NonSignalled)
	{
		m_ActiveData=&m_Data;
	}

	/**
	 * Destroys the instance by shutting down the queue
	 */
	virtual ~WorkDispatchQueue()
	{
		Shutdown();
	}

	WorkDispatchQueue(const WorkDispatchQueue&)=delete;
	WorkDispatchQueue &operator=(const WorkDispatchQueue&)=delete;

	/**
	 * Adds a work item to the queue.
	 * If the queue has been shut down the method will fail
	 */
	void Enqueue(const T &data)
	{
		Lock<CriticalSection> lock(m_SyncRoot);
		DoEnqueue(data);
	}

	/**
	 * Attempts to add a work item to the queue.
	 * If the queue has been shut down then the work item will not be queued
	 * @returns true if the item was queued for processing, false if it could not be queue
	 */
	bool TryEnqueue(const T &data)
	{
		Lock<CriticalSection> lock(m_SyncRoot);

		if(m_Shutdown) return false;

		DoEnqueue(data);
		return true;
	}

	/**
	 * Shuts the queue down.
	 * When this method returns no more work may be enqueued
	 */
	void Shutdown()
	{
		bool shouldWait=false;

		{
			Lock<CriticalSection> lock(m_SyncRoot);

			if(m_Shutdown) return;

			m_StopProcessing=true;
			m_Shutdown=true;

			// We only need to block if the thread is currently running
			if(m_ThreadActive) shouldWait=true;
		}

		if(shouldWait)
		{
			m_StopEvent.Wait();
		}
	}
};

}} // end of namespace