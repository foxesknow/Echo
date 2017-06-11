#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo\MemoryMappedFile.h>

namespace EchoUnitTest 
{

TEST_CLASS(MemoryMappedFileTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		MemoryMappedFile file;
	}
};

} // end of namespace