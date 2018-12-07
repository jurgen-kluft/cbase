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
			ascii::copy(ascii::crunes("This is a test string"), cz);
		}
	}
}
UNITTEST_SUITE_END
