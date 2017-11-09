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
			const s32 str_buffer_size = 256;
			char str_buffer[str_buffer_size + 1];
			uchar* str = (uchar*)&str_buffer[0];
			uchar* str_end = str + str_buffer_size;
			uchar const* fmt = (uchar const*)"%f";
			uchar const* fmt_end = fmt + 2;

			f64 d = 3.1415;
			s32 l = ascii::sprintf(str, str_end, fmt, fmt_end, x_va(d));
			CHECK_EQUAL(8, l);
			const uchar* answer_str = (const uchar*)"3.141500";
			CHECK_EQUAL(0, ascii::compare(answer_str, ascii::len(answer_str), str, ascii::len(str)));
		}

		UNITTEST_TEST(format_string)
		{
			const s32 str_buffer_size = 256;
			char str_buffer[str_buffer_size + 1];
			uchar* str = (uchar*)&str_buffer[0];
			uchar* str_end = str + str_buffer_size;
			uchar const* fmt = (uchar const*)"the %s";
			uchar const* fmt_end = fmt + 6;

			const char* teststr = "test string";
			s32 l = ascii::sprintf(str, str_end, fmt, fmt_end, x_va(teststr));
			CHECK_EQUAL(15, l);
			const uchar* answer_str = (const uchar*)"the test string";
			CHECK_EQUAL(0, ascii::compare(answer_str, NULL, str, NULL));
		}
	}
}
UNITTEST_SUITE_END
