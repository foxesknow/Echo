#pragma once

#include <utility>

#include <Echo/WinInclude.h>

namespace Echo { namespace Execution {

class Buffer
{
private:
	void *m_Data;
	size_t m_Size;

public:
	explicit Buffer(size_t size)
	{
		m_Data=new BYTE[size];
		m_Size=size;
	}

	Buffer(Buffer &&rhs) : m_Data(nullptr), m_Size(0)
	{
		std::swap(m_Data,rhs.m_Data);
		std::swap(m_Size,rhs.m_Size);
	}

	Buffer(const Buffer &&)=delete;	

	Buffer &operator=(const Buffer &)=delete;

	Buffer &operator=(Buffer &&rhs) ECHO_NOEXCEPT
	{
		if(this!=&rhs)
		{
			delete []m_Data;

			m_Data=nullptr;
			m_Size=0;

			std::swap(m_Data,rhs.m_Data);
			std::swap(m_Size,rhs.m_Size);
		}

		return *this;
	}

	~Buffer()
	{
		delete []m_Data;
	}

	void Fill(BYTE value)
	{
		::FillMemory(m_Data,m_Size,value);
	}

	void Clear()
	{
		Fill(0);
	}

	size_t Size() const ECHO_NOEXCEPT
	{
		return m_Size;
	}

	void *Data() ECHO_NOEXCEPT
	{
		return m_Data;
	}

	const void *Data() const ECHO_NOEXCEPT
	{
		return m_Data;
	}

	template<typename T>
	T *DataAs() ECHO_NOEXCEPT
	{
		return static_cast<T*>(m_Data);
	}

	template<typename T>
	const T *DataAs() const ECHO_NOEXCEPT
	{
		return static_cast<T*>(m_Data);
	}
};

}} // end of namespace