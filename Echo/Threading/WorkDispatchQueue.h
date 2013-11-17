#pragma once

#include <Echo/Execution/OnDestruct.h>


#include "CriticalSection.h"
#include "Events.h"
#include "ThreadException.h"
#include "ThreadPool.h"

#include <deque>

namespace Echo { namespace Threading {

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

	void SwitchActive()
	{
		if(m_ActiveData==&m_Data)
		{
			m_ActiveData=&m_SwapData;
		}
		else
		{
			m_ActiveData=&m_Data;
		}
	}

	void DoEnqueue(T &data)
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

	void ProcessQueue()
	{
		Lock<CriticalSection> lock(m_SyncRoot);

		while(m_ActiveData->size()!=0 && m_StopProcessing==false)
		{
			std::deque<T> &data=*m_ActiveData;
			SwitchActive();

			// We can get exit the lock now
			Unlock<CriticalSection> unlock(m_SyncRoot);

			// Make sure we clear out the data regardless of what happens
			std::function<void()> tidyup=[&]{data.clear();};
			Echo::Execution::OnDestruct onDestruct(tidyup);

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

	void ProcessItems(std::deque<T> &data)
	{
		for(T &item : data)
		{
			ProcessItem(item);
		}
	}

protected:
	virtual void ProcessItem(T &item)=0;

	void ProcessRemainingItems(bool value)
	{
		m_ProcessRemainingItems=value;
	}

	bool ProcessRemainingItems()const
	{
		return m_ProcessRemainingItems;
	}

public:
	WorkDispatchQueue(ThreadPool &pool) : m_Pool(pool), m_StopEvent(InitialState::NonSignalled)
	{
		m_ActiveData=&m_Data;
	}

	virtual ~WorkDispatchQueue()
	{
		Shutdown();
	}

	WorkDispatchQueue(const WorkDispatchQueue&)=delete;
	WorkDispatchQueue &operator=(const WorkDispatchQueue&)=delete;

	void Enqueue(T &data)
	{
		Lock<CriticalSection> lock(m_SyncRoot);
		DoEnqueue(data);
	}

	bool TryEnqueue(T &data)
	{
		Lock<CriticalSection> lock(m_SyncRoot);

		if(m_Shutdown) return false;

		DoEnqueue(data);
		return true;
	}

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