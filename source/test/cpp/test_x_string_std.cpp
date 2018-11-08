#include "xbase/x_va_list.h"
#include "xbase/x_runes.h"
#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xstring_ascii)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(len)
		{
			ascii::crunes src = "this is a system string";

			CHECK_EQUAL(23, ascii::len(src.m_str, NULL));

			ascii::pcrune src_end_1 = ascii::endof(src.m_str, NULL);
			CHECK_EQUAL(23, ascii::len(src.m_str, NULL));
			CHECK_EQUAL(src.m_str + 23, src_end_1);

			ascii::pcrune src_end_2 = ascii::endof(src.m_str, src.m_str + 20);
			CHECK_EQUAL(20, ascii::len(src.m_str, src.m_str + 20));
			CHECK_EQUAL(src.m_str + 20, src_end_2);

			ascii::pcrune src_end_3 = ascii::endof(src.m_str, src.m_str + 30);
			CHECK_EQUAL(23, ascii::len(src.m_str, src.m_str + 30));
			CHECK_EQUAL(src.m_str + 23, src_end_3);
		}

		UNITTEST_TEST(size)
		{
			CHECK_EQUAL(0, ascii::len("", NULL));

			ascii::crunes str = "this is a system string";
			CHECK_EQUAL(23, ascii::size(str));
		}

		UNITTEST_TEST(copy)
		{ 
			ascii::crunes str("this is a system string");

			ascii::runez<32> dst;
			ascii::copy(dst, str);

			CHECK_EQUAL(0, ascii::compare(str, dst));
			CHECK_EQUAL(0, ascii::compare(str, dst));
			CHECK_EQUAL(23, ascii::size(str));

			ascii::runez<16> str2;
			ascii::copy(str2, str);

			CHECK_EQUAL(16, ascii::size(str2));
			CHECK_EQUAL(-1, ascii::compare(str2, str));

			CHECK_EQUAL(0, ascii::compare(str2, ascii::crunes("this is a system")));
		}

		UNITTEST_TEST(find)
		{
			ascii::crunes str1("this is a system string");

			ascii::crunes f1 = ascii::find(str1, 'e');
			CHECK_EQUAL('e', ascii::first_char(f1));
			CHECK_TRUE(ascii::find(str1, 'E').is_empty());
			CHECK_FALSE(ascii::find(str1, 'E', ascii::CASE_IGNORE).is_empty());

			ascii::crunes tofind = "system";
			ascii::crunes found = ascii::find(str1, tofind);
			CHECK_TRUE(found == ascii::crunes("system"));

			ascii::crunes str3 = "SYSTEM";
			CHECK_TRUE(ascii::find(str1, str3).is_empty());
			CHECK_FALSE(ascii::find(str1, str3, ascii::CASE_IGNORE).is_empty());
		}

		UNITTEST_TEST(find_one_of)
		{
			ascii::crunes str1 = "this is a system string";

			ascii::crunes set1 = "bcde";
			CHECK_TRUE(ascii::find_one_of(str1, set1) == ascii::crunes("em string"));

			ascii::crunes set2 = "BCDE";
			CHECK_TRUE(ascii::find_one_of(str1, set2, ascii::CASE_IGNORE) == ascii::crunes("em string"));
		}

		UNITTEST_TEST(replace)
		{
			ascii::runez<128> dst;
			ascii::crunes str1 = "this is a system string";
			ascii::copy(dst, str1);
			ascii::crunes str2 = "this is a copied string";

			ascii::crunes find_str = "system";
			ascii::crunes found = ascii::find(dst, find_str);
			CHECK_TRUE(found == ascii::crunes("system"));

			ascii::crunes replace_str = "copied";
			ascii::replace(dst, find_str, replace_str);

			CHECK_EQUAL(0, ascii::compare(dst, str2));
		}

		UNITTEST_TEST(compare)
		{
			ascii::crunes str1 = "this is a system string";
			ascii::crunes str2 = "this is a system string";
			CHECK_EQUAL(0, ascii::compare(str1, str2));

			ascii::crunes str3 = "a";
			ascii::crunes str4 = "b";
			CHECK_EQUAL(-1, ascii::compare(str3, str4));
			CHECK_EQUAL( 0, ascii::compare(str3, str3));
			CHECK_EQUAL( 0, ascii::compare(str4, str4));
			CHECK_EQUAL( 1, ascii::compare(str4, str3));

			ascii::crunes str5 = "a";
			ascii::crunes str6 = "A";
			ascii::crunes str7 = "b";
			ascii::crunes str8 = "B";
			CHECK_EQUAL( 1, ascii::compare(str5, str6));
			CHECK_EQUAL( 0, ascii::compare(str5, str6, ascii::CASE_IGNORE));
			CHECK_EQUAL( 1, ascii::compare(str7, str8));
			CHECK_EQUAL( 0, ascii::compare(str7, str8, ascii::CASE_IGNORE));
		}

		UNITTEST_TEST(concatenate)
		{
			ascii::runez<128> dst;
			ascii::crunes str1 = "this is a ";
			ascii::copy(dst, str1);

			ascii::crunes str2 = "copied string";
			ascii::concatenate(dst, str2);

			ascii::crunes str3 = "this is a copied string";
			CHECK_EQUAL(0, ascii::compare(dst, str3));
		}

		UNITTEST_TEST(parse_bool)
		{
			bool value;
			ascii::crunes str = "True";
			ascii::parse(str, value);
			CHECK_EQUAL(true, value);
			ascii::crunes str2 = "Off";
			ascii::parse(str2, value);
			CHECK_EQUAL(false, value);
			ascii::crunes str3 = "On";
			ascii::parse(str3, value);
			CHECK_EQUAL(true, value);
			ascii::crunes str4 = "false";
			ascii::parse(str4, value);
			CHECK_EQUAL(false, value);
			ascii::crunes str6 = "Yes";
			ascii::parse(str6, value);
			CHECK_EQUAL(true, value);
			ascii::crunes str5 = "No";
			ascii::parse(str5, value);
			CHECK_EQUAL(false, value);
		}

		UNITTEST_TEST(parse_s32)
		{
			s32 value;
			ascii::crunes str = "1";
			ascii::parse(str, value);
			CHECK_EQUAL(1, value);
			ascii::crunes str2 = "2";
			ascii::parse(str2, value);
			CHECK_EQUAL(2, value);
			ascii::crunes str3 = "256";
			ascii::parse(str3, value);
			CHECK_EQUAL(256, value);
		}

		UNITTEST_TEST(parse_u32)
		{
			u32 value;
			ascii::crunes str = "1";
			ascii::parse(str, value);
			CHECK_EQUAL(1, value);
			ascii::crunes str2 = "2";
			ascii::parse(str2, value);
			CHECK_EQUAL(2, value);
			ascii::crunes str3 = "256";
			ascii::parse(str3, value);
			CHECK_EQUAL(256, value);
		}

		UNITTEST_TEST(parse_s64)
		{
			s64 value;
			ascii::crunes str = "1";
			ascii::parse(str, value);
			CHECK_EQUAL(1, value);
			ascii::crunes str2 = "2";
			ascii::parse(str2, value);
			CHECK_EQUAL(2, value);
			ascii::crunes str3 = "256";
			ascii::parse(str3, value);
			CHECK_EQUAL(256, value);
		}

		UNITTEST_TEST(parse_u64)
		{
			u64 value;
			ascii::crunes str = "1";
			ascii::parse(str, value);
			CHECK_EQUAL(1, value);
			ascii::crunes str2 = "2";
			ascii::parse(str2, value);
			CHECK_EQUAL(2, value);
			ascii::crunes str3 = "256";
			ascii::parse(str3, value);
			CHECK_EQUAL(256, value);
		}

		UNITTEST_TEST(parse_f32)
		{
			f32 value;
			ascii::crunes str = "1.1";
			ascii::parse(str, value);
			CHECK_EQUAL(1.1f, value);
			ascii::crunes str2 = "2.5";
			ascii::parse(str2, value);
			CHECK_EQUAL(2.5f, value);
			ascii::crunes str3 = "-256.33";
			ascii::parse(str3, value);
			CHECK_EQUAL(-256.33f, value);
		}

		UNITTEST_TEST(parse_f64)
		{
			f64 value;
			ascii::crunes str = "1.1";
			ascii::parse(str, value);
			CHECK_EQUAL(1.1, value);
			ascii::crunes str2 = "2.5";
			ascii::parse(str2, value);
			CHECK_EQUAL(2.5, value);
			ascii::crunes str3 = "-256.33";
			ascii::parse(str3, value);
			CHECK_EQUAL(-256.33, value);
		}

		UNITTEST_TEST(is_decimal)
		{
			ascii::crunes decimal_str = "2017";
			ascii::crunes non_decimal_str = "20a1a";
			CHECK_EQUAL(true, ascii::is_decimal(decimal_str));
			CHECK_EQUAL(false, ascii::is_decimal(non_decimal_str));
		}

		UNITTEST_TEST(is_hexadecimal)
		{
			ascii::crunes hexadecimal_str = "20aabbccddeeff";
			ascii::crunes non_hexadecimal_str = "20aabbccddeeffw";
			CHECK_EQUAL(true, ascii::is_hexadecimal(hexadecimal_str));
			CHECK_EQUAL(false, ascii::is_hexadecimal(non_hexadecimal_str));
			ascii::crunes hexadecimal_with_prefix_str = "0x20aabbccddeeff";
			CHECK_EQUAL(true, ascii::is_hexadecimal(hexadecimal_with_prefix_str, true));
		}

		UNITTEST_TEST(is_float)
		{
			ascii::crunes float_str = "3.1415";
			ascii::crunes non_float_str = "3a.14_15";
			CHECK_EQUAL(true, ascii::is_float(float_str));
			CHECK_EQUAL(false, ascii::is_float(non_float_str));
		}

		UNITTEST_TEST(is_GUID)
		{
			ascii::crunes guid_str = "11335577:22446688:557799BB:88AACCEE";
			ascii::crunes non_guid_str = "335577:446688:7799BB:AACCEE";
			CHECK_EQUAL(true, ascii::is_GUID(guid_str));
			CHECK_EQUAL(false, ascii::is_GUID(non_guid_str));
		}

		UNITTEST_TEST(tostring_s32)
		{
			ascii::runez<32> str;
			s32 value = 31415;
			ascii::to_string(str, value);
			CHECK_EQUAL(5, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "31415"));
		}

		UNITTEST_TEST(tostring_u32)
		{
			ascii::runez<32> str;
			u32 value = 31415;
			ascii::to_string(str, value);
			CHECK_EQUAL(5, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "31415"));
		}

		UNITTEST_TEST(tostring_s64)
		{
			ascii::runez<32> str;
			s64 value = 31415;
			ascii::to_string(str, value);
			CHECK_EQUAL(5, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "31415"));
		}

		UNITTEST_TEST(tostring_u64)
		{
			ascii::runez<32> str;
			u64 value = 31415;
			ascii::to_string(str, value);
			CHECK_EQUAL(5, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "31415"));
		}

		UNITTEST_TEST(tostring_f32)
		{
			ascii::runez<32> str;
			f32 value = 3.1415f;
			ascii::to_string(str, value, 4);
			CHECK_EQUAL(6, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "3.1415"));
		}

		UNITTEST_TEST(tostring_f64)
		{
			ascii::runez<32> str;
			f64 value = 3.1415;
			ascii::to_string(str, value, 4);
			CHECK_EQUAL(6, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "3.1415"));
		}
		
		UNITTEST_TEST(is)
		{
			CHECK_EQUAL(true, ascii::is_space(' '));
			CHECK_EQUAL(false, ascii::is_space('!'));
			CHECK_EQUAL(true, ascii::is_upper('A'));
			CHECK_EQUAL(false, ascii::is_upper('a'));
			CHECK_EQUAL(false, ascii::is_lower('A'));
			CHECK_EQUAL(true, ascii::is_lower('a'));

			CHECK_EQUAL(true, ascii::is_alpha('A'));
			CHECK_EQUAL(true, ascii::is_alpha('a'));
			CHECK_EQUAL(true, ascii::is_alpha('F'));
			CHECK_EQUAL(true, ascii::is_alpha('f'));
			CHECK_EQUAL(true, ascii::is_alpha('G'));
			CHECK_EQUAL(true, ascii::is_alpha('g'));
			CHECK_EQUAL(false, ascii::is_alpha('9'));
			CHECK_EQUAL(false, ascii::is_alpha('9'));

			CHECK_EQUAL(true, ascii::is_digit('9'));
			CHECK_EQUAL(true, ascii::is_digit('9'));
			CHECK_EQUAL(false, ascii::is_digit('a'));
			CHECK_EQUAL(false, ascii::is_digit('a'));

			CHECK_EQUAL(true, ascii::is_hexa('A'));
			CHECK_EQUAL(true, ascii::is_hexa('a'));
			CHECK_EQUAL(false, ascii::is_hexa('g'));
			CHECK_EQUAL(false, ascii::is_hexa('H'));

			CHECK_EQUAL(true, ascii::is_equal('a', 'A', ascii::CASE_IGNORE));
			CHECK_EQUAL(true, ascii::is_equal('a', 'a', ascii::CASE_IGNORE));
			CHECK_EQUAL(false, ascii::is_equal('a', 'B', ascii::CASE_IGNORE));
			CHECK_EQUAL(true, ascii::is_equal('z', 'Z', ascii::CASE_IGNORE));
			CHECK_EQUAL(false, ascii::is_equal('=', '+', ascii::CASE_IGNORE));
			CHECK_EQUAL(true, ascii::is_equal('?', '?', ascii::CASE_IGNORE));
		}

		UNITTEST_TEST(to)
		{
			CHECK_EQUAL('B', ascii::to_upper('b'));
			CHECK_EQUAL('b', ascii::to_lower('B'));
			CHECK_EQUAL('0', ascii::to_upper('0'));
			CHECK_EQUAL('9', ascii::to_lower('9'));

			CHECK_EQUAL(0, ascii::to_digit('0'));
			CHECK_EQUAL(3, ascii::to_digit('3'));
			CHECK_EQUAL(9, ascii::to_digit('9'));

			CHECK_EQUAL(5, ascii::to_number('5'));
			CHECK_EQUAL(10, ascii::to_number('a'));
			CHECK_EQUAL(11, ascii::to_number('B'));
			CHECK_EQUAL(15, ascii::to_number('F'));
		}

		UNITTEST_TEST(is_upper)
		{
			ascii::crunes str = "THIS IS AN UPPERCASE STRING";
			CHECK_EQUAL(true, ascii::is_upper(str));
			ascii::crunes str2 = "THIS IS UPPERCASE STRING with some lowercase";
			CHECK_EQUAL(false, ascii::is_upper(str2));
		}

		UNITTEST_TEST(is_lower)
		{
			ascii::crunes str1 = "this is a lowercase string";
			CHECK_EQUAL(true, ascii::is_lower(str1));
			ascii::crunes str2 = "THIS IS UPPERCASE STRING with some lowercase";
			CHECK_EQUAL(false, ascii::is_lower(str2));
		}

		UNITTEST_TEST(is_capitalized)
		{
			ascii::crunes str1 = "This Is A Capitalized String";
			CHECK_EQUAL(true, ascii::is_capitalized(str1));
			ascii::crunes str2 = "This Is Not all Capitalized";
			CHECK_EQUAL(false, ascii::is_capitalized(str2));
		}

		UNITTEST_TEST(is_delimited)
		{
			ascii::crunes str1 = "<this Is A delimited String>";
			CHECK_EQUAL(true, ascii::is_delimited(str1, '<', '>'));
			ascii::crunes str2 = "[This Is Not all Capitalized";
			CHECK_EQUAL(false, ascii::is_delimited(str2, '[', ']'));
		}

		UNITTEST_TEST(is_quoted)
		{
			ascii::crunes str1 = "'this Is A quoted String'";
			CHECK_EQUAL(true, ascii::is_delimited(str1, '\'', '\''));
			ascii::crunes str2 = "'This Is Not correctly quoted Capitalized\"";
			CHECK_EQUAL(false, ascii::is_delimited(str2, '\'', '\''));
		}

		UNITTEST_TEST(to_upper)
		{
			ascii::runez<128> str;
			ascii::crunes str1 = "this is a lower case string";
			ascii::crunes str2 = "THIS IS A LOWER CASE STRING";
			ascii::copy(str, str1);
			ascii::to_upper(str);
			CHECK_EQUAL(0, ascii::compare(str2, str));
		}

		UNITTEST_TEST(to_lower)
		{
			ascii::runez<128> str;
			ascii::crunes str1 = "THIS IS AN UPPER CASE STRING";
			ascii::crunes str2 = "this is an upper case string";
			ascii::copy(str, str1);
			ascii::to_lower(str);
			CHECK_EQUAL(0, ascii::compare(str2, str));
		}

		UNITTEST_TEST(starts_with)
		{
			ascii::crunes str1 = "a simple string";
			ascii::crunes str2 = "need a longer string";
			ascii::crunes start2 = "need";

			CHECK_EQUAL(true, ascii::starts_with (str1, 'a'));
			CHECK_EQUAL(false, ascii::starts_with(str2, 'a'));

			CHECK_EQUAL(false, ascii::starts_with(str1, start2));
			CHECK_EQUAL(true, ascii::starts_with (str2, start2));
		}

		UNITTEST_TEST(first_char)
		{
			ascii::crunes str1 = "a simple string";
			CHECK_EQUAL('a', ascii::first_char(str1));
		}

		UNITTEST_TEST(cprintf)
		{
			s32 const i = 100;
			ascii::crunes str = "hello";

			ascii::crunes fmt = "%d %s";
			s32 length = ascii::cprintf(fmt, x_va(i), x_va(str));
			CHECK_EQUAL(9, length);

			ascii::runez<32> buffer;
			ascii::sprintf(buffer, fmt, x_va(i), x_va(str));
			CHECK_EQUAL(0, ascii::compare(buffer, ascii::crunes("100 hello")));
		}

		UNITTEST_TEST(vcprintf)
		{
			s32 i = 100;
			ascii::crunes str = "hello";
			ascii::crunes fmt = "%d %s";
			s32 length = ascii::vcprintf(fmt, x_va_list(x_va(i), x_va(str)));
			CHECK_EQUAL(9, length);
		}

		UNITTEST_TEST(sprintf)
		{
			ascii::runez<32> buffer;

			s32 i = 100;

			ascii::crunes str = "hello";
			ascii::crunes fmt = "%d %s";

			ascii::sprintf(buffer, fmt, x_va(i), x_va(str));
			CHECK_EQUAL(9, buffer.size());
			CHECK_TRUE(ascii::compare(buffer, "100 hello") == 0);

			// Check all format functionality?
		}

		UNITTEST_TEST(sprintf_bool)
		{
			ascii::runez<32> buffer;

			// ---------------------------------------------------------------------------
			// Boolean, True/False and Yes/No verification
			buffer.reset();
			ascii::sprintf(buffer, "%b", x_va(true));
			CHECK_EQUAL(buffer.size(), 4);
			CHECK_TRUE(ascii::compare(buffer, "true")==0);

			buffer.reset();
			ascii::sprintf(buffer, "%B", x_va(true));
			CHECK_EQUAL(buffer.size(), 4);
			CHECK_TRUE(ascii::compare(buffer, "TRUE")==0);

			buffer.reset();
			ascii::sprintf(buffer, "%b", x_va(false));
			CHECK_EQUAL(buffer.size(), 5);
			CHECK_TRUE(ascii::compare(buffer, "false")==0);

			buffer.reset();
			ascii::sprintf(buffer, "%#b", x_va(false));
			CHECK_EQUAL(buffer.size(), 5);
			CHECK_TRUE(ascii::compare(buffer, "False")==0);

			buffer.reset();
			ascii::sprintf(buffer, "%y", x_va(true));
			CHECK_EQUAL(buffer.size(), 3);
			CHECK_TRUE(ascii::compare(buffer, "yes")==0);

			buffer.reset();
			ascii::sprintf(buffer, "%y", x_va(false));
			CHECK_EQUAL(buffer.size(), 2);
			CHECK_TRUE(ascii::compare(buffer, "no")==0);

			buffer.reset();
			ascii::sprintf(buffer, "%Y", x_va(true));
			CHECK_EQUAL(buffer.size(), 3);
			CHECK_TRUE(ascii::compare(buffer, "YES")==0);

			buffer.reset();
			ascii::sprintf(buffer, "%#y", x_va(true));
			CHECK_EQUAL(buffer.size(), 3);
			CHECK_TRUE(ascii::compare(buffer, "Yes")==0);
			// ---------------------------------------------------------------------------

		}

		UNITTEST_TEST(vsprintf)
		{
			ascii::runez<32> buffer;

			s32 i = 100;
			ascii::crunes str = "hello";
			ascii::crunes fmt = "%d %s";
			ascii::vsprintf(buffer, fmt, x_va_list(x_va(i), x_va(str)));
			CHECK_EQUAL(9, buffer.size());
			CHECK_TRUE(ascii::compare(buffer, "100 hello")==0);
		}

		UNITTEST_TEST(sscanf)
		{
			// Test scanf
			ascii::crunes str = "1.0 100";
			ascii::crunes format = "%f %u";
			
			f32 myfloat;
			u32 myint;
			ascii::sscanf(str, format, x_va_r(&myfloat), x_va_r(&myint));

			CHECK_EQUAL(1.0f, myfloat);
			CHECK_EQUAL(100, myint);
		}


	}
}
UNITTEST_SUITE_END
