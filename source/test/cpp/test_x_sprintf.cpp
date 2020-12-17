#include "xbase/x_runes.h"
#include "xbase/x_printf.h"

#if defined(TARGET_PS3)
	#include <math.h>
#endif

#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xsprintf)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(format_double)
		{
			runez_t<ascii::rune, 256> str;
			crunes_t fmt("%f");

			f64 d = 3.1415;
			sprintf(str, fmt, x_va(d));
			crunes_t answer_str("3.141500");
			CHECK_EQUAL(0, compare(answer_str, str));
		}

		UNITTEST_TEST(format_string)
		{
			runez_t<ascii::rune, 256> str;
			crunes_t fmt("the %s");

			sprintf(str, fmt, x_va("test string"));
			CHECK_EQUAL(0, compare(str, "the test string"));
		}
	}
}
UNITTEST_SUITE_END
