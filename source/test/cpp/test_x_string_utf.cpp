#include "xbase/x_allocator.h"
#include "xbase/x_carray.h"
#include "xbase/x_va_list.h"
#include "xbase/x_string_ascii.h"
#include "xbase/x_string_utf.h"
#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;


UNITTEST_SUITE_BEGIN(xstring_utf)
{
	UNITTEST_FIXTURE(utf8)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(type_size_checks)
		{
			CHECK_EQUAL(1, sizeof(uchar));
			CHECK_EQUAL(1, sizeof(uchar8));
			CHECK_EQUAL(2, sizeof(uchar16));
			CHECK_EQUAL(4, sizeof(uchar32));
		}

		UNITTEST_TEST(strlen_char)
		{
			// s32  		strlen		(char const* _str, char const** _end = 0);
			const uchar8 utf8_str1[] = { 0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0, 0, 0, 0 };	// fr�nch
			u32 utf8_str1_len = utf8::size(utf8_str1);
			CHECK_EQUAL(6, utf8_str1_len);
		}

		UNITTEST_TEST(strlen_ustr16)
		{
			//s32  		strlen		(ustr16 const* _str, ustr16 const** _end = 0);

		}

		UNITTEST_TEST(numBytes_utf8)
		{
			const uchar8* str1 = (const uchar8*)"test";
			CHECK_EQUAL(4, utf8::size(str1));
			const uchar8 str2[] = { 0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0 };	// fr�nch
			const uchar8* str = (const uchar8*)str2;
			uchar32 c;
			str = utf::read(str, c);
			str = utf::read(str, c);
			str = utf::read(str, c);
			CHECK_EQUAL(2, utf::size(c));
		}

		UNITTEST_TEST(read1_utf8)
		{
			const uchar8* str1 = (const uchar8*)"test";
			const uchar8* str11 = (const uchar8*)"test";
			for (s32 i=0; i<5; ++i)
			{
				uchar32 ch;
				str1 = utf::read(str1, ch);
				CHECK_EQUAL((uchar32)str11[i], ch);
				CHECK_EQUAL(1, utf::size(ch));
			}

			const uchar8 str2[] = { 0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0x0 };	// fr�nch
			const uchar8* str = (const uchar8*)str2;

			uchar32 c;
			str = utf::read(str, c);
			str = utf::read(str, c);
			str = utf::read(str, c);
			CHECK_EQUAL(2, utf::size(c));	// take the �

			str = (const uchar8*)str2;
			for (s32 i=0; i<6; ++i)
			{
				str = utf::read(str, c);
				if (i==2)
				{
					CHECK_EQUAL(2, utf::size(c));
					CHECK_EQUAL(0xe9, c);
				}
				else if (i>2)
				{
					CHECK_EQUAL(1, utf::size(c));
					CHECK_EQUAL(c, (uchar32)str2[i+1]);
				}
				else
				{
					CHECK_EQUAL(1, utf::size(c));
					CHECK_EQUAL(c, (uchar32)str2[i]);
				}
			}
		}

		UNITTEST_TEST(numBytes_char)
		{
			CHECK_EQUAL(1, utf::size(uchar32('A')));
			CHECK_EQUAL(1, utf::size(uchar32('0')));
			CHECK_EQUAL(2, utf::size(uchar32(0xe9)));
		}
	}

	UNITTEST_FIXTURE(utf32)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(type_size_checks)
		{
			CHECK_EQUAL(1, sizeof(uchar));
			CHECK_EQUAL(1, sizeof(uchar8));
			CHECK_EQUAL(2, sizeof(uchar16));
			CHECK_EQUAL(4, sizeof(uchar32));
		}
	}
}
UNITTEST_SUITE_END
