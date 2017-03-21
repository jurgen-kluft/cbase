#include "xbase\x_carray.h"
#include "xbase\x_va_list.h"
#include "xbase\x_string_ascii.h"
#include "xbase\x_string_utf.h"
#include "xunittest\xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;


UNITTEST_SUITE_BEGIN(xutf)
{
	UNITTEST_FIXTURE(utilities)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(type_size_checks)
		{
			CHECK_EQUAL(1, sizeof(char));
			CHECK_EQUAL(1, sizeof(uchar8));
			CHECK_EQUAL(1, sizeof(ustr8));
			CHECK_EQUAL(2, sizeof(uchar16));
			CHECK_EQUAL(2, sizeof(ustr16));
			CHECK_EQUAL(4, sizeof(uchar32));
			CHECK_EQUAL(4, sizeof(ustr32));
		}

		UNITTEST_TEST(strlen_char)
		{
			// s32  		strlen		(char const* _str, char const** _end = 0);
			const char utf8_str1[] = { (char)0x66,(char)0x72,(char)0xC3,(char)0xA9,(char)0x6E,(char)0x63,(char)0x68,(char)0 };	// frénch
			u32 utf8_str1_len = StrLen(utf8_str1);
			CHECK_EQUAL(6, utf8_str1_len);
		}

		UNITTEST_TEST(strlen_ustr16)
		{
			//s32  		strlen		(ustr16 const* _str, ustr16 const** _end = 0);

		}

		UNITTEST_TEST(numBytes_utf8)
		{
			const char* str1 = (const char*)"test";
			CHECK_EQUAL(1, LenInBytes(*str1));
			const char str2[] = { (char)0x66,(char)0x72,(char)0xC3,(char)0xA9,(char)0x6E,(char)0x63,(char)0x68,(char)0 };	// frénch
			const char* str = (const char*)str2;
			uchar32 c = ReadChar(str);
			c = ReadChar(str);
			c = ReadChar(str);	// take the é
			CHECK_EQUAL(2, LenInBytes(c));
		}

		UNITTEST_TEST(read1_utf8)
		{
			const char* str1 = (const char*)"test";
			const char* str11 = (const char*)"test";
			for (s32 i=0; i<5; ++i)
			{
				uchar32 ch = ReadChar(str1);
				CHECK_EQUAL((uchar32)str11[i], ch);
				CHECK_EQUAL(1, LenInBytes(ch));
			}

			const char str2[] = { (char)0x66,(char)0x72,(char)0xC3,(char)0xA9,(char)0x6E,(char)0x63,(char)0x68,(char)0 };	// frénch
			const char* str = (const char*)str2;
			ReadChar(str);
			ReadChar(str);
			uchar32 c = ReadChar(str);
			CHECK_EQUAL(2, LenInBytes(c));	// take the é
			str = (const char*)str2;
			for (s32 i=0; i<6; ++i)
			{
				uchar32 ch = ReadChar(str);
				if (i==2)
				{
					CHECK_EQUAL(2, LenInBytes(ch));
					CHECK_EQUAL(0xC3A9, ch);
				}
				else if (i>2)
				{
					CHECK_EQUAL(1, LenInBytes(ch));
					CHECK_EQUAL(ch, (uchar32)str2[i+1]);
				}
				else
				{
					CHECK_EQUAL(1, LenInBytes(ch));
					CHECK_EQUAL(ch, (uchar32)str2[i]);
				}
			}
		}

		UNITTEST_TEST(numBytes_char)
		{
			CHECK_EQUAL(1, LenInBytes(uchar32('A')));
			CHECK_EQUAL(1, LenInBytes(uchar32('0')));
			CHECK_EQUAL(2, LenInBytes(uchar32(0xC3A9)));
		}
	}


	UNITTEST_FIXTURE(string)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(construct)
		{
			xstring str(gTestAllocator);
			
			const char* ascii_test_string = "this is a test string";
			str = ascii_test_string;

			CHECK_EQUAL(*str[0], ascii_test_string[0]);
			CHECK_EQUAL(*str[1], ascii_test_string[1]);
		}

		UNITTEST_TEST(len)
		{
			xstring str(gTestAllocator);

			const char* ascii_test_string = "this is a test string";
			str = ascii_test_string;

			CHECK_EQUAL(str.len(), LenInChars(ascii_test_string));
		}

		UNITTEST_TEST(copy)
		{
			xstring str(gTestAllocator);

			const char* ascii_test_string = "this is a test string";
			str = ascii_test_string;

			xstring str_copy = str.copy();

			CHECK_EQUAL(str_copy.len(), LenInChars(ascii_test_string));
		}

		UNITTEST_TEST(view)
		{
			xstring str(gTestAllocator);

			const char* ascii_test_string = "this is a test string";
			str = ascii_test_string;

			xstring str_view = str(10, str.len());

			CHECK_EQUAL(11, str_view.len());
		}

	}

}
UNITTEST_SUITE_END
