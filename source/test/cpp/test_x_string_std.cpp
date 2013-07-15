#include "xbase\x_types.h"
#include "xbase\x_va_list.h"
#include "xbase\x_string_std.h"
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

		UNITTEST_TEST(x_strcpy)
		{
			const char* str = "this is a system string";

			char str1[24];
			x_strcpy(str1, sizeof(str1), str);
			CHECK_EQUAL(23, x_strlen(str1));
			CHECK_EQUAL(0, x_strcmp(str1, str));
			char str2[20];
			x_strcpy(str2, sizeof(str2), str);
			CHECK_EQUAL(19, x_strlen(str2));
			CHECK_EQUAL(-1, x_strcmp(str2, str));
			CHECK_EQUAL(0, x_strcmp(str2, "this is a system st"));
		}

		UNITTEST_TEST(x_strlen)
		{
			CHECK_EQUAL(0, x_strlen(""));

			const char* str = "this is a system string";
			CHECK_EQUAL(23, x_strlen(str));
		}

		UNITTEST_TEST(x_strcmp)
		{
			const char* str1 = "this is a system string";
			const char* str2 = "this is a system string";
			CHECK_EQUAL(0, x_strcmp(str1, str2));

			const char* str3 = "a";
			const char* str4 = "b";
			CHECK_EQUAL(-1, x_strcmp(str3, str4));
		}

		UNITTEST_TEST(x_strncmp)
		{
		}

		UNITTEST_TEST(x_stricmp)
		{
		}

		UNITTEST_TEST(x_strnicmp)
		{
		}

		UNITTEST_TEST(x_stristr)
		{
		}

		UNITTEST_TEST(x_strstr)
		{
		}

		UNITTEST_TEST(x_strscn)
		{
		}

		UNITTEST_TEST(x_memchr)
		{
		}

		UNITTEST_TEST(x_strcat)
		{
		}

		UNITTEST_TEST(x_strtof)
		{
		}

		UNITTEST_TEST(x_strtod)
		{
		}

		UNITTEST_TEST(x_f64toa)
		{
			char str[16];
			s32 len = x_f64toa(3.1415, 2, str, sizeof(str));
			CHECK_EQUAL(4, len);
		}
		
		UNITTEST_TEST(isEqualNoCase)
		{
			CHECK_EQUAL(true, x_isEqualNoCase('a', 'A'));
			CHECK_EQUAL(true, x_isEqualNoCase('a', 'a'));
			CHECK_EQUAL(false, x_isEqualNoCase('a', 'B'));
			CHECK_EQUAL(true, x_isEqualNoCase('z', 'Z'));
			CHECK_EQUAL(false, x_isEqualNoCase('=', '+'));
			CHECK_EQUAL(true, x_isEqualNoCase('?', '?'));
		}

		UNITTEST_TEST(x_cprintf)
		{
			s32 testInteger = 100;
			const char* testStr = "hello";
			const char* formatStr = "%d %s";
			s32 length = x_cprintf(formatStr, x_va(testInteger), x_va(testStr));
			CHECK_EQUAL(9, length);
		}

		UNITTEST_TEST(x_vcprintf)
		{
			s32 testInteger = 100;
			const char* testStr = "hello";
			const char* formatStr = "%d %s";
			s32 length = x_vcprintf(formatStr, x_va_list(x_va(testInteger), x_va(testStr)));
			CHECK_EQUAL(9, length);
		}

		UNITTEST_TEST(x_sprintf)
		{
			char buffer[16];
			s32 testInteger = 100;
			const char* testStr = "hello";
			const char* formatStr = "%d %s";
			s32 length = x_sprintf(buffer, sizeof(buffer)-1, formatStr, x_va(testInteger), x_va(testStr));
			CHECK_EQUAL(9, length);
			CHECK_TRUE(x_strcmp(buffer, "100 hello")==0);

			// Check all format functionality?

			// ---------------------------------------------------------------------------
			// Boolean, True/False and Yes/No verification
			length = x_sprintf(buffer, sizeof(buffer)-1, "%b", x_va(true));
			CHECK_EQUAL(4, length);
			CHECK_TRUE(x_strcmp(buffer, "true")==0);

			length = x_sprintf(buffer, sizeof(buffer)-1, "%B", x_va(true));
			CHECK_EQUAL(4, length);
			CHECK_TRUE(x_strcmp(buffer, "TRUE")==0);

			length = x_sprintf(buffer, sizeof(buffer)-1, "%b", x_va(false));
			CHECK_EQUAL(5, length);
			CHECK_TRUE(x_strcmp(buffer, "false")==0);

			length = x_sprintf(buffer, sizeof(buffer)-1, "%#b", x_va(false));
			CHECK_EQUAL(5, length);
			CHECK_TRUE(x_strcmp(buffer, "False")==0);

			length = x_sprintf(buffer, sizeof(buffer)-1, "%y", x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(x_strcmp(buffer, "yes")==0);

			length = x_sprintf(buffer, sizeof(buffer)-1, "%y", x_va(false));
			CHECK_EQUAL(2, length);
			CHECK_TRUE(x_strcmp(buffer, "no")==0);

			length = x_sprintf(buffer, sizeof(buffer)-1, "%Y", x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(x_strcmp(buffer, "YES")==0);

			length = x_sprintf(buffer, sizeof(buffer)-1, "%#y", x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(x_strcmp(buffer, "Yes")==0);
			// ---------------------------------------------------------------------------

		}

		UNITTEST_TEST(x_vsprintf)
		{
			char buffer[16];
			s32 testInteger = 100;
			const char* testStr = "hello";
			const char* formatStr = "%d %s";
			s32 length = x_vsprintf(buffer, sizeof(buffer)-1, formatStr, x_va_list(x_va(testInteger), x_va(testStr)));
			CHECK_EQUAL(9, length);
			CHECK_TRUE(x_strcmp(buffer, "100 hello")==0);
		}

		UNITTEST_TEST(x_sscanf)
		{
			// Test scanf
			const char* str = "1.0 100";
			const char* format = "%f %u";
			
			f32 myfloat;
			u32 myint;
			x_sscanf(str, format, x_va_r(&myfloat), x_va_r(&myint));

			CHECK_EQUAL(1.0f, myfloat);
			CHECK_EQUAL(100, myint);
		}


	}
}
UNITTEST_SUITE_END
