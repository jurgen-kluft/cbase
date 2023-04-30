#include "cbase/c_target.h"
#include "cbase/c_debug.h"
#include "cbase/c_double.h"
#include "cbase/c_console.h"
#include "cbase/c_limits.h"
#include "cbase/c_memory.h"
#include "cbase/c_runes.h"
#include "cbase/c_va_list.h"
#include "cbase/c_printf.h"

// define this globally (e.g. gcc -DPRINTF_INCLUDE_CONFIG_H ...) to include the
// printf_config.h header file
// default: undefined
#ifdef PRINTF_INCLUDE_CONFIG_H
#    include "printf_config.h"
#endif

// 'ntoa' conversion buffer size, this must be big enough to hold one converted
// numeric number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_NTOA_BUFFER_SIZE
#    define PRINTF_NTOA_BUFFER_SIZE 32U
#endif

// 'ftoa' conversion buffer size, this must be big enough to hold one converted
// float number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_FTOA_BUFFER_SIZE
#    define PRINTF_FTOA_BUFFER_SIZE 32U
#endif

// support for the floating point type (%f)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
#    define PRINTF_SUPPORT_FLOAT
#endif

// support for exponential floating point notation (%e/%g)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#    define PRINTF_SUPPORT_EXPONENTIAL
#endif

// define the default floating point precision
// default: 6 digits
#ifndef PRINTF_DEFAULT_FLOAT_PRECISION
#    define PRINTF_DEFAULT_FLOAT_PRECISION 6U
#endif

// define the largest float suitable to print with %f
// default: 1e9
#ifndef PRINTF_MAX_FLOAT
#    define PRINTF_MAX_FLOAT 1e9
#endif

// support for the s64 types (%llu or %p)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_LONG_LONG
#    define PRINTF_SUPPORT_LONG_LONG
#endif

// support for the ptr_t type (%t)
// ptr_t is normally defined in <stddef.h> as s32 or s64 type
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
#    define PRINTF_SUPPORT_PTRDIFF_T
#endif

namespace ncore
{
// internal flag definitions
#define FLAGS_ZEROPAD   (1U << 0U)
#define FLAGS_LEFT      (1U << 1U)
#define FLAGS_PLUS      (1U << 2U)
#define FLAGS_SPACE     (1U << 3U)
#define FLAGS_HASH      (1U << 4U)
#define FLAGS_UPPERCASE (1U << 5U)
#define FLAGS_CHAR      (1U << 6U)
#define FLAGS_SHORT     (1U << 7U)
#define FLAGS_LONG      (1U << 8U)
#define FLAGS_LONG_LONG (1U << 9U)
#define FLAGS_PRECISION (1U << 10U)
#define FLAGS_ADAPT_EXP (1U << 11U)

#define DBL_MAX D_F64_MAX

    struct va_iter_t
    {
        const va_t* args;
        s32         size;
        s32         max;
    };

    template <typename T>
    inline T va_default()
    {
        return T();
    }

#define va_arg(va, type) ((va.size < va.max) ? ((type)va.args[va.size++]) : (va_default<type>()))

    // output function type
    typedef void (*out_fct_type)(char character, void* buffer, u64 idx, u64 maxlen);

    // wrapper (used as buffer) for output function type
    struct out_fct_wrap_type
    {
        void (*fct)(char character, void* arg);
        void* arg;
    };

    // internal buffer output
    static inline void _out_buffer(char character, void* buffer, u64 idx, u64 maxlen)
    {
        if (idx < maxlen)
        {
            ((char*)buffer)[idx] = character;
        }
    }

    // internal null output
    static inline void _out_null(char character, void* buffer, u64 idx, u64 maxlen)
    {
        (void)character;
        (void)buffer;
        (void)idx;
        (void)maxlen;
    }

    void _putflush() {}

    // internal _putchar wrapper
    static inline void _out_char(char character, void* buffer, u64 idx, u64 maxlen)
    {
        (void)buffer;
        (void)idx;
        (void)maxlen;
        if (character)
        {
            // _putchar(character);
            // TODO, should output to console
        }
    }

    // internal output function wrapper
    static inline void _out_fct(char character, void* buffer, u64 idx, u64 maxlen)
    {
        (void)idx;
        (void)maxlen;
        if (character)
        {
            // buffer is the output fct pointer
            ((out_fct_wrap_type*)buffer)->fct(character, ((out_fct_wrap_type*)buffer)->arg);
        }
    }

    // internal output function wrapper for runes_writer
    static inline void _out_runeswriter(char character, void* buffer, u64 idx, u64 maxlen)
    {
        (void)idx;
        (void)maxlen;
        if (character)
        {
            ((runes_writer_t*)buffer)->write(character);
        }
    }

    // internal secure strlen
    // \return The length of the string (excluding the terminating 0) limited by 'maxsize'
    static inline u32 _strnlen_s(const char* str, u64 maxsize)
    {
        const char* s = str;
        while (*s && maxsize > 0)
        {
            ++s;
            --maxsize;
        }
        return (u32)(s - str);
    }

    // internal test if char is a digit (0-9)
    // \return true if char is a digit
    static inline bool _is_digit(char ch) { return (ch >= '0') && (ch <= '9'); }

    // internal ASCII string to u32 conversion
    static u32 _atoi(const char** str)
    {
        u32 i = 0U;
        while (_is_digit(**str))
        {
            i = i * 10U + (u32)(*((*str)++) - '0');
        }
        return i;
    }

    // output the specified string in reverse, taking care of any zero-padding
    static u64 _out_rev(out_fct_type out, char* buffer, u64 idx, u64 maxlen, const char* buf, u64 len, u32 width, u32 flags)
    {
        const u64 start_idx = idx;

        // pad spaces up to given width
        if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD))
        {
            for (u64 i = len; i < width; i++)
            {
                out(' ', buffer, idx++, maxlen);
            }
        }

        // reverse string
        while (len)
        {
            out(buf[--len], buffer, idx++, maxlen);
        }

        // append pad spaces up to given width
        if (flags & FLAGS_LEFT)
        {
            while (idx - start_idx < width)
            {
                out(' ', buffer, idx++, maxlen);
            }
        }

        return idx;
    }

    // internal itoa format
    static u64 _ntoa_format(out_fct_type out, char* buffer, u64 idx, u64 maxlen, char* buf, u64 len, bool negative, u32 base, u32 prec, u32 width, u32 flags)
    {
        // pad leading zeros
        if (!(flags & FLAGS_LEFT))
        {
            if (width && (flags & FLAGS_ZEROPAD) && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE))))
            {
                width--;
            }
            while ((len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE))
            {
                buf[len++] = '0';
            }
            while ((flags & FLAGS_ZEROPAD) && (len < width) && (len < PRINTF_NTOA_BUFFER_SIZE))
            {
                buf[len++] = '0';
            }
        }

        // handle hash
        if (flags & FLAGS_HASH)
        {
            if (!(flags & FLAGS_PRECISION) && len && ((len == prec) || (len == width)))
            {
                len--;
                if (len && (base == 16U))
                {
                    len--;
                }
            }
            if ((base == 16U) && !(flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE))
            {
                buf[len++] = 'x';
            }
            else if ((base == 16U) && (flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE))
            {
                buf[len++] = 'X';
            }
            else if ((base == 2U) && (len < PRINTF_NTOA_BUFFER_SIZE))
            {
                buf[len++] = 'b';
            }
            if (len < PRINTF_NTOA_BUFFER_SIZE)
            {
                buf[len++] = '0';
            }
        }

        if (len < PRINTF_NTOA_BUFFER_SIZE)
        {
            if (negative)
            {
                buf[len++] = '-';
            }
            else if (flags & FLAGS_PLUS)
            {
                buf[len++] = '+';  // ignore the space if the '+' exists
            }
            else if (flags & FLAGS_SPACE)
            {
                buf[len++] = ' ';
            }
        }

        return _out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
    }

    // internal itoa for 's32' type
    static u64 _ntoa_long(out_fct_type out, char* buffer, u64 idx, u64 maxlen, u32 value, bool negative, u32 base, u32 prec, u32 width, u32 flags)
    {
        char buf[PRINTF_NTOA_BUFFER_SIZE];
        u64  len = 0U;

        // no hash for 0 values
        if (!value)
        {
            flags &= ~FLAGS_HASH;
        }

        // write if precision != 0 and value is != 0
        if (!(flags & FLAGS_PRECISION) || value)
        {
            do
            {
                const char digit = (char)(value % base);
                buf[len++]       = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
                value /= base;
            } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
        }

        return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (u32)base, prec, width, flags);
    }

// internal itoa for 's64' type
#if defined(PRINTF_SUPPORT_LONG_LONG)
    static u64 _ntoa_long_long(out_fct_type out, char* buffer, u64 idx, u64 maxlen, u64 value, bool negative, u64 base, u32 prec, u32 width, u32 flags)
    {
        char buf[PRINTF_NTOA_BUFFER_SIZE];
        u64  len = 0U;

        // no hash for 0 values
        if (!value)
        {
            flags &= ~FLAGS_HASH;
        }

        // write if precision != 0 and value is != 0
        if (!(flags & FLAGS_PRECISION) || value)
        {
            do
            {
                const char digit = (char)(value % base);
                buf[len++]       = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
                value /= base;
            } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
        }

        return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (u32)base, prec, width, flags);
    }
#endif  // PRINTF_SUPPORT_LONG_LONG

#if defined(PRINTF_SUPPORT_FLOAT)

#    if defined(PRINTF_SUPPORT_EXPONENTIAL)
    // forward declaration so that _ftoa can switch to exp notation for values > PRINTF_MAX_FLOAT
    static u64 _etoa(out_fct_type out, char* buffer, u64 idx, u64 maxlen, double value, u32 prec, u32 width, u32 flags);
#    endif

    // internal ftoa for fixed decimal floating point
    static u64 _ftoa(out_fct_type out, char* buffer, u64 idx, u64 maxlen, double value, u32 prec, u32 width, u32 flags)
    {
        char   buf[PRINTF_FTOA_BUFFER_SIZE];
        u64    len  = 0U;
        double diff = 0.0;

        // powers of 10
        static const double pow10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

        // test for special values
        if (value != value)
            return _out_rev(out, buffer, idx, maxlen, "nan", 3, width, flags);
        if (value < -DBL_MAX)
            return _out_rev(out, buffer, idx, maxlen, "fni-", 4, width, flags);
        if (value > DBL_MAX)
            return _out_rev(out, buffer, idx, maxlen, (flags & FLAGS_PLUS) ? "fni+" : "fni", (flags & FLAGS_PLUS) ? 4U : 3U, width, flags);

        // test for very large values
        // standard printf behavior is to print EVERY whole number digit -- which could be 100s of characters overflowing your buffers == bad
        if ((value > PRINTF_MAX_FLOAT) || (value < -PRINTF_MAX_FLOAT))
        {
#    if defined(PRINTF_SUPPORT_EXPONENTIAL)
            return _etoa(out, buffer, idx, maxlen, value, prec, width, flags);
#    else
            return 0U;
#    endif
        }

        // test for negative
        bool negative = false;
        if (value < 0)
        {
            negative = true;
            value    = 0 - value;
        }

        // set default precision, if not set explicitly
        if (!(flags & FLAGS_PRECISION))
        {
            prec = PRINTF_DEFAULT_FLOAT_PRECISION;
        }
        // limit precision to 9, cause a prec >= 10 can lead to overflow errors
        while ((len < PRINTF_FTOA_BUFFER_SIZE) && (prec > 9U))
        {
            buf[len++] = '0';
            prec--;
        }

        int    whole = (int)value;
        double tmp   = (value - whole) * pow10[prec];
        u32    frac  = (u32)tmp;
        diff         = tmp - frac;

        if (diff > 0.5)
        {
            ++frac;
            // handle rollover, e.g. case 0.99 with prec 1 is 1.0
            if (frac >= pow10[prec])
            {
                frac = 0;
                ++whole;
            }
        }
        else if (diff < 0.5) {}
        else if ((frac == 0U) || (frac & 1U))
        {
            // if halfway, round up if odd OR if last digit is 0
            ++frac;
        }

        if (prec == 0U)
        {
            diff = value - (double)whole;
            if ((!(diff < 0.5) || (diff > 0.5)) && (whole & 1))
            {
                // exactly 0.5 and ODD, then round up
                // 1.5 -> 2, but 2.5 -> 2
                ++whole;
            }
        }
        else
        {
            u32 count = prec;
            // now do fractional part, as an unsigned number
            while (len < PRINTF_FTOA_BUFFER_SIZE)
            {
                --count;
                buf[len++] = (char)(48U + (frac % 10U));
                if (!(frac /= 10U))
                {
                    break;
                }
            }
            // add extra 0s
            while ((len < PRINTF_FTOA_BUFFER_SIZE) && (count-- > 0U))
            {
                buf[len++] = '0';
            }
            if (len < PRINTF_FTOA_BUFFER_SIZE)
            {
                // add decimal
                buf[len++] = '.';
            }
        }

        // do whole part, number is reversed
        while (len < PRINTF_FTOA_BUFFER_SIZE)
        {
            buf[len++] = (char)(48 + (whole % 10));
            if (!(whole /= 10))
            {
                break;
            }
        }

        // pad leading zeros
        if (!(flags & FLAGS_LEFT) && (flags & FLAGS_ZEROPAD))
        {
            if (width && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE))))
            {
                width--;
            }
            while ((len < width) && (len < PRINTF_FTOA_BUFFER_SIZE))
            {
                buf[len++] = '0';
            }
        }

        if (len < PRINTF_FTOA_BUFFER_SIZE)
        {
            if (negative)
            {
                buf[len++] = '-';
            }
            else if (flags & FLAGS_PLUS)
            {
                buf[len++] = '+';  // ignore the space if the '+' exists
            }
            else if (flags & FLAGS_SPACE)
            {
                buf[len++] = ' ';
            }
        }

        return _out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
    }

#    if defined(PRINTF_SUPPORT_EXPONENTIAL)
    // internal ftoa variant for exponential floating-point type, contributed by Martijn Jasperse <m.jasperse@gmail.com>
    static u64 _etoa(out_fct_type out, char* buffer, u64 idx, u64 maxlen, double value, u32 prec, u32 width, u32 flags)
    {
        // check for NaN and special values
        if ((value != value) || (value > DBL_MAX) || (value < -DBL_MAX))
        {
            return _ftoa(out, buffer, idx, maxlen, value, prec, width, flags);
        }

        // determine the sign
        const bool negative = value < 0;
        if (negative)
        {
            value = -value;
        }

        // default precision
        if (!(flags & FLAGS_PRECISION))
        {
            prec = PRINTF_DEFAULT_FLOAT_PRECISION;
        }

        // determine the decimal exponent
        // based on the algorithm by David Gay (https://www.ampl.com/netlib/fp/dtoa.c)
        union
        {
            u64    U;
            double F;
        } conv;

        conv.F   = value;
        int exp2 = (int)((conv.U >> 52U) & 0x07FFU) - 1023;             // effectively log2
        conv.U   = (conv.U & ((1ULL << 52U) - 1U)) | (1023ULL << 52U);  // drop the exponent so conv.F is now in [1,2)
        // now approximate log10 from the log2 integer part and an expansion of ln around 1.5
        int expval = (int)(0.1760912590558 + exp2 * 0.301029995663981 + (conv.F - 1.5) * 0.289529654602168);
        // now we want to compute 10^expval but we want to be sure it won't overflow
        exp2            = (int)(expval * 3.321928094887362 + 0.5);
        const double z  = expval * 2.302585092994046 - exp2 * 0.6931471805599453;
        const double z2 = z * z;
        conv.U          = (u64)(exp2 + 1023) << 52U;
        // compute exp(z) using continued fractions, see https://en.wikipedia.org/wiki/Exponential_function#Continued_fractions_for_ex
        conv.F *= 1 + 2 * z / (2 - z + (z2 / (6 + (z2 / (10 + z2 / 14)))));
        // correct for rounding errors
        if (value < conv.F)
        {
            expval--;
            conv.F /= 10;
        }

        // the exponent format is "%+03d" and largest value is "307", so set aside 4-5 characters
        u32 minwidth = ((expval < 100) && (expval > -100)) ? 4U : 5U;

        // in "%g" mode, "prec" is the number of *significant figures* not decimals
        if (flags & FLAGS_ADAPT_EXP)
        {
            // do we want to fall-back to "%f" mode?
            if ((value >= 1e-4) && (value < 1e6))
            {
                if ((int)prec > expval)
                {
                    prec = (unsigned)((int)prec - expval - 1);
                }
                else
                {
                    prec = 0;
                }
                flags |= FLAGS_PRECISION;  // make sure _ftoa respects precision
                // no characters in exponent
                minwidth = 0U;
                expval   = 0;
            }
            else
            {
                // we use one sigfig for the whole part
                if ((prec > 0) && (flags & FLAGS_PRECISION))
                {
                    --prec;
                }
            }
        }

        // will everything fit?
        u32 fwidth = width;
        if (width > minwidth)
        {
            // we didn't fall-back so subtract the characters required for the exponent
            fwidth -= minwidth;
        }
        else
        {
            // not enough characters, so go back to default sizing
            fwidth = 0U;
        }
        if ((flags & FLAGS_LEFT) && minwidth)
        {
            // if we're padding on the right, DON'T pad the floating part
            fwidth = 0U;
        }

        // rescale the float value
        if (expval)
        {
            value /= conv.F;
        }

        // output the floating part
        const u64 start_idx = idx;
        idx                 = _ftoa(out, buffer, idx, maxlen, negative ? -value : value, prec, fwidth, flags & ~FLAGS_ADAPT_EXP);

        // output the exponent part
        if (minwidth)
        {
            // output the exponential symbol
            out((flags & FLAGS_UPPERCASE) ? 'E' : 'e', buffer, idx++, maxlen);
            // output the exponent value
            idx = _ntoa_long(out, buffer, idx, maxlen, (expval < 0) ? -expval : expval, expval < 0, 10, 0, minwidth - 1, FLAGS_ZEROPAD | FLAGS_PLUS);
            // might need to right-pad spaces
            if (flags & FLAGS_LEFT)
            {
                while (idx - start_idx < width)
                    out(' ', buffer, idx++, maxlen);
            }
        }
        return idx;
    }
#    endif  // PRINTF_SUPPORT_EXPONENTIAL
#endif      // PRINTF_SUPPORT_FLOAT

    // internal vsnprintf
    static int _vsnprintf(out_fct_type out, char* buffer, const u64 maxlen, const char* format, const char* format_end, va_iter_t& va)
    {
        u32 flags, width, precision, n;
        u64          idx = 0U;

        if (!buffer)
        {
            // use null output function
            out = _out_null;
        }

        while (*format && format < format_end)
        {
            // format specifier?  %[flags][width][.precision][length]
            if (*format != '%')
            {
                // no
                out(*format, buffer, idx++, maxlen);
                format++;
                continue;
            }
            else
            {
                // yes, evaluate it
                format++;
            }

            // evaluate flags
            flags = 0U;
            do
            {
                switch (*format)
                {
                    case '0':
                        flags |= FLAGS_ZEROPAD;
                        format++;
                        n = 1U;
                        break;
                    case '-':
                        flags |= FLAGS_LEFT;
                        format++;
                        n = 1U;
                        break;
                    case '+':
                        flags |= FLAGS_PLUS;
                        format++;
                        n = 1U;
                        break;
                    case ' ':
                        flags |= FLAGS_SPACE;
                        format++;
                        n = 1U;
                        break;
                    case '#':
                        flags |= FLAGS_HASH;
                        format++;
                        n = 1U;
                        break;
                    default: n = 0U; break;
                }
            } while (n);

            // evaluate width field
            width = 0U;
            if (_is_digit(*format))
            {
                width = _atoi(&format);
            }
            else if (*format == '*')
            {
                const int w = va_arg(va, int);
                if (w < 0)
                {
                    flags |= FLAGS_LEFT;  // reverse padding
                    width = (u32)-w;
                }
                else
                {
                    width = (u32)w;
                }
                format++;
            }

            // evaluate precision field
            precision = 0U;
            if (*format == '.')
            {
                flags |= FLAGS_PRECISION;
                format++;
                if (_is_digit(*format))
                {
                    precision = _atoi(&format);
                }
                else if (*format == '*')
                {
                    const int prec = (int)va_arg(va, int);
                    precision      = prec > 0 ? (u32)prec : 0U;
                    format++;
                }
            }

            // evaluate length field
            switch (*format)
            {
                case 'l':
                    flags |= FLAGS_LONG;
                    format++;
                    if (*format == 'l')
                    {
                        flags |= FLAGS_LONG_LONG;
                        format++;
                    }
                    break;
                case 'h':
                    flags |= FLAGS_SHORT;
                    format++;
                    if (*format == 'h')
                    {
                        flags |= FLAGS_CHAR;
                        format++;
                    }
                    break;
#if defined(PRINTF_SUPPORT_PTRDIFF_T)
                case 't':
                    flags |= (sizeof(ptr_t) == sizeof(s32) ? FLAGS_LONG : FLAGS_LONG_LONG);
                    format++;
                    break;
#endif
                case 'j':
                    flags |= (sizeof(uint_t) == sizeof(s32) ? FLAGS_LONG : FLAGS_LONG_LONG);
                    format++;
                    break;
                case 'z':
                    flags |= (sizeof(u64) == sizeof(s32) ? FLAGS_LONG : FLAGS_LONG_LONG);
                    format++;
                    break;
                default: break;
            }

            // evaluate specifier
            switch (*format)
            {
                case 'd':
                case 'i':
                case 'u':
                case 'x':
                case 'X':
                case 'o':
                case 'b':
                {
                    // set the base
                    u32 base;
                    if (*format == 'x' || *format == 'X')
                    {
                        base = 16U;

                        // uppercase
                        if (*format == 'X')
                        {
                            flags |= FLAGS_UPPERCASE;
                        }
                    }
                    else if (*format == 'o')
                    {
                        base = 8U;
                    }
                    else if (*format == 'b')
                    {
                        base = 2U;
                    }
                    else
                    {
                        base = 10U;
                        flags &= ~FLAGS_HASH;  // no hash for dec format
                    }

                    // no plus or space flag for u, x, X, o, b
                    if ((*format != 'i') && (*format != 'd'))
                    {
                        flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
                    }

                    // ignore '0' flag when precision is given
                    if (flags & FLAGS_PRECISION)
                    {
                        flags &= ~FLAGS_ZEROPAD;
                    }

                    // convert the integer
                    if ((*format == 'i') || (*format == 'd'))
                    {
                        // signed
                        if (flags & FLAGS_LONG_LONG)
                        {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                            const s64 value = va_arg(va, s64);
                            idx             = _ntoa_long_long(out, buffer, idx, maxlen, (u64)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
#endif
                        }
                        else if (flags & FLAGS_LONG)
                        {
                            const s32 value = va_arg(va, s32);
                            idx             = _ntoa_long(out, buffer, idx, maxlen, (u32)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
                        }
                        else
                        {
                            const int value = (flags & FLAGS_CHAR) ? (char)va_arg(va, int) : (flags & FLAGS_SHORT) ? (short int)va_arg(va, int) : va_arg(va, int);
                            idx             = _ntoa_long(out, buffer, idx, maxlen, (u32)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
                        }
                    }
                    else
                    {
                        // unsigned
                        if (flags & FLAGS_LONG_LONG)
                        {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                            idx = _ntoa_long_long(out, buffer, idx, maxlen, va_arg(va, u64), false, base, precision, width, flags);
#endif
                        }
                        else if (flags & FLAGS_LONG)
                        {
                            idx = _ntoa_long(out, buffer, idx, maxlen, va_arg(va, u32), false, base, precision, width, flags);
                        }
                        else
                        {
                            const u32 value = (flags & FLAGS_CHAR) ? (unsigned char)va_arg(va, u32) : (flags & FLAGS_SHORT) ? (unsigned short int)va_arg(va, u32) : va_arg(va, u32);
                            idx                      = _ntoa_long(out, buffer, idx, maxlen, value, false, base, precision, width, flags);
                        }
                    }
                    format++;
                    break;
                }
#if defined(PRINTF_SUPPORT_FLOAT)
                case 'f':
                case 'F':
                    if (*format == 'F')
                        flags |= FLAGS_UPPERCASE;
                    idx = _ftoa(out, buffer, idx, maxlen, va_arg(va, double), precision, width, flags);
                    format++;
                    break;
#    if defined(PRINTF_SUPPORT_EXPONENTIAL)
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                    if ((*format == 'g') || (*format == 'G'))
                        flags |= FLAGS_ADAPT_EXP;
                    if ((*format == 'E') || (*format == 'G'))
                        flags |= FLAGS_UPPERCASE;
                    idx = _etoa(out, buffer, idx, maxlen, va_arg(va, double), precision, width, flags);
                    format++;
                    break;
#    endif  // PRINTF_SUPPORT_EXPONENTIAL
#endif      // PRINTF_SUPPORT_FLOAT
                case 'c':
                {
                    u32 l = 1U;
                    // pre padding
                    if (!(flags & FLAGS_LEFT))
                    {
                        while (l++ < width)
                        {
                            out(' ', buffer, idx++, maxlen);
                        }
                    }
                    // char output
                    out((char)va_arg(va, int), buffer, idx++, maxlen);
                    // post padding
                    if (flags & FLAGS_LEFT)
                    {
                        while (l++ < width)
                        {
                            out(' ', buffer, idx++, maxlen);
                        }
                    }
                    format++;
                    break;
                }

                case 's':
                {
                    const char*  p = va_arg(va, const char*);
                    u32 l = _strnlen_s(p, precision ? precision : (u64)-1);
                    // pre padding
                    if (flags & FLAGS_PRECISION)
                    {
                        l = (l < precision ? l : precision);
                    }
                    if (!(flags & FLAGS_LEFT))
                    {
                        while (l++ < width)
                        {
                            out(' ', buffer, idx++, maxlen);
                        }
                    }
                    // string output
                    while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision--))
                    {
                        out(*(p++), buffer, idx++, maxlen);
                    }
                    // post padding
                    if (flags & FLAGS_LEFT)
                    {
                        while (l++ < width)
                        {
                            out(' ', buffer, idx++, maxlen);
                        }
                    }
                    format++;
                    break;
                }

                case 'p':
                {
                    width = sizeof(void*) * 2U;
                    flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
#if defined(PRINTF_SUPPORT_LONG_LONG)
                    const bool is_ll = sizeof(ptr_t) == sizeof(s64);
                    if (is_ll)
                    {
                        idx = _ntoa_long_long(out, buffer, idx, maxlen, (ptr_t)va_arg(va, void*), false, 16U, precision, width, flags);
                    }
                    else
                    {
#endif
                        idx = _ntoa_long(out, buffer, idx, maxlen, (u32)((ptr_t)va_arg(va, void*)), false, 16U, precision, width, flags);
#if defined(PRINTF_SUPPORT_LONG_LONG)
                    }
#endif
                    format++;
                    break;
                }

                case '%':
                    out('%', buffer, idx++, maxlen);
                    format++;
                    break;

                default:
                    out(*format, buffer, idx++, maxlen);
                    format++;
                    break;
            }
        }

        // termination
        out((char)0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);

        // return written chars without terminating \0
        return (int)idx;
    }

    ///////////////////////////////////////////////////////////////////////////////
    int vsnprintf_(char* buffer, const char* buffer_end, const char* format, const char* format_end, va_iter_t& va)
    {
        const u64 maxlen = buffer_end - buffer;
        const int ret    = _vsnprintf(_out_buffer, buffer, maxlen, format, format_end, va);
        _putflush();
        return ret;
    }

    int printf_(const char* format, const char* format_end, const va_t* argv, s32 argc)
    {
        va_iter_t va_iter = {argv, 0, argc};
        char      buffer[1];
        const int ret = _vsnprintf(_out_char, buffer, (u64)-1, format, format_end, va_iter);
        _putflush();
        return ret;
    }

    int sprintf_(char* buffer, const char* buffer_end, const char* format, const char* format_end, const va_t* argv, s32 argc)
    {
        va_iter_t va_iter = {argv, 0, argc};
        const u64 maxlen  = buffer_end - buffer;
        const int ret     = _vsnprintf(_out_buffer, buffer, maxlen, format, format_end, va_iter);
        _putflush();
        return ret;
    }

    int snprintf_(char* buffer, const char* buffer_end, const char* format, const char* format_end, const va_t* argv, s32 argc)
    {
        va_iter_t va_iter = {argv, 0, argc};
        const u64 maxlen  = buffer_end - buffer;
        const int ret     = _vsnprintf(_out_buffer, buffer, maxlen, format_end, format, va_iter);
        _putflush();
        return ret;
    }

    int vprintf_(const char* format, const char* format_end, const va_t* argv, s32 argc)
    {
        va_iter_t va_iter = {argv, 0, argc};
        char      buffer[1];
        const int ret = _vsnprintf(_out_char, buffer, (u64)-1, format, format_end, va_iter);
        _putflush();
        return ret;
    }

    int fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, const char* format_end, const va_t* argv, s32 argc)
    {
        va_iter_t               va_iter      = {argv, 0, argc};
        const out_fct_wrap_type out_fct_wrap = {out, arg};
        const int               ret          = _vsnprintf(_out_fct, (char*)(ptr_t)&out_fct_wrap, (u64)-1, format, format_end, va_iter);
        _putflush();
        return ret;
    }

    int vzprintf(irunes_writer_t& writer, const crunes_t& format, const va_t* argv, s32 argc)
    {
        va_iter_t   va_iter      = {argv, 0, argc};
        const char* format_begin = &format.m_ascii.m_bos[format.m_ascii.m_str];
        const char* format_end   = &format.m_ascii.m_bos[format.m_ascii.m_end];
        const int   ret          = _vsnprintf(_out_runeswriter, (char*)(ptr_t)&writer, (u64)-1, format_begin, format_end, va_iter);
        writer.flush();
        return ret;
    }

};  // namespace ncore
