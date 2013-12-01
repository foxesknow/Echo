#include "stdafx.h"
#include "CppUnitTest.h"

#include <chrono>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo/tstring.h>

namespace EchoUnitTest {

TEST_CLASS(TString)
{
public:
	TEST_METHOD(Trim_string)
	{
		std::string text1="  Hello";
		auto trim1=tstd::Trim(text1);
		Assert::IsTrue(trim1=="Hello",nullptr,LINE_INFO());

		std::string text2="Hello world";
		auto trim2=tstd::Trim(text2);
		Assert::IsTrue(trim2=="Hello world",nullptr,LINE_INFO());

		std::string text3="Hello   ";
		auto trim3=tstd::Trim(text3);
		Assert::IsTrue(trim3=="Hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(Trim_wstring)
	{
		std::wstring text1=L"  Hello";
		auto trim1=tstd::Trim(text1);
		Assert::IsTrue(trim1==L"Hello",nullptr,LINE_INFO());

		std::wstring text2=L"Hello world";
		auto trim2=tstd::Trim(text2);
		Assert::IsTrue(trim2==L"Hello world",nullptr,LINE_INFO());

		std::wstring text3=L"Hello   ";
		auto trim3=tstd::Trim(text3);
		Assert::IsTrue(trim3==L"Hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(Trim_string_CustomWhitespace)
	{
		std::string text=">>Hello<<";
		auto trim=tstd::Trim(text,"><");
		Assert::IsTrue(trim=="Hello",nullptr,LINE_INFO());
	}

	TEST_METHOD(Trim_wstring_CustomWhitespace)
	{
		std::wstring text=L">>Hello<<";
		auto trim=tstd::Trim(text,L"><");
		Assert::IsTrue(trim==L"Hello",nullptr,LINE_INFO());
	}
};

} // end of namesapce