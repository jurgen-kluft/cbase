#include "xbase/x_chars.h"
#include "xbase/x_string_ascii.h"
#include "xunittest/xunittest.h"

using namespace xcore;


UNITTEST_SUITE_BEGIN(xchars)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test_xucharz_128)
		{
			xucharz<128> cz;
			cz = "This is a test string";
		}
	}
}
UNITTEST_SUITE_END
