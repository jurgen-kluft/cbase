#include "xbase\x_carray.h"
#include "xbase\x_va_list.h"
#include "xbase\x_string_utf.h"
#include "xunittest\xunittest.h"

using namespace xcore;


UNITTEST_SUITE_BEGIN(xutf)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(strlen_ustr8)
		{
			// s32  		strlen		(ustr8 const* _str, ustr8 const** _end = 0);
			const ustr8 utf8_str1[] = { 0x66,0x72,0xC3,0xA9,0x6E,0x63,0x68,0 };	// frénch
			u32 utf8_str1_len = utf::strlen(utf8_str1);
			CHECK_EQUAL(6, utf8_str1_len);
		}

		UNITTEST_TEST(strlen_ustr16)
		{
			//s32  		strlen		(ustr16 const* _str, ustr16 const** _end = 0);

		}

		UNITTEST_TEST(isLegal_utf8)
		{
			const ustr8* str1 = (const ustr8*)"hi!";
			CHECK_TRUE(utf::isLegal(str1, str1 + utf::strlen(str1)));

			const ustr8* str2 = (const ustr8*)"hi\255!";
			CHECK_FALSE(utf::isLegal(str2, str2 + 4));
		}

	}
}
UNITTEST_SUITE_END
