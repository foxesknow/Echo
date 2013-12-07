#pragma once

#include <string>
#include <algorithm>
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

/*
 * Numeric conversion functions
 */
#ifdef UNICODE
	inline tstring to_tstring(int value)					{return std::to_wstring(value);}
	inline tstring to_tstring(long value)					{return std::to_wstring(value);}
	inline tstring to_tstring(long long value)				{return std::to_wstring(value);}
	inline tstring to_tstring(unsigned value)				{return std::to_wstring(value);}
	inline tstring to_tstring(unsigned long value)			{return std::to_wstring(value);}
	inline tstring to_tstring(unsigned long long value)		{return std::to_wstring(value);}
	inline tstring to_tstring(float value)					{return std::to_wstring(value);}
	inline tstring to_tstring(double value)					{return std::to_wstring(value);}
	inline tstring to_tstring(long double value)			{return std::to_wstring(value);}

	inline tstd::tstring to_tstring(const std::string &t)	{tstring temp; temp.assign(t.begin(),t.end()); return temp;}
	inline tstd::tstring to_tstring(const std::wstring &t)	{return t;}

#else

	inline tstring to_tstring(int value)					{return std::to_string(value);}
	inline tstring to_tstring(long value)					{return std::to_string(value);}
	inline tstring to_tstring(long long value)				{return std::to_string(value);}
	inline tstring to_tstring(unsigned value)				{return std::to_string(value);}
	inline tstring to_tstring(unsigned long value)			{return std::to_string(value);}
	inline tstring to_tstring(unsigned long long value)		{return std::to_string(value);}
	inline tstring to_tstring(float value)					{return std::to_string(value);}
	inline tstring to_tstring(double value)					{return std::to_string(value);}
	inline tstring to_tstring(long double value)			{return std::to_string(value);}

	inline tstd::tstring to_tstring(const std::string &t)	{return t;}
	inline tstd::tstring to_tstring(const std::wstring &t)	{tstring temp; temp.assign(t.begin(),t.end()); return temp;}
#endif

/**
 * Converts a string to a string
 */
inline std::string to_string(const std::string &t)		
{
	return t;
}

/**
 * Converts a wide string to a string
 */
inline std::string to_string(const std::wstring &t)		
{
	std::string temp; 
	temp.assign(t.begin(),t.end()); 
	return temp;
}

/**
 * Converts a string to a wide string
 */
inline std::wstring to_wstring(const std::string &t)	
{
	std::wstring temp; 
	temp.assign(t.begin(),t.end()); 
	return temp;
}

/**
 * Converts a wide string to a wide string
 */
inline std::wstring to_wstring(const std::wstring &t)	
{
	return t;
}

/**
 * Trims leading and trailing whitespace from a string
 * @param text  the text to strip whitespace from
 * @params whitespace  the characters that are deemed to be whitespace
 * @returns a string
 */
inline std::string trim(const std::string &text, const std::string &whitespace=" \t")
{
	auto begin=text.find_first_not_of(whitespace);
	if(begin==std::string::npos) return "";

	auto end=text.find_last_not_of(whitespace);
	auto range=(end-begin)+1;

	return text.substr(begin,range);
}

/**
 * Trims leading and trailing whitespace from a string
 * @param text  the text to strip whitespace from
 * @params whitespace  the characters that are deemed to be whitespace
 * @returns a string
 */
inline std::wstring trim(const std::wstring &text, const std::wstring &whitespace=L" \t")
{
	auto begin=text.find_first_not_of(whitespace);
	if(begin==std::string::npos) return L"";

	auto end=text.find_last_not_of(whitespace);
	auto range=(end-begin)+1;

	return text.substr(begin,range);
}

/**
 * Converts a string to upper case
 * @param text  the text to convert
 * @return a new string
 */
inline std::string to_upper(const std::string &text)
{
	std::string copy=text;
	std::transform(copy.begin(),copy.end(),copy.begin(),toupper);

	return copy;
}

/**
 * Converts a string to upper case
 * @param text  the text to convert
 * @return a new string
 */
inline std::wstring to_upper(const std::wstring &text)
{
	std::wstring copy=text;
	std::transform(copy.begin(),copy.end(),copy.begin(),toupper);

	return copy;
}

/**
 * Converts a string to lower case
 * @param text  the text to convert
 * @return a new string
 */
inline std::string to_lower(const std::string &text)
{
	std::string copy=text;
	std::transform(copy.begin(),copy.end(),copy.begin(),tolower);

	return copy;
}

/**
 * Converts a string to lower case
 * @param text  the text to convert
 * @return a new string
 */
inline std::wstring to_lower(const std::wstring &text)
{
	std::wstring copy=text;
	std::transform(copy.begin(),copy.end(),copy.begin(),tolower);

	return copy;
}

} // end of namespace