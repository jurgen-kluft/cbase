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
            runes_t str = ascii::make_const_runes("this is a system string");

            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            runes_t dst    = utf32::make_runes(dst_runes, 0, 0, 256 - 1);

            nrunes::copy(str, dst);

            CHECK_EQUAL(0, nrunes::compare(str, dst));

            utf32::rune str2_runes[16 + 1];
            str2_runes[0] = 0;
            str2_runes[1] = 0;
            runes_t str2    = utf32::make_runes(str2_runes, 0, 0, 16);

            nrunes::copy(str, str2);

            CHECK_EQUAL(-1, nrunes::compare(str2, str));
            CHECK_EQUAL(0, nrunes::compare(str2, ascii::make_const_runes("this is a system")));
        }

        UNITTEST_TEST(find)
        {
            runes_t str1 = ascii::make_const_runes("this is a system string admin!");

            runes_t f1 = nrunes::find(str1, 'e');
            CHECK_EQUAL((uchar32)'e', nrunes::first_char(f1));
            CHECK_TRUE(is_empty(nrunes::find(str1, 'E')));
            CHECK_FALSE(is_empty(nrunes::find(str1, 'E', false)));

            runes_t tofind = ascii::make_const_runes("system");
            runes_t found  = nrunes::find(str1, tofind);
            CHECK_TRUE(found == ascii::make_const_runes("system"));

            runes_t str3 = ascii::make_const_runes("SYSTEM");
            CHECK_TRUE(is_empty(nrunes::find(str1, str3)));
            CHECK_FALSE(is_empty(nrunes::find(str1, str3, false)));

            runes_t str4 = ascii::make_const_runes("adMin!");
            CHECK_TRUE(is_empty(nrunes::find(str1, str4)));
            CHECK_FALSE(is_empty(nrunes::find(str1, str4, false)));
        }

        UNITTEST_TEST(find_one_of)
        {
            runes_t str1 = ascii::make_const_runes("this is a system string");

            runes_t set1  = ascii::make_const_runes("bcde");
            runes_t found = nrunes::findOneOf(str1, set1);
            CHECK_TRUE(found == ascii::make_const_runes("e"));

            runes_t set2 = ascii::make_const_runes("BCDE");
            found         = nrunes::findOneOf(str1, set2, false);
            CHECK_TRUE(found == ascii::make_const_runes("e"));
        }

        UNITTEST_TEST(replace)
        {
            ascii::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            runes_t dst  = ascii::make_runes(dst_runes, 0, 0, 256 - 1);

            runes_t str1 = ascii::make_const_runes("this is a system string");
            nrunes::copy(str1, dst);
            runes_t str2 = ascii::make_const_runes("this is a copied string");

            runes_t find_str = ascii::make_const_runes("system");
            runes_t found    = make_runes(nrunes::find(dst, find_str));
            CHECK_TRUE(found == ascii::make_const_runes("system"));

            runes_t replace_str = ascii::make_const_runes("copied");
            nrunes::findReplace(dst, find_str, replace_str);

            CHECK_EQUAL(0, nrunes::compare(dst, str2));
        }

        UNITTEST_TEST(compare)
        {
            runes_t str1 = ascii::make_const_runes("this is a system string");
            runes_t str2 = ascii::make_const_runes("this is a system string");
            CHECK_EQUAL(0, nrunes::compare(str1, str2));

            runes_t str3 = ascii::make_const_runes("a");
            runes_t str4 = ascii::make_const_runes("b");
            CHECK_EQUAL(-1, nrunes::compare(str3, str4));
            CHECK_EQUAL(0, nrunes::compare(str3, str3));
            CHECK_EQUAL(0, nrunes::compare(str4, str4));
            CHECK_EQUAL(1, nrunes::compare(str4, str3));

            runes_t str5 = ascii::make_const_runes("a");
            runes_t str6 = ascii::make_const_runes("A");
            runes_t str7 = ascii::make_const_runes("b");
            runes_t str8 = ascii::make_const_runes("B");
            CHECK_EQUAL(1, nrunes::compare(str5, str6));
            CHECK_EQUAL(0, nrunes::compare(str5, str6, false));
            CHECK_EQUAL(1, nrunes::compare(str7, str8));
            CHECK_EQUAL(0, nrunes::compare(str7, str8, false));
        }

        UNITTEST_TEST(concatenate)
        {
            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            runes_t dst    = utf32::make_runes(dst_runes, 0, 0, 256);

            runes_t str1 = ascii::make_const_runes("this is a ");
            nrunes::copy(str1, dst);

            runes_t str2 = ascii::make_const_runes("copied string");
            nrunes::concatenate(dst, str2);

            runes_t str3 = ascii::make_const_runes("this is a copied string");
            CHECK_EQUAL(0, nrunes::compare(dst, str3));
        }

        UNITTEST_TEST(parse_bool)
        {
            bool     value;
            runes_t str = ascii::make_const_runes("True");
            nrunes::parse(str, value);
            CHECK_EQUAL(true, value);
            runes_t str2 = ascii::make_const_runes("Off");
            nrunes::parse(str2, value);
            CHECK_EQUAL(false, value);
            runes_t str3 = ascii::make_const_runes("On");
            nrunes::parse(str3, value);
            CHECK_EQUAL(true, value);
            runes_t str4 = ascii::make_const_runes("false");
            nrunes::parse(str4, value);
            CHECK_EQUAL(false, value);
            runes_t str6 = ascii::make_const_runes("Yes");
            nrunes::parse(str6, value);
            CHECK_EQUAL(true, value);
            runes_t str5 = ascii::make_const_runes("No");
            nrunes::parse(str5, value);
            CHECK_EQUAL(false, value);
        }

        UNITTEST_TEST(parse_s32)
        {
            s32      value;
            runes_t str = ascii::make_const_runes("1");
            nrunes::parse(str, value);
            CHECK_EQUAL(1, value);
            runes_t str2 = ascii::make_const_runes("2");
            nrunes::parse(str2, value);
            CHECK_EQUAL(2, value);
            runes_t str3 = ascii::make_const_runes("256");
            nrunes::parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_u32)
        {
            u32      value;
            runes_t str = ascii::make_const_runes("1");
            nrunes::parse(str, value);
            CHECK_EQUAL((u32)1, value);
            runes_t str2 = ascii::make_const_runes("2");
            nrunes::parse(str2, value);
            CHECK_EQUAL((u32)2, value);
            runes_t str3 = ascii::make_const_runes("256");
            nrunes::parse(str3, value);
            CHECK_EQUAL((u32)256, value);
        }

        UNITTEST_TEST(parse_s64)
        {
            s64      value;
            runes_t str = ascii::make_const_runes("1");
            nrunes::parse(str, value);
            CHECK_EQUAL(1, value);
            runes_t str2 = ascii::make_const_runes("2");
            nrunes::parse(str2, value);
            CHECK_EQUAL(2, value);
            runes_t str3 = ascii::make_const_runes("256");
            nrunes::parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_u64)
        {
            u64      value;
            runes_t str = ascii::make_const_runes("1");
            nrunes::parse(str, value);
            CHECK_EQUAL((u64)1, value);
            runes_t str2 = ascii::make_const_runes("2");
            nrunes::parse(str2, value);
            CHECK_EQUAL((u64)2, value);
            runes_t str3 = ascii::make_const_runes("256");
            nrunes::parse(str3, value);
            CHECK_EQUAL((u64)256, value);
        }

        UNITTEST_TEST(parse_f32)
        {
            f32      value;
            runes_t str = ascii::make_const_runes("1.1");
            nrunes::parse(str, value);
            CHECK_EQUAL(1.1f, value);
            runes_t str2 = ascii::make_const_runes("2.5");
            nrunes::parse(str2, value);
            CHECK_EQUAL(2.5f, value);
            runes_t str3 = ascii::make_const_runes("-256.33");
            nrunes::parse(str3, value);
            CHECK_EQUAL(-256.33f, value);
        }

        UNITTEST_TEST(parse_f64)
        {
            f64      value;
            runes_t str = ascii::make_const_runes("1.1");
            nrunes::parse(str, value);
            CHECK_EQUAL(1.1, value);
            runes_t str2 = ascii::make_const_runes("2.5");
            nrunes::parse(str2, value);
            CHECK_EQUAL(2.5, value);
            runes_t str3 = ascii::make_const_runes("-256.33");
            nrunes::parse(str3, value);
            CHECK_EQUAL(-256.33, value);
        }

        UNITTEST_TEST(is_decimal)
        {
            runes_t decimal_str     = ascii::make_const_runes("2017");
            runes_t non_decimal_str = ascii::make_const_runes("20a1a");
            CHECK_EQUAL(true, nrunes::is_decimal(decimal_str));
            CHECK_EQUAL(false, nrunes::is_decimal(non_decimal_str));
        }

        UNITTEST_TEST(is_hexadecimal)
        {
            runes_t hexadecimal_str     = ascii::make_const_runes("20aabbccddeeff");
            runes_t non_hexadecimal_str = ascii::make_const_runes("20aabbccddeeffw");
            CHECK_EQUAL(true, nrunes::is_hexadecimal(hexadecimal_str));
            CHECK_EQUAL(false, nrunes::is_hexadecimal(non_hexadecimal_str));
            runes_t hexadecimal_with_prefix_str = ascii::make_const_runes("0x20aabbccddeeff");
            CHECK_EQUAL(true, nrunes::is_hexadecimal(hexadecimal_with_prefix_str, true));
        }

        UNITTEST_TEST(is_float)
        {
            runes_t float_str     = ascii::make_const_runes("3.1415");
            runes_t non_float_str = ascii::make_const_runes("3a.14_15");
            CHECK_EQUAL(true, nrunes::is_float(float_str));
            CHECK_EQUAL(false, nrunes::is_float(non_float_str));
        }

        UNITTEST_TEST(is_GUID)
        {
            runes_t guid_str     = ascii::make_const_runes("11335577:22446688:557799BB:88AACCEE");
            runes_t non_guid_str = ascii::make_const_runes("335577:446688:7799BB:AACCEE");
            CHECK_EQUAL(true, nrunes::is_GUID(guid_str));
            CHECK_EQUAL(false, nrunes::is_GUID(non_guid_str));
        }

        UNITTEST_TEST(tostring_s32)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            runes_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            s32 value = 31415;
            nrunes::to_string(str, value);
            CHECK_EQUAL(0, nrunes::compare(str, ascii::make_const_runes("31415")));
        }

        UNITTEST_TEST(tostring_u32)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            runes_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            u32 value = 31415;
            nrunes::to_string(str, value);
            CHECK_EQUAL(0, nrunes::compare(str, ascii::make_const_runes("31415")));
        }

        UNITTEST_TEST(tostring_s64)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            runes_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            s64 value = 31415;
            nrunes::to_string(str, value);
            CHECK_EQUAL(0, nrunes::compare(str, ascii::make_const_runes("31415")));
        }

        UNITTEST_TEST(tostring_u64)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            runes_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            u64 value = 31415;
            nrunes::to_string(str, value);
            CHECK_EQUAL(0, nrunes::compare(str, ascii::make_const_runes("31415")));
        }

        UNITTEST_TEST(tostring_f32)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            runes_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            f32 value = 3.1415f;
            nrunes::to_string(str, value, 4);
            CHECK_EQUAL(0, nrunes::compare(str, ascii::make_const_runes("3.1415")));
        }

        UNITTEST_TEST(tostring_f64)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            runes_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            f64 value = 3.1415;
            nrunes::to_string(str, value, 4);
            CHECK_EQUAL(0, nrunes::compare(str, ascii::make_const_runes("3.1415")));
        }

        UNITTEST_TEST(is)
        {
            CHECK_EQUAL(true, nrunes::is_space(' '));
            CHECK_EQUAL(false, nrunes::is_space('!'));
            CHECK_EQUAL(true, nrunes::is_upper('A'));
            CHECK_EQUAL(false, nrunes::is_upper('a'));
            CHECK_EQUAL(false, nrunes::is_lower('A'));
            CHECK_EQUAL(true, nrunes::is_lower('a'));

            CHECK_EQUAL(true, nrunes::is_alpha('A'));
            CHECK_EQUAL(true, nrunes::is_alpha('a'));
            CHECK_EQUAL(true, nrunes::is_alpha('F'));
            CHECK_EQUAL(true, nrunes::is_alpha('f'));
            CHECK_EQUAL(true, nrunes::is_alpha('G'));
            CHECK_EQUAL(true, nrunes::is_alpha('g'));
            CHECK_EQUAL(false, nrunes::is_alpha('9'));
            CHECK_EQUAL(false, nrunes::is_alpha('9'));

            CHECK_EQUAL(true, nrunes::is_digit('9'));
            CHECK_EQUAL(true, nrunes::is_digit('9'));
            CHECK_EQUAL(false, nrunes::is_digit('a'));
            CHECK_EQUAL(false, nrunes::is_digit('a'));

            CHECK_EQUAL(true, nrunes::is_hexa('A'));
            CHECK_EQUAL(true, nrunes::is_hexa('a'));
            CHECK_EQUAL(false, nrunes::is_hexa('g'));
            CHECK_EQUAL(false, nrunes::is_hexa('H'));

            CHECK_EQUAL(true, nrunes::is_equalfold('a', 'A'));
            CHECK_EQUAL(true, nrunes::is_equalfold('a', 'a'));
            CHECK_EQUAL(false, nrunes::is_equalfold('a', 'B'));
            CHECK_EQUAL(true, nrunes::is_equalfold('z', 'Z'));
            CHECK_EQUAL(false, nrunes::is_equalfold('=', '+'));
            CHECK_EQUAL(true, nrunes::is_equalfold('?', '?'));
        }

        UNITTEST_TEST(to)
        {
            CHECK_EQUAL((uchar32)'B', nrunes::to_upper('b'));
            CHECK_EQUAL((uchar32)'b', nrunes::to_lower('B'));
            CHECK_EQUAL((uchar32)'0', nrunes::to_upper('0'));
            CHECK_EQUAL((uchar32)'9', nrunes::to_lower('9'));

            CHECK_EQUAL((u32)0, nrunes::to_digit('0'));
            CHECK_EQUAL((u32)3, nrunes::to_digit('3'));
            CHECK_EQUAL((u32)9, nrunes::to_digit('9'));

            CHECK_EQUAL((u32)5, nrunes::hex_to_number('5'));
            CHECK_EQUAL((u32)10, nrunes::hex_to_number('a'));
            CHECK_EQUAL((u32)11, nrunes::hex_to_number('B'));
            CHECK_EQUAL((u32)15, nrunes::hex_to_number('F'));
        }

        UNITTEST_TEST(is_upper)
        {
            runes_t str = ascii::make_const_runes("THIS IS AN UPPERCASE STRING");
            CHECK_EQUAL(true, nrunes::is_upper(str));
            runes_t str2 = ascii::make_const_runes("THIS IS UPPERCASE STRING with some lowercase");
            CHECK_EQUAL(false, nrunes::is_upper(str2));
        }

        UNITTEST_TEST(is_lower)
        {
            runes_t str1 = ascii::make_const_runes("this is a lowercase string");
            CHECK_EQUAL(true, nrunes::is_lower(str1));
            runes_t str2 = ascii::make_const_runes("THIS IS UPPERCASE STRING with some lowercase");
            CHECK_EQUAL(false, nrunes::is_lower(str2));
        }

        UNITTEST_TEST(is_capitalized)
        {
            runes_t str1 = ascii::make_const_runes("This Is A Capitalized String");
            CHECK_EQUAL(true, nrunes::is_capitalized(str1));
            runes_t str2 = ascii::make_const_runes("This Is Not all Capitalized");
            CHECK_EQUAL(false, nrunes::is_capitalized(str2));
        }

        UNITTEST_TEST(is_delimited)
        {
            runes_t str1 = ascii::make_const_runes("<this Is A delimited String>");
            CHECK_EQUAL(true, nrunes::is_delimited(str1, '<', '>'));
            runes_t str2 = ascii::make_const_runes("[This Is Not all Capitalized");
            CHECK_EQUAL(false, nrunes::is_delimited(str2, '[', ']'));
        }

        UNITTEST_TEST(is_quoted)
        {
            runes_t str1 = ascii::make_const_runes("'this Is A quoted String'");
            CHECK_EQUAL(true, nrunes::is_delimited(str1, '\'', '\''));
            runes_t str2 = ascii::make_const_runes("'This Is Not correctly quoted Capitalized\"");
            CHECK_EQUAL(false, nrunes::is_delimited(str2, '\'', '\''));
        }

        UNITTEST_TEST(to_upper)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            runes_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            runes_t str1 = ascii::make_const_runes("this is a lower case string");
            runes_t str2 = ascii::make_const_runes("THIS IS A LOWER CASE STRING");
            nrunes::copy(str1, str);
            nrunes::to_upper(str);
            CHECK_EQUAL(0, nrunes::compare(str2, str));
        }

        UNITTEST_TEST(to_lower)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            runes_t str    = utf32::make_runes(str_runes, 0, 0, 256);

            runes_t str1 = ascii::make_const_runes("THIS IS AN UPPER CASE STRING");
            runes_t str2 = ascii::make_const_runes("this is an upper case string");
            nrunes::copy(str1, str);
            nrunes::to_lower(str);
            CHECK_EQUAL(0, nrunes::compare(str2, str));
        }

        UNITTEST_TEST(starts_with)
        {
            runes_t str1   = ascii::make_const_runes("a simple string");
            runes_t str2   = ascii::make_const_runes("need a longer string");
            runes_t start2 = ascii::make_const_runes("need");

            CHECK_EQUAL(true, nrunes::starts_with(str1, 'a'));
            CHECK_EQUAL(false, nrunes::starts_with(str2, 'a'));

            CHECK_EQUAL(false, nrunes::starts_with(str1, start2));
            CHECK_EQUAL(true, nrunes::starts_with(str2, start2));
        }

        UNITTEST_TEST(first_char)
        {
            runes_t str1 = ascii::make_const_runes("a simple string");
            CHECK_EQUAL((uchar32)'a', nrunes::first_char(str1));
        }

        UNITTEST_TEST(cprintf)
        {
            s32 const i   = 100;
            runes_t  str = ascii::make_const_runes("hello");

            runes_t fmt    = ascii::make_const_runes("%d %s");
            s32      length = cprintf(fmt, va_t(i), va_t(str));
            CHECK_EQUAL(9, length);

            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            runes_t dst    = utf32::make_runes(dst_runes, 0, 0, 256);

            sprintf(dst, fmt, va_t(i), va_t(str));
            CHECK_EQUAL(0, nrunes::compare(dst, ascii::make_const_runes("100 hello")));
        }

        UNITTEST_TEST(vcprintf)
        {
            s32      i      = 100;
            runes_t str    = ascii::make_const_runes("hello");
            runes_t fmt    = ascii::make_const_runes("%d %s");
            s32      length = cprintf(fmt, va_t(i), va_t(str));
            CHECK_EQUAL(9, length);
        }

        UNITTEST_TEST(sprintf)
        {
            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            runes_t dst    = utf32::make_runes(dst_runes, 0, 0, 256);

            s32 i = 100;

            runes_t str = ascii::make_const_runes("hello");
            runes_t fmt = ascii::make_const_runes("%d %s");

            sprintf(dst, fmt, va_t(i), va_t(str));
            CHECK_TRUE(nrunes::compare(dst, ascii::make_const_runes("100 hello")) == 0);

            // Check all format functionality?
        }

        UNITTEST_TEST(sprintf_bool)
        {
            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            runes_t dst    = utf32::make_runes(dst_runes, 0, 0, 256);

            // ---------------------------------------------------------------------------
            // Boolean, True/False and Yes/No verification
            reset(dst);
            sprintf(dst, ascii::make_const_runes("%b"), va_t(true));
            CHECK_TRUE(nrunes::compare(dst, ascii::make_const_runes("true")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%B"), va_t(true));
            CHECK_TRUE(nrunes::compare(dst, ascii::make_const_runes("TRUE")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%b"), va_t(false));
            CHECK_TRUE(nrunes::compare(dst, ascii::make_const_runes("false")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%#b"), va_t(false));
            CHECK_TRUE(nrunes::compare(dst, ascii::make_const_runes("False")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%y"), va_t(true));
            CHECK_TRUE(nrunes::compare(dst, ascii::make_const_runes("yes")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%y"), va_t(false));
            CHECK_TRUE(nrunes::compare(dst, ascii::make_const_runes("no")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%Y"), va_t(true));
            CHECK_TRUE(nrunes::compare(dst, ascii::make_const_runes("YES")) == 0);

            reset(dst);
            sprintf(dst, ascii::make_const_runes("%#y"), va_t(true));
            CHECK_TRUE(nrunes::compare(dst, ascii::make_const_runes("Yes")) == 0);
            // ---------------------------------------------------------------------------
        }

        UNITTEST_TEST(vsprintf)
        {
            utf32::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            runes_t dst    = utf32::make_runes(dst_runes, 0, 0, 256);

            s32      i   = 100;
            runes_t str = ascii::make_const_runes("hello");
            runes_t fmt = ascii::make_const_runes("%d %s");
            sprintf(dst, fmt, va_t(i), va_t(str));
            CHECK_TRUE(nrunes::compare(dst, ascii::make_const_runes("100 hello")) == 0);
        }

        UNITTEST_TEST(sscanf)
        {
            // Test scanf
            runes_t str    = ascii::make_const_runes("1.0 100");
            runes_t format = ascii::make_const_runes("%f %u");

            f32 myfloat;
            u32 myint;
            sscanf(str, format, va_r_t(&myfloat), va_r_t(&myint));

            CHECK_EQUAL(1.0f, myfloat);
            CHECK_EQUAL((u32)100, myint);
        }

        // ---------------------------------------------------------------------------
        UNITTEST_TEST(path_parser_1)
        {
            runes_t fullpath = ascii::make_const_runes("C:\\projects\\binary_reader\\bin\\binary_reader.cpp.old");
            runes_t out_device;
            runes_t out_path;
            runes_t out_filename;
            runes_t out_extension;
            runes_t out_first_folder;

            runes_t slash     = ascii::make_const_runes("\\");
            runes_t devicesep = ascii::make_const_runes(":\\");
            out_device         = nrunes::findSelectUntilIncluded(fullpath, devicesep);
            runes_t filepath  = nrunes::selectAfterExclude(fullpath, out_device);
            out_path           = nrunes::findLastSelectUntilIncluded(filepath, slash);
            out_filename       = nrunes::selectAfterExclude(fullpath, out_path);
            out_filename       = nrunes::findLastSelectUntil(out_filename, '.');
            out_extension      = nrunes::selectAfterExclude(fullpath, out_filename);

            nrunes::trimRight(out_device, devicesep);

            out_first_folder = nrunes::findSelectUntil(out_path, slash);

            runes_t device = ascii::make_const_runes("C");
            CHECK_TRUE(device == out_device);
            runes_t path = ascii::make_const_runes("projects\\binary_reader\\bin\\");
            CHECK_TRUE(path == out_path);
            runes_t filename = ascii::make_const_runes("binary_reader.cpp");
            CHECK_TRUE(filename == out_filename);
            runes_t extension = ascii::make_const_runes(".old");
            CHECK_TRUE(extension == out_extension);
            runes_t first_folder = ascii::make_const_runes("projects");
            CHECK_TRUE(first_folder == out_first_folder);
        }

        UNITTEST_TEST(path_parser_2)
        {
            runes_t fullpath = ascii::make_const_runes("C:\\binary_reader.cpp.old");
            runes_t out_device;
            runes_t out_path;
            runes_t out_filename;
            runes_t out_extension;
            runes_t out_first_folder;

            runes_t slash     = ascii::make_const_runes("\\");
            runes_t devicesep = ascii::make_const_runes(":\\");
            out_device         = nrunes::findSelectUntilIncluded(fullpath, devicesep);
            runes_t filepath  = nrunes::selectAfterExclude(fullpath, out_device);
            out_path           = nrunes::findLastSelectUntilIncluded(filepath, slash);
            out_filename       = nrunes::selectAfterExclude(fullpath, out_path);
            out_filename       = nrunes::findLastSelectUntil(out_filename, '.');
            out_extension      = nrunes::selectAfterExclude(fullpath, out_filename);

            nrunes::trimRight(out_device, devicesep);

            out_first_folder = nrunes::findSelectUntil(out_path, slash);

            runes_t device = ascii::make_const_runes("C");
            CHECK_TRUE(device == out_device);
            runes_t path = ascii::make_const_runes("");
            CHECK_TRUE(path == out_path);
            runes_t filename = ascii::make_const_runes("binary_reader.cpp");
            CHECK_TRUE(filename == out_filename);
            runes_t extension = ascii::make_const_runes(".old");
            CHECK_TRUE(extension == out_extension);
            runes_t first_folder = ascii::make_const_runes("");
            CHECK_TRUE(first_folder == out_first_folder);
        }
    }
}
UNITTEST_SUITE_END
