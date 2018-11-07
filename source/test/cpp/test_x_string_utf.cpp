#include "xbase/x_allocator.h"
#include "xbase/x_carray.h"
#include "xbase/x_va_list.h"
#include "xbase/x_string_ascii.h"
#include "xbase/x_string_utf.h"
#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::xalloc* gTestAllocator;


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
			CHECK_EQUAL(1, sizeof(ascii::rune));
			CHECK_EQUAL(1, sizeof(utf8::rune));
			CHECK_EQUAL(2, sizeof(utf16::rune));
			CHECK_EQUAL(4, sizeof(utf32::rune));
		}

		UNITTEST_TEST(strlen_char)
		{
			// s32  		strlen		(char const* _str, char const** _end = 0);
			const uchar8 utf8_str1[] = { 0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0, 0, 0, 0 };	// fr�nch
			u32 utf8_str1_len = utf8::len(utf8_str1, NULL);
			CHECK_EQUAL(6, utf8_str1_len);
		}

		UNITTEST_TEST(strlen_ustr16)
		{
			//s32  		strlen		(ustr16 const* _str, ustr16 const** _end = 0);

		}

		UNITTEST_TEST(numBytes_utf8)
		{
			const uchar8* str1 = (const uchar8*)"test";
			CHECK_EQUAL(4, utf8::len(str1, NULL));
			const uchar8 str2[] = { 0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0 };	// fr�nch

			utf8::crunes str(str2, utf8::endof(str2, NULL));
			uchar32 c;
			c = utf::read(str);
			c = utf::read(str);
			c = utf::read(str);
			CHECK_EQUAL(2, utf::size(c));
		}

		UNITTEST_TEST(copy_ascii_to_utf32)
		{
			ascii::crunes src("test");
			CHECK_EQUAL('t', src.m_str[0]);
			CHECK_EQUAL('e', src.m_str[1]);

			utf32::runez<16> dst;
			utf::copy(src, dst);
			
			CHECK_EQUAL('t', dst.m_str[0]);
		}

		UNITTEST_TEST(read1_utf8)
		{
			utf8::crunes str1((utf8::pcrune)"test", nullptr);
			ascii::crunes str11((ascii::pcrune)"test", nullptr);
			for (s32 i=0; i<5; ++i)
			{
				uchar32 ch = utf::read(str1);
				CHECK_EQUAL((uchar32)str11.m_str[i], ch);
				CHECK_EQUAL(1, utf::size(ch));
			}

			const uchar8 str2[] = { 0x66, 0x72, 0xC3, 0xA9, 0x6E, 0x63, 0x68, 0x0 };	// fr�nch

			utf8::crunes str(str2, nullptr);
			uchar32 c;
			c = utf::read(str);
			c = utf::read(str);
			c = utf::read(str);
			CHECK_EQUAL(2, utf::size(c));	// take the �

			str = utf8::crunes(str2, nullptr);
			for (s32 i=0; i<6; ++i)
			{
				c = utf::read(str);
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
