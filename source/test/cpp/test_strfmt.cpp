#include "cbase/c_carray.h"
#include "cbase/c_strfmt.h"

#include "cunittest/cunittest.h"

using namespace ncore;

namespace ncore
{
    struct DateTime
    {
        u64 ticks;
    };

    namespace fmt
    {
        // should we have a boolean return value to indicate whether the format is correctly handled?
        void FormatDateTime(encoded_arg_t const& argValue, fmt::strspan_t& dst, fmt::state_t& state)
        {
            DateTime dt = {argValue.first.u64};

            char buf[128];
            fmt::toStr(buf, 128, "{}/{:02d}/{:02d}", 2023, 12, 25);
            char* end = buf + ascii::strlen(buf);

            fmt::state_t::format_string(dst, state, buf, end);
        }

        template <>
        struct arg_t<DateTime>
        {
            static inline encoded_arg_t encode(DateTime v) { encoded_arg_t e; e.first.u64 = v.ticks; return e; }
            static inline DateTime      decode(encoded_arg_t const& v) { return DateTime{v.first.u64}; }
            static inline format_func_t formatter() { return FormatDateTime; }
        };

    }  // namespace fmt

}  // namespace ncore

UNITTEST_SUITE_BEGIN(test_strfmt)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        char str[128]{};

        // Boolean type
        UNITTEST_TEST(boolean_type)
        {
            fmt::toStr(str, 128, "{}", false);
            CHECK_EQUAL(str, "false");
            fmt::toStr(str, 128, "{}", true);
            CHECK_EQUAL(str, "true");

            fmt::toStr(str, 128, "{:d}", false);
            CHECK_EQUAL(str, "0");
            fmt::toStr(str, 128, "{:d}", true);
            CHECK_EQUAL(str, "1");

            fmt::toStr(str, 128, "{:x}", false);
            CHECK_EQUAL(str, "0");
            fmt::toStr(str, 128, "{:x}", true);
            CHECK_EQUAL(str, "1");

            fmt::toStr(str, 128, "{:X}", false);
            CHECK_EQUAL(str, "0");
            fmt::toStr(str, 128, "{:X}", true);
            CHECK_EQUAL(str, "1");

            fmt::toStr(str, 128, "{:o}", false);
            CHECK_EQUAL(str, "0");
            fmt::toStr(str, 128, "{:o}", true);
            CHECK_EQUAL(str, "1");

            fmt::toStr(str, 128, "{:b}", false);
            CHECK_EQUAL(str, "0");
            fmt::toStr(str, 128, "{:b}", true);
            CHECK_EQUAL(str, "1");

            fmt::toStr(str, 128, "{:B}", false);
            CHECK_EQUAL(str, "0");
            fmt::toStr(str, 128, "{:B}", true);
            CHECK_EQUAL(str, "1");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", true));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", true));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", true));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", true));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", true));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", true));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", true));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", true));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", true));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", true));
        }

        UNITTEST_TEST(char_type)
        {
            fmt::toStr(str, 128, "{}", 'N');
            CHECK_EQUAL(str, "N");
            fmt::toStr(str, 128, "{:c}", 'N');
            CHECK_EQUAL(str, "N");
            fmt::toStr(str, 128, "{:d}", 'N');
            CHECK_EQUAL(str, "78");
            fmt::toStr(str, 128, "{:x}", 'N');
            CHECK_EQUAL(str, "4e");
            fmt::toStr(str, 128, "{:X}", 'N');
            CHECK_EQUAL(str, "4E");
            fmt::toStr(str, 128, "{:o}", 'N');
            CHECK_EQUAL(str, "116");
            fmt::toStr(str, 128, "{:b}", 'N');
            CHECK_EQUAL(str, "1001110");
            fmt::toStr(str, 128, "{:B}", 'N');
            CHECK_EQUAL(str, "1001110");

            CHECK_TRUE(fmt::toStr(str, 128, "{:f}", 'N'));
            CHECK_TRUE(fmt::toStr(str, 128, "{:F}", 'N'));
            CHECK_TRUE(fmt::toStr(str, 128, "{:e}", 'N'));
            CHECK_TRUE(fmt::toStr(str, 128, "{:E}", 'N'));
            CHECK_TRUE(fmt::toStr(str, 128, "{:g}", 'N'));
            CHECK_TRUE(fmt::toStr(str, 128, "{:G}", 'N'));
#if 0
            CHECK_TRUE(fmt::toStr(str, 128, "{:p}", 'N'));
            CHECK_TRUE(fmt::toStr(str, 128, "{:P}", 'N'));
            CHECK_TRUE(fmt::toStr(str, 128, "{:s}", 'N'));
#endif
        }

        // Integer (int) type
        UNITTEST_TEST(signed_integer_type)
        {
            int value = 123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111011");

            value = -123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "-123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "-123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "-7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "-7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "-173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "-1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "-1111011");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Integer (unsigned int) type
        UNITTEST_TEST(unsigned_integer_type)
        {
            unsigned int value = 123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111011");

            value = static_cast<unsigned int>(-123);

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "4294967173");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "4294967173");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "ffffff85");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "FFFFFF85");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "37777777605");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "11111111111111111111111110000101");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "11111111111111111111111110000101");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Integer (s8) type
        UNITTEST_TEST(signed_8bit_integer_type)
        {
            s8 value = 123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111011");

            value = -123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "-123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "-123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "-7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "-7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "-173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "-1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "-1111011");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Integer (u8) type
        UNITTEST_TEST(unsigned_8bit_integer_type)
        {
            u8 value = 123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111011");

            value = static_cast<u8>(-123);

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "133");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "133");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "85");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "85");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "205");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "10000101");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "10000101");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Integer (s16) type
        UNITTEST_TEST(signed_16bit_integer_type)
        {
            s16 value = 123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111011");

            value = -123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "-123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "-123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "-7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "-7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "-173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "-1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "-1111011");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Integer (u16) type
        UNITTEST_TEST(unsigned_16bit_integer_type)
        {
            u16 value = 123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111011");

            value = static_cast<u16>(-123);

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "65413");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "65413");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "ff85");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "FF85");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "177605");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111111110000101");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111111110000101");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Integer (s32) type
        UNITTEST_TEST(signed_32bit_integer_type)
        {
            s32 value = 123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111011");

            value = -123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "-123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "-123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "-7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "-7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "-173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "-1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "-1111011");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Integer (u32) type
        UNITTEST_TEST(unsigned_32bit_integer_type)
        {
            u32 value = 123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111011");

            value = static_cast<u32>(-123);

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "4294967173");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "4294967173");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "ffffff85");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "FFFFFF85");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "37777777605");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "11111111111111111111111110000101");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "11111111111111111111111110000101");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Integer (s64) type
        UNITTEST_TEST(signed_64bit_integer_type)
        {
            s64 value = 123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111011");

            value = -123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "-123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "-123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "-7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "-7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "-173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "-1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "-1111011");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Integer (u64) type
        UNITTEST_TEST(unsigned_64bit_integer_type)
        {
            u64 value = 123;

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "123");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "7b");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "7B");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "173");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111011");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111011");

            value = static_cast<u64>(-123);

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "18446744073709551493");
            fmt::toStr(str, 128, "{:d}", value);
            CHECK_EQUAL(str, "18446744073709551493");
            fmt::toStr(str, 128, "{:x}", value);
            CHECK_EQUAL(str, "ffffffffffffff85");
            fmt::toStr(str, 128, "{:X}", value);
            CHECK_EQUAL(str, "FFFFFFFFFFFFFF85");
            fmt::toStr(str, 128, "{:o}", value);
            CHECK_EQUAL(str, "1777777777777777777605");
            fmt::toStr(str, 128, "{:b}", value);
            CHECK_EQUAL(str, "1111111111111111111111111111111111111111111111111111111110000101");
            fmt::toStr(str, 128, "{:B}", value);
            CHECK_EQUAL(str, "1111111111111111111111111111111111111111111111111111111110000101");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Floating point (float) type
        UNITTEST_TEST(float_type)
        {
            float value = 123.456789f;

            char std_str[128]{};

            fmt::toStr(str, 128, "{}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%g", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:f}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%f", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:F}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%F", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:e}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%e", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:E}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%E", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:g}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%g", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:G}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%G", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);

            value = -123.456789f;

            fmt::toStr(str, 128, "{}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%g", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:f}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%f", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:F}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%F", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:e}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%e", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:E}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%E", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:g}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%g", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:G}", value);
#if 0

            UnitTest::gStringPrint(std_str, 128 - 1, "%G", static_cast<double>(value));
            CHECK_EQUAL(str, std_str);

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:d}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:x}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:X}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:o}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:b}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:B}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
#endif
        }

        // Floating point (double) type
        UNITTEST_TEST(double_type)
        {
            double value = 123.456789;

            char std_str[128]{};

            fmt::toStr(str, 128, "{}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%g", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:f}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%f", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:F}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%F", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:e}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%e", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:E}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%E", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:g}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%g", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:G}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%G", value);
            CHECK_EQUAL(str, std_str);

            value = -123.456789;

            fmt::toStr(str, 128, "{}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%g", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:f}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%f", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:F}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%F", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:e}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%e", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:E}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%E", value);
            CHECK_EQUAL(str, std_str);

#if 0
            fmt::toStr(str, 128, "{:g}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%g", value);
            CHECK_EQUAL(str, std_str);
            fmt::toStr(str, 128, "{:G}", value);
            UnitTest::gStringPrint(std_str, 128 - 1, "%G", value);
            CHECK_EQUAL(str, std_str);

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:d}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:x}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:X}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:o}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:b}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:B}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:p}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:P}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
#endif
        }

        // Pointer (void*) type
        UNITTEST_TEST(void_pointer_type)
        {
            void* value = reinterpret_cast<void*>(1000);

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "3e8");
            fmt::toStr(str, 128, "{:p}", value);
            CHECK_EQUAL(str, "3e8");
            fmt::toStr(str, 128, "{:P}", value);
            CHECK_EQUAL(str, "3E8");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:d}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:x}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:X}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:o}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:b}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:B}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // Constant pointer (const void*) type
        UNITTEST_TEST(const_void_pointer_type)
        {
            const void* value = reinterpret_cast<void*>(1000);

            fmt::toStr(str, 128, "{}", value);
            CHECK_EQUAL(str, "3e8");
            fmt::toStr(str, 128, "{:p}", value);
            CHECK_EQUAL(str, "3e8");
            fmt::toStr(str, 128, "{:P}", value);
            CHECK_EQUAL(str, "3E8");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:d}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:x}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:X}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:o}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:b}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:B}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", value));
            CHECK_FALSE(fmt::toStr(str, 128, "{:s}", value));
        }

        // String (convertible to usf::StringView) type
        UNITTEST_TEST(string_type)
        {
            char s1[]{"str"};
            fmt::toStr(str, 128, "{}", s1);
            CHECK_EQUAL(str, "str");
            fmt::toStr(str, 128, "{:s}", s1);
            CHECK_EQUAL(str, "str");

            const char s2[]{"str"};
            fmt::toStr(str, 128, "{}", s2);
            CHECK_EQUAL(str, "str");
            fmt::toStr(str, 128, "{:s}", s2);
            CHECK_EQUAL(str, "str");

            char* s3 = s1;
            fmt::toStr(str, 128, "{}", s3);
            CHECK_EQUAL(str, "str");
            fmt::toStr(str, 128, "{:s}", s3);
            CHECK_EQUAL(str, "str");

            const char* s4 = s2;
            fmt::toStr(str, 128, "{}", s4);
            CHECK_EQUAL(str, "str");
            fmt::toStr(str, 128, "{:s}", s4);
            CHECK_EQUAL(str, "str");

            fmt::strspan_t sv(s1);
            fmt::toStr(str, 128, "{}", sv);
            CHECK_EQUAL(str, "str");
            fmt::toStr(str, 128, "{:s}", sv);
            CHECK_EQUAL(str, "str");

            CHECK_FALSE(fmt::toStr(str, 128, "{:c}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:d}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:x}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:X}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:o}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:b}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:B}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:f}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:F}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:e}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:E}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:g}", sv));
            CHECK_FALSE(fmt::toStr(str, 128, "{:G}", sv));
        }

        UNITTEST_TEST(custom_type)
        {
            DateTime dt = {1234567890};
            fmt::toStr(str, 128, "{:14}", dt);
            CHECK_EQUAL(str, "    2023/12/25");
        }

        const void* ptr1 = reinterpret_cast<void*>(1000);
        void*       ptr2 = reinterpret_cast<void*>(1000);

        UNITTEST_TEST(width_without_type)
        {
            fmt::toStr(str, 128, "{:14}", false);
            CHECK_EQUAL("         false", str);
            fmt::toStr(str, 128, "{:14}", 'N');
            CHECK_EQUAL(str, "             N");
            fmt::toStr(str, 128, "{:14}", 123);
            CHECK_EQUAL(str, "           123");
            fmt::toStr(str, 128, "{:14}", 1.234);
            CHECK_EQUAL(str, "         1.234");
            fmt::toStr(str, 128, "{:14}", ptr1);
            CHECK_EQUAL(str, "           3e8");
            fmt::toStr(str, 128, "{:14}", ptr2);
            CHECK_EQUAL(str, "           3e8");
            fmt::toStr(str, 128, "{:14}", "str");
            CHECK_EQUAL(str, "           str");
        }

        UNITTEST_TEST(width_with_type)
        {
            fmt::toStr(str, 128, "{:14c}", 'N');
            CHECK_EQUAL(str, "             N");
            fmt::toStr(str, 128, "{:14d}", 123);
            CHECK_EQUAL(str, "           123");
            fmt::toStr(str, 128, "{:14x}", 123);
            CHECK_EQUAL(str, "            7b");
            fmt::toStr(str, 128, "{:14o}", 123);
            CHECK_EQUAL(str, "           173");
            fmt::toStr(str, 128, "{:14b}", 123);
            CHECK_EQUAL(str, "       1111011");
            fmt::toStr(str, 128, "{:14f}", 1.234);
            CHECK_EQUAL(str, "      1.234000");
            fmt::toStr(str, 128, "{:14e}", 1.234);
            CHECK_EQUAL(str, "  1.234000e+00");
            fmt::toStr(str, 128, "{:14g}", 1.234);
            CHECK_EQUAL(str, "         1.234");
            fmt::toStr(str, 128, "{:14p}", ptr1);
            CHECK_EQUAL(str, "           3e8");
            fmt::toStr(str, 128, "{:14s}", "str");
            CHECK_EQUAL(str, "           str");
        }

        UNITTEST_TEST(precision_with_type)
        {
            fmt::toStr(str, 128, "{:.1f}", 1.234);
            CHECK_EQUAL(str, "1.2");
            fmt::toStr(str, 128, "{:.1e}", 1.234);
            CHECK_EQUAL(str, "1.2e+00");
            fmt::toStr(str, 128, "{:.1g}", 1.234);
            CHECK_EQUAL(str, "1");
            fmt::toStr(str, 128, "{:.1s}", "str");
            CHECK_EQUAL(str, "s");
        }

        UNITTEST_TEST(test_positional_args)
        {
            fmt::toStr(str, 32, "{0}{1}{0}", "abra", "cad");
            CHECK_EQUAL(str, "abracadabra");

            // The following test shows both the positional and
            // sequencial arguments working seamlessly together.

            fmt::toStr(str, 32, "{}{}{0}", "abra", "cad");
            CHECK_EQUAL(str, "abracadabra");

            fmt::toStr(str, 32, "{0}{1}{}", "abra", "cad");
            CHECK_EQUAL(str, "abracadabra");

            fmt::toStr(str, 32, "{}{1}{0}", "abra", "cad");
            CHECK_EQUAL(str, "abracadabra");
        }

        static bool test_float_conversion(const double value)
        {
            const char types[]{'f', 'e', 'g'};

            for (int t = 0; t < 3; ++t)  // Test fixed, scientific and general format
            {
                for (int p = 0; p <= 9; ++p)  // Test precision from 0 to 9
                {
                    char std_str[64]{};
                    char usf_str[64]{};

                    char std_fmt[8]{'%', '.', "0123456789"[p], types[t], 0};
                    char usf_fmt[8]{'{', ':', '.', "0123456789"[p], types[t], '}', 0};

                    UnitTest::gStringPrint(std_str, 64 - 1, std_fmt, value);

                    fmt::toStr(usf_str, 64 - 1, usf_fmt, value);

                    bool result_ok = UnitTest::gAreStringsEqual(usf_str, std_str);
                    if (!result_ok)
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        // Some random floating point numbers to test...
        constexpr double test_values_fp[]{
          0.00085499999999999997,
          1.065,
          1.345499999999999918287585387588478624820709228515625,
          2.7144439999999949719722280860878527164459228515625,
          4.71333299999994981277495753602124750614166259765625,
          18.4755549999999999499777914024889469146728515625,
          4.82444399999994999461705447174608707427978515625,
          9.1811109999998503639062619186006486415863037109375,
          1e-12,
          -1e-12,
          9.1811109999998503639062619186006486415863037109375e-11,
          1.8446743E19,
          -1.8446743E-9,
          1e-12,
          1.8446743E-12,
          -1.8446743E-12,
          5.55555,
          5.0,
          4.9999,
          5.000111,
          5.005,
          5.5,
          5.50,
          5.501,
          0.000254,
          0.0005004,
          0.99e-13,
          0.9888,
          1.3455,
          0.9,
          -123.5,
          -123.51,
          -122.5,
          -122.51,
          -123.49,
          -122.49,
          0,
          0.0001,
          -9999.99,
          9.9e-13,
          9.9e-12,
          18446742974197923840.00001,
          1.84467429741979238400000,
          1844.67429741979238400000,
          1.8446743E19,
          1.8446743E18,
          -1.8446743E19,
          -1.8446743E18,
          4528212345678.946638528859811704183484516925,
          1e-14,
          0.9999e-14,
          2.98023223876953125E-8,
        };

        UNITTEST_TEST(test_float_conversion)
        {
            for (s32 i = 0; i < sizeof(test_values_fp) / sizeof(test_values_fp[0]); ++i)
            {
                CHECK_TRUE(test_float_conversion(test_values_fp[i]));
            }
        }
    }
}
UNITTEST_SUITE_END
