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

		UNITTEST_TEST(StrCpy)
		{
			const char* str = "this is a system string";

			char str1[24];
			StrCpy(str1, sizeof(str1), str);
			CHECK_EQUAL(23, StrLen(str1));
			CHECK_EQUAL(0, StrCmp(str1, str));
			char str2[20];
			StrCpy(str2, sizeof(str2), str);
			CHECK_EQUAL(19, StrLen(str2));
			CHECK_EQUAL(-1, StrCmp(str2, str));
			CHECK_EQUAL(0, StrCmp(str2, "this is a system st"));
		}

		UNITTEST_TEST(StrLen)
		{
			CHECK_EQUAL(0, StrLen(""));

			const char* str = "this is a system string";
			CHECK_EQUAL(23, StrLen(str));
		}

		UNITTEST_TEST(StrCmp)
		{
			const char* str1 = "this is a system string";
			const char* str2 = "this is a system string";
			CHECK_EQUAL(0, StrCmp(str1, str2));

			const char* str3 = "a";
			const char* str4 = "b";
			CHECK_EQUAL(-1, StrCmp(str3, str4));
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
			char str[16];
			s32 len = F64ToStr(3.1415, 2, str, sizeof(str));
			CHECK_EQUAL(4, len);
		}
		
		UNITTEST_TEST(isEqualNoCase)
		{
			CHECK_EQUAL(true, EqualNoCase('a', 'A'));
			CHECK_EQUAL(true, EqualNoCase('a', 'a'));
			CHECK_EQUAL(false, EqualNoCase('a', 'B'));
			CHECK_EQUAL(true, EqualNoCase('z', 'Z'));
			CHECK_EQUAL(false, EqualNoCase('=', '+'));
			CHECK_EQUAL(true, EqualNoCase('?', '?'));
		}

		UNITTEST_TEST(CPrintf)
		{
			s32 testInteger = 100;
			const char* testStr = "hello";
			const char* formatStr = "%d %s";
			s32 length = CPrintf(formatStr, x_va(testInteger), x_va(testStr));
			CHECK_EQUAL(9, length);
		}

		UNITTEST_TEST(VCPrintf)
		{
			s32 testInteger = 100;
			const char* testStr = "hello";
			const char* formatStr = "%d %s";
			s32 length = VCPrintf(formatStr, x_va_list(x_va(testInteger), x_va(testStr)));
			CHECK_EQUAL(9, length);
		}

		UNITTEST_TEST(SPrintf)
		{
			char buffer[16];
			s32 testInteger = 100;
			const char* testStr = "hello";
			const char* formatStr = "%d %s";
			s32 length = SPrintf(buffer, sizeof(buffer)-1, formatStr, x_va(testInteger), x_va(testStr));
			CHECK_EQUAL(9, length);
			CHECK_TRUE(StrCmp(buffer, "100 hello")==0);

			// Check all format functionality?

			// ---------------------------------------------------------------------------
			// Boolean, True/False and Yes/No verification
			length = SPrintf(buffer, sizeof(buffer)-1, "%b", x_va(true));
			CHECK_EQUAL(4, length);
			CHECK_TRUE(StrCmp(buffer, "true")==0);

			length = SPrintf(buffer, sizeof(buffer)-1, "%B", x_va(true));
			CHECK_EQUAL(4, length);
			CHECK_TRUE(StrCmp(buffer, "TRUE")==0);

			length = SPrintf(buffer, sizeof(buffer)-1, "%b", x_va(false));
			CHECK_EQUAL(5, length);
			CHECK_TRUE(StrCmp(buffer, "false")==0);

			length = SPrintf(buffer, sizeof(buffer)-1, "%#b", x_va(false));
			CHECK_EQUAL(5, length);
			CHECK_TRUE(StrCmp(buffer, "False")==0);

			length = SPrintf(buffer, sizeof(buffer)-1, "%y", x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(StrCmp(buffer, "yes")==0);

			length = SPrintf(buffer, sizeof(buffer)-1, "%y", x_va(false));
			CHECK_EQUAL(2, length);
			CHECK_TRUE(StrCmp(buffer, "no")==0);

			length = SPrintf(buffer, sizeof(buffer)-1, "%Y", x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(StrCmp(buffer, "YES")==0);

			length = SPrintf(buffer, sizeof(buffer)-1, "%#y", x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(StrCmp(buffer, "Yes")==0);
			// ---------------------------------------------------------------------------

		}

		UNITTEST_TEST(VSPrintf)
		{
			char buffer[16];
			s32 testInteger = 100;
			const char* testStr = "hello";
			const char* formatStr = "%d %s";
			s32 length = VSPrintf(buffer, sizeof(buffer)-1, formatStr, x_va_list(x_va(testInteger), x_va(testStr)));
			CHECK_EQUAL(9, length);
			CHECK_TRUE(StrCmp(buffer, "100 hello")==0);
		}

		UNITTEST_TEST(SScanf)
		{
			// Test scanf
			const char* str = "1.0 100";
			const char* format = "%f %u";
			
			f32 myfloat;
			u32 myint;
			SScanf(str, format, x_va_r(&myfloat), x_va_r(&myint));

			CHECK_EQUAL(1.0f, myfloat);
			CHECK_EQUAL(100, myint);
		}


	}
}
UNITTEST_SUITE_END
