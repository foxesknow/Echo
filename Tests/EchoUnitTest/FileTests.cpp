#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo/File.h>

namespace EchoUnitTest 
{

TEST_CLASS(FileTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		File file;
	}
};

} // end of namespace