#pragma once

#include <utility>

#include <Echo\WinInclude.h>
#include <Echo\HandleTraits.h>
#include <Echo\Handle.h>
#include <Echo\tstring.h>

#include <Echo\File.h>

namespace Echo 
{

/**
 * Manages a memory mapped file
 */
class MemoryMappedFile : public Handle
{
private:
	typedef HandleNull Traits;

protected:
	/**
	 * Initializes the instance from an existing handle
	 */
	MemoryMappedFile(HANDLE handle) : Handle(handle)
	{
	}

	/**
	 * Closes the underlying handle
	 */
	void Close()
	{
		Traits::Destroy(UnderlyingHandle());
		UnderlyingHandle(Traits::InvalidValue());
	}

public:
	/**
	 * Initializes the instance such that it cannot map anything
	 */
	MemoryMappedFile() : Handle(nullptr)
	{
	}

	/**
	 * Initializes the instance by moving another instance in
	 */
	MemoryMappedFile(MemoryMappedFile &&rhs) : Handle(Traits::InvalidValue())
	{
		Swap(rhs);
	}

	MemoryMappedFile(const MemoryMappedFile&) = delete;
	MemoryMappedFile &operator=(const MemoryMappedFile&) = delete;

	/**
	 * Assign the instance from another instance
	 */
	MemoryMappedFile &operator=(MemoryMappedFile &&rhs)
	{
		if(this != &rhs)
		{
			Swap(rhs);
			rhs.Close();
		}

		return *this;
	}

	/**
	 * Destroy the mapping by closing the underlying handle
	 */
	virtual ~MemoryMappedFile() override
	{
		Traits::Destroy(UnderlyingHandle());
	}

	/**
	 * Detached the OS handle from the instance, leaving the instance is a valid, but unusable state
	 * @returns the OS handle
	 */
	virtual HANDLE Detach() override
	{
		auto handle = UnderlyingHandle();
		UnderlyingHandle(Traits::InvalidValue());

		return handle;
	}

	void Flush(const void *address, SIZE_T bytesToFlush=0) const
	{
		auto success = ::FlushViewOfFile(address,bytesToFlush);
		if(!success) throw IOException(_T("Flush failed"));
	}

	/**
	 * Maps a portion of the memory mapped file into memory
	 * @params offset  the offset into the file to map
	 * @params bytesToMap  how many bytes to map into view
	 * @params desiredAccess  the access required (eg FILE_MAP_READ or FILE_MAP_WRITE)
	 * @returns a pointer to the mapped memory
	 */
	void *Map(DWORD64 offset, SIZE_T bytesToMap, DWORD desiredAccess) const
	{
		DWORD low = static_cast<DWORD>(offset);
		DWORD high = static_cast<DWORD>(offset>>32);

		void *address = ::MapViewOfFile(UnderlyingHandle(), desiredAccess, high, low, bytesToMap);
		if(address == nullptr) throw IOException(_T("Map failed"));

		return address;
	}

	/**
	 * Maps a portion of the memory mapped file into memory
	 * @params offset  the offset into the file to map
	 * @params bytesToMap  how many bytes to map into view
	 * @params desiredAccess  the access required (eg FILE_MAP_READ or FILE_MAP_WRITE)
	 * @returns a typed pointer to the mapped memory
	 */
	template<typename T>
	T *MapAs(DWORD64 offset, SIZE_T bytesToMap, DWORD desiredAccess) const
	{
		void *address = Map(offset,bytesToMap,desiredAccess);
		return reinterpret_cast<T*>(address);
	}

	/**
	 * Unmaps a portion of memory
	 * @param address  the address mapped with either Map or MapAs
	 */
	void Unmap(const void *address) const
	{
		auto success = ::UnmapViewOfFile(address);
		if(!success) throw IOException(_T("Unmap failed"));
	}

	/**
	 * Creates a memory mapped file from an underlying file
	 * @param file  the file to map into memory
	 * @param maximumMappingSize  the maximum size of the mapping object. Set to zero to map equal the size of the file
	 * @param protection  the page protection required (eq PAGE_READONLY, PAGE_READWRITE)
	 * @returns a MemoryMappedFile instance
	 */
	static MemoryMappedFile FromFile(const File &file, DWORD64 maximumMappingSize, DWORD protection)
	{
		auto fileHandle = file.UnderlyingHandle();

		DWORD low = static_cast<DWORD>(maximumMappingSize);
		DWORD high = static_cast<DWORD>(maximumMappingSize>>32);

		auto mappingHandle = ::CreateFileMapping(fileHandle, nullptr, protection, high, low, nullptr);
		if(mappingHandle == Traits::InvalidValue()) throw IOException(_T("could not create memory mapped file from file"));

		return MemoryMappedFile(mappingHandle);
	}

	/**
	 * Creates a memory mapped file that is backed by the system paging file
	 * @param mappingSize  how big the in memory file should be
	 * @param optionalMappingName  a name that can be used to open the same memory mapped file in another process
	 * @param protection  the page protection required (eq PAGE_READONLY, PAGE_READWRITE)
	 * @returns a MemoryMappedFile instance
	 */
	static MemoryMappedFile InMemory(DWORD64 mappingSize, DWORD protection, const tstd::tstring &optionalMappingName=_T(""))
	{
		DWORD low = static_cast<DWORD>(mappingSize);
		DWORD high = static_cast<DWORD>(mappingSize >> 32);

		const TCHAR *mappingName = nullptr;
		if(optionalMappingName.length() != 0) mappingName = optionalMappingName.c_str();

		auto mappingHandle = ::CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, protection, high, low, mappingName);
		if(mappingHandle == Traits::InvalidValue()) throw IOException(_T("could not create in-memory mapped file"));

		return MemoryMappedFile(mappingHandle);
	}

	/**
	 * Opens an existing file mapping
	 * @param mappingName  the name of the file mapping
	 * @param fileMappingAccessRights  the access rights (for example FILE_MAP_ALL_ACCESS)
	 * @returns a memory mapped file instance
	 */
	static MemoryMappedFile OpenExisting(const tstd::tstring &mappingName, DWORD fileMappingAccessRights)
	{
		auto handle = ::OpenFileMapping(fileMappingAccessRights, FALSE, mappingName.c_str());
		if(handle == Traits::InvalidValue()) throw IOException(_T("could not open existing mapping"));

		return MemoryMappedFile(handle);
	} 
};

} // end of namespace