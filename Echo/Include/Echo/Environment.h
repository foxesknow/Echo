#pragma once

#include <Echo\WinInclude.h>
#include <Echo\tstring.h>
#include <Echo\Exceptions.h>

#include <string>
#include <mutex>
#include <vector>

namespace Echo {

class Environment final
{
private:
	static std::once_flag s_InitFlag;

	static SYSTEM_INFO s_SystemInfo;

	static void InitializeSystemInfo()
	{
		::GetSystemInfo(&s_SystemInfo);
	}

	static void Initialize()
	{
		std::call_once(s_InitFlag, []{InitializeSystemInfo();});
	}

public:
	Environment() = delete;
	Environment(const Environment&) = delete;
	Environment &operator=(const Environment&) = delete;

	/**
	 * Returns the number of processors in the system
	 */
	static DWORD ProcessorCount() noexcept
	{
		Initialize();
		return s_SystemInfo.dwNumberOfProcessors;
	}

	/**
	 * Returns the number of bytes in a page
	 */
	static DWORD PageSize() noexcept
	{
		Initialize();
		return s_SystemInfo.dwPageSize;
	}

	/**
	 * Returns the name of the machine
	 */
	static tstd::tstring MachineName()
	{
		TCHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD size = sizeof(buffer) / sizeof(buffer[0]);

		auto success = ::GetComputerName(buffer, &size);
		if(!success) throw WindowsException(_T("MachineName failed"));

		return buffer;
	}

	/**
	 * Returns the username
	 */
	static tstd::tstring Username()
	{
		TCHAR buffer[257];
		DWORD size = sizeof(buffer) / sizeof(buffer[0]);

		auto success = ::GetUserName(buffer, &size);
		if(!success) throw WindowsException(_T("Username failed"));

		return buffer;
	}

	/**
	 * Returns the command line for the process
	 */
	static tstd::tstring CommandLine() noexcept
	{
		tstd::tstring commandLine = ::GetCommandLine();
		return commandLine;
	}

	/**
	 * Returns the command line as a vector, similar to argc/argv in main()
	 */
	static std::vector<tstd::tstring> CommandLineAsVector()
	{
		int argCount = 0;
		LPWSTR *argList = ::CommandLineToArgvW(::GetCommandLineW(), &argCount);
		if(argList == nullptr) throw WindowsException(_T("CommandLineAsVector failed"));

		std::vector<tstd::tstring> parts;

		for(int i = 0; i < argCount; i++)
		{
			std::wstring part = argList[i];
			parts.push_back(tstd::to_tstring(part));
		}

		::LocalFree(argList);

		return parts;
		
	}
};

__declspec(selectany) SYSTEM_INFO Environment::s_SystemInfo;
__declspec(selectany) std::once_flag Environment::s_InitFlag;

} // end of namespace