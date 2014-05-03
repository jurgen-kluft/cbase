#include "xbase\x_string_ascii.h"
#include "xbase\x_string_utf.h"

#if defined(TARGET_PS3)
	#include <math.h>
#endif

#include "xunittest\xunittest.h"

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
			char buffer[256];
			f64 d = 3.1415;
			s32 l = x_sprintf(buffer, sizeof(buffer) - 1, "%f", x_va(d));
			CHECK_EQUAL(8, l);
			CHECK_EQUAL(0, x_strCompare("3.141500", buffer));
		}

		UNITTEST_TEST(format_string)
		{
			char buffer[256];
			const char* teststr = "test string";
			s32 l = x_sprintf(buffer, sizeof(buffer) - 1, "the %s", x_va(teststr));
			CHECK_EQUAL(15, l);
			CHECK_EQUAL(0, x_strCompare("the test string", buffer));
		}

		UNITTEST_TEST(format_string8)
		{
			char buffer[256];
			const char* teststr = "test string";
			s32 l = utf::x_sprintf((ustr8*)buffer, sizeof(buffer) - 1, (const ustr8*)"the %s", x_va(teststr));
			CHECK_EQUAL(15, l);
			CHECK_EQUAL(0, x_strCompare("the test string", buffer));
		}


	}
}
UNITTEST_SUITE_END
