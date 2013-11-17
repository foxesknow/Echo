#pragma once

#include <functional>
#include <utility>

namespace Echo { namespace Execution {

//
// Calls a function when destructed
//
class OnDestruct
{
private:
	std::function<void()> m_Function;
	bool m_ShouldExecute=true;

public:
	OnDestruct(const std::function<void()> &function) : m_Function(function)
	{
	}

	OnDestruct(OnDestruct &&rhs) : m_Function(std::move(rhs.m_Function))
	{
		rhs.m_ShouldExecute=false;
	}

	OnDestruct(const OnDestruct&)=delete;

	OnDestruct &operator=(const OnDestruct&)=delete;

	OnDestruct &operator=(OnDestruct &&rhs)
	{
		if(this!=&rhs)
		{
			std::swap(m_Function,rhs.m_Function);
			rhs.m_ShouldExecute=false;
		}

		return *this;
	}

	~OnDestruct()
	{
		if(m_ShouldExecute)
		{
			m_Function();
		}
	}
};

}} // end of namespace