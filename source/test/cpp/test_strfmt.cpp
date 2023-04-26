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
        void FormatDateTime(u8 argType, u64 argValue, fmt::str_t& dst, fmt::state_t& state)
        {
            DateTime dt = { argValue};

			char buf[128];
			fmt::toStr(buf, 128, "{}/{:02d}/{:02d}", 2023, 12, 25);
            char* end = buf + ascii::strlen(buf);
			
            fmt::state_t::format_string(dst, state, buf, end);
        }

        template <> struct arg_t<DateTime>
        {
            static inline u64           encode(DateTime v) { return *((u64*)(&v.ticks)); }
            static inline DateTime      decode(u64 v) { return DateTime{ v }; }
            static inline format_func_t formatter() { return FormatDateTime; }
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

        char str[128]{};
        
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

        
    }
}
UNITTEST_SUITE_END
