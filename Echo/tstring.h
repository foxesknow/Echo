#pragma once

#include <string>
#include <sstream>
#include <tchar.h>

namespace tstd
{

#ifdef UNICODE
	typedef std::wstring tstring;
	typedef std::wstringstream tstringstream;
	typedef std::wistringstream tistringstream;
	typedef std::wostringstream tostringstream;
	typedef std::wstringbuf tstringbuf;
#else
	typedef std::string tstring;
	typedef std::stringstream tstringstream;
	typedef std::istringstream tistringstream;
	typedef std::ostringstream tostringstream;
	typedef std::stringbuf tstringbuf;
#endif

} // end of namespace