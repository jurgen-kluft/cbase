#include "xbase/x_runes.h"
#include "xbase/x_runes.h"
#include "xunittest/xunittest.h"

using namespace xcore;


UNITTEST_SUITE_BEGIN(xchars)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test_runez_128)
		{
			ascii::runez<128> cz;
			ascii::copy(cz, ascii::crunes("This is a test string"));
		}
	}
}
UNITTEST_SUITE_END
