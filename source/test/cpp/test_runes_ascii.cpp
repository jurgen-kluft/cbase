#include "cbase/c_va_list.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"
#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(runes_ascii)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}

        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(copy)
        {
            str_t str = ascii::make_const_runes("this is a system string");

            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            str_t dst    = utf32::make_runes(dst_runes, 0, 0, 256 - 1);

            copy(str, dst);

            CHECK_EQUAL(0, compare(str, dst));

            utf32::rune str2_runes[16 + 1];
            str2_runes[0] = 0;
            str2_runes[1] = 0;
            str_t str2    = utf32::make_runes(str2_runes, 0, 0, 16);

            copy(str, str2);

            CHECK_EQUAL(-1, compare(str2, str));
            CHECK_EQUAL(0, compare(str2, ascii::make_const_runes("this is a system")));
        }

        UNITTEST_TEST(find)
        {
            str_t str1 = ascii::make_const_runes("this is a system string admin!");

            str_t f1 = find(str1, 'e');
            CHECK_EQUAL((uchar32)'e', first_char(f1));
            CHECK_TRUE(is_empty(find(str1, 'E')));
            CHECK_FALSE(is_empty(find(str1, 'E', false)));

            str_t tofind = ascii::make_const_runes("system");
            str_t found  = find(str1, tofind);
            CHECK_TRUE(found == ascii::make_const_runes("system"));

            str_t str3 = ascii::make_const_runes("SYSTEM");
            CHECK_TRUE(is_empty(find(str1, str3)));
            CHECK_FALSE(is_empty(find(str1, str3, false)));

            str_t str4 = ascii::make_const_runes("adMin!");
            CHECK_TRUE(is_empty(find(str1, str4)));
            CHECK_FALSE(is_empty(find(str1, str4, false)));
        }

        UNITTEST_TEST(find_one_of)
        {
            str_t str1 = ascii::make_const_runes("this is a system string");

            str_t set1  = ascii::make_const_runes("bcde");
            str_t found = findOneOf(str1, set1);
            CHECK_TRUE(found == ascii::make_const_runes("e"));

            str_t set2 = ascii::make_const_runes("BCDE");
            found         = findOneOf(str1, set2, false);
            CHECK_TRUE(found == ascii::make_const_runes("e"));
        }

        UNITTEST_TEST(replace)
        {
            ascii::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            str_t dst  = ascii::make_runes(dst_runes, 0, 0, 256 - 1);

            str_t str1 = ascii::make_const_runes("this is a system string");
            copy(str1, dst);
            str_t str2 = ascii::make_const_runes("this is a copied string");

            str_t find_str = ascii::make_const_runes("system");
            str_t found    = make_runes(find(dst, find_str));
            CHECK_TRUE(found == ascii::make_const_runes("system"));

            str_t replace_str = ascii::make_const_runes("copied");
            findReplace(dst, find_str, replace_str);

            CHECK_EQUAL(0, compare(dst, str2));
        }

        UNITTEST_TEST(compare)
        {
            str_t str1 = ascii::make_const_runes("this is a system string");
            str_t str2 = ascii::make_const_runes("this is a system string");
            CHECK_EQUAL(0, compare(str1, str2));

            str_t str3 = ascii::make_const_runes("a");
            str_t str4 = ascii::make_const_runes("b");
            CHECK_EQUAL(-1, compare(str3, str4));
            CHECK_EQUAL(0, compare(str3, str3));
            CHECK_EQUAL(0, compare(str4, str4));
            CHECK_EQUAL(1, compare(str4, str3));

            str_t str5 = ascii::make_const_runes("a");
            str_t str6 = ascii::make_const_runes("A");
            str_t str7 = ascii::make_const_runes("b");
            str_t str8 = ascii::make_const_runes("B");
            CHECK_EQUAL(1, compare(str5, str6));
            CHECK_EQUAL(0, compare(str5, str6, false));
            CHECK_EQUAL(1, compare(str7, str8));
            CHECK_EQUAL(0, compare(str7, str8, false));
        }

        UNITTEST_TEST(concatenate)
        {
            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            str_t dst    = utf32::make_runes(dst_runes, 0, 0, 256);

            str_t str1 = ascii::make_const_runes("this is a ");
            copy(str1, dst);

            str_t str2 = ascii::make_const_runes("copied string");
            concatenate(dst, str2);

            str_t str3 = ascii::make_const_runes("this is a copied string");
            CHECK_EQUAL(0, compare(dst, str3));
        }

        UNITTEST_TEST(parse_bool)
        {
            bool     value;
            str_t str = ascii::make_const_runes("True");
            parse(str, value);
            CHECK_EQUAL(true, value);
            str_t str2 = ascii::make_const_runes("Off");
            parse(str2, value);
            CHECK_EQUAL(false, value);
            str_t str3 = ascii::make_const_runes("On");
            parse(str3, value);
            CHECK_EQUAL(true, value);
            str_t str4 = ascii::make_const_runes("false");
            parse(str4, value);
            CHECK_EQUAL(false, value);
            str_t str6 = ascii::make_const_runes("Yes");
            parse(str6, value);
            CHECK_EQUAL(true, value);
            str_t str5 = ascii::make_const_runes("No");
            parse(str5, value);
            CHECK_EQUAL(false, value);
        }

        UNITTEST_TEST(parse_s32)
        {
            s32      value;
            str_t str = ascii::make_const_runes("1");
            parse(str, value);
            CHECK_EQUAL(1, value);
            str_t str2 = ascii::make_const_runes("2");
            parse(str2, value);
            CHECK_EQUAL(2, value);
            str_t str3 = ascii::make_const_runes("256");
            parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_u32)
        {
            u32      value;
            str_t str = ascii::make_const_runes("1");
            parse(str, value);
            CHECK_EQUAL((u32)1, value);
            str_t str2 = ascii::make_const_runes("2");
            parse(str2, value);
            CHECK_EQUAL((u32)2, value);
            str_t str3 = ascii::make_const_runes("256");
            parse(str3, value);
            CHECK_EQUAL((u32)256, value);
        }

        UNITTEST_TEST(parse_s64)
        {
            s64      value;
            str_t str = ascii::make_const_runes("1");
            parse(str, value);
            CHECK_EQUAL(1, value);
            str_t str2 = ascii::make_const_runes("2");
            parse(str2, value);
            CHECK_EQUAL(2, value);
            str_t str3 = ascii::make_const_runes("256");
            parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_u64)
        {
            u64      value;
            str_t str = ascii::make_const_runes("1");
            parse(str, value);
            CHECK_EQUAL((u64)1, value);
            str_t str2 = ascii::make_const_runes("2");
            parse(str2, value);
            CHECK_EQUAL((u64)2, value);
            str_t str3 = ascii::make_const_runes("256");
            parse(str3, value);
            CHECK_EQUAL((u64)256, value);
        }

        UNITTEST_TEST(parse_f32)
        {
            f32      value;
            str_t str = ascii::make_const_runes("1.1");
            parse(str, value);
            CHECK_EQUAL(1.1f, value);
            str_t str2 = ascii::make_const_runes("2.5");
            parse(str2, value);
            CHECK_EQUAL(2.5f, value);
            str_t str3 = ascii::make_const_runes("-256.33");
            parse(str3, value);
            CHECK_EQUAL(-256.33f, value);
        }

        UNITTEST_TEST(parse_f64)
        {
            f64      value;
            str_t str = ascii::make_const_runes("1.1");
            parse(str, value);
            CHECK_EQUAL(1.1, value);
            str_t str2 = ascii::make_const_runes("2.5");
            parse(str2, value);
            CHECK_EQUAL(2.5, value);
            str_t str3 = ascii::make_const_runes("-256.33");
            parse(str3, value);
            CHECK_EQUAL(-256.33, value);
        }

        UNITTEST_TEST(is_decimal)
        {
            str_t decimal_str     = ascii::make_const_runes("2017");
            str_t non_decimal_str = ascii::make_const_runes("20a1a");
            CHECK_EQUAL(true, is_decimal(decimal_str));
            CHECK_EQUAL(false, is_decimal(non_decimal_str));
        }

        UNITTEST_TEST(is_hexadecimal)
        {
            str_t hexadecimal_str     = ascii::make_const_runes("20aabbccddeeff");
            str_t non_hexadecimal_str = ascii::make_const_runes("20aabbccddeeffw");
            CHECK_EQUAL(true, is_hexadecimal(hexadecimal_str));
            CHECK_EQUAL(false, is_hexadecimal(non_hexadecimal_str));
            str_t hexadecimal_with_prefix_str = ascii::make_const_runes("0x20aabbccddeeff");
            CHECK_EQUAL(true, is_hexadecimal(hexadecimal_with_prefix_str, true));
        }

        UNITTEST_TEST(is_float)
        {
            str_t float_str     = ascii::make_const_runes("3.1415");
            str_t non_float_str = ascii::make_const_runes("3a.14_15");
            CHECK_EQUAL(true, is_float(float_str));
            CHECK_EQUAL(false, is_float(non_float_str));
        }

        UNITTEST_TEST(is_GUID)
        {
            str_t guid_str     = ascii::make_const_runes("11335577:22446688:557799BB:88AACCEE");
            str_t non_guid_str = ascii::make_const_runes("335577:446688:7799BB:AACCEE");
            CHECK_EQUAL(true, is_GUID(guid_str));
            CHECK_EQUAL(false, is_GUID(non_guid_str));
        }

        UNITTEST_TEST(tostring_s32)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            str_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            s32 value = 31415;
            to_string(str, value);
            CHECK_EQUAL(0, compare(str, ascii::make_const_runes("31415")));
        }

        UNITTEST_TEST(tostring_u32)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            str_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            u32 value = 31415;
            to_string(str, value);
            CHECK_EQUAL(0, compare(str, ascii::make_const_runes("31415")));
        }

        UNITTEST_TEST(tostring_s64)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            str_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            s64 value = 31415;
            to_string(str, value);
            CHECK_EQUAL(0, compare(str, ascii::make_const_runes("31415")));
        }

        UNITTEST_TEST(tostring_u64)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            str_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            u64 value = 31415;
            to_string(str, value);
            CHECK_EQUAL(0, compare(str, ascii::make_const_runes("31415")));
        }

        UNITTEST_TEST(tostring_f32)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            str_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            f32 value = 3.1415f;
            to_string(str, value, 4);
            CHECK_EQUAL(0, compare(str, ascii::make_const_runes("3.1415")));
        }

        UNITTEST_TEST(tostring_f64)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            str_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            f64 value = 3.1415;
            to_string(str, value, 4);
            CHECK_EQUAL(0, compare(str, ascii::make_const_runes("3.1415")));
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
            CHECK_EQUAL((uchar32)'B', to_upper('b'));
            CHECK_EQUAL((uchar32)'b', to_lower('B'));
            CHECK_EQUAL((uchar32)'0', to_upper('0'));
            CHECK_EQUAL((uchar32)'9', to_lower('9'));

            CHECK_EQUAL((u32)0, to_digit('0'));
            CHECK_EQUAL((u32)3, to_digit('3'));
            CHECK_EQUAL((u32)9, to_digit('9'));

            CHECK_EQUAL((u32)5, hex_to_number('5'));
            CHECK_EQUAL((u32)10, hex_to_number('a'));
            CHECK_EQUAL((u32)11, hex_to_number('B'));
            CHECK_EQUAL((u32)15, hex_to_number('F'));
        }

        UNITTEST_TEST(is_upper)
        {
            str_t str = ascii::make_const_runes("THIS IS AN UPPERCASE STRING");
            CHECK_EQUAL(true, is_upper(str));
            str_t str2 = ascii::make_const_runes("THIS IS UPPERCASE STRING with some lowercase");
            CHECK_EQUAL(false, is_upper(str2));
        }

        UNITTEST_TEST(is_lower)
        {
            str_t str1 = ascii::make_const_runes("this is a lowercase string");
            CHECK_EQUAL(true, is_lower(str1));
            str_t str2 = ascii::make_const_runes("THIS IS UPPERCASE STRING with some lowercase");
            CHECK_EQUAL(false, is_lower(str2));
        }

        UNITTEST_TEST(is_capitalized)
        {
            str_t str1 = ascii::make_const_runes("This Is A Capitalized String");
            CHECK_EQUAL(true, is_capitalized(str1));
            str_t str2 = ascii::make_const_runes("This Is Not all Capitalized");
            CHECK_EQUAL(false, is_capitalized(str2));
        }

        UNITTEST_TEST(is_delimited)
        {
            str_t str1 = ascii::make_const_runes("<this Is A delimited String>");
            CHECK_EQUAL(true, is_delimited(str1, '<', '>'));
            str_t str2 = ascii::make_const_runes("[This Is Not all Capitalized");
            CHECK_EQUAL(false, is_delimited(str2, '[', ']'));
        }

        UNITTEST_TEST(is_quoted)
        {
            str_t str1 = ascii::make_const_runes("'this Is A quoted String'");
            CHECK_EQUAL(true, is_delimited(str1, '\'', '\''));
            str_t str2 = ascii::make_const_runes("'This Is Not correctly quoted Capitalized\"");
            CHECK_EQUAL(false, is_delimited(str2, '\'', '\''));
        }

        UNITTEST_TEST(to_upper)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            str_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            str_t str1 = ascii::make_const_runes("this is a lower case string");
            str_t str2 = ascii::make_const_runes("THIS IS A LOWER CASE STRING");
            copy(str1, str);
            to_upper(str);
            CHECK_EQUAL(0, compare(str2, str));
        }

        UNITTEST_TEST(to_lower)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            str_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            str_t str1 = ascii::make_const_runes("THIS IS AN UPPER CASE STRING");
            str_t str2 = ascii::make_const_runes("this is an upper case string");
            copy(str1, str);
            to_lower(str);
            CHECK_EQUAL(0, compare(str2, str));
        }

        UNITTEST_TEST(starts_with)
        {
            str_t str1   = ascii::make_const_runes("a simple string");
            str_t str2   = ascii::make_const_runes("need a longer string");
            str_t start2 = ascii::make_const_runes("need");

            CHECK_EQUAL(true, starts_with(str1, 'a'));
            CHECK_EQUAL(false, starts_with(str2, 'a'));

            CHECK_EQUAL(false, starts_with(str1, start2));
            CHECK_EQUAL(true, starts_with(str2, start2));
        }

        UNITTEST_TEST(first_char)
        {
            str_t str1 = ascii::make_const_runes("a simple string");
            CHECK_EQUAL((uchar32)'a', first_char(str1));
        }

        UNITTEST_TEST(cprintf)
        {
            s32 const i   = 100;
            str_t  str = ascii::make_const_runes("hello");

            str_t fmt    = ascii::make_const_runes("%d %s");
            s32      length = cprintf(fmt, va_t(i), va_t(str));
            CHECK_EQUAL(9, length);

            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            str_t dst    = utf32::make_runes(dst_runes, 0, 0, 256);

            sprintf(dst, fmt, va_t(i), va_t(str));
            CHECK_EQUAL(0, compare(dst, ascii::make_const_runes("100 hello")));
        }

        UNITTEST_TEST(vcprintf)
        {
            s32      i      = 100;
            str_t str    = ascii::make_const_runes("hello");
            str_t fmt    = ascii::make_const_runes("%d %s");
            s32      length = cprintf(fmt, va_t(i), va_t(str));
            CHECK_EQUAL(9, length);
        }

        UNITTEST_TEST(sprintf)
        {
            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            str_t dst    = utf32::make_runes(dst_runes, 0, 0, 256);

            s32 i = 100;

            str_t str = ascii::make_const_runes("hello");
            str_t fmt = ascii::make_const_runes("%d %s");

            sprintf(dst, fmt, va_t(i), va_t(str));
            CHECK_TRUE(compare(dst, ascii::make_const_runes("100 hello")) == 0);

            // Check all format functionality?
        }

        UNITTEST_TEST(sprintf_bool)
        {
            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            str_t dst    = utf32::make_runes(dst_runes, 0, 0, 256);

            // ---------------------------------------------------------------------------
            // Boolean, True/False and Yes/No verification
            reset(dst);
            sprintf(dst, ascii::make_const_runes("%b"), va_t(true));
            CHECK_TRUE(compare(dst, ascii::make_const_runes("true")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%B"), va_t(true));
            CHECK_TRUE(compare(dst, ascii::make_const_runes("TRUE")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%b"), va_t(false));
            CHECK_TRUE(compare(dst, ascii::make_const_runes("false")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%#b"), va_t(false));
            CHECK_TRUE(compare(dst, ascii::make_const_runes("False")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%y"), va_t(true));
            CHECK_TRUE(compare(dst, ascii::make_const_runes("yes")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%y"), va_t(false));
            CHECK_TRUE(compare(dst, ascii::make_const_runes("no")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%Y"), va_t(true));
            CHECK_TRUE(compare(dst, ascii::make_const_runes("YES")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%#y"), va_t(true));
            CHECK_TRUE(compare(dst, ascii::make_const_runes("Yes")) == 0);
            // ---------------------------------------------------------------------------
        }

        UNITTEST_TEST(vsprintf)
        {
            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            str_t dst    = utf32::make_runes(dst_runes, 0, 0, 256);

            s32      i   = 100;
            str_t str = ascii::make_const_runes("hello");
            str_t fmt = ascii::make_const_runes("%d %s");
            sprintf(dst, fmt, va_t(i), va_t(str));
            CHECK_TRUE(compare(dst, ascii::make_const_runes("100 hello")) == 0);
        }

        UNITTEST_TEST(sscanf)
        {
            // Test scanf
            str_t str    = ascii::make_const_runes("1.0 100");
            str_t format = ascii::make_const_runes("%f %u");

            f32 myfloat;
            u32 myint;
            sscanf(str, format, va_r_t(&myfloat), va_r_t(&myint));

            CHECK_EQUAL(1.0f, myfloat);
            CHECK_EQUAL((u32)100, myint);
        }

        // ---------------------------------------------------------------------------
        UNITTEST_TEST(path_parser_1)
        {
            str_t fullpath = ascii::make_const_runes("C:\\projects\\binary_reader\\bin\\binary_reader.cpp.old");
            str_t out_device;
            str_t out_path;
            str_t out_filename;
            str_t out_extension;
            str_t out_first_folder;

            str_t slash     = ascii::make_const_runes("\\");
            str_t devicesep = ascii::make_const_runes(":\\");
            out_device         = findSelectUntilIncluded(fullpath, devicesep);
            str_t filepath  = selectAfterExclude(fullpath, out_device);
            out_path           = findLastSelectUntilIncluded(filepath, slash);
            out_filename       = selectAfterExclude(fullpath, out_path);
            out_filename       = findLastSelectUntil(out_filename, '.');
            out_extension      = selectAfterExclude(fullpath, out_filename);

            trimRight(out_device, devicesep);

            out_first_folder = findSelectUntil(out_path, slash);

            str_t device = ascii::make_const_runes("C");
            CHECK_TRUE(device == out_device);
            str_t path = ascii::make_const_runes("projects\\binary_reader\\bin\\");
            CHECK_TRUE(path == out_path);
            str_t filename = ascii::make_const_runes("binary_reader.cpp");
            CHECK_TRUE(filename == out_filename);
            str_t extension = ascii::make_const_runes(".old");
            CHECK_TRUE(extension == out_extension);
            str_t first_folder = ascii::make_const_runes("projects");
            CHECK_TRUE(first_folder == out_first_folder);
        }

        UNITTEST_TEST(path_parser_2)
        {
            str_t fullpath = ascii::make_const_runes("C:\\binary_reader.cpp.old");
            str_t out_device;
            str_t out_path;
            str_t out_filename;
            str_t out_extension;
            str_t out_first_folder;

            str_t slash     = ascii::make_const_runes("\\");
            str_t devicesep = ascii::make_const_runes(":\\");
            out_device         = findSelectUntilIncluded(fullpath, devicesep);
            str_t filepath  = selectAfterExclude(fullpath, out_device);
            out_path           = findLastSelectUntilIncluded(filepath, slash);
            out_filename       = selectAfterExclude(fullpath, out_path);
            out_filename       = findLastSelectUntil(out_filename, '.');
            out_extension      = selectAfterExclude(fullpath, out_filename);

            trimRight(out_device, devicesep);

            out_first_folder = findSelectUntil(out_path, slash);

            str_t device = ascii::make_const_runes("C");
            CHECK_TRUE(device == out_device);
            str_t path = ascii::make_const_runes("");
            CHECK_TRUE(path == out_path);
            str_t filename = ascii::make_const_runes("binary_reader.cpp");
            CHECK_TRUE(filename == out_filename);
            str_t extension = ascii::make_const_runes(".old");
            CHECK_TRUE(extension == out_extension);
            str_t first_folder = ascii::make_const_runes("");
            CHECK_TRUE(first_folder == out_first_folder);
        }
    }
}
UNITTEST_SUITE_END
