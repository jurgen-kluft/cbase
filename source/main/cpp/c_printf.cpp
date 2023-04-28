#include "cbase/c_target.h"
#include "cbase/c_debug.h"
#include "cbase/c_double.h"
#include "cbase/c_console.h"
#include "cbase/c_memory.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"

namespace ncore
{
#define BUFF_SIZE 512

    typedef struct
    {
        bool minus;
        bool plus;
        bool space;
        bool zero;
        bool hash;
        s32  precision;
        s32  is_precision_set;
        s32  width;
        char length;
        char specifier;
    } flags;

    void s21_strcpy(char* dest, const char* src)
    {
        while (*src)
            *dest++ = *src++;
        *dest = '\0';
    }

    void s21_strcat(char* dest, const char* src)
    {
        while (*dest)
            dest++;
        while (*src)
            *dest++ = *src++;
        *dest = '\0';
    }

    char* s21_strchr(char* str, char character)
    {
        while (*str != character)
        {
            if (!*str++)
                return 0;
        }
        return (char*)str;
    }

    s32 wcstombs(char* dest, const wchar_t* src, s32 n)
    {
        s32 count = 0;
        while (n--)
        {
            if ((*dest++ = (char)*src++) == '\0')
                break;
            count++;
        }
        return count;
    }

    // TODO:
    // - pass down destination as a [begin, cursor, end] string range
    //   - so we always have string length
    //   - so we can do bounds checking

    struct str_t
    {
        char*       begin;
        char*       cursor;
        char const* end;

        inline s32 length() const { return (s32)(cursor - begin); }
    };

    s32 s21_sprintf(char* str, char const* str_end, const char* format, const char* format_end, ...);

    // helper functions
    void              whole_num_to_string(s64 val, char* ret, s32 base);
    void              unsigned_num_to_string(u64 val, char* ret, s32 base);
    bool              check_integer_specifier(char c);
    bool              is_all_zeroes(char* buff);
    void              to_upper(char* str);
    void              prepend_ox(char* buff, flags f);
    void              double_to_string(double val, char* ret, flags f);
    void              prepend_mantiss(char* str, s32 pow, char sign);
    void              remove_trailing_zeroes(char* buff);
    static inline s32 is_digit(char c) { return (c >= '0' && c <= '9'); }

    // parse format
    const char* get_flags(const char* format, flags* f);
    const char* get_width(const char* format, flags* f, va_list_t& va, s32& va_idx);
    const char* get_precision(const char* format, flags* f, va_list_t& va, s32& va_idx);
    const char* get_length(const char* format, flags* f);

    // parse values
    void handle_value(flags f, char* buff, va_list_t& va, s32& va_idx);
    void parse_int(flags, char* buff, va_list_t& va, s32& va_idx);
    void parse_unsigned(flags f, char* buff, va_list_t& va, s32& va_idx);
    void parse_octal(flags f, char* buff, va_list_t& va, s32& va_idx);
    void parse_hex(flags f, char* buff, va_list_t& va, s32& va_idx);
    void parse_char(flags f, char* buff, va_list_t& va, s32& va_idx);
    void parse_string(flags f, char* buff, va_list_t& va, s32& va_idx);
    void parse_pointer(flags f, char* buff, va_list_t& va, s32& va_idx);
    void parse_float(flags f, char* buff, va_list_t& va, s32& va_idx);
    void parse_mantiss(flags f, char* buff, va_list_t& va, s32& va_idx);
    void parse_float_g_G(flags f, char* buff, va_list_t& va, s32& va_idx);

    // format values
    void format_precision(char* buff, flags f);
    void format_flags(char* buff, flags f);
    void format_wchar(flags f, char* buff, wchar_t w_c);
    void format_char(flags f, char* buff, char c);
    void format_string(flags f, char* buff, char* str);
    void format_wide_string(flags f, char* buff, wchar_t* wstr);

    s32 s21_sprintf(char* str, char const* str_end, const char* format, const char* format_end, va_list_t& va_list)
    {
        s32   va_idx        = 0;
        flags f             = {0};
        char* str_beginning = str;
        while (*format)
        {
            if (*format != '%')
            {
                *str++ = *format++;
                continue;
            }
            else
            {
                format++;
                nmem::memset(&f, 0, sizeof(flags));
            }

            format = get_flags(format, &f);
            format = get_width(format, &f, va_list, va_idx);
            format = get_precision(format, &f, va_list, va_idx);
            format = get_length(format, &f);

            f.specifier = *format;
            format++;

            char buff[BUFF_SIZE] = {'\0'};
            handle_value(f, buff, va_list, va_idx);
            for (s32 i = 0; buff[i]; i++, str++)
                *str = buff[i];

            if (f.specifier == 'n')
            {
                s32* ret = va_list[va_idx++];
                *ret     = str - str_beginning;
            }
        }

        *str = '\0';

        return str - str_beginning;
    }

    const char* get_flags(const char* format, flags* f)
    {
        while (*format == '-' || *format == '+' || *format == ' ' || *format == '0' || *format == '#')
        {
            switch (*format)
            {
                case '0': f->zero = true; break;
                case '-': f->minus = true; break;
                case '+': f->plus = true; break;
                case ' ': f->space = true; break;
                case '#': f->hash = true; break;
            }
            format++;
        }
        return format;
    }

    const char* get_width(const char* format, flags* f, va_list_t& va, s32& va_idx)
    {
        if (*format == '*')
        {
            format++;
            f->width = va[va_idx];
        }
        else if (is_digit(*format))
        {
            s32 value = 0;
            while (is_digit(*format))
            {
                value *= 10;
                value += *format++ - '0';
            }
            f->width = value;
        }
        return format;
    }

    const char* get_precision(const char* format, flags* f, va_list_t& va, s32& va_idx)
    {
        if (*format == '.')
        {
            f->is_precision_set = true;
            format++;
        }

        if (*format == '*')
        {
            format++;
            f->precision = va[va_idx];
        }
        if (is_digit(*format))
        {
            s32 value = 0;
            while (is_digit(*format))
            {
                value *= 10;
                value += *format++ - '0';
            }
            f->precision = value;
        }
        return format;
    }

    const char* get_length(const char* format, flags* f)
    {
        switch (*format)
        {
            case 'h':
                f->length = 'h';
                format++;
                break;
            case 'l':
                f->length = 'l';
                format++;
                break;
            case 'L': f->length = 'L'; format++;
        }
        return format;
    }

    void handle_value(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        if (f.specifier == 'd' || f.specifier == 'i')
            parse_int(f, buff, va, va_idx);
        else if (f.specifier == 'u')
            parse_unsigned(f, buff, va, va_idx);
        else if (f.specifier == 'o')
            parse_octal(f, buff, va, va_idx);
        else if (f.specifier == 'x' || f.specifier == 'X')
            parse_hex(f, buff, va, va_idx);
        else if (f.specifier == '%')
            buff[0] = '%';
        else if (f.specifier == 'c')
            parse_char(f, buff, va, va_idx);
        else if (f.specifier == 's')
            parse_string(f, buff, va, va_idx);
        else if (f.specifier == 'p')
            parse_pointer(f, buff, va, va_idx);
        else if (f.specifier == 'f')
            parse_float(f, buff, va, va_idx);
        else if (f.specifier == 'e' || f.specifier == 'E')
            parse_mantiss(f, buff, va, va_idx);
        else if (f.specifier == 'g' || f.specifier == 'G')
            parse_float_g_G(f, buff, va, va_idx);
        if (f.specifier == 'G' || f.specifier == 'E' || f.specifier == 'X')
            to_upper(buff);
    }

    void parse_int(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        s64 val = va[va_idx];

        switch (f.length)
        {
            case 0: val = (s32)val; break;
            case 'h': val = (s16)val;
        }
        whole_num_to_string(val, buff, 10);
        format_precision(buff, f);
        format_flags(buff, f);
    }

    void whole_num_to_string(s64 val, char* ret, s32 base)
    {
        char tmp[24] = {'\0'};
        s32  idx     = 24 - 2;

        bool neg = val < 0 ? 1 : 0;
        val      = neg ? -val : val;

        if (val == 0)
            tmp[idx] = '0';

        while (val > 0)
        {
            idx--;
            tmp[idx] = "0123456789abcdef"[val % base];
            val /= base;
        }
        for (s32 j = 0; tmp[idx]; idx++, j++)
        {
            if (neg && j == 0)
            {
                ret[j++] = '-';
            }

            ret[j] = tmp[idx];
        }
    }

    void format_precision(char* buff, flags f)
    {
        char tmp[BUFF_SIZE] = {'\0'};

        s32 sign = 0;
        s32 len  = ascii::strlen(buff);

        if (buff[0] == '-')
        {
            tmp[0] = '-';
            len--;
            sign = 1;
        }

        if (f.precision > len)
        {
            s32 idx;
            for (idx = sign; idx < f.precision - len + sign; idx++)
                tmp[idx] = '0';

            for (s32 i = sign; buff[i]; i++, idx++)
                tmp[idx] = buff[i];

            s21_strcpy(buff, tmp);
        }

        if (f.is_precision_set && f.precision == 0 && check_integer_specifier(f.specifier) && buff[0] == '0')
            buff[0] = '\0';
    }

    bool check_integer_specifier(char c)
    {
        char specs[] = {'d', 'i', 'o', 'u', 'x', 'X'};
        bool res     = false;
        for (s32 i = 0; i < sizeof(specs); i++)
        {
            if (specs[i] == c)
            {
                res = true;
                break;
            }
        }
        return res;
    }

    void format_flags(char* buff, flags f)
    {
        s32  sign_len  = 0;
        char sign_char = 0;
        if (f.plus && f.specifier != 'u')
        {
            if (buff[0] != '-')
            {
                sign_char = '+';
                sign_len  = 1;
            }
        }
        else if (f.space && buff[0] != '-' && f.specifier != 'u')
        {
            sign_char = ' ';
            sign_len  = 1;
        }

        s32 const len = ascii::strlen(buff);
        if (f.width > len)
        {
            const s32  fill_cnt  = (f.width - len);
            const char fill_char = (!f.minus) ? (f.zero ? '0' : ' ') : ' ';

            // make space for insertion (in-place)
            for (s32 i = len; i >= 0; i--)
                buff[i + fill_cnt + sign_len] = buff[i];

            // insert sign
            if (sign_len > 0)
                buff[fill_cnt] = sign_char;

            // fill with fill_char
            for (s32 i = 0; i < fill_cnt; i++)
                buff[i] = fill_char;
        }
    }

    void unsigned_num_to_string(u64 val, char* ret, s32 base)
    {
        char buf[32 + 1] = {'\0'};
        s32  idx         = BUFF_SIZE - 1;
        if (val == 0)
        {
            buf[idx] = '0';
            idx--;
        }

        // TODO we could do this in-place instead of using a temporary buffer

        for (; val && idx; --idx, val /= base)
            buf[idx] = "0123456789abcdef"[val % base];
        for (s32 j = 0; buf[idx + 1]; idx++, j++)
            ret[j] = buf[idx + 1];
    }

    void parse_unsigned(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        u64 val = va[va_idx];
        switch (f.length)
        {
            case 'h': val = (u16)val; break;
            case 'l': val = (u64)val; break;
            case 0: val = (u32)val;
        }
        unsigned_num_to_string(val, buff, 10);
        format_precision(buff, f);
        format_flags(buff, f);
    }

    void parse_octal(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        buff[0] = '0';
        whole_num_to_string((s64)va[va_idx], buff + f.hash, 8);
        format_precision(buff, f);
        format_flags(buff, f);
    }

    void to_upper(char* str)
    {
        while (*str)
        {
            if (*str >= 'a' && *str <= 'z')
                *str = *str - 'a' + 'A';
            str++;
        }
    }

    bool is_all_zeroes(char* buff)
    {
        for (s32 i = 0; buff[i]; i++)
            if (buff[i] != '0')
                return false;
        return true;
    }

    void parse_hex(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        u64 val = va[va_idx];
        switch (f.length)
        {
            case 0: val = (u32)val; break;
            case 'h': val = (u16)val; break;
            case 'l': val = (u64)val; break;
        }
        unsigned_num_to_string(val, buff, 16);
        format_precision(buff, f);
        if (f.hash)
        {
            prepend_ox(buff, f);
        }
        format_flags(buff, f);
    }

    void prepend_ox(char* buff, flags f)
    {
        if (!is_all_zeroes(buff) || f.specifier == 'p')
        {
            nmem::memmove(buff + 2, buff, ascii::strlen(buff));
            buff[0] = '0';
            buff[1] = 'x';
        }
    }

    void parse_char(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        if (f.length == 'l')
        {
            wchar_t w_c = (s32)va[va_idx];
            format_wchar(f, buff, w_c);
        }
        else
        {
            char c = (s32)va[va_idx];
            format_char(f, buff, c);
        }
    }

    void format_wchar(flags f, char* buff, wchar_t w_c)
    {
        if (!f.minus && f.width)
        {
            char tmp[BUFF_SIZE] = {'\0'};
            wcstombs(tmp, &w_c, BUFF_SIZE);
            for (s32 i = 0; i < f.width - ascii::strlen(tmp); i++)
                buff[i] = ' ';
            s21_strcat(buff, tmp);
        }
        else if (f.width)
        {
            wcstombs(buff, &w_c, BUFF_SIZE);
            for (s32 i = ascii::strlen(buff); i < f.width; i++)
                buff[i] = ' ';
        }
        else
        {
            wcstombs(buff, &w_c, BUFF_SIZE);
        }
    }
    void format_char(flags f, char* buff, char c)
    {
        if (!f.minus && f.width)
        {
            for (s32 i = 0; i < f.width; i++)
            {
                buff[i] = ' ';
                if (i == f.width - 1)
                    buff[i] = c;
            }
        }
        else if (f.width)
        {
            buff[0] = c;
            for (s32 i = 1; i < f.width; i++)
                buff[i] = ' ';
        }
        else
        {
            buff[0] = c;
        }
    }

    void parse_string(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        if (f.length == 'l')
        {
            const wchar_t* wstr = va[va_idx];
            format_wide_string(f, buff, wstr);
        }
        else
        {
            const char* str = va[va_idx];
            format_string(f, buff, str);
        }
    }
    void format_string(flags f, char* buff, const char* str)
    {
        char tmp[BUFF_SIZE] = {'\0'};
        s21_strcpy(tmp, str);
        if (f.is_precision_set)
            tmp[f.precision] = '\0';

        s32 shift = f.width - ascii::strlen(tmp);
        s32 len   = ascii::strlen(tmp);

        if (f.minus && shift > 0)
        {
            s21_strcpy(buff, tmp);
            nmem::memset(buff + len, ' ', shift);
        }
        else if (shift > 0)
        {
            nmem::memset(buff, ' ', shift);
            s21_strcpy(buff + shift, tmp);
        }
        else
        {
            s21_strcpy(buff, tmp);
        }
    }
    void format_wide_string(flags f, char* buff, const wchar_t* wstr)
    {
        char tmp[BUFF_SIZE] = {'\0'};
        char str[BUFF_SIZE] = {'\0'};

        wcstombs(str, wstr, BUFF_SIZE);
        s21_strcpy(tmp, str);
        if (f.is_precision_set)
            tmp[f.precision] = '\0';

        s32 shift = f.width - ascii::strlen(tmp);
        s32 len   = ascii::strlen(tmp);

        if (f.minus && shift > 0)
        {
            s21_strcpy(buff, tmp);
            nmem::memset(buff + len, ' ', shift);
        }
        else if (shift > 0)
        {
            nmem::memset(buff, ' ', shift);
            s21_strcpy(buff + shift, tmp);
        }
        else
        {
            s21_strcpy(buff, tmp);
        }
    }

    void parse_pointer(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        unsigned_num_to_string((u64)va[va_idx], buff, 16);
        format_precision(buff, f);
        prepend_ox(buff, f);
        format_flags(buff, f);
    }

    void parse_float(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        double val = 0;
        if (f.length == 'L')
        {
            val = (double)va[va_idx];
        }
        else
        {
            val = (float)va[va_idx];
        }

        if (!f.is_precision_set)
        {
            f.precision = 6;
        }

        double_to_string(val, buff, f);
        format_flags(buff, f);
    }

    void double_to_string(double val, char* ret, flags f) {}

    void remove_trailing_zeroes(char* buff)
    {
        s32   len = ascii::strlen(buff);
        char* dot = s21_strchr(buff, '.');
        if (dot)
        {
            for (s32 i = len - 1; buff[i] != '.'; i--)
            {
                if (buff[i] == '0')
                    buff[i] = '\0';
                else
                    break;
            }
            if (dot[1] == '\0')
                dot[0] = '\0';
        }
    }

    void parse_mantiss(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        double val = 0;
        if (f.length == 'L')
        {
            val = double(va[va_idx]);
        }
        else
        {
            val = (float)(va[va_idx]);
        }

        int  pow  = 0;
        char sign = (int)val == 0 ? '-' : '+';

        if ((int)val - val)
        {
            while ((int)val == 0)
            {
                pow++;
                val *= 10;
            }
        }
        else
        {
            sign = '+';
        }
        while ((int)val / 10 != 0)
        {
            pow++;
            val /= 10;
        }

        if (!f.is_precision_set)
            f.precision = 6;

        double_to_string(val, buff, f);
        prepend_mantiss(buff, pow, sign);
        format_flags(buff, f);
    }

    void prepend_mantiss(char* str, int pow, char sign)
    {
        int len      = ascii::strlen(str);
        str[len]     = 'e';
        str[len + 1] = sign;
        str[len + 3] = pow % 10 + '0';
        pow /= 10;
        str[len + 2] = pow % 10 + '0';
        str[len + 4] = '\0';
    }

    void parse_float_g_G(flags f, char* buff, va_list_t& va, s32& va_idx)
    {
        double val = 0;
        if (f.length == 'L')
        {
            val = (double)va[va_idx];
        }
        else
        {
            val = (float)va[va_idx];
        }

        if (!f.is_precision_set)
        {
            f.precision = 6;
        }

        if (f.precision == 0)
            f.precision = 1;

        int    precision = f.precision;
        double m_val     = val;
        int    pow       = 0;
        if ((int)val - val)
        {
            while ((int)m_val == 0)
            {
                pow++;
                m_val *= 10;
            }
        }
        if (pow > 4)
        {
            f.precision = 0;
            double_to_string(m_val, buff, f);
        }
        else
        {
            f.precision = 10;
            double_to_string(val, buff, f);
        }

        format_gG_precision(buff, precision);

        if (pow > 4)
            prepend_mantiss(buff, pow, '-');

        remove_trailing_zeroes(buff);
        format_flags(buff, f);
    }

    void format_gG_precision(char* buff, s32 precision)
    {
        s32 sig_digs       = 0;
        s32 len            = ascii::strlen(buff);
        int not_zero_found = 0;
        for (s32 i = 0; i < ascii::strlen(buff); i++)
        {
            if ((buff[i] == '0' && !not_zero_found) || buff[i] == '.')
                continue;
            else
                not_zero_found = 1;

            if (is_digit(buff[i]) && not_zero_found)
            {
                sig_digs++;
            }
            if (sig_digs == precision && i + 1 < len)
            {
                int next    = buff[i + 1] == '.' ? 2 : 1;
                buff[i]     = buff[i + next] - '0' > 5 ? (char)(buff[i] + 1) : buff[i];
                buff[i + 1] = '\0';
                break;
            }
        }
    }

}; // namespace ncore
