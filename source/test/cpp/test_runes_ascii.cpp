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
            crunes_t str = make_crunes("this is a system string");

            utf32::rune dst_runes[256];
            dst_runes[0].r = 0;
            dst_runes[1].r = 0;
            runes_t dst    = make_runes(dst_runes, 0, 0, 256 - 1);

            nrunes::copy(str, dst);

            CHECK_EQUAL(0, nrunes::compare(str, dst));

            utf32::rune str2_runes[16 + 1];
            str2_runes[0].r = 0;
            str2_runes[1].r = 0;
            runes_t str2    = make_runes(str2_runes, 0, 0, 16);

            nrunes::copy(str, str2);

            CHECK_EQUAL(-1, nrunes::compare(str2, str));
            CHECK_EQUAL(0, nrunes::compare(str2, make_crunes("this is a system")));
        }

        UNITTEST_TEST(find)
        {
            crunes_t str1 = make_crunes("this is a system string admin!");

            crunes_t f1 = nrunes::find(str1, 'e');
            CHECK_EQUAL('e', nrunes::first_char(f1));
            CHECK_TRUE(is_empty(nrunes::find(str1, 'E')));
            CHECK_FALSE(is_empty(nrunes::find(str1, 'E', false)));

            crunes_t tofind = make_crunes("system");
            crunes_t found  = nrunes::find(str1, tofind);
            CHECK_TRUE(found == make_crunes("system"));

            crunes_t str3 = make_crunes("SYSTEM");
            CHECK_TRUE(is_empty(nrunes::find(str1, str3)));
            CHECK_FALSE(is_empty(nrunes::find(str1, str3, false)));

            crunes_t str4 = make_crunes("adMin!");
            CHECK_TRUE(is_empty(nrunes::find(str1, str4)));
            CHECK_FALSE(is_empty(nrunes::find(str1, str4, false)));
        }

        UNITTEST_TEST(find_one_of)
        {
            crunes_t str1 = make_crunes("this is a system string");

            crunes_t set1  = make_crunes("bcde");
            crunes_t found = nrunes::findOneOf(str1, set1);
            CHECK_TRUE(found == make_crunes("e"));

            crunes_t set2 = make_crunes("BCDE");
            found         = nrunes::findOneOf(str1, set2, false);
            CHECK_TRUE(found == make_crunes("e"));
        }

        UNITTEST_TEST(replace)
        {
            ascii::rune dst_runes[256];
            dst_runes[0] = 0;
            dst_runes[1] = 0;
            runes_t dst  = make_runes(dst_runes, 0, 0, 256 - 1);

            crunes_t str1 = make_crunes("this is a system string");
            nrunes::copy(str1, dst);
            crunes_t str2 = make_crunes("this is a copied string");

            crunes_t find_str = make_crunes("system");
            crunes_t found    = make_crunes(nrunes::find(dst, find_str));
            CHECK_TRUE(found == make_crunes("system"));

            crunes_t replace_str = make_crunes("copied");
            nrunes::findReplace(dst, find_str, replace_str);

            CHECK_EQUAL(0, nrunes::compare(dst, str2));
        }

        UNITTEST_TEST(compare)
        {
            crunes_t str1 = make_crunes("this is a system string");
            crunes_t str2 = make_crunes("this is a system string");
            CHECK_EQUAL(0, nrunes::compare(str1, str2));

            crunes_t str3 = make_crunes("a");
            crunes_t str4 = make_crunes("b");
            CHECK_EQUAL(-1, nrunes::compare(str3, str4));
            CHECK_EQUAL(0, nrunes::compare(str3, str3));
            CHECK_EQUAL(0, nrunes::compare(str4, str4));
            CHECK_EQUAL(1, nrunes::compare(str4, str3));

            crunes_t str5 = make_crunes("a");
            crunes_t str6 = make_crunes("A");
            crunes_t str7 = make_crunes("b");
            crunes_t str8 = make_crunes("B");
            CHECK_EQUAL(1, nrunes::compare(str5, str6));
            CHECK_EQUAL(0, nrunes::compare(str5, str6, false));
            CHECK_EQUAL(1, nrunes::compare(str7, str8));
            CHECK_EQUAL(0, nrunes::compare(str7, str8, false));
        }

        UNITTEST_TEST(concatenate)
        {
            utf32::rune dst_runes[256];
            dst_runes[0].r = 0;
            dst_runes[1].r = 0;
            runes_t dst    = make_runes(dst_runes, 0, 0, 256);

            crunes_t str1 = make_crunes("this is a ");
            nrunes::copy(str1, dst);

            crunes_t str2 = make_crunes("copied string");
            nrunes::concatenate(dst, str2);

            crunes_t str3 = make_crunes("this is a copied string");
            CHECK_EQUAL(0, nrunes::compare(dst, str3));
        }

        UNITTEST_TEST(parse_bool)
        {
            bool     value;
            crunes_t str = make_crunes("True");
            nrunes::parse(str, value);
            CHECK_EQUAL(true, value);
            crunes_t str2 = make_crunes("Off");
            nrunes::parse(str2, value);
            CHECK_EQUAL(false, value);
            crunes_t str3 = make_crunes("On");
            nrunes::parse(str3, value);
            CHECK_EQUAL(true, value);
            crunes_t str4 = make_crunes("false");
            nrunes::parse(str4, value);
            CHECK_EQUAL(false, value);
            crunes_t str6 = make_crunes("Yes");
            nrunes::parse(str6, value);
            CHECK_EQUAL(true, value);
            crunes_t str5 = make_crunes("No");
            nrunes::parse(str5, value);
            CHECK_EQUAL(false, value);
        }

        UNITTEST_TEST(parse_s32)
        {
            s32      value;
            crunes_t str = make_crunes("1");
            nrunes::parse(str, value);
            CHECK_EQUAL(1, value);
            crunes_t str2 = make_crunes("2");
            nrunes::parse(str2, value);
            CHECK_EQUAL(2, value);
            crunes_t str3 = make_crunes("256");
            nrunes::parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_u32)
        {
            u32      value;
            crunes_t str = make_crunes("1");
            nrunes::parse(str, value);
            CHECK_EQUAL(1, value);
            crunes_t str2 = make_crunes("2");
            nrunes::parse(str2, value);
            CHECK_EQUAL(2, value);
            crunes_t str3 = make_crunes("256");
            nrunes::parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_s64)
        {
            s64      value;
            crunes_t str = make_crunes("1");
            nrunes::parse(str, value);
            CHECK_EQUAL(1, value);
            crunes_t str2 = make_crunes("2");
            nrunes::parse(str2, value);
            CHECK_EQUAL(2, value);
            crunes_t str3 = make_crunes("256");
            nrunes::parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_u64)
        {
            u64      value;
            crunes_t str = make_crunes("1");
            nrunes::parse(str, value);
            CHECK_EQUAL(1, value);
            crunes_t str2 = make_crunes("2");
            nrunes::parse(str2, value);
            CHECK_EQUAL(2, value);
            crunes_t str3 = make_crunes("256");
            nrunes::parse(str3, value);
            CHECK_EQUAL(256, value);
        }

        UNITTEST_TEST(parse_f32)
        {
            f32      value;
            crunes_t str = make_crunes("1.1");
            nrunes::parse(str, value);
            CHECK_EQUAL(1.1f, value);
            crunes_t str2 = make_crunes("2.5");
            nrunes::parse(str2, value);
            CHECK_EQUAL(2.5f, value);
            crunes_t str3 = make_crunes("-256.33");
            nrunes::parse(str3, value);
            CHECK_EQUAL(-256.33f, value);
        }

        UNITTEST_TEST(parse_f64)
        {
            f64      value;
            crunes_t str = make_crunes("1.1");
            nrunes::parse(str, value);
            CHECK_EQUAL(1.1, value);
            crunes_t str2 = make_crunes("2.5");
            nrunes::parse(str2, value);
            CHECK_EQUAL(2.5, value);
            crunes_t str3 = make_crunes("-256.33");
            nrunes::parse(str3, value);
            CHECK_EQUAL(-256.33, value);
        }

        UNITTEST_TEST(is_decimal)
        {
            crunes_t decimal_str     = make_crunes("2017");
            crunes_t non_decimal_str = make_crunes("20a1a");
            CHECK_EQUAL(true, nrunes::is_decimal(decimal_str));
            CHECK_EQUAL(false, nrunes::is_decimal(non_decimal_str));
        }

        UNITTEST_TEST(is_hexadecimal)
        {
            crunes_t hexadecimal_str     = make_crunes("20aabbccddeeff");
            crunes_t non_hexadecimal_str = make_crunes("20aabbccddeeffw");
            CHECK_EQUAL(true, nrunes::is_hexadecimal(hexadecimal_str));
            CHECK_EQUAL(false, nrunes::is_hexadecimal(non_hexadecimal_str));
            crunes_t hexadecimal_with_prefix_str = make_crunes("0x20aabbccddeeff");
            CHECK_EQUAL(true, nrunes::is_hexadecimal(hexadecimal_with_prefix_str, true));
        }

        UNITTEST_TEST(is_float)
        {
            crunes_t float_str     = make_crunes("3.1415");
            crunes_t non_float_str = make_crunes("3a.14_15");
            CHECK_EQUAL(true, nrunes::is_float(float_str));
            CHECK_EQUAL(false, nrunes::is_float(non_float_str));
        }

        UNITTEST_TEST(is_GUID)
        {
            crunes_t guid_str     = make_crunes("11335577:22446688:557799BB:88AACCEE");
            crunes_t non_guid_str = make_crunes("335577:446688:7799BB:AACCEE");
            CHECK_EQUAL(true, nrunes::is_GUID(guid_str));
            CHECK_EQUAL(false, nrunes::is_GUID(non_guid_str));
        }

        UNITTEST_TEST(tostring_s32)
        {
            utf32::rune str_runes[256];
            str_runes[0].r = 0;
            str_runes[1].r = 0;
            runes_t str    = make_runes(str_runes, 0, 0, 256);

            s32 value = 31415;
            nrunes::to_string(str, value);
            CHECK_EQUAL(0, nrunes::compare(str, make_crunes("31415")));
        }

        UNITTEST_TEST(tostring_u32)
        {
            utf32::rune str_runes[256];
            str_runes[0].r = 0;
            str_runes[1].r = 0;
            runes_t str    = make_runes(str_runes, 0, 0, 256);

            u32 value = 31415;
            nrunes::to_string(str, value);
            CHECK_EQUAL(0, nrunes::compare(str, make_crunes("31415")));
        }

        UNITTEST_TEST(tostring_s64)
        {
            utf32::rune str_runes[256];
            str_runes[0].r = 0;
            str_runes[1].r = 0;
            runes_t str    = make_runes(str_runes, 0, 0, 256);

            s64 value = 31415;
            nrunes::to_string(str, value);
            CHECK_EQUAL(0, nrunes::compare(str, make_crunes("31415")));
        }

        UNITTEST_TEST(tostring_u64)
        {
            utf32::rune str_runes[256];
            str_runes[0].r = 0;
            str_runes[1].r = 0;
            runes_t str    = make_runes(str_runes, 0, 0, 256);

            u64 value = 31415;
            nrunes::to_string(str, value);
            CHECK_EQUAL(0, nrunes::compare(str, make_crunes("31415")));
        }

        UNITTEST_TEST(tostring_f32)
        {
            utf32::rune str_runes[256];
            str_runes[0].r = 0;
            str_runes[1].r = 0;
            runes_t str    = make_runes(str_runes, 0, 0, 256);

            f32 value = 3.1415f;
            nrunes::to_string(str, value, 4);
            CHECK_EQUAL(0, nrunes::compare(str, make_crunes("3.1415")));
        }

        UNITTEST_TEST(tostring_f64)
        {
            utf32::rune str_runes[256];
            str_runes[0].r = 0;
            str_runes[1].r = 0;
            runes_t str    = make_runes(str_runes, 0, 0, 256);

            f64 value = 3.1415;
            nrunes::to_string(str, value, 4);
            CHECK_EQUAL(0, nrunes::compare(str, make_crunes("3.1415")));
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
            CHECK_EQUAL('B', nrunes::to_upper('b'));
            CHECK_EQUAL('b', nrunes::to_lower('B'));
            CHECK_EQUAL('0', nrunes::to_upper('0'));
            CHECK_EQUAL('9', nrunes::to_lower('9'));

            CHECK_EQUAL(0, nrunes::to_digit('0'));
            CHECK_EQUAL(3, nrunes::to_digit('3'));
            CHECK_EQUAL(9, nrunes::to_digit('9'));

            CHECK_EQUAL(5, nrunes::hex_to_number('5'));
            CHECK_EQUAL(10, nrunes::hex_to_number('a'));
            CHECK_EQUAL(11, nrunes::hex_to_number('B'));
            CHECK_EQUAL(15, nrunes::hex_to_number('F'));
        }

        UNITTEST_TEST(is_upper)
        {
            crunes_t str = make_crunes("THIS IS AN UPPERCASE STRING");
            CHECK_EQUAL(true, nrunes::is_upper(str));
            crunes_t str2 = make_crunes("THIS IS UPPERCASE STRING with some lowercase");
            CHECK_EQUAL(false, nrunes::is_upper(str2));
        }

        UNITTEST_TEST(is_lower)
        {
            crunes_t str1 = make_crunes("this is a lowercase string");
            CHECK_EQUAL(true, nrunes::is_lower(str1));
            crunes_t str2 = make_crunes("THIS IS UPPERCASE STRING with some lowercase");
            CHECK_EQUAL(false, nrunes::is_lower(str2));
        }

        UNITTEST_TEST(is_capitalized)
        {
            crunes_t str1 = make_crunes("This Is A Capitalized String");
            CHECK_EQUAL(true, nrunes::is_capitalized(str1));
            crunes_t str2 = make_crunes("This Is Not all Capitalized");
            CHECK_EQUAL(false, nrunes::is_capitalized(str2));
        }

        UNITTEST_TEST(is_delimited)
        {
            crunes_t str1 = make_crunes("<this Is A delimited String>");
            CHECK_EQUAL(true, nrunes::is_delimited(str1, '<', '>'));
            crunes_t str2 = make_crunes("[This Is Not all Capitalized");
            CHECK_EQUAL(false, nrunes::is_delimited(str2, '[', ']'));
        }

        UNITTEST_TEST(is_quoted)
        {
            crunes_t str1 = make_crunes("'this Is A quoted String'");
            CHECK_EQUAL(true, nrunes::is_delimited(str1, '\'', '\''));
            crunes_t str2 = make_crunes("'This Is Not correctly quoted Capitalized\"");
            CHECK_EQUAL(false, nrunes::is_delimited(str2, '\'', '\''));
        }

        UNITTEST_TEST(to_upper)
        {
            utf32::rune str_runes[256];
            str_runes[0].r = 0;
            str_runes[1].r = 0;
            runes_t str    = make_runes(str_runes, 0, 0, 256);

            crunes_t str1 = make_crunes("this is a lower case string");
            crunes_t str2 = make_crunes("THIS IS A LOWER CASE STRING");
            nrunes::copy(str1, str);
            nrunes::to_upper(str);
            CHECK_EQUAL(0, nrunes::compare(str2, str));
        }

        UNITTEST_TEST(to_lower)
        {
            utf32::rune str_runes[256];
            str_runes[0].r = 0;
            str_runes[1].r = 0;
            runes_t str    = make_runes(str_runes, 0, 0, 256);

            crunes_t str1 = make_crunes("THIS IS AN UPPER CASE STRING");
            crunes_t str2 = make_crunes("this is an upper case string");
            nrunes::copy(str1, str);
            nrunes::to_lower(str);
            CHECK_EQUAL(0, nrunes::compare(str2, str));
        }

        UNITTEST_TEST(starts_with)
        {
            crunes_t str1   = make_crunes("a simple string");
            crunes_t str2   = make_crunes("need a longer string");
            crunes_t start2 = make_crunes("need");

            CHECK_EQUAL(true, nrunes::starts_with(str1, 'a'));
            CHECK_EQUAL(false, nrunes::starts_with(str2, 'a'));

            CHECK_EQUAL(false, nrunes::starts_with(str1, start2));
            CHECK_EQUAL(true, nrunes::starts_with(str2, start2));
        }

        UNITTEST_TEST(first_char)
        {
            crunes_t str1 = make_crunes("a simple string");
            CHECK_EQUAL('a', nrunes::first_char(str1));
        }

        UNITTEST_TEST(cprintf)
        {
            s32 const i   = 100;
            crunes_t  str = make_crunes("hello");

            crunes_t fmt    = make_crunes("%d %s");
            s32      length = cprintf(fmt, va_t(i), va_t(str));
            CHECK_EQUAL(9, length);

            utf32::rune dst_runes[256];
            dst_runes[0].r = 0;
            dst_runes[1].r = 0;
            runes_t dst    = make_runes(dst_runes, 0, 0, 256);

            sprintf(dst, fmt, va_t(i), va_t(str));
            CHECK_EQUAL(0, nrunes::compare(dst, make_crunes("100 hello")));
        }

        UNITTEST_TEST(vcprintf)
        {
            s32      i      = 100;
            crunes_t str    = make_crunes("hello");
            crunes_t fmt    = make_crunes("%d %s");
            s32      length = cprintf(fmt, va_t(i), va_t(str));
            CHECK_EQUAL(9, length);
        }

        UNITTEST_TEST(sprintf)
        {
            utf32::rune dst_runes[256];
            dst_runes[0].r = 0;
            dst_runes[1].r = 0;
            runes_t dst    = make_runes(dst_runes, 0, 0, 256);

            s32 i = 100;

            crunes_t str = make_crunes("hello");
            crunes_t fmt = make_crunes("%d %s");

            sprintf(dst, fmt, va_t(i), va_t(str));
            CHECK_TRUE(nrunes::compare(dst, make_crunes("100 hello")) == 0);

            // Check all format functionality?
        }

        UNITTEST_TEST(sprintf_bool)
        {
            utf32::rune dst_runes[256];
            dst_runes[0].r = 0;
            dst_runes[1].r = 0;
            runes_t dst    = make_runes(dst_runes, 0, 0, 256);

            // ---------------------------------------------------------------------------
            // Boolean, True/False and Yes/No verification
            reset(dst);
            sprintf(dst, make_crunes("%b"), va_t(true));
            CHECK_TRUE(nrunes::compare(dst, make_crunes("true")) == 0);

            reset(dst);
            sprintf(dst, make_crunes("%B"), va_t(true));
            CHECK_TRUE(nrunes::compare(dst, make_crunes("TRUE")) == 0);

            reset(dst);
            sprintf(dst, make_crunes("%b"), va_t(false));
            CHECK_TRUE(nrunes::compare(dst, make_crunes("false")) == 0);

            reset(dst);
            sprintf(dst, make_crunes("%#b"), va_t(false));
            CHECK_TRUE(nrunes::compare(dst, make_crunes("False")) == 0);

            reset(dst);
            sprintf(dst, make_crunes("%y"), va_t(true));
            CHECK_TRUE(nrunes::compare(dst, make_crunes("yes")) == 0);

            reset(dst);
            sprintf(dst, make_crunes("%y"), va_t(false));
            CHECK_TRUE(nrunes::compare(dst, make_crunes("no")) == 0);

            reset(dst);
            sprintf(dst, make_crunes("%Y"), va_t(true));
            CHECK_TRUE(nrunes::compare(dst, make_crunes("YES")) == 0);

            reset(dst);
            sprintf(dst, make_crunes("%#y"), va_t(true));
            CHECK_TRUE(nrunes::compare(dst, make_crunes("Yes")) == 0);
            // ---------------------------------------------------------------------------
        }

        UNITTEST_TEST(vsprintf)
        {
            utf32::rune dst_runes[256];
            dst_runes[0].r = 0;
            dst_runes[1].r = 0;
            runes_t dst    = make_runes(dst_runes, 0, 0, 256);

            s32      i   = 100;
            crunes_t str = make_crunes("hello");
            crunes_t fmt = make_crunes("%d %s");
            sprintf(dst, fmt, va_t(i), va_t(str));
            CHECK_TRUE(nrunes::compare(dst, make_crunes("100 hello")) == 0);
        }

        UNITTEST_TEST(sscanf)
        {
            // Test scanf
            crunes_t str    = make_crunes("1.0 100");
            crunes_t format = make_crunes("%f %u");

            f32 myfloat;
            u32 myint;
            sscanf(str, format, va_r_t(&myfloat), va_r_t(&myint));

            CHECK_EQUAL(1.0f, myfloat);
            CHECK_EQUAL(100, myint);
        }

        // ---------------------------------------------------------------------------
        UNITTEST_TEST(path_parser_1)
        {
            crunes_t fullpath = make_crunes("C:\\projects\\binary_reader\\bin\\binary_reader.cpp.old");
            crunes_t out_device;
            crunes_t out_path;
            crunes_t out_filename;
            crunes_t out_extension;
            crunes_t out_first_folder;

            crunes_t slash     = make_crunes("\\");
            crunes_t devicesep = make_crunes(":\\");
            out_device         = nrunes::findSelectUntilIncluded(fullpath, devicesep);
            crunes_t filepath  = nrunes::selectAfterExclude(fullpath, out_device);
            out_path           = nrunes::findLastSelectUntilIncluded(filepath, slash);
            out_filename       = nrunes::selectAfterExclude(fullpath, out_path);
            out_filename       = nrunes::findLastSelectUntil(out_filename, '.');
            out_extension      = nrunes::selectAfterExclude(fullpath, out_filename);

            nrunes::trimRight(out_device, devicesep);

            out_first_folder = nrunes::findSelectUntil(out_path, slash);

            crunes_t device = make_crunes("C");
            CHECK_TRUE(device == out_device);
            crunes_t path = make_crunes("projects\\binary_reader\\bin\\");
            CHECK_TRUE(path == out_path);
            crunes_t filename = make_crunes("binary_reader.cpp");
            CHECK_TRUE(filename == out_filename);
            crunes_t extension = make_crunes(".old");
            CHECK_TRUE(extension == out_extension);
            crunes_t first_folder = make_crunes("projects");
            CHECK_TRUE(first_folder == out_first_folder);
        }

        UNITTEST_TEST(path_parser_2)
        {
            crunes_t fullpath = make_crunes("C:\\binary_reader.cpp.old");
            crunes_t out_device;
            crunes_t out_path;
            crunes_t out_filename;
            crunes_t out_extension;
            crunes_t out_first_folder;

            crunes_t slash     = make_crunes("\\");
            crunes_t devicesep = make_crunes(":\\");
            out_device         = nrunes::findSelectUntilIncluded(fullpath, devicesep);
            crunes_t filepath  = nrunes::selectAfterExclude(fullpath, out_device);
            out_path           = nrunes::findLastSelectUntilIncluded(filepath, slash);
            out_filename       = nrunes::selectAfterExclude(fullpath, out_path);
            out_filename       = nrunes::findLastSelectUntil(out_filename, '.');
            out_extension      = nrunes::selectAfterExclude(fullpath, out_filename);

            nrunes::trimRight(out_device, devicesep);

            out_first_folder = nrunes::findSelectUntil(out_path, slash);

            crunes_t device = make_crunes("C");
            CHECK_TRUE(device == out_device);
            crunes_t path = make_crunes("");
            CHECK_TRUE(path == out_path);
            crunes_t filename = make_crunes("binary_reader.cpp");
            CHECK_TRUE(filename == out_filename);
            crunes_t extension = make_crunes(".old");
            CHECK_TRUE(extension == out_extension);
            crunes_t first_folder = make_crunes("");
            CHECK_TRUE(first_folder == out_first_folder);
        }
    }
}
UNITTEST_SUITE_END
