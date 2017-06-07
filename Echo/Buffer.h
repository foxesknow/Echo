#pragma once

#include <utility>

#include "WinInclude.h"

namespace Echo
{

/**
 * A class that manages a buffer
 */
class Buffer
{
private:
	BYTE *m_Data;
	size_t m_Size;

public:
	/**
	 * Initializes the instance
	 * @param size  the size of the buffer, in bytes
	 */
	explicit Buffer(size_t size)
	{
		m_Data=new BYTE[size];
		m_Size=size;
	}

	/**
	 * Initializes the instance via a move
	 */
	Buffer(Buffer &&rhs) : m_Data(nullptr), m_Size(0)
	{
		std::swap(m_Data,rhs.m_Data);
		std::swap(m_Size,rhs.m_Size);
	}

	Buffer(const Buffer &&)=delete;	
	Buffer &operator=(const Buffer &)=delete;

	/**
	 * Moves an existing buffer.
	 * The contents of the current buffer are deleted
	 */
	Buffer &operator=(Buffer &&rhs) noexcept
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

	/**
	 * Destroys the instance by deleting the buffer
	 */
	~Buffer()
	{
		delete []m_Data;
	}

	/**
	 * Fills the contents of the buffer with a value
	 */
	void Fill(BYTE value)
	{
		::FillMemory(m_Data,m_Size,value);
	}

	/**
	 * Clears the buffer by filling its contents with zero
	 */
	void Clear()
	{
		Fill(0);
	}

	/**
	 * Returns the size of the buffer, in bytes
	 */
	size_t Size() const noexcept
	{
		return m_Size;
	}

	/**
	 * Returns a pointer to the buffer
	 */
	void *Data() noexcept
	{
		return m_Data;
	}

	/**
	 * Returns a pointer to the buffer
	 */
	const void *Data() const noexcept
	{
		return m_Data;
	}

	/**
	 * Returns a pointer to the buffer as a specified type
	 */
	template<typename T>
	T *DataAs() noexcept
	{
		return static_cast<T*>(m_Data);
	}

	/**
	 * Returns a pointer to the buffer as a specified type
	 */
	template<typename T>
	const T *DataAs() const noexcept
	{
		return static_cast<T*>(m_Data);
	}
};

} // end of namespace