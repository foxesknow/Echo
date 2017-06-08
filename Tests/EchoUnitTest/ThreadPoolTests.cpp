#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\ThreadPool.h>

namespace EchoUnitTest 
{

TEST_CLASS(ThreadPoolTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		bool flag=false;

		ThreadPool pool;
	}
};

} // end of namespace