#include "xbase/x_va_list.h"
#include "xbase/x_string_ascii.h"
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
			const char* src = "this is a system string";

			CHECK_EQUAL(src + 23, ascii::len(src));

			s32 len;
			ascii::pcrune src_end_1 = ascii::len(src, &len);
			CHECK_EQUAL(23, len);
			CHECK_EQUAL(src + len, src_end_1);

			ascii::pcrune src_end_2 = ascii::len(src, src + 20, &len);
			CHECK_EQUAL(20, len);
			CHECK_EQUAL(src + len, src_end_2);

			ascii::pcrune src_end_3 = ascii::len(src, src + 30, &len);
			CHECK_EQUAL(23, len);
			CHECK_EQUAL(src + len, src_end_3);
		}

		UNITTEST_TEST(size)
		{
			CHECK_EQUAL(0, ascii::size(""));

			const char* str = "this is a system string";
			CHECK_EQUAL(23, ascii::size(str));
		}

		UNITTEST_TEST(copy)
		{
			const char* src = "this is a system string";

			char dst[24 + 1];
			dst[24] = '\0';
			ascii::copy(dst, dst + 24, src, ascii::len(src));

			CHECK_EQUAL(23, ascii::size(src));
			CHECK_EQUAL(0, ascii::compare(dst, ascii::len(dst), src, ascii::len(src)));

			char str2[20];
			str2[20 - 1] = '\0';
			ascii::copy(str2, str2 + sizeof(str2) - 1, src, ascii::len(src));

			CHECK_EQUAL(19, ascii::size(str2));
			CHECK_EQUAL(-1, ascii::compare(str2, src));

			CHECK_EQUAL(0, ascii::compare(str2, "this is a system st"));
		}

		UNITTEST_TEST(find)
		{
			const char* str1 = "this is a system string";

			CHECK_EQUAL((str1 + 14), ascii::find(str1, ascii::len(str1), 'e'));
			CHECK_NULL(ascii::find(str1, ascii::len(str1), 'E'));
			CHECK_EQUAL((str1 + 14), ascii::find(str1, ascii::len(str1), 'E', ascii::CASE_IGNORE));

			const char* str2 = "system";
			const char* found_pos = ascii::find(str1, ascii::len(str1), str2, ascii::len(str2));
			CHECK_EQUAL((str1 + 10), found_pos);

			const char* str3 = "SYSTEM";
			CHECK_NULL(ascii::find(str1, ascii::len(str1), str3, ascii::len(str3)));
			CHECK_EQUAL(str1 + 10, ascii::find(str1, NULL, str3, NULL, ascii::CASE_IGNORE));
		}

		UNITTEST_TEST(find_one_of)
		{
			const char* str1 = "this is a system string";

			const char* set1 = "bcde";
			CHECK_EQUAL((str1 + 14), ascii::find_one_of(str1, NULL, set1, NULL));

			const char* set2 = "BCDE";
			CHECK_EQUAL((str1 + 14), ascii::find_one_of(str1, NULL, set2, NULL, ascii::CASE_IGNORE));
		}

		UNITTEST_TEST(replace)
		{
			char dst[101];
			const char* str1 = "this is a system string";
			char* dst_end = ascii::copy(dst, dst + 100, str1, ascii::len(str1));
			const char* str2 = "this is a copied string";

			const char* find_str = "system";
			const char* pos = ascii::find(dst, dst_end, find_str, ascii::len(find_str));

			const char* replace_str = "copied";
			ascii::replace(ascii::pos(dst, pos), pos + 6, dst + 100, replace_str, ascii::len(replace_str));

			CHECK_EQUAL(0, ascii::compare(dst, str2));
		}

		UNITTEST_TEST(compare)
		{
			const char* str1 = "this is a system string";
			const char* str2 = "this is a system string";
			CHECK_EQUAL(0, ascii::compare(str1, str2));

			const char* str3 = "a";
			const char* str4 = "b";
			CHECK_EQUAL(-1, ascii::compare(str3, ascii::len(str3), str4, ascii::len(str4)));
			CHECK_EQUAL( 0, ascii::compare(str3, ascii::len(str3), str3, ascii::len(str3)));
			CHECK_EQUAL( 0, ascii::compare(str4, ascii::len(str4), str4, ascii::len(str4)));
			CHECK_EQUAL( 1, ascii::compare(str4, ascii::len(str4), str3, ascii::len(str3)));

			const char* str5 = "a";
			const char* str6 = "A";
			const char* str7 = "b";
			const char* str8 = "B";
			CHECK_EQUAL( 1, ascii::compare(str5, str6));
			CHECK_EQUAL( 0, ascii::compare(str5, str6, ascii::CASE_IGNORE));
			CHECK_EQUAL( 1, ascii::compare(str7, str8));
			CHECK_EQUAL( 0, ascii::compare(str7, str8, ascii::CASE_IGNORE));
		}

		UNITTEST_TEST(concatenate)
		{
			char dst[101];
			dst[100] = '\0';
			const char* str1 = "this is a ";
			ascii::copy(dst, dst + 100, str1, ascii::len(str1));

			const char* str2 = "copied string";
			ascii::concatenate(dst, ascii::len(dst), dst + 100, str2, NULL);

			const char* str3 = "this is a copied string";
			CHECK_EQUAL(0, ascii::compare(dst, ascii::len(dst), str3, ascii::len(str3)));
		}

		UNITTEST_TEST(parse_bool)
		{
			bool value;
			const char* str = "True";
			ascii::parse(str, NULL, value);
			CHECK_EQUAL(true, value);
			const char* str2 = "Off";
			ascii::parse(str2, ascii::len(str2), value);
			CHECK_EQUAL(false, value);
			const char* str3 = "On";
			ascii::parse(str3, ascii::len(str3), value);
			CHECK_EQUAL(true, value);
			const char* str4 = "false";
			ascii::parse(str4, ascii::len(str4), value);
			CHECK_EQUAL(false, value);
			const char* str6 = "Yes";
			ascii::parse(str6, ascii::len(str6), value);
			CHECK_EQUAL(true, value);
			const char* str5 = "No";
			ascii::parse(str5, ascii::len(str5), value);
			CHECK_EQUAL(false, value);
		}

		UNITTEST_TEST(parse_s32)
		{
			s32 value;
			const char* str = "1";
			ascii::parse(str, ascii::len(str), value);
			CHECK_EQUAL(1, value);
			const char* str2 = "2";
			ascii::parse(str2, ascii::len(str2), value);
			CHECK_EQUAL(2, value);
			const char* str3 = "256";
			ascii::parse(str3, ascii::len(str3), value);
			CHECK_EQUAL(256, value);
		}

		UNITTEST_TEST(parse_u32)
		{
			u32 value;
			const char* str = "1";
			ascii::parse(str, ascii::len(str), value);
			CHECK_EQUAL(1, value);
			const char* str2 = "2";
			ascii::parse(str2, ascii::len(str2), value);
			CHECK_EQUAL(2, value);
			const char* str3 = "256";
			ascii::parse(str3, ascii::len(str3), value);
			CHECK_EQUAL(256, value);
		}

		UNITTEST_TEST(parse_s64)
		{
			s64 value;
			const char* str = "1";
			ascii::parse(str, ascii::len(str), value);
			CHECK_EQUAL(1, value);
			const char* str2 = "2";
			ascii::parse(str2, ascii::len(str2), value);
			CHECK_EQUAL(2, value);
			const char* str3 = "256";
			ascii::parse(str3, ascii::len(str3), value);
			CHECK_EQUAL(256, value);
		}

		UNITTEST_TEST(parse_u64)
		{
			u64 value;
			const char* str = "1";
			ascii::parse(str, ascii::len(str), value);
			CHECK_EQUAL(1, value);
			const char* str2 = "2";
			ascii::parse(str2, ascii::len(str2), value);
			CHECK_EQUAL(2, value);
			const char* str3 = "256";
			ascii::parse(str3, ascii::len(str3), value);
			CHECK_EQUAL(256, value);
		}

		UNITTEST_TEST(parse_f32)
		{
			f32 value;
			const char* str = "1.1";
			ascii::parse(str, ascii::len(str), value);
			CHECK_EQUAL(1.1f, value);
			const char* str2 = "2.5";
			ascii::parse(str2, ascii::len(str2), value);
			CHECK_EQUAL(2.5f, value);
			const char* str3 = "-256.33";
			ascii::parse(str3, ascii::len(str3), value);
			CHECK_EQUAL(-256.33f, value);
		}

		UNITTEST_TEST(parse_f64)
		{
			f64 value;
			const char* str = "1.1";
			ascii::parse(str, ascii::len(str), value);
			CHECK_EQUAL(1.1, value);
			const char* str2 = "2.5";
			ascii::parse(str2, ascii::len(str2), value);
			CHECK_EQUAL(2.5, value);
			const char* str3 = "-256.33";
			ascii::parse(str3, ascii::len(str3), value);
			CHECK_EQUAL(-256.33, value);
		}

		UNITTEST_TEST(is_decimal)
		{
			const char* decimal_str = "2017";
			const char* non_decimal_str = "20a1a";
			CHECK_EQUAL(true, ascii::is_decimal(decimal_str, ascii::len(decimal_str)));
			CHECK_EQUAL(false, ascii::is_decimal(non_decimal_str, ascii::len(non_decimal_str)));
		}

		UNITTEST_TEST(is_hexadecimal)
		{
			const char* hexadecimal_str = "20aabbccddeeff";
			const char* non_hexadecimal_str = "20aabbccddeeffw";
			CHECK_EQUAL(true, ascii::is_hexadecimal(hexadecimal_str, ascii::len(hexadecimal_str)));
			CHECK_EQUAL(false, ascii::is_hexadecimal(non_hexadecimal_str, ascii::len(non_hexadecimal_str)));
			const char* hexadecimal_with_prefix_str = "0x20aabbccddeeff";
			CHECK_EQUAL(true, ascii::is_hexadecimal(hexadecimal_with_prefix_str, ascii::len(hexadecimal_with_prefix_str), true));
		}

		UNITTEST_TEST(is_float)
		{
			const char* float_str = "3.1415";
			const char* non_float_str = "3a.14_15";
			CHECK_EQUAL(true, ascii::is_float(float_str, ascii::len(float_str)));
			CHECK_EQUAL(false, ascii::is_float(non_float_str, ascii::len(non_float_str)));
		}

		UNITTEST_TEST(is_GUID)
		{
			const char* guid_str = "11335577:22446688:557799BB:88AACCEE";
			const char* non_guid_str = "335577:446688:7799BB:AACCEE";
			CHECK_EQUAL(true, ascii::is_GUID(guid_str, ascii::len(guid_str)));
			CHECK_EQUAL(false, ascii::is_GUID(non_guid_str, ascii::len(non_guid_str)));
		}

		UNITTEST_TEST(tostring_s32)
		{
			char str_buffer[16 + 1];
			char * str = &str_buffer[0];
			char * eos = &str_buffer[16];
			eos[0] = '\0';

			s32 value = 31415;
			char * end = ascii::to_string(str, eos, eos, value);
			CHECK_EQUAL(5, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "31415"));
		}

		UNITTEST_TEST(tostring_u32)
		{
			char str_buffer[16 + 1];
			char * str = &str_buffer[0];
			char * eos = &str_buffer[16];
			eos[0] = '\0';

			u32 value = 31415;
			char * end = ascii::to_string(str, eos, eos, value);
			CHECK_EQUAL(5, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "31415"));
		}

		UNITTEST_TEST(tostring_s64)
		{
			char str_buffer[16 + 1];
			char * str = &str_buffer[0];
			char * eos = &str_buffer[16];
			eos[0] = '\0';

			s64 value = 31415;
			char * end = ascii::to_string(str, eos, eos, value);
			CHECK_EQUAL(5, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "31415"));
		}

		UNITTEST_TEST(tostring_u64)
		{
			char str_buffer[16 + 1];
			char * str = &str_buffer[0];
			char * eos = &str_buffer[16];
			eos[0] = '\0';

			u64 value = 31415;
			char * end = ascii::to_string(str, eos, eos, value);
			CHECK_EQUAL(5, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "31415"));
		}

		UNITTEST_TEST(tostring_f32)
		{
			char str_buffer[16 + 1];
			char * str = &str_buffer[0];
			char * eos = &str_buffer[16];
			eos[0] = '\0';

			f32 value = 3.1415f;
			char * end = ascii::to_string(str, eos, eos, value, 4);
			CHECK_EQUAL(6, ascii::size(str));
			CHECK_EQUAL(0, ascii::compare(str, "3.1415"));
		}

		UNITTEST_TEST(tostring_f64)
		{
			char str_buffer[16 + 1];
			char * str = &str_buffer[0];
			char * eos = &str_buffer[16];
			eos[0] = '\0';

			f64 value = 3.1415;
			char * end = ascii::to_string(str, eos, eos, value, 4);
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
			CHECK_EQUAL(false, ascii::is_alpha('G'));
			CHECK_EQUAL(false, ascii::is_alpha('g'));
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
			const char* str = "THIS IS AN UPPERCASE STRING";
			CHECK_EQUAL(true, ascii::is_upper(str, ascii::len(str)));
			const char* str2 = "THIS IS UPPERCASE STRING with some lowercase";
			CHECK_EQUAL(false, ascii::is_upper(str2, ascii::len(str2)));
		}

		UNITTEST_TEST(is_lower)
		{
			const char* str1 = "this is a lowercase string";
			CHECK_EQUAL(true, ascii::is_lower(str1, ascii::len(str1)));
			const char* str2 = "THIS IS UPPERCASE STRING with some lowercase";
			CHECK_EQUAL(false, ascii::is_lower(str2, ascii::len(str2)));
		}

		UNITTEST_TEST(is_capitalized)
		{
			const char* str1 = "This Is A Capitalized String";
			CHECK_EQUAL(true, ascii::is_capitalized(str1, ascii::len(str1)));
			const char* str2 = "This Is Not all Capitalized";
			CHECK_EQUAL(false, ascii::is_capitalized(str2, ascii::len(str2)));
		}

		UNITTEST_TEST(is_delimited)
		{
			const char* str1 = "<this Is A delimited String>";
			CHECK_EQUAL(true, ascii::is_delimited(str1, ascii::len(str1), '<', '>'));
			const char* str2 = "[This Is Not all Capitalized";
			CHECK_EQUAL(false, ascii::is_delimited(str2, ascii::len(str2), '[', ']'));
		}

		UNITTEST_TEST(is_quoted)
		{
			const char* str1 = "'this Is A quoted String'";
			CHECK_EQUAL(true, ascii::is_delimited(str1, ascii::len(str1), '\''));
			const char* str2 = "'This Is Not correctly quoted Capitalized\"";
			CHECK_EQUAL(false, ascii::is_delimited(str2, ascii::len(str2), '\''));
		}

		UNITTEST_TEST(to_upper)
		{
			char str[101];
			const char* str1 = "this is a lower case string";
			const char* str2 = "THIS IS A LOWER CASE STRING";
			char* str_end = ascii::copy(str, str + 100, str1, ascii::len(str1));
			ascii::to_upper(str, str_end);
			CHECK_EQUAL(0, ascii::compare(str2, str));
		}

		UNITTEST_TEST(to_lower)
		{
			char str[101];
			const char* str1 = "THIS IS AN UPPER CASE STRING";
			const char* str2 = "this is an upper case string";
			char* str_end = ascii::copy(str, str + 100, str1, ascii::len(str1));
			ascii::to_lower(str, str_end);
			CHECK_EQUAL(0, ascii::compare(str2, str));
		}

		UNITTEST_TEST(starts_with)
		{
			const char* str1 = "a simple string";
			const char* str2 = "need a longer string";
			const char* start2 = "need";

			CHECK_EQUAL(true, ascii::starts_with(str1, ascii::len(str1), 'a'));
			CHECK_EQUAL(false, ascii::starts_with(str2, ascii::len(str2), 'a'));

			CHECK_EQUAL(false, ascii::starts_with(str1, ascii::len(str1), start2, ascii::len(start2)));
			CHECK_EQUAL(true, ascii::starts_with(str2, ascii::len(str2), start2, ascii::len(start2)));
		}

		UNITTEST_TEST(ends_with)
		{
			const char* str1 = "a simple string";
			const char* str2 = "need a longer string!";
			const char* end2 = "string";

			CHECK_EQUAL(true, ascii::ends_with(str1, ascii::len(str1), 'g'));
			CHECK_EQUAL(false, ascii::ends_with(str2, ascii::len(str2), 'g'));

			CHECK_EQUAL(true, ascii::ends_with(str2, ascii::len(str2), end2, ascii::len(end2)));
			CHECK_EQUAL(false, ascii::ends_with(str1, ascii::len(str1), end2, ascii::len(end2)));
		}

		UNITTEST_TEST(first_char)
		{
			const char* str1 = "a simple string";
			CHECK_EQUAL('a', ascii::first_char(str1, ascii::len(str1)));
		}

		UNITTEST_TEST(last_char)
		{
			const char* str1 = "a simple string";
			CHECK_EQUAL('g', ascii::last_char(str1, ascii::len(str1)));
		}

		UNITTEST_TEST(cprintf)
		{
			s32 const i = 100;
			const char* str = "hello";

			const char* fmt = "%d %s";
			const char* fmt_end = fmt + 5;
			s32 length = ascii::cprintf(fmt, fmt_end, x_va(i), x_va(str));

			CHECK_EQUAL(9, length);
		}

		UNITTEST_TEST(vcprintf)
		{
			s32 i = 100;
			const char* str = "hello";
			const char* fmt = "%d %s";
			const char* fmt_end = fmt + 5;
			s32 length = ascii::vcprintf(fmt, fmt_end, x_va_list(x_va(i), x_va(str)));
			CHECK_EQUAL(9, length);
		}

		UNITTEST_TEST(sprintf)
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
			CHECK_TRUE(ascii::compare(buffer, "100 hello") == 0);

			// Check all format functionality?
		}

		UNITTEST_TEST(sprintf_bool)
		{
			char buffer_buffer[16 + 1];
			char * buffer = &buffer_buffer[0];
			char * buffer_end = &buffer_buffer[16];

			s32 length;

			// ---------------------------------------------------------------------------
			// Boolean, True/False and Yes/No verification
			length = ascii::sprintf(buffer, buffer_end, "%b", NULL, x_va(true));
			CHECK_EQUAL(4, length);
			CHECK_TRUE(ascii::compare(buffer, "true")==0);

			length = ascii::sprintf(buffer, buffer_end, "%B", NULL, x_va(true));
			CHECK_EQUAL(4, length);
			CHECK_TRUE(ascii::compare(buffer, "TRUE")==0);

			length = ascii::sprintf(buffer, buffer_end, "%b", NULL, x_va(false));
			CHECK_EQUAL(5, length);
			CHECK_TRUE(ascii::compare(buffer, "false")==0);

			length = ascii::sprintf(buffer, buffer_end, "%#b", NULL, x_va(false));
			CHECK_EQUAL(5, length);
			CHECK_TRUE(ascii::compare(buffer, "False")==0);

			length = ascii::sprintf(buffer, buffer_end, "%y", NULL, x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(ascii::compare(buffer, "yes")==0);

			length = ascii::sprintf(buffer, buffer_end, "%y", NULL, x_va(false));
			CHECK_EQUAL(2, length);
			CHECK_TRUE(ascii::compare(buffer, "no")==0);

			length = ascii::sprintf(buffer, buffer_end, "%Y", NULL, x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(ascii::compare(buffer, "YES")==0);

			length = ascii::sprintf(buffer, buffer_end, "%#y", NULL, x_va(true));
			CHECK_EQUAL(3, length);
			CHECK_TRUE(ascii::compare(buffer, "Yes")==0);
			// ---------------------------------------------------------------------------

		}

		UNITTEST_TEST(vsprintf)
		{
			char buffer_buffer[16 + 1];
			char * buffer = &buffer_buffer[0];
			char * buffer_end = &buffer_buffer[16];

			s32 i = 100;
			const char* str = "hello";
			const char* fmt = "%d %s";
			s32 length = ascii::vsprintf(buffer, buffer_end, fmt, NULL, x_va_list(x_va(i), x_va(str)));
			CHECK_EQUAL(9, length);
			CHECK_TRUE(ascii::compare(buffer, "100 hello")==0);
		}

		UNITTEST_TEST(sscanf)
		{
			// Test scanf
			const char* str = "1.0 100";
			const char* format = "%f %u";
			
			f32 myfloat;
			u32 myint;
			ascii::sscanf(str, NULL, format, NULL, x_va_r(&myfloat), x_va_r(&myint));

			CHECK_EQUAL(1.0f, myfloat);
			CHECK_EQUAL(100, myint);
		}


	}
}
UNITTEST_SUITE_END
