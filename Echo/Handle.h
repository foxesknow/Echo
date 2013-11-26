#pragma once

#include "WinInclude.h"

/**
 * Base class for a handle
 */
class Handle
{
private:
	HANDLE m_Handle;

protected:
	/**
	 * Initializes the instance
	 * @param handle  the handle to attach to the instance
	 */
	Handle(HANDLE handle) ECHO_NOEXCEPT : m_Handle(handle)
	{
	}

	Handle(Handle &&rhs)=delete;
	Handle(const Handle &)=delete;
	Handle &operator=(const Handle &)=delete;

	/**
	 * Swaps two handles
	 * @param rhs  the object to swap handles with
	 */
	void Swap(Handle &rhs) ECHO_NOEXCEPT
	{
		std::swap(m_Handle,rhs.m_Handle);
	}

	/**
	 * Sets the underlying OS handle
	 * @param handle  the handle to use
	 */
	void UnderlyingHandle(HANDLE handle) ECHO_NOEXCEPT
	{
		m_Handle=handle;
	}

public:
	/**
	 * Destroys the instance.
	 * NOTE: This method does nothing. It is down to derived instances to know how to release their handle
	 */
	virtual ~Handle()=0
	{
		// It's down to the derived class to know how to release the handle
	}

	/**
	 * Gets the underlying OS handle
	 * @returns an OS handle
	 */
	HANDLE UnderlyingHandle() const ECHO_NOEXCEPT
	{
		return m_Handle;
	}

	/**
	 * Detaches the handle from its object, leaving it in a destructable but unusable state
	 * @returns the handle managed by the object
	 */
	virtual HANDLE Detach()=0;
};
