#pragma once

#include <string>
#include <sstream>

namespace std
{

#ifdef UNICODE
	typedef std::wstring tstring;
	typedef std::wstringstream tstringstream;
#else
	typedef std::string tstring;
	typedef std::stringstream tstringstream;
#endif

} // end of namespace