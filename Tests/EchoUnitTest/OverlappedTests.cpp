#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\Overlapped.h>

namespace EchoUnitTest 
{

TEST_CLASS(OverlappedTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		Overlapped overlapped;
	}
};

} // end of namespace