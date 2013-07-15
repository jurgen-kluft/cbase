#include "xbase\x_target.h"
#include "xbase\x_types.h"
#include "xbase\x_string.h"

#include "xunittest\xunittest.h"

using namespace xcore;


UNITTEST_SUITE_BEGIN(xccstring)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(Constructors)
		{
			xccstring str1("a c style string");
			CHECK_TRUE(str1=="a c style string");

			xccstring str3(str1);
			CHECK_TRUE(str3=="a c style string");

			xccstring strn("xstring style string");
			xccstring str4(strn);
			CHECK_TRUE(str4=="xstring style string");

			xccstring str6(strn);
			xccstring str5(str6);
			CHECK_TRUE(str5=="xstring style string");
		}

		UNITTEST_TEST(GetLength)
		{
			xccstring str1("a c style string");
			CHECK_EQUAL(16, str1.getLength());

			xccstring str2;
			CHECK_EQUAL(0, str2.getLength());
		}

		UNITTEST_TEST(IsEmpty)
		{
			xccstring str1("a c style string");
			CHECK_FALSE(str1.isEmpty());

			xccstring str2;
			CHECK_TRUE(str2.isEmpty());
		}
		
		UNITTEST_TEST(IsUpper)
		{
			xccstring str1("a c style string");
			CHECK_FALSE(str1.isUpper());
			xccstring str2("A C STYLE STRING");
			CHECK_TRUE(str2.isUpper());
		}

		UNITTEST_TEST(IsLower)
		{
			xccstring str1("a c style string");
			CHECK_TRUE(str1.isLower());
			xccstring str2("A C STYLE STRING");
			CHECK_FALSE(str2.isLower());
		}

		UNITTEST_TEST(IsCapitalized)
		{
			xccstring str1("String");
			CHECK_TRUE(str1.isCapitalized());
			xccstring str2("STRING");
			CHECK_FALSE(str2.isCapitalized());
			xccstring str3("StrinG");
			CHECK_FALSE(str3.isCapitalized());
		}

		UNITTEST_TEST(IsQuoted1)
		{
			xccstring str1("\"string\"");
			CHECK_TRUE(str1.isQuoted());
			xccstring str2("\"string");
			CHECK_FALSE(str2.isQuoted());
			xccstring str3("string\"");
			CHECK_FALSE(str3.isQuoted());
			xccstring str4("string");
			CHECK_FALSE(str4.isQuoted());
		}

		UNITTEST_TEST(IsQuoted2)
		{
			xccstring str1("*string*");
			CHECK_TRUE(str1.isQuoted('*'));
			xccstring str2("*string");
			CHECK_FALSE(str2.isQuoted('*'));
			xccstring str3("string*");
			CHECK_FALSE(str3.isQuoted('*'));
			xccstring str4("string");
			CHECK_FALSE(str4.isQuoted('*'));
		}

		UNITTEST_TEST(IsDelimited)
		{
			xccstring str1("<string>");
			CHECK_TRUE(str1.isDelimited('<', '>'));
			xccstring str2("<string");
			CHECK_FALSE(str2.isDelimited('<', '>'));
			xccstring str3("string>");
			CHECK_FALSE(str3.isDelimited('<', '>'));
			xccstring str4("string");
			CHECK_FALSE(str4.isDelimited('<', '>'));
		}

		UNITTEST_TEST(GetAt)
		{
			xccstring str1("<string>");
			CHECK_EQUAL('<', str1.getAt(0));
			CHECK_EQUAL('s', str1.getAt(1));
			CHECK_EQUAL('g', str1.getAt(6));
		}

		UNITTEST_TEST(FirstChar)
		{
			xccstring str1("<string>");
			CHECK_EQUAL('<', str1.firstChar());
		}

		UNITTEST_TEST(LastChar)
		{
			xccstring str1("<string>");
			CHECK_EQUAL('>', str1.lastChar());
		}

	}
}
UNITTEST_SUITE_END
