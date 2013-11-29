#pragma once

#include <utility>

#include <Echo/WinInclude.h>
#include <Echo/HandleTraits.h>
#include <Echo/Handle.h>
#include <Echo/tstring.h>

#include "File.h"

namespace Echo { namespace IO {

class MemoryMappedFile : public Handle
{
private:
	typedef HandleNull Traits;

protected:
	MemoryMappedFile(HANDLE handle) : Handle(handle)
	{
	}

	void Close()
	{
		Traits::Destroy(UnderlyingHandle());
		UnderlyingHandle(Traits::InvalidValue());
	}

public:
	MemoryMappedFile() : Handle(nullptr)
	{
	}

	MemoryMappedFile(MemoryMappedFile &&rhs) : Handle(Traits::InvalidValue())
	{
		Swap(rhs);
	}

	MemoryMappedFile(const MemoryMappedFile&)=delete;
	MemoryMappedFile &operator=(const MemoryMappedFile&)=delete;

	MemoryMappedFile &operator=(MemoryMappedFile &&rhs)
	{
		if(this!=&rhs)
		{
			Swap(rhs);
			rhs.Close();
		}

		return *this;
	}


	virtual ~MemoryMappedFile() override
	{
		Traits::Destroy(UnderlyingHandle());
	}

	virtual HANDLE Detach() override
	{
		auto handle=UnderlyingHandle();
		UnderlyingHandle(Traits::InvalidValue());

		return handle;
	}

	void *Map(DWORD64 offset, SIZE_T bytesToMap, DWORD desiredAccess) const
	{
		DWORD low=static_cast<DWORD>(offset);
		DWORD high=static_cast<DWORD>(offset>>32);

		void *address=::MapViewOfFile(UnderlyingHandle(),desiredAccess,high,low,bytesToMap);
		if(address==nullptr) throw IOException(_T("could now map view into memory mapped file"));

		return address;
	}

	template<typename T>
	T *MapAs(DWORD64 offset, SIZE_T bytesToMap, DWORD desiredAccess) const
	{
		void *address=Map(offset,bytesToMap,desiredAccess);
		return reinterpret_cast<T*>(address);
	}

	void Unmap(const void *address) const
	{
		auto success=::UnmapViewOfFile(address);
		if(!success) throw IOException(_T("could now unmap view"));
	}

	static MemoryMappedFile FromFile(const File &file, DWORD64 maximumMappingSize, DWORD protection)
	{
		auto fileHandle=file.UnderlyingHandle();

		DWORD low=static_cast<DWORD>(maximumMappingSize);
		DWORD high=static_cast<DWORD>(maximumMappingSize>>32);

		auto mappingHandle=::CreateFileMapping(fileHandle,nullptr,protection,high,low,nullptr);
		if(mappingHandle==Traits::InvalidValue()) throw IOException(_T("could not create memory mapped file from file"));

		return MemoryMappedFile(mappingHandle);
	}

	static MemoryMappedFile InMemory(DWORD64 maximumMappingSize, DWORD protection, const tstd::tstring &optionalMappingName=_T(""))
	{
		DWORD low=static_cast<DWORD>(maximumMappingSize);
		DWORD high=static_cast<DWORD>(maximumMappingSize>>32);

		const TCHAR *mappingName=nullptr;
		if(optionalMappingName.length()!=0) mappingName=optionalMappingName.c_str();

		auto mappingHandle=::CreateFileMapping(INVALID_HANDLE_VALUE,nullptr,protection,high,low,mappingName);
		if(mappingHandle==Traits::InvalidValue()) throw IOException(_T("could not create in-memory mapped file"));

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
		auto handle=::OpenFileMapping(fileMappingAccessRights,FALSE,mappingName.c_str());
		if(handle==Traits::InvalidValue()) throw IOException(_T("could not open existing mapping"));

		return MemoryMappedFile(handle);
	} 
};

}} // end of namespace