#pragma once

#include <utility>

#include "HandleTraits.h"
#include "WinInclude.h"
#include "Handle.h"
#include "Exceptions.h"

#include "AsyncResult.h"
#include "IOException.h"
#include "IReaderWriter.h"

namespace Echo 
{

class File : public Handle, public IReaderWriter
{
public:
	typedef HandleInvalidValue Traits;

private:

	void CheckHandle()const
	{
		if(UnderlyingHandle()==Traits::InvalidValue())
		{
			throw IOException(_T("invalid file handle"));
		}
	}

protected:
	File(HANDLE handle) : Handle(handle)
	{
	}

public:
	/**
	 * Moves an existing file
	 * @param rhs  the object to move
	 */
	File(File &&rhs) noexcept : Handle(Traits::InvalidValue())
	{
		Swap(rhs);
	}

	/**
	 * Initializes the file with an invalid handle
	 */
	File() noexcept : Handle(Traits::InvalidValue())
	{
	}

	File(const File &rhs)=delete;

	/**
	 * Closes the file, if open
	 */
	~File()noexcept override
	{
		HANDLE handle=UnderlyingHandle();
		Traits::Destroy(handle);
	}

	File &operator=(const File &rhs)=delete;

	/**
	 * Moves the file on the right into this object.
	 * If this file is open it will be closes
	 * @param rhs  the file to move
	 */
	File &operator==(File &&rhs) noexcept
	{
		if(this!=&rhs)
		{
			Swap(rhs);
			rhs.Close();
		}

		return *this;
	}

	/**
	 * Closes the file, if open
	 */
	virtual void Close()noexcept override
	{
		HANDLE handle=UnderlyingHandle();
		Traits::Destroy(handle);
		UnderlyingHandle(Traits::InvalidValue());
	}

	void Flush() const
	{
		auto success=::FlushFileBuffers(UnderlyingHandle());
		if(!success) throw IOException(_T("Flush failed"));
	}

	/**
	 * Returns the size of the file
	 * @returns the size of the file
	 */
	LONGLONG Size() const
	{
		CheckHandle();

		LARGE_INTEGER size;
		BOOL success=::GetFileSizeEx(UnderlyingHandle(),&size);

		if(!success) throw IOException(_T("Size failed"));
		return size.QuadPart;
	}

	/**
	 * Synchronously writes to the file
	 * @param buffer  a pointer to the data to write
	 * @param bytesToWrite  how much data to write
	 * @returns the number of bytes written to the file
	 */
	virtual DWORD Write(const void *buffer, DWORD bytesToWrite) const override
	{
		if(buffer==nullptr) throw ArgumentNullException(_T("buffer"));
		CheckHandle();

		HANDLE handle=UnderlyingHandle();
		
		DWORD bytesWritten=0;
		auto success=::WriteFile(handle,buffer,bytesToWrite,&bytesWritten,nullptr);

		// We're synchronous, so we don't need to check for pending IO;
		if(!success) throw IOException(_T("Write failed"));

		return bytesWritten;
	}

	/**
	 * Asynchronously writes to the file
	 * @param buffer  a pointer to the data to write
	 * @param bytesToWrite  how much data to write
	 * @param overlapped  an overlapped structure describing the async operation
	 * @returns Complete if the write completed immediately, Pending if the write is pending completion
	 */
	virtual AsyncResult WriteAsync(const void *buffer, DWORD bytesToWrite, OVERLAPPED &overlapped) const override
	{
		if(buffer==nullptr) throw ArgumentNullException(_T("buffer"));
		CheckHandle();

		HANDLE handle=UnderlyingHandle();
		
		DWORD bytesWritten=0;
		auto success=::WriteFile(handle,buffer,bytesToWrite,&bytesWritten,&overlapped);

		if(success)
		{
			return AsyncResult::Complete;
		}
		else
		{
			if(GetLastError()==ERROR_IO_PENDING) 
			{
				return AsyncResult::Pending;
			}
			else
			{
				throw IOException(_T("WriteAsync failed"));
			}
		}
	}

	/**
	 * Synchronously reads from the file
	 * @param buffer  a pointer to where the read data will be stored
	 * @param bytesToRead  how much data to read
	 * @returns the number of bytes read from the file
	 */
	virtual DWORD Read(void *buffer, DWORD bytesToRead) const override
	{
		if(buffer==nullptr) throw ArgumentNullException(_T("buffer"));
		CheckHandle();

		HANDLE handle=UnderlyingHandle();

		DWORD bytesRead=0;
		auto success=::ReadFile(handle,buffer,bytesToRead,&bytesRead,nullptr);

		// We're synchronous, so we don't need to check for pending IO;
		if(!success) throw IOException(_T("Write failed"));

		return bytesRead;
	}

	/**
	 * Asynchronously reads from the file
	 * @param buffer  a pointer to where the read data will be stored
	 * @param bytesToRead  how much data to read
	 * @param overlapped  an overlapped structure describing the async operation
	 * @returns Complete if the read completed immediately, Pending if the read is pending completion
	 */
	virtual AsyncResult ReadAsync(void *buffer, DWORD bytesToRead, OVERLAPPED &overlapped) const override
	{
		if(buffer==nullptr) throw ArgumentNullException(_T("buffer"));
		CheckHandle();

		HANDLE handle=UnderlyingHandle();

		DWORD bytesRead=0;
		auto success=::ReadFile(handle,buffer,bytesToRead,&bytesRead,&overlapped);

		if(success)
		{
			return AsyncResult::Complete;
		}
		else
		{
			if(GetLastError()==ERROR_IO_PENDING) 
			{
				return AsyncResult::Pending;
			}
			else
			{
				throw IOException(_T("ReadAsync failed"));
			}
		}
	}

	/**
	 * Waits for a pending operation to complete
	 * @param overlapped  the overlapped structure that was passed to the async operation
	 * @returns the number of bytes transferred by the async operation
	 */
	virtual DWORD WaitForAsyncToComplete(OVERLAPPED &overlapped) const override
	{
		CheckHandle();

		DWORD bytesTransferred=0;
		HANDLE handle=UnderlyingHandle();
		auto success=::GetOverlappedResult(handle,&overlapped,&bytesTransferred,TRUE);

		if(!success) throw IOException(_T("WaitForAsyncToComplete failed"));

		return bytesTransferred;
	}

	/**
	 * Sets the handle to an invalid value and returns the OS value
	 * @returns the OS handle
	 */
	virtual HANDLE Detach()noexcept override
	{
		HANDLE handle=UnderlyingHandle();
		UnderlyingHandle(Traits::InvalidValue());

		return handle;
	}

	/**
	 * Creates or replaces a file
	 */
	static File Create(const tstd::tstring &filename, DWORD access)
	{
		return Open(filename,access,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL);
	}

	/**
	 * Creates or replaces a file which is opened for asychronous IO
	 */
	static File CreateAsync(const tstd::tstring &filename, DWORD access)
	{
		return Open(filename,access,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED);
	}

	/**
	 * Opens an existing file for reading
	 */
	static File OpenRead(const tstd::tstring &filename)
	{
		return Open(filename,GENERIC_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL);
	}

	/**
	 * Opens an existing file for asynchronous reading
	 */
	static File OpenReadAsync(const tstd::tstring &filename)
	{
		return Open(filename,GENERIC_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED);
	}

	/**
	 * Opens an existing file for writing
	 */
	static File OpenWrite(const tstd::tstring &filename)
	{
		return Open(filename,GENERIC_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL);
	}

	/**
	 * Opens an existing file for asynchronous writing
	 */
	static File OpenWriteAsync(const tstd::tstring &filename)
	{
		return Open(filename,GENERIC_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED);
	}

	/**
	 * Opens an existing file for reading and writing
	 */
	static File OpenReadWrite(const tstd::tstring &filename)
	{
		return Open(filename,GENERIC_WRITE|GENERIC_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL);
	}

	/**
	 * Opens an existing file for asynchronous reading and writing
	 */
	static File OpenReadWriteAsync(const tstd::tstring &filename)
	{
		return Open(filename,GENERIC_WRITE|GENERIC_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED);
	}

	/**
	 * Opens a file
	 */
	static File Open(const tstd::tstring &filename, DWORD access, DWORD shareMode, DWORD disposition, DWORD attributes)
	{
		HANDLE handle=::CreateFile(filename.c_str(),access,shareMode,nullptr,disposition,attributes,nullptr);
		
		if(handle==Traits::InvalidValue()) throw IOException(_T("unable to open file"));

		return File(handle);
	}

	static File Attach(HANDLE handle)
	{
		if(handle==Traits::InvalidValue()) throw IOException(_T("invalid file handle"));

		return File(handle);
	}
};


} // end of namespace