#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\Exceptions.h>

namespace EchoUnitTest 
{

TEST_CLASS(ExceptionTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		Exception exception(_T("hello"));
		auto what = tstd::to_tstring(exception.what());

		Assert::AreEqual(what, tstd::to_tstring("hello"));
	}

	TEST_METHOD(Win32_Error)
	{
		using namespace Echo;

		::SetLastError(ERROR_FILE_NOT_FOUND);

		WindowsException exception(_T("something went wrong"));
		
		auto asString = exception.ErrorCodeAsString();
		Assert::IsTrue(asString.size() != 0);
	}
};

} // end of namespace