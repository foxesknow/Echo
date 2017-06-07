#pragma once

#include <Echo/WinInclude.h>
#include <Echo/Events.h>

namespace Echo { namespace IO {

/**
 * Wraps an OVERLAPPED structure with some extra features
 */
struct Overlapped : OVERLAPPED
{
public:
	/**
	 * Initializes the instance
	 */
	Overlapped()
	{
		OVERLAPPED *overlapped=this;
		::ZeroMemory(overlapped,sizeof(OVERLAPPED));
	}

	/**
	 * Attaches an event to the hEvent member
	 */
	void Attach(const Echo::Threading::Event &event)
	{
		hEvent=event.UnderlyingHandle();
	}

	/**
	 * Sets the position for the overlapped operation
	 */
	void Position(DWORD64 position)
	{
		DWORD low=static_cast<DWORD>(position);
		DWORD high=static_cast<DWORD>(position>>32);

		Offset=low;
		OffsetHigh=high;
	}
};

}} // end of namespace