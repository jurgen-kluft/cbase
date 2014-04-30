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

		UNITTEST_TEST(numBytes_ustr8)
		{
			CHECK_TRUE(utf::isLegal(uchar8((u32)'A')));
			CHECK_EQUAL(1, utf::numBytes(uchar8((u32)'A')));
			CHECK_TRUE(utf::isLegal(uchar8((u32)'0')));
			CHECK_EQUAL(1, utf::numBytes(uchar8((u32)'0')));
			CHECK_TRUE(utf::isLegal(uchar8((u32)0xA9C3)));
			CHECK_EQUAL(2, utf::numBytes(uchar8((u32)0xA9C3)));
		}

		UNITTEST_TEST(numBytes_ustr16)
		{
			CHECK_TRUE(utf::isLegal(uchar16((u32)'A')));
			CHECK_EQUAL(2, utf::numBytes(uchar16((u32)'A')));
			CHECK_TRUE(utf::isLegal(uchar16((u32)'0')));
			CHECK_EQUAL(2, utf::numBytes(uchar16((u32)'0')));
			CHECK_TRUE(utf::isLegal(uchar16((u32)0x6C34)));			// 'z'
			CHECK_EQUAL(2, utf::numBytes(uchar16((u32)0x6C34)));
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
