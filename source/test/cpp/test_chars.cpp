#include "cbase/c_runes.h"
#include "cbase/c_runes.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_chars)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test_runez_128)
		{
            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            runes_t dst(dst_runes, 0, 0, 256);

			nrunes::copy(crunes_t("This is a test string"), dst);
		}
	}
}
UNITTEST_SUITE_END
