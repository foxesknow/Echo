#pragma once

#include <Echo/WinInclude.h>

#include "IWorkItemDispatcher.h"
#include "ThreadException.h"

#include <functional>
#include <utility>
#include <memory>

namespace Echo { namespace Threading {

class ThreadPool : IWorkItemDispatcher
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
		ThreadData(ThreadPool *pool, std::function<void()> function) : m_Pool(pool), m_Function(std::move(function))
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
	ThreadPool() : m_Pool(nullptr)
	{
	}

	void Start()
	{
		if(m_Pool!=nullptr) throw ThreadException(_T("thread pool already started"));
		m_Pool=::CreateThreadpool(nullptr);

		::InitializeThreadpoolEnvironment(&m_Environment);		

		m_CleanupGroup=::CreateThreadpoolCleanupGroup();
		::SetThreadpoolCallbackCleanupGroup(&m_Environment,m_CleanupGroup,Cleanup);

		::SetThreadpoolCallbackPool(&m_Environment,m_Pool);
	}

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

	LONG OutstandingWork()const ECHO_NOEXCEPT
	{
		LONG outstanding=::InterlockedCompareExchange(&m_OutstandingWork,0,0);
		return outstanding;
	}

	bool CancelOutstanding()const ECHO_NOEXCEPT
	{
		return m_CancelOutstanding;
	}

	void CancelOutstanding(bool value) ECHO_NOEXCEPT
	{
		m_CancelOutstanding=value;
	}

	void MinimumThreads(DWORD value)
	{
		EnsureRunning();
		::SetThreadpoolThreadMinimum(m_Pool,value);
	}

	void MaximumThreads(DWORD value)
	{
		EnsureRunning();
		::SetThreadpoolThreadMaximum(m_Pool,value);
	}

	virtual void Submit(const std::function<void()> &function) override
	{
		EnsureRunning();

		auto threadData=new ThreadData(this,function);
		auto work=::CreateThreadpoolWork(WorkCallback,threadData,&m_Environment);
		::InterlockedIncrement(&m_OutstandingWork);
		::SubmitThreadpoolWork(work);
	}
};

}} // end of namespace