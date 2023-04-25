#include "cbase/c_carray.h"
#include "cbase/c_strfmt.h"
#include "cunittest/cunittest.h"

#include <ctime>

using namespace ncore;

namespace ncore
{
    struct DateTime
    {
        std::time_t ticks;

        void now()
        {
            ticks = std::time(nullptr);
        }
    };

    namespace fmt
    {
        // should we have a boolean return value to indicate whether the format is correctly handled?
        void FormatDateTime(u8 argType, u64 argValue, fmt::BasicStringSpan& dst, fmt::ArgFormatState& state)
        {
            DateTime dt = { (std::time_t)argValue};

            std::tm* time = std::localtime((const std::time_t*)&dt.ticks);

			char buf[128];
			char* end = fmt::toStr(buf, 128, "{}/{:02d}/{:02d}", 1900 + time->tm_year, time->tm_mon + 1, time->tm_mday);
			s32 len = (s32)(end - buf);
			
            state.write_alignment(dst, len, false);

            // write out the date time string
            // using 'state' information to format the string
        }

        template <> struct arg_t<DateTime>
        {
            static inline u64           encode(DateTime v) { return *((u64*)(&v.ticks)); }
            static inline DateTime      decode(u64 v) { return DateTime{ (std::time_t)v }; }
            static inline ArgFormatFunc formatter() { return FormatDateTime; }
        };

        template <> struct typed<DateTime>
        {
            static const u8 value = kCustom;
        };

    } // namespace fmt

} // namespace ncore

UNITTEST_SUITE_BEGIN(test_strfmt)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        char        str[128]{};
        const void* ptr1 = reinterpret_cast<void*>(1000);
        void*       ptr2 = reinterpret_cast<void*>(1000);

        UNITTEST_TEST(width_without_type)
        {

            fmt::toStr(str, 128, "{:14}", false);
            CHECK_EQUAL("         false", str);
            fmt::toStr(str, 128, "{:14}", 'N');
            CHECK_EQUAL(str, "N             ");
            fmt::toStr(str, 128, "{:14}", 123);
            CHECK_EQUAL(str, "           123");
            fmt::toStr(str, 128, "{:14}", 1.234);
            CHECK_EQUAL(str, "         1.234");
            fmt::toStr(str, 128, "{:14}", ptr1);
            CHECK_EQUAL(str, "           3e8");
            fmt::toStr(str, 128, "{:14}", ptr2);
            CHECK_EQUAL(str, "           3e8");
            fmt::toStr(str, 128, "{:14}", "str");
            CHECK_EQUAL(str, "str           ");
        }

        UNITTEST_TEST(width_with_type)
        {
            fmt::toStr(str, 128, "{:14c}", 'N');
            CHECK_EQUAL(str, "N             ");
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
            CHECK_EQUAL(str, "str           ");
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

        UNITTEST_TEST(precision_invalid_type)
        {
            CHECK_NULL(fmt::toStr(str, 128, "{:.1c}", 'N'));
            CHECK_NULL(fmt::toStr(str, 128, "{:.1d}", 123));
            CHECK_NULL(fmt::toStr(str, 128, "{:.1x}", 123));
            CHECK_NULL(fmt::toStr(str, 128, "{:.1o}", 123));
            CHECK_NULL(fmt::toStr(str, 128, "{:.1b}", 123));
        }

        UNITTEST_TEST(custom_type)
        {
            DateTime dt = {1234567890};
            fmt::toStr(str, 128, "{:.1f}", dt);
            CHECK_EQUAL(str, "?");
        }
    }
}
UNITTEST_SUITE_END
