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
			nrunes::runestr_t<ascii::rune, 128> cz;
			copy(crunes_t("This is a test string"), cz);
		}
	}
}
UNITTEST_SUITE_END
