#include "xbase/x_runes.h"
#include "xbase/x_runes.h"
#include "xunittest/xunittest.h"

using namespace ncore;


UNITTEST_SUITE_BEGIN(xchars)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test_runez_128)
		{
			runez_t<ascii::rune, 128> cz;
			copy(crunes_t("This is a test string"), cz);
		}
	}
}
UNITTEST_SUITE_END
