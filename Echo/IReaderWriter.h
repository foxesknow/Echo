#pragma once

#include <Echo/WinInclude.h>

#include "AsyncResult.h"

namespace Echo 
{

/**
 * Defines the behaviour for a source that can be read or written to
 */
class IReaderWriter
{
protected:
	virtual ~IReaderWriter()
	{
	}

public:
	/**
	 * Closes the file, if open
	 */
	virtual void Close() noexcept=0;

	/**
	 * Synchronously writes to the file
	 * @param buffer  a pointer to the data to write
	 * @param bytesToWrite  how much data to write
	 * @returns the number of bytes written to the file
	 */
	virtual DWORD Write(const void *buffer, DWORD bytesToWrite) const=0;

	/**
	 * Asynchronously writes to the file
	 * @param buffer  a pointer to the data to write
	 * @param bytesToWrite  how much data to write
	 * @param overlapped  an overlapped structure describing the async operation
	 * @returns Complete if the write completed immediately, Pending if the write is pending completion
	 */
	virtual AsyncResult WriteAsync(const void *buffer, DWORD bytesToWrite, OVERLAPPED &overlapped) const=0;

	/**
	 * Synchronously reads from the file
	 * @param buffer  a pointer to where the read data will be stored
	 * @param bytesToRead  how much data to read
	 * @returns the number of bytes read from the file
	 */
	virtual DWORD Read(void *buffer, DWORD bytesToRead) const=0;

	/**
	 * Asynchronously reads from the file
	 * @param buffer  a pointer to where the read data will be stored
	 * @param bytesToRead  how much data to read
	 * @param overlapped  an overlapped structure describing the async operation
	 * @returns Complete if the read completed immediately, Pending if the read is pending completion
	 */
	virtual AsyncResult ReadAsync(void *buffer, DWORD bytesToRead, OVERLAPPED &overlapped) const=0;

	/**
	 * Waits for a pending operation to complete
	 * @param overlapped  the overlapped structure that was passed to the async operation
	 * @returns the number of bytes transferred by the async operation
	 */
	virtual DWORD WaitForAsyncToComplete(OVERLAPPED &overlapped) const=0;
};

} // end of namespace