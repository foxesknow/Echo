#include "stdafx.h"
#include "CppUnitTest.h"

#include <chrono>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo/tstring.h>

namespace EchoUnitTest {

TEST_CLASS(TString)
{
public:
	TEST_METHOD(trim_string)
	{
		std::string text1="  Hello";
		auto trim1=tstd::trim(text1);
		Assert::IsTrue(trim1=="Hello",nullptr,LINE_INFO());

		std::string text2="Hello world";
		auto trim2=tstd::trim(text2);
		Assert::IsTrue(trim2=="Hello world",nullptr,LINE_INFO());

		std::string text3="Hello   ";
		auto trim3=tstd::trim(text3);
		Assert::IsTrue(trim3=="Hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(trim_wstring)
	{
		std::wstring text1=L"  Hello";
		auto trim1=tstd::trim(text1);
		Assert::IsTrue(trim1==L"Hello",nullptr,LINE_INFO());

		std::wstring text2=L"Hello world";
		auto trim2=tstd::trim(text2);
		Assert::IsTrue(trim2==L"Hello world",nullptr,LINE_INFO());

		std::wstring text3=L"Hello   ";
		auto trim3=tstd::trim(text3);
		Assert::IsTrue(trim3==L"Hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(trim_string_CustomWhitespace)
	{
		std::string text=">>Hello<<";
		auto trim=tstd::trim(text,"><");
		Assert::IsTrue(trim=="Hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(trim_wstring_CustomWhitespace)
	{
		std::wstring text=L">>Hello<<";
		auto trim=tstd::trim(text,L"><");
		Assert::IsTrue(trim==L"Hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(to_lower_string)
	{
		std::string text="HELLO";
		Assert::IsTrue(tstd::to_lower(text)=="hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(to_lower_wstring)
	{
		std::wstring text=L"HELLO";
		Assert::IsTrue(tstd::to_lower(text)==L"hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(to_upper_string)
	{
		std::string text="hello";
		Assert::IsTrue(tstd::to_upper(text)=="HELLO",nullptr,LINE_INFO());
	}

	TEST_METHOD(to_upper_wstring)
	{
		std::wstring text=L"hello";
		Assert::IsTrue(tstd::to_upper(text)==L"HELLO",nullptr,LINE_INFO());
	}

	TEST_METHOD(to_wstring_string)
	{
		std::string text="hello";
		std::wstring converted=tstd::to_wstring(text);

		Assert::IsTrue(converted==L"hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(to_wstring_wstring)
	{
		std::wstring text=L"hello";
		std::wstring converted=tstd::to_wstring(text);

		Assert::IsTrue(converted==L"hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(to_string_string)
	{
		std::string text="hello";
		std::string converted=tstd::to_string(text);

		Assert::IsTrue(converted=="hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(to_string_wstring)
	{
		std::wstring text=L"hello";
		std::string converted=tstd::to_string(text);

		Assert::IsTrue(converted=="hello",nullptr,LINE_INFO());
	}
};

} // end of namesapce