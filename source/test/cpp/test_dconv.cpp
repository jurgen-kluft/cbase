#include "cbase/c_dconv.h"
#include "cbase/c_double.h"
#include "cbase/c_memory.h"
#include "cbase/c_runes.h"

#include "cunittest/cunittest.h"

#ifndef _MSC_VER
#    include <stdint.h>
#else
#    define strtoull _strtoui64
#    define __DBL_DENORM_MIN__ ((double)4.94065645841246544177e-324L)
#    define __DBL_MAX__ ((double)1.79769313486231570815e+308L)
#endif

using namespace ncore;


/**
 *
 *  Parse printf(3)-like format flags
 *
 *  @returns  1  if parsed normally
 *            0  if there were syntax errors
 *
 */
static int parse_printf_format_flags(const char* printf_format, int* format_char, unsigned int* format_flags, int* format_width, int* format_precision)
{
    if (*printf_format != '%')
        return 0;

    int state = 0;
    for (const char* p = printf_format + 1; *p; ++p)
    {
        char ch = *p;

        // 1. Parse flags in prefix
        if (state == 0)
        {
            if (ch == '#')
            {
                (*format_flags) |= DoubleConvert::FLAG_SHARP;
                continue;
            }
            else if (ch == '-')
            {
                (*format_flags) |= DoubleConvert::FLAG_LEFT_JUSTIFY;
                continue;
            }
            else if (ch == '+')
            {
                (*format_flags) |= DoubleConvert::FLAG_PRINT_PLUS;
                continue;
            }
            else if (ch == ' ')
            {
                (*format_flags) |= DoubleConvert::FLAG_SPACE_IF_PLUS;
                continue;
            }
            else if (ch == '0')
            {
                (*format_flags) |= DoubleConvert::FLAG_PAD_WITH_ZERO;
                state = 1;
                continue;
            }
        }

        // 2. Parse width
        if (state <= 1)
        {
            if ((ch >= '0') && (ch <= '9'))
            {
                (*format_flags) |= DoubleConvert::FLAG_HAVE_WIDTH;
                (*format_width) = 10 * (*format_width) + (ch - '0');
                state           = 1;
                continue;
            }
            else if (ch == '.')
            {
                (*format_precision) = 0;
                state               = 2;
                continue;
            }
        }

        // 3. Parse precision
        if ((state == 2) && (ch >= '0') && (ch <= '9'))
        {
            (*format_precision) = 10 * (*format_precision) + (ch - '0');
            continue;
        }

        // 4. Parse format char
        if (state <= 2)
        {
            if ((ch == 'E') || (ch == 'F') || (ch == 'G'))
            {
                (*format_flags) |= DoubleConvert::FLAG_UPPERCASE;
                *format_char = (ch - 'A') + 'a';
                state        = 3;
                continue;
            }
            else if ((ch == 'e') || (ch == 'f') || (ch == 'g'))
            {
                *format_char = ch;
                state        = 3;
                continue;
            }
        }

        // 5. Handle syntax errors
        return 0;
    }
    return (state == 3);
}

UNITTEST_SUITE_BEGIN(test_dconv)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        static bool single_static_test(const char* fmt, const char* str, double val, int flag_reverse_test)
        {
            // 1. Parse format string
            int          format_char      = 0;
            unsigned int format_flags     = 0;
            int          format_width     = 0;
            int          format_precision = DoubleConvert::DEFAULT_PRECISION;

            if (parse_printf_format_flags(fmt, &format_char, &format_flags, &format_width, &format_precision) == 0)
            {
                // Can't parse format string
                return false;
            }

            // 2. Check conversion val -> str
            char alt_str[128];
            nmem::memset(alt_str, 0, sizeof(alt_str));
            int   alt_str_size          = sizeof(alt_str) - 1;
            char* alt_str_end           = alt_str;
            int   dconvstr_print_status = dconvstr_print(&alt_str_end, &alt_str_size, val, format_char, format_flags, format_width, format_precision);
            if (!dconvstr_print_status)
            {
                // Can't convert double to string
                return false;
            }

            *alt_str_end = 0;
            if (0 != ascii::compare(str, alt_str))
            {
                // Formatting result not as expected
                return false;
            }

            // 3. Check conversion str -> val
            if (flag_reverse_test)
            {
                const char* str_expected_end     = str + ascii::strlen(str);
                const char* str_actual_end       = nullptr;
                double      alt_val              = 0.0;
                int         erange_condition     = 1;
                int         dconvstr_scan_status = dconvstr_scan(str, &str_actual_end, &alt_val, &erange_condition);
                if (!dconvstr_scan_status)
                {
                    // Unexpected internal error in reverse test
                    return false;
                }
                if (erange_condition)
                {
                    // Unexpected ERANGE condition in reverse test
                    return false;
                }
                if (str_actual_end != str_expected_end)
                {
                    // Unexpected syntax error in reverse test
                    return false;
                }
                if (0 != nmem::memcmp(&alt_val, &val, sizeof(double)))
                {
                    // Scanning result not as expected
                    return false;
                }
            }
            return true;
        }

        static bool ensure_not_parseable(const char* str)
        {
            const char* str_end              = str + ascii::strlen(str);
            const char* str_returned_end     = nullptr;
            double      val                  = 0.0;
            int         erange_condition     = 1;
            int         dconvstr_scan_status = dconvstr_scan(str, &str_returned_end, &val, &erange_condition);
            if (!dconvstr_scan_status)
            {
                // Unexpected internal error in non-parseability test
                return false;
            }
            if (str_returned_end == str_end)
            {
                // Unexpected positive result in non-parseability test
                return false;
            }
            return true;
        }

        UNITTEST_TEST(test)
        {
            CHECK_TRUE(single_static_test("%17.17f", "1.00000000000000000", 1.0, 1));
            CHECK_TRUE(single_static_test("%17.17f", "-1.00000000000000000", -1.0, 1));
            CHECK_TRUE(single_static_test("%17.17f", "10.00000000000000000", 10.0, 1));
            CHECK_TRUE(single_static_test("%17.17f", "-10.00000000000000000", -10.0, 1));
            CHECK_TRUE(single_static_test("%17.17f", "11.00000000000000000", 11.0, 1));

            CHECK_TRUE(single_static_test("%+#22.15e", "+7.894561230000000e+08", 789456123.0, 1));
            CHECK_TRUE(single_static_test("%-#22.15e", "7.894561230000000e+08 ", 789456123.0, 0));
            CHECK_TRUE(single_static_test("%#22.15e", " 7.894561230000000e+08", 789456123.0, 1));
            CHECK_TRUE(single_static_test("%#1.1g", "8.e+08", 789456123.0, 0));
            CHECK_TRUE(single_static_test("%.0f", "1", 0.6, 0));
            CHECK_TRUE(single_static_test("%2.4e", "8.6000e+00", 8.6, 1));
            CHECK_TRUE(single_static_test("%2.4g", "8.6", 8.6, 1));

            CHECK_TRUE(single_static_test("%e", "1.234000e+01", 12.34, 1));
            CHECK_TRUE(single_static_test("%e", "1.234000e-01", 0.1234, 1));
            CHECK_TRUE(single_static_test("%e", "1.234000e-03", 0.001234, 1));
            CHECK_TRUE(single_static_test("%.60e", "1.000000000000000000000000000000000000000000000000000000000000e+20", 1e20, 1));
            CHECK_TRUE(single_static_test("%e", "1.000000e-01", 0.1, 1));
            CHECK_TRUE(single_static_test("%f", "12.340000", 12.34, 1));
            CHECK_TRUE(single_static_test("%f", "0.123400", 0.1234, 1));
            CHECK_TRUE(single_static_test("%f", "0.001234", 0.001234, 1));
            CHECK_TRUE(single_static_test("%g", "12.34", 12.34, 1));
            CHECK_TRUE(single_static_test("%g", "0.1234", 0.1234, 1));
            CHECK_TRUE(single_static_test("%g", "0.001234", 0.001234, 1));
            CHECK_TRUE(single_static_test("%.60g", "100000000000000000000", 1e20, 1));
            CHECK_TRUE(single_static_test("%f", "0.000000", 0.0, 1));
            CHECK_TRUE(single_static_test("%f", "-0.000000", -0.0, 1));
            CHECK_TRUE(single_static_test("%g", "0", 0.0, 1));
            CHECK_TRUE(single_static_test("%g", "-0", -0.0, 1));

            CHECK_TRUE(single_static_test("%6.5f", "0.10000", 0.099999999860301614, 0));
            CHECK_TRUE(single_static_test("%6.5f", "0.10000", .1, 1));
            CHECK_TRUE(single_static_test("%5.4f", "0.5000", .5, 1));
            CHECK_TRUE(single_static_test("%15.5e", "   4.94066e-324", __DBL_DENORM_MIN__, 0));
            CHECK_TRUE(single_static_test("%15.5e", "   1.79769e+308", __DBL_MAX__, 0));
            CHECK_TRUE(single_static_test("%e", "1.234568e+06", 1234567.8, 0));
            CHECK_TRUE(single_static_test("%f", "1234567.800000", 1234567.8, 1));
            CHECK_TRUE(single_static_test("%g", "1.23457e+06", 1234567.8, 0));
            CHECK_TRUE(single_static_test("%g", "123.456", 123.456, 1));
            CHECK_TRUE(single_static_test("%g", "1e+06", 1000000.0, 1));
            CHECK_TRUE(single_static_test("%g", "10", 10.0, 1));
            CHECK_TRUE(single_static_test("%g", "0.02", 0.02, 1));

            CHECK_TRUE(ensure_not_parseable("."));
            CHECK_TRUE(ensure_not_parseable(".e"));
            CHECK_TRUE(ensure_not_parseable(".e+"));
            CHECK_TRUE(ensure_not_parseable(".e-"));
            CHECK_TRUE(ensure_not_parseable("1.0e"));
            CHECK_TRUE(ensure_not_parseable("1.0e+"));
            CHECK_TRUE(ensure_not_parseable("1.0e-"));
            CHECK_TRUE(ensure_not_parseable("e+100"));
            CHECK_TRUE(ensure_not_parseable("1.1."));
            CHECK_TRUE(ensure_not_parseable("1.1.e+22"));
            CHECK_TRUE(ensure_not_parseable(".1."));
            CHECK_TRUE(ensure_not_parseable(".1.e+22"));
            CHECK_TRUE(ensure_not_parseable("1.1e0+22"));
            CHECK_TRUE(ensure_not_parseable("1.1e0-22"));
            CHECK_TRUE(ensure_not_parseable(".-1"));
            CHECK_TRUE(ensure_not_parseable("1.1e+.5"));
            CHECK_TRUE(ensure_not_parseable("1.1e-.5"));
            CHECK_TRUE(ensure_not_parseable("1.1e-22-"));
            CHECK_TRUE(ensure_not_parseable("1.1e-22+"));
        }
    }
}
UNITTEST_SUITE_END
