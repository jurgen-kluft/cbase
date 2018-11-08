#include "xbase/x_runes.h"
#include "xbase/x_runes.h"

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
			ascii::runez<256> str;
			ascii::crunes fmt("%f");

			f64 d = 3.1415;
			ascii::sprintf(str, fmt, x_va(d));
			ascii::crunes answer_str("3.141500");
			CHECK_EQUAL(0, ascii::compare(answer_str, str));
		}

		UNITTEST_TEST(format_string)
		{
			ascii::runez<256> str;
			ascii::crunes fmt("the %s");

			ascii::sprintf(str, fmt, x_va("test string"));
			CHECK_EQUAL(0, ascii::compare(str, "the test string"));
		}
	}
}
UNITTEST_SUITE_END
