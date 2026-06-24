#include "cbase/c_runes.h"
#include "cbase/c_printf.h"

#if defined(TARGET_PS3)
#    include <math.h>
#endif

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(sprintf)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(format_double)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            str_t str  = utf32::make_runes(str_runes, 0, 0, 256);
            str_t fmt  = ascii::make_const_runes("%f");

            f64 d = 3.1415;
            sprintf(str, fmt, va_t(d));
            str_t answer_str = ascii::make_const_runes("3.141500");
            CHECK_EQUAL(0, compare(answer_str, str));
        }

        UNITTEST_TEST(format_string)
        {
            utf32::rune str_runes[256];
            str_runes[0] = 0;
            str_runes[1] = 0;
            str_t str  = utf32::make_runes(str_runes, 0, 0, 256);
            str_t fmt  = ascii::make_const_runes("the %s");

            sprintf(str, fmt, va_t("test string"));
            str_t cmp = ascii::make_const_runes("the test string");
            CHECK_EQUAL(0, compare(str, cmp));
        }
    }
}
UNITTEST_SUITE_END
