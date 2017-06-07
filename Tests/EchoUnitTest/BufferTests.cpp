#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Echo/Buffer.h>

namespace EchoUnitTest 
{

TEST_CLASS(BufferTests)
{
public:
	TEST_METHOD(Construct)
	{
		using namespace Echo;

		Buffer buffer(16);
		Assert::AreEqual((size_t)16,buffer.Size(),nullptr,LINE_INFO());
		Assert::IsNotNull(buffer.Data(),nullptr,LINE_INFO());
	}

	TEST_METHOD(Clear)
	{
		using namespace Echo;

		Buffer buffer(16);
		buffer.Clear();
		
		BYTE *data=buffer.DataAs<BYTE>();

		for(size_t i=0; i<buffer.Size(); i++)
		{
			Assert::AreEqual((BYTE)0,data[i],nullptr,LINE_INFO());
		}
	}

	TEST_METHOD(Fill)
	{
		using namespace Echo;

		Buffer buffer(16);
		buffer.Fill(10);
		
		BYTE *data=buffer.DataAs<BYTE>();

		for(size_t i=0; i<buffer.Size(); i++)
		{
			Assert::AreEqual((BYTE)10,data[i],nullptr,LINE_INFO());
		}
	}

};

} // end of namespace