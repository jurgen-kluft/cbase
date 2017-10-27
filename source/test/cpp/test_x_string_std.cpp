#include "xbase\x_va_list.h"
#include "xbase\x_string_ascii.h"
#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xstring_std)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(StrCopy)
		{
			const char* src = "this is a system string";
			
			char dst[24];
			ascii::copy(dst, dst + 24 - 1, src, ascii::len(src));

			CHECK_EQUAL(23, ascii::size(src));
			CHECK_EQUAL(0, ascii::compare(dst, ascii::len(dst), src, ascii::len(src)));

			char str2[20];
			ascii::copy(str2, str2 + sizeof(str2), src, ascii::len(src));

			CHECK_EQUAL(19, ascii::size(str2));
			CHECK_EQUAL(-1, ascii::compare(str2, src));
			CHECK_EQUAL(0, ascii::compare(str2, "this is a system st"));
		}

		UNITTEST_TEST(StrLen)
		{
			CHECK_EQUAL(0, ascii::size(""));

			const char* str = "this is a system string";
			CHECK_EQUAL(23, ascii::size(str));
		}

		UNITTEST_TEST(StrCmp)
		{
			const char* str1 = "this is a system string";
			const char* str2 = "this is a system string";
			CHECK_EQUAL(0, ascii::compare(str1, str2));

			const char* str3 = "a";
			const char* str4 = "b";
			CHECK_EQUAL(-1, ascii::compare(str3, str4));
		}

		UNITTEST_TEST(Strncmp)
		{
		}

		UNITTEST_TEST(Stricmp)
		{
		}

		UNITTEST_TEST(Strnicmp)
		{
		}

		UNITTEST_TEST(Stristr)
		{
		}

		UNITTEST_TEST(Strstr)
		{
		}

		UNITTEST_TEST(Strscn)
		{
		}

		UNITTEST_TEST(Memchr)
		{
		}

		UNITTEST_TEST(Strcat)
		{
		}

		UNITTEST_TEST(Strtof)
		{
		}

		UNITTEST_TEST(Strtod)
		{
		}

		UNITTEST_TEST(F64ToStr)
		{
			char str_buffer[16 + 1];
			char * str = &str_buffer[0];
			char * eos = &str_buffer[16];

			char * end = ascii::to_string(str, eos, eos, 3.1415, 2);
			CHECK_EQUAL(4, ascii::size(str));
		}
		
		UNITTEST_TEST(isEqualNoCase)
		{
			CHECK_EQUAL(true, ascii::is_equal('a', 'A', ascii::CASE_IGNORE));
			CHECK_EQUAL(true, ascii::is_equal('a', 'a', ascii::CASE_IGNORE));
			CHECK_EQUAL(false, ascii::is_equal('a', 'B', ascii::CASE_IGNORE));
			CHECK_EQUAL(true, ascii::is_equal('z', 'Z', ascii::CASE_IGNORE));
			CHECK_EQUAL(false, ascii::is_equal('=', '+', ascii::CASE_IGNORE));
			CHECK_EQUAL(true, ascii::is_equal('?', '?', ascii::CASE_IGNORE));
		}

		UNITTEST_TEST(CPrintf)
		{
			s32 const i = 100;
			const char* str = "hello";

			const char* fmt = "%d %s";
			const char* fmt_end = fmt + 5;
			s32 length = ascii::cprintf(fmt, fmt_end, x_va(i), x_va(str));

			CHECK_EQUAL(9, length);
		}

		UNITTEST_TEST(VCPrintf)
		{
			s32 i = 100;
			const char* str = "hello";
			const char* fmt = "%d %s";
			const char* fmt_end = fmt + 5;
			s32 length = ascii::vcprintf(fmt, fmt_end, x_va_list(x_va(i), x_va(str)));
			CHECK_EQUAL(9, length);
		}

		UNITTEST_TEST(SPrintf)
		{
			char buffer_buffer[16 + 1];
			char * buffer = &buffer_buffer[0];
			char * buffer_end = &buffer_buffer[16];
			
			s32 i = 100;

			const char* str = "hello";
			const char* fmt = "%d %s";
			const char* fmt_end = fmt + 5;

			s32 length = ascii::sprintf(buffer, buffer_end, fmt, fmt_end, x_va(i), x_va(str));
			CHECK_EQUAL(9, length);
			CHECK_TRUE(ascii::compare(buffer, "100 hello")==0);

			// Check all format functionality?

			// ---------------------------------------------------------------------------
			// Boolean, True/False and Yes/No verification
			length = ascii::sprintf(buffer, buffer_end, "%b", x_va(true));
			CHECK_EQUAL(4, length);
			CHECK_TRUE(ascii::compare(buffer, "true")==0);

			length = ascii::sprintf(buffer, buffer_end, "%B", x_va(true));
			CHECK_EQUAL(4, length);
			CHECK_TRUE(ascii::compare(buffer, "TRUE")==0);

			length = ascii::sprintf(buffer, buffer_end, "%b", x_va(false));
			CHECK_EQUAL(5, length);
			CHECK_TRUE(ascii::compare(buffer, "false")==0);

			length = ascii::sprintf(buffer, buffer_end, "%#b", x_va(false));
			CHECK_EQUAL(5, length);
			CHECK_TRUE(ascii::compare(buffer, "False")==0);

			length = ascii::sprintf(buffer, buffer_end, "%y", x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(ascii::compare(buffer, "yes")==0);

			length = ascii::sprintf(buffer, buffer_end, "%y", x_va(false));
			CHECK_EQUAL(2, length);
			CHECK_TRUE(ascii::compare(buffer, "no")==0);

			length = ascii::sprintf(buffer, buffer_end, "%Y", x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(ascii::compare(buffer, "YES")==0);

			length = ascii::sprintf(buffer, buffer_end, "%#y", x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(ascii::compare(buffer, "Yes")==0);
			// ---------------------------------------------------------------------------

		}

		UNITTEST_TEST(VSPrintf)
		{
			char buffer_buffer[16 + 1];
			char * buffer = &buffer_buffer[0];
			char * buffer_end = &buffer_buffer[16];

			s32 i = 100;
			const char* str = "hello";
			const char* fmt = "%d %s";
			s32 length = ascii::vsprintf(buffer, buffer_end, fmt, x_va_list(x_va(i), x_va(str)));
			CHECK_EQUAL(9, length);
			CHECK_TRUE(ascii::compare(buffer, "100 hello")==0);
		}

		UNITTEST_TEST(SScanf)
		{
			// Test scanf
			const char* str = "1.0 100";
			const char* format = "%f %u";
			
			f32 myfloat;
			u32 myint;
			ascii::sscanf(str, format, x_va_r(&myfloat), x_va_r(&myint));

			CHECK_EQUAL(1.0f, myfloat);
			CHECK_EQUAL(100, myint);
		}


	}
}
UNITTEST_SUITE_END
