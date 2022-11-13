#include "cbase/c_va_list.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(test_string_ascii)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}

        UNITTEST_FIXTURE_TEARDOWN() {}


        UNITTEST_TEST(size)
        {
			crunes_t str1 = "";
			CHECK_EQUAL(0, str1.size());

            crunes_t str2 = "this is a system string";
            CHECK_EQUAL(23, str2.size());
        }

        UNITTEST_TEST(copy)
        {
            crunes_t str("this is a system string");

            runez_t<ascii::rune, 32> dst;
            copy(str, dst);

            CHECK_EQUAL(0, compare(str, dst));
            CHECK_EQUAL(23, str.size());

            runez_t<ascii::rune, 16> str2;
            copy(str, str2);

            CHECK_EQUAL(16, str2.size());
            CHECK_EQUAL(-1, compare(str2, str));

            CHECK_EQUAL(0, compare(str2, crunes_t("this is a system")));
        }

        UNITTEST_TEST(find)
        {
            crunes_t str1("this is a system string admin!");

            crunes_t f1 = find(str1, 'e');
            CHECK_EQUAL('e', first_char(f1));
            CHECK_TRUE(find(str1, 'E').is_empty());
            CHECK_FALSE(find(str1, 'E', false).is_empty());

            crunes_t tofind = "system";
            crunes_t found  = find(str1, tofind);
            CHECK_TRUE(found == crunes_t("system"));

			crunes_t str3 = "SYSTEM";
			CHECK_TRUE(find(str1, str3).is_empty());
			CHECK_FALSE(find(str1, str3, false).is_empty());
		
			crunes_t str4 = "adMin!";
			CHECK_TRUE(find(str1, str4).is_empty());
			CHECK_FALSE(find(str1, str4, false).is_empty());
		}

        UNITTEST_TEST(find_one_of)
        {
            crunes_t str1 = "this is a system string";

            crunes_t set1  = "bcde";
            crunes_t found = findOneOf(str1, set1);
            CHECK_TRUE(found == crunes_t("e"));

            crunes_t set2 = "BCDE";
            found              = findOneOf(str1, set2, false);
            CHECK_TRUE(found == crunes_t("e"));
        }

        UNITTEST_TEST(replace)
        {
            runez_t<ascii::rune, 128> dst;
            crunes_t     str1 = "this is a system string";
            copy(str1, dst);
            crunes_t str2 = "this is a copied string";

            crunes_t find_str = "system";
            crunes_t found    = find(dst, find_str);
            CHECK_TRUE(found == crunes_t("system"));

            crunes_t replace_str = "copied";
            findReplace(dst, find_str, replace_str);

            CHECK_EQUAL(0, compare(dst, str2));
        }

        UNITTEST_TEST(compare)
        {
            crunes_t str1 = "this is a system string";
            crunes_t str2 = "this is a system string";
            CHECK_EQUAL(0, compare(str1, str2));

            crunes_t str3 = "a";
            crunes_t str4 = "b";
            CHECK_EQUAL(-1, compare(str3, str4));
            CHECK_EQUAL(0, compare(str3, str3));
            CHECK_EQUAL(0, compare(str4, str4));
            CHECK_EQUAL(1, compare(str4, str3));

            crunes_t str5 = "a";
            crunes_t str6 = "A";
            crunes_t str7 = "b";
            crunes_t str8 = "B";
            CHECK_EQUAL(1, compare(str5, str6));
            CHECK_EQUAL(0, compare(str5, str6, false));
            CHECK_EQUAL(1, compare(str7, str8));
            CHECK_EQUAL(0, compare(str7, str8, false));
        }

        UNITTEST_TEST(concatenate)
        {
            runez_t<ascii::rune, 128> dst;
            crunes_t     str1 = "this is a ";
            copy(str1, dst);

            crunes_t str2 = "copied string";
            concatenate(dst, str2);

            crunes_t str3 = "this is a copied string";
            CHECK_EQUAL(0, compare(dst, str3));
        }

        UNITTEST_TEST(parse_bool)
        {
            bool          value;
            crunes_t str = "True";
            parse(str, value);
            CHECK_EQUAL(true, value);
            crunes_t str2 = "Off";
            parse(str2, value);
            CHECK_EQUAL(false, value);
            crunes_t str3 = "On";
            parse(str3, value);
            CHECK_EQUAL(true, value);
            crunes_t str4 = "false";
            parse(str4, value);
            CHECK_EQUAL(false, value);
            crunes_t str6 = "Yes";
            parse(str6, value);
            CHECK_EQUAL(true, value);
            crunes_t str5 = "No";
            parse(str5, value);
            CHECK_EQUAL(false, value);
        }

        UNITTEST_TEST(parse_s32)
        {
            s32           value;
            crunes_t str = "1";
            parse(str, value);
            CHECK_EQUAL(1, value);
            crunes_t str2 = "2";
            parse(str2, value);
            CHECK_EQUAL(2, value);
            crunes_t str3 = "256";
            parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_u32)
        {
            u32           value;
            crunes_t str = "1";
            parse(str, value);
            CHECK_EQUAL(1, value);
            crunes_t str2 = "2";
            parse(str2, value);
            CHECK_EQUAL(2, value);
            crunes_t str3 = "256";
            parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_s64)
        {
            s64           value;
            crunes_t str = "1";
            parse(str, value);
            CHECK_EQUAL(1, value);
            crunes_t str2 = "2";
            parse(str2, value);
            CHECK_EQUAL(2, value);
            crunes_t str3 = "256";
            parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_u64)
        {
            u64           value;
            crunes_t str = "1";
            parse(str, value);
            CHECK_EQUAL(1, value);
            crunes_t str2 = "2";
            parse(str2, value);
            CHECK_EQUAL(2, value);
            crunes_t str3 = "256";
            parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_f32)
        {
            f32           value;
            crunes_t str = "1.1";
            parse(str, value);
            CHECK_EQUAL(1.1f, value);
            crunes_t str2 = "2.5";
            parse(str2, value);
            CHECK_EQUAL(2.5f, value);
            crunes_t str3 = "-256.33";
            parse(str3, value);
            CHECK_EQUAL(-256.33f, value);
        }

        UNITTEST_TEST(parse_f64)
        {
            f64           value;
            crunes_t str = "1.1";
            parse(str, value);
            CHECK_EQUAL(1.1, value);
            crunes_t str2 = "2.5";
            parse(str2, value);
            CHECK_EQUAL(2.5, value);
            crunes_t str3 = "-256.33";
            parse(str3, value);
            CHECK_EQUAL(-256.33, value);
        }

        UNITTEST_TEST(is_decimal)
        {
            crunes_t decimal_str     = "2017";
            crunes_t non_decimal_str = "20a1a";
            CHECK_EQUAL(true, is_decimal(decimal_str));
            CHECK_EQUAL(false, is_decimal(non_decimal_str));
        }

        UNITTEST_TEST(is_hexadecimal)
        {
            crunes_t hexadecimal_str     = "20aabbccddeeff";
            crunes_t non_hexadecimal_str = "20aabbccddeeffw";
            CHECK_EQUAL(true, is_hexadecimal(hexadecimal_str));
            CHECK_EQUAL(false, is_hexadecimal(non_hexadecimal_str));
            crunes_t hexadecimal_with_prefix_str = "0x20aabbccddeeff";
            CHECK_EQUAL(true, is_hexadecimal(hexadecimal_with_prefix_str, true));
        }

        UNITTEST_TEST(is_float)
        {
            crunes_t float_str     = "3.1415";
            crunes_t non_float_str = "3a.14_15";
            CHECK_EQUAL(true, is_float(float_str));
            CHECK_EQUAL(false, is_float(non_float_str));
        }

        UNITTEST_TEST(is_GUID)
        {
            crunes_t guid_str     = "11335577:22446688:557799BB:88AACCEE";
            crunes_t non_guid_str = "335577:446688:7799BB:AACCEE";
            CHECK_EQUAL(true, is_GUID(guid_str));
            CHECK_EQUAL(false, is_GUID(non_guid_str));
        }

        UNITTEST_TEST(tostring_s32)
        {
            runez_t<ascii::rune, 32> str;
            s32              value = 31415;
            to_string(str, value);
            CHECK_EQUAL(5, str.size());
            CHECK_EQUAL(0, compare(str, "31415"));
        }

        UNITTEST_TEST(tostring_u32)
        {
            runez_t<ascii::rune, 32> str;
            u32              value = 31415;
            to_string(str, value);
            CHECK_EQUAL(5, str.size());
            CHECK_EQUAL(0, compare(str, "31415"));
        }

        UNITTEST_TEST(tostring_s64)
        {
            runez_t<ascii::rune, 32> str;
            s64              value = 31415;
            to_string(str, value);
            CHECK_EQUAL(5, str.size());
            CHECK_EQUAL(0, compare(str, "31415"));
        }

        UNITTEST_TEST(tostring_u64)
        {
            runez_t<ascii::rune, 32> str;
            u64              value = 31415;
            to_string(str, value);
            CHECK_EQUAL(5, str.size());
            CHECK_EQUAL(0, compare(str, "31415"));
        }

        UNITTEST_TEST(tostring_f32)
        {
            runez_t<ascii::rune, 32> str;
            f32              value = 3.1415f;
            to_string(str, value, 4);
            CHECK_EQUAL(6, str.size());
            CHECK_EQUAL(0, compare(str, "3.1415"));
        }

        UNITTEST_TEST(tostring_f64)
        {
            runez_t<ascii::rune, 32> str;
            f64              value = 3.1415;
            to_string(str, value, 4);
            CHECK_EQUAL(6, str.size());
            CHECK_EQUAL(0, compare(str, "3.1415"));
        }

        UNITTEST_TEST(is)
        {
            CHECK_EQUAL(true, is_space(' '));
            CHECK_EQUAL(false, is_space('!'));
            CHECK_EQUAL(true, is_upper('A'));
            CHECK_EQUAL(false, is_upper('a'));
            CHECK_EQUAL(false, is_lower('A'));
            CHECK_EQUAL(true, is_lower('a'));

            CHECK_EQUAL(true, is_alpha('A'));
            CHECK_EQUAL(true, is_alpha('a'));
            CHECK_EQUAL(true, is_alpha('F'));
            CHECK_EQUAL(true, is_alpha('f'));
            CHECK_EQUAL(true, is_alpha('G'));
            CHECK_EQUAL(true, is_alpha('g'));
            CHECK_EQUAL(false, is_alpha('9'));
            CHECK_EQUAL(false, is_alpha('9'));

            CHECK_EQUAL(true, is_digit('9'));
            CHECK_EQUAL(true, is_digit('9'));
            CHECK_EQUAL(false, is_digit('a'));
            CHECK_EQUAL(false, is_digit('a'));

            CHECK_EQUAL(true, is_hexa('A'));
            CHECK_EQUAL(true, is_hexa('a'));
            CHECK_EQUAL(false, is_hexa('g'));
            CHECK_EQUAL(false, is_hexa('H'));

            CHECK_EQUAL(true, is_equalfold('a', 'A'));
            CHECK_EQUAL(true, is_equalfold('a', 'a'));
            CHECK_EQUAL(false, is_equalfold('a', 'B'));
            CHECK_EQUAL(true, is_equalfold('z', 'Z'));
            CHECK_EQUAL(false, is_equalfold('=', '+'));
            CHECK_EQUAL(true, is_equalfold('?', '?'));
        }

        UNITTEST_TEST(to)
        {
            CHECK_EQUAL('B', to_upper('b'));
            CHECK_EQUAL('b', to_lower('B'));
            CHECK_EQUAL('0', to_upper('0'));
            CHECK_EQUAL('9', to_lower('9'));

            CHECK_EQUAL(0, to_digit('0'));
            CHECK_EQUAL(3, to_digit('3'));
            CHECK_EQUAL(9, to_digit('9'));

            CHECK_EQUAL(5, to_number('5'));
            CHECK_EQUAL(10, to_number('a'));
            CHECK_EQUAL(11, to_number('B'));
            CHECK_EQUAL(15, to_number('F'));
        }

        UNITTEST_TEST(is_upper)
        {
            crunes_t str = "THIS IS AN UPPERCASE STRING";
            CHECK_EQUAL(true, is_upper(str));
            crunes_t str2 = "THIS IS UPPERCASE STRING with some lowercase";
            CHECK_EQUAL(false, is_upper(str2));
        }

        UNITTEST_TEST(is_lower)
        {
            crunes_t str1 = "this is a lowercase string";
            CHECK_EQUAL(true, is_lower(str1));
            crunes_t str2 = "THIS IS UPPERCASE STRING with some lowercase";
            CHECK_EQUAL(false, is_lower(str2));
        }

        UNITTEST_TEST(is_capitalized)
        {
            crunes_t str1 = "This Is A Capitalized String";
            CHECK_EQUAL(true, is_capitalized(str1));
            crunes_t str2 = "This Is Not all Capitalized";
            CHECK_EQUAL(false, is_capitalized(str2));
        }

        UNITTEST_TEST(is_delimited)
        {
            crunes_t str1 = "<this Is A delimited String>";
            CHECK_EQUAL(true, is_delimited(str1, '<', '>'));
            crunes_t str2 = "[This Is Not all Capitalized";
            CHECK_EQUAL(false, is_delimited(str2, '[', ']'));
        }

        UNITTEST_TEST(is_quoted)
        {
            crunes_t str1 = "'this Is A quoted String'";
            CHECK_EQUAL(true, is_delimited(str1, '\'', '\''));
            crunes_t str2 = "'This Is Not correctly quoted Capitalized\"";
            CHECK_EQUAL(false, is_delimited(str2, '\'', '\''));
        }

        UNITTEST_TEST(to_upper)
        {
            runez_t<ascii::rune, 128> str;
            crunes_t     str1 = "this is a lower case string";
            crunes_t     str2 = "THIS IS A LOWER CASE STRING";
            copy(str1, str);
            to_upper(str);
            CHECK_EQUAL(0, compare(str2, str));
        }

        UNITTEST_TEST(to_lower)
        {
            runez_t<ascii::rune, 128> str;
            crunes_t     str1 = "THIS IS AN UPPER CASE STRING";
            crunes_t     str2 = "this is an upper case string";
            copy(str1, str);
            to_lower(str);
            CHECK_EQUAL(0, compare(str2, str));
        }

        UNITTEST_TEST(starts_with)
        {
            crunes_t str1   = "a simple string";
            crunes_t str2   = "need a longer string";
            crunes_t start2 = "need";

            CHECK_EQUAL(true, starts_with(str1, 'a'));
            CHECK_EQUAL(false, starts_with(str2, 'a'));

            CHECK_EQUAL(false, starts_with(str1, start2));
            CHECK_EQUAL(true, starts_with(str2, start2));
        }

        UNITTEST_TEST(first_char)
        {
            crunes_t str1 = "a simple string";
            CHECK_EQUAL('a', first_char(str1));
        }

        UNITTEST_TEST(cprintf)
        {
            s32 const     i   = 100;
            crunes_t str = "hello";

            crunes_t fmt    = "%d %s";
            s32           length = cprintf(fmt, va_t(i), va_t(str));
            CHECK_EQUAL(9, length);

            runez_t<ascii::rune, 32> buffer;
            sprintf(buffer, fmt, va_t(i), va_t(str));
            CHECK_EQUAL(0, compare(buffer, crunes_t("100 hello")));
        }

        UNITTEST_TEST(vcprintf)
        {
            s32           i      = 100;
            crunes_t str    = "hello";
            crunes_t fmt    = "%d %s";
            s32           length = vcprintf(fmt, va_list_t(va_t(i), va_t(str)));
            CHECK_EQUAL(9, length);
        }

        UNITTEST_TEST(sprintf)
        {
            runez_t<ascii::rune, 32> buffer;

            s32 i = 100;

            crunes_t str = "hello";
            crunes_t fmt = "%d %s";

            sprintf(buffer, fmt, va_t(i), va_t(str));
            CHECK_EQUAL(9, buffer.size());
            CHECK_TRUE(compare(buffer, "100 hello") == 0);

            // Check all format functionality?
        }

        UNITTEST_TEST(sprintf_bool)
        {
            runez_t<ascii::rune, 32> buffer;

            // ---------------------------------------------------------------------------
            // Boolean, True/False and Yes/No verification
            buffer.reset();
            sprintf(buffer, "%b", va_t(true));
            CHECK_EQUAL(buffer.size(), 4);
            CHECK_TRUE(compare(buffer, "true") == 0);

            buffer.reset();
            sprintf(buffer, "%B", va_t(true));
            CHECK_EQUAL(buffer.size(), 4);
            CHECK_TRUE(compare(buffer, "TRUE") == 0);

            buffer.reset();
            sprintf(buffer, "%b", va_t(false));
            CHECK_EQUAL(buffer.size(), 5);
            CHECK_TRUE(compare(buffer, "false") == 0);

            buffer.reset();
            sprintf(buffer, "%#b", va_t(false));
            CHECK_EQUAL(buffer.size(), 5);
            CHECK_TRUE(compare(buffer, "False") == 0);

            buffer.reset();
            sprintf(buffer, "%y", va_t(true));
            CHECK_EQUAL(buffer.size(), 3);
            CHECK_TRUE(compare(buffer, "yes") == 0);

            buffer.reset();
            sprintf(buffer, "%y", va_t(false));
            CHECK_EQUAL(buffer.size(), 2);
            CHECK_TRUE(compare(buffer, "no") == 0);

            buffer.reset();
            sprintf(buffer, "%Y", va_t(true));
            CHECK_EQUAL(buffer.size(), 3);
            CHECK_TRUE(compare(buffer, "YES") == 0);

            buffer.reset();
            sprintf(buffer, "%#y", va_t(true));
            CHECK_EQUAL(buffer.size(), 3);
            CHECK_TRUE(compare(buffer, "Yes") == 0);
            // ---------------------------------------------------------------------------
        }

        UNITTEST_TEST(vsprintf)
        {
            runez_t<ascii::rune, 32> buffer;

            s32           i   = 100;
            crunes_t str = "hello";
            crunes_t fmt = "%d %s";
            vsprintf(buffer, fmt, va_list_t(va_t(i), va_t(str)));
            CHECK_EQUAL(9, buffer.size());
            CHECK_TRUE(compare(buffer, "100 hello") == 0);
        }

        UNITTEST_TEST(sscanf)
        {
            // Test scanf
            crunes_t str    = "1.0 100";
            crunes_t format = "%f %u";

            f32 myfloat;
            u32 myint;
            sscanf(str, format, va_r_t(&myfloat), va_r_t(&myint));

            CHECK_EQUAL(1.0f, myfloat);
            CHECK_EQUAL(100, myint);
        }

	
        // ---------------------------------------------------------------------------
		UNITTEST_TEST(path_parser_1)
		{
			crunes_t fullpath("C:\\projects\\binary_reader\\bin\\binary_reader.cpp.old");
			crunes_t out_device;
			crunes_t out_path;
			crunes_t out_filename;
			crunes_t out_extension;
			crunes_t out_first_folder;

			crunes_t slash("\\");
			crunes_t devicesep(":\\");
			out_device = findSelectUntilIncluded(fullpath, devicesep);
			crunes_t filepath = selectAfterExclude(fullpath, out_device);
			out_path = findLastSelectUntilIncluded(filepath, slash);
			out_filename = selectAfterExclude(fullpath, out_path);
			out_filename = findLastSelectUntil(out_filename, '.');
			out_extension = selectAfterExclude(fullpath, out_filename);

			trimRight(out_device, devicesep);

			out_first_folder = findSelectUntil(out_path, slash);

			crunes_t device("C");
			CHECK_TRUE(device == out_device);
			crunes_t path("projects\\binary_reader\\bin\\");
			CHECK_TRUE(path == out_path);
			crunes_t filename("binary_reader.cpp");
			CHECK_TRUE(filename == out_filename);
			crunes_t extension(".old");
			CHECK_TRUE(extension == out_extension);
			crunes_t first_folder("projects");
			CHECK_TRUE(first_folder == out_first_folder);
		}

		UNITTEST_TEST(path_parser_2)
		{
			crunes_t fullpath("C:\\binary_reader.cpp.old");
			crunes_t out_device;
			crunes_t out_path;
			crunes_t out_filename;
			crunes_t out_extension;
			crunes_t out_first_folder;

			crunes_t slash("\\");
			crunes_t devicesep(":\\");
			out_device = findSelectUntilIncluded(fullpath, devicesep);
			crunes_t filepath = selectAfterExclude(fullpath, out_device);
			out_path = findLastSelectUntilIncluded(filepath, slash);
			out_filename = selectAfterExclude(fullpath, out_path);
			out_filename = findLastSelectUntil(out_filename, '.');
			out_extension = selectAfterExclude(fullpath, out_filename);

			trimRight(out_device, devicesep);

			out_first_folder = findSelectUntil(out_path, slash);

			crunes_t device("C");
			CHECK_TRUE(device == out_device);
			crunes_t path("");
			CHECK_TRUE(path == out_path);
			crunes_t filename("binary_reader.cpp");
			CHECK_TRUE(filename == out_filename);
			crunes_t extension(".old");
			CHECK_TRUE(extension == out_extension);
			crunes_t first_folder("");
			CHECK_TRUE(first_folder == out_first_folder);
		}
	}
}
UNITTEST_SUITE_END
