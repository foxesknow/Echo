#pragma once

#include <Echo/WinInclude.h>

#include "IFunctionDispatcher.h"
#include "ThreadException.h"

#include <functional>
#include <utility>
#include <memory>

namespace Echo 
{

/**
 * A thread pool
 */
class ThreadPool : public IFunctionDispatcher
{
private:
	PTP_POOL m_Pool;
	TP_CALLBACK_ENVIRON m_Environment;
	PTP_CLEANUP_GROUP m_CleanupGroup;

	bool m_CancelOutstanding=true;

	mutable LONG m_OutstandingWork;

	class ThreadData
	{
	private:
		ThreadPool *m_Pool;
		std::function<void()> m_Function;

	public:
		ThreadData(ThreadPool *pool, const std::function<void()> &function) : m_Pool(pool), m_Function(function)
		{
		}

		std::function<void()> &Function()
		{
			return m_Function;
		}

		ThreadPool *Pool()
		{
			return m_Pool;
		}
	};

	static void CALLBACK WorkCallback(PTP_CALLBACK_INSTANCE, void *context, PTP_WORK work)
	{
		LONG *counter=nullptr;
		::CloseThreadpoolWork(work);

		{
			std::unique_ptr<ThreadData> threadData(reinterpret_cast<ThreadData*>(context));
			counter=&threadData->Pool()->m_OutstandingWork;
			auto function=threadData->Function();
			function();
		}

		::InterlockedDecrement(counter);
	}

	/**
	 * Releases any memory allocated for the pool when we're closing the pool down
	 */
	static void CALLBACK Cleanup(void *context, void*)
	{
		ThreadData *threadData=reinterpret_cast<ThreadData*>(context);
		if(threadData)
		{
			delete threadData;
		}
	}

	void EnsureRunning() const
	{
		if(m_Pool==nullptr) throw ThreadException(_T("thread pool not started"));
	}

public:
	/**
	 * Initializes the instance
	 */
	ThreadPool() : m_Pool(nullptr)
	{
	}

	/**
	 * Destroys the instance.
	 * If the pool has been started any memory allocated to it will be freed
	 */
	~ThreadPool()
	{
		if(m_Pool!=nullptr) 
		{
			::CloseThreadpoolCleanupGroupMembers(m_CleanupGroup,m_CancelOutstanding,nullptr);
			
			::CloseThreadpool(m_Pool);
			::CloseThreadpoolCleanupGroup(m_CleanupGroup);
			::DestroyThreadpoolEnvironment(&m_Environment);
		}
	}

	/**
	 * Starts the pool
	 */
	void Start()
	{
		if(m_Pool!=nullptr) throw ThreadException(_T("thread pool already started"));
		m_Pool=::CreateThreadpool(nullptr);

		::InitializeThreadpoolEnvironment(&m_Environment);		

		m_CleanupGroup=::CreateThreadpoolCleanupGroup();
		::SetThreadpoolCallbackCleanupGroup(&m_Environment,m_CleanupGroup,Cleanup);

		::SetThreadpoolCallbackPool(&m_Environment,m_Pool);
	}

	/**	
	 * Returns the number of items waiting to be run on the pool
	 */
	LONG OutstandingWork()const noexcept
	{
		LONG outstanding=::InterlockedCompareExchange(&m_OutstandingWork,0,0);
		return outstanding;
	}

	/**
	 * Indicates if we should cancel any outstanding items when the pool is destroyed
	 */
	bool CancelOutstanding()const noexcept
	{
		return m_CancelOutstanding;
	}

	/**
	 * Indicates if we should cancel any outstanding items when the pool is destroyed
	 */
	void CancelOutstanding(bool value) noexcept
	{
		m_CancelOutstanding=value;
	}

	/**
	 * Sets the minimum number of threads for the pool
	 */
	void MinimumThreads(DWORD value)
	{
		EnsureRunning();
		::SetThreadpoolThreadMinimum(m_Pool,value);
	}
	
	/**
	 * Sets the maximum number of threads for the pool
	 */
	void MaximumThreads(DWORD value)
	{
		EnsureRunning();
		::SetThreadpoolThreadMaximum(m_Pool,value);
	}

	/**
	 * Submits an item of work to the thread pool
	 */
	virtual void Submit(const std::function<void()> &function) override
	{
		EnsureRunning();

		auto threadData=new ThreadData(this,std::move(function));
		auto work=::CreateThreadpoolWork(WorkCallback,threadData,&m_Environment);
		::InterlockedIncrement(&m_OutstandingWork);
		::SubmitThreadpoolWork(work);
	}
};

} // end of namespace