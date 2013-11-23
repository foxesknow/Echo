#pragma once

#include <Echo/WinInclude.h>
#include <Echo/Threading/Events.h>

namespace Echo { namespace IO {

struct Overlapped : OVERLAPPED
{
public:
	Overlapped()
	{
		OVERLAPPED *overlapped=this;
		::ZeroMemory(overlapped,sizeof(OVERLAPPED));
	}

	void Attach(const Echo::Threading::Event &event)
	{
		hEvent=event.UnderlyingHandle();
	}

	void Position(DWORD64 position)
	{
		DWORD low=static_cast<DWORD>(position);
		DWORD high=static_cast<DWORD>(position>>32);

		Offset=low;
		OffsetHigh=high;
	}

	void Position(DWORD position)
	{
		Offset=position;
		OffsetHigh=0;
	}
};

}} // end of namespace