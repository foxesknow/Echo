#pragma once

#include <functional>
#include <utility>

namespace Echo 
{

/**
 * Calls a function when destructed
 */
class OnDestruct
{
private:
	std::function<void()> m_Function;
	bool m_ShouldExecute=true;

public:
	/**
	 * Initializes the instance
	 * @param function  the function to execute when destroyed
	 */
	OnDestruct(const std::function<void()> &function) : m_Function(function)
	{
	}

	/**
	 * Initializes the instance
	 */
	OnDestruct(OnDestruct &&rhs) : m_Function(std::move(rhs.m_Function))
	{
		rhs.m_ShouldExecute = false;
	}

	OnDestruct(const OnDestruct&) = delete;
	OnDestruct &operator=(const OnDestruct&) = delete;

	/**
	 * Moves the right hand side into the instance
	 */
	OnDestruct &operator=(OnDestruct &&rhs)
	{
		if(this != &rhs)
		{
			std::swap(m_Function, rhs.m_Function);
			rhs.m_ShouldExecute = false;
		}

		return *this;
	}

	/**
	 * Destroys the instance, executing the function if necessary
	 */
	~OnDestruct()
	{
		if(m_ShouldExecute)
		{
			m_Function();
		}
	}
};

} // end of namespace