#include "xbase\x_carray.h"
#include "xbase\x_va_list.h"
#include "xbase\x_string_ascii.h"
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

		UNITTEST_TEST(type_size_checks)
		{
			CHECK_EQUAL(4, sizeof(uchar8));
			CHECK_EQUAL(1, sizeof(ustr8));
			CHECK_EQUAL(4, sizeof(uchar16));
			CHECK_EQUAL(2, sizeof(ustr16));
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

		UNITTEST_TEST(numBytes_utf8)
		{
			const ustr8* str1 = (const ustr8*)"test";
			CHECK_EQUAL(1, utf::numBytes(uchar8(str1->c)));
			const ustr8 str2[] = { 0x66,0x72,0xC3,0xA9,0x6E,0x63,0x68,0 };	// frénch
			CHECK_EQUAL(2, utf::numBytes(uchar8(str2[2].c)));	// take the é
		}

		UNITTEST_TEST(read1_utf8)
		{
			const ustr8* str1 = (const ustr8*)"test";
			for (s32 i=0; i<5; ++i)
			{
				uchar8 ch;
				CHECK_EQUAL(1, utf::read(&str1[i], ch));
				CHECK_TRUE(ch.c == str1[i].c);
			}

			const ustr8 str2[] = { 0x66,0x72,0xC3,0xA9,0x6E,0x63,0x68,0 };	// frénch
			CHECK_EQUAL(2, utf::numBytes(uchar8(str2[2].c)));	// take the é
			for (s32 i=0; i<7; ++i)
			{
				uchar8 ch;
				if (i==2)
				{
					CHECK_EQUAL(2, utf::read(&str2[i], ch));
					CHECK_EQUAL(0xA9C3, ch.c);
				}
				else if (i>2)
				{
					CHECK_EQUAL(1, utf::read(&str2[i+1], ch));
					CHECK_TRUE(ch.c == str2[i+1].c);
				}
				else
				{
					CHECK_EQUAL(1, utf::read(&str2[i], ch));
					CHECK_TRUE(ch.c == str2[i].c);
				}
			}
		}

		UNITTEST_TEST(utf_uptr8)
		{
			char str1[256];
			utf::uptr8 i1   = (ustr8*)str1;
			utf::uptr8 end1 = i1 + 256;

			char const* str2 = "This is another simple ASCII string.";
			utf::ucptr8 i2   = (ustr8 const*)str2;
			utf::ucptr8 end2 = i2 + i2.strlen();

			CHECK_TRUE(end2.at_end());

			// copy str2 to str1;
			while (i1 != end1 && !i2.at_end())
			{
				utf::ulen8 n = i1.copy_char_from(i2);
				CHECK_EQUAL(1,n.blen());
				CHECK_EQUAL(1,n.clen());
				i1 += n;
				i2 += n;
			}
			*i1 = '\0';
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

			char cstr2[] = { 'h', 'i', 0xFF, '!', '0' };
			const ustr8* str2 = (const ustr8*)cstr2;
			CHECK_FALSE(utf::isLegal(str2, str2 + 4));
		}

	}
}
UNITTEST_SUITE_END
