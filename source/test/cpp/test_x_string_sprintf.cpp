#include "xbase/x_string_ascii.h"
#include "xbase/x_string_utf.h"

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
			xuchars256 str;
			xcuchars fmt("%f");

			f64 d = 3.1415;
			xuchars strc = str.chars();
			ascii::sprintf(strc, fmt, x_va(d));
			xcuchars answer_str("3.141500");
			CHECK_EQUAL(0, ascii::compare(answer_str, str.cchars()));
		}

		UNITTEST_TEST(format_string)
		{
			xuchars256 str;
			xcuchars fmt("the %s");

			ascii::sprintf(str.chars(), fmt, x_va("test string"));
	
			CHECK_EQUAL(0, str.cchars().compare("the test string"));
		}
	}
}
UNITTEST_SUITE_END
