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

    struct flags_t
    {
        void reset()
        {
            minus            = false;
            plus             = false;
            space            = false;
            zero             = false;
            hash             = false;
            precision        = 0;
            is_precision_set = 0;
            width            = 0;
            length           = 0;
            specifier        = 0;
        }

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
    };

    namespace internal
    {
        s32 str_len(const char* str)
        {
            s32 len = 0;
            while (*str++)
                len++;
            return len;
        }

        s32 wstr_len(const wchar_t* str)
        {
            s32 len = 0;
            while (*str++)
                len++;
            return len;
        }

        void str_reverse(char* _str, char* _end)
        {
            char* beg = _str;
            char* end = _end - 1;
            while (beg < end)
            {
                char tmp = *beg;
                *beg     = *end;
                *end     = tmp;
                beg++;
                end--;
            }
        }

    } // namespace internal

    // TODO:
    // - pass down destination as a [begin, cursor, end] string range
    //   - so we always have string length
    //   - so we can do bounds checking

    struct str_t
    {
        char*       str;
        char*       end;
        char const* eos;

        inline s32 length() const { return (s32)(end - str); }
    };

    struct cstr_t
    {
        char const* begin;
        char const* end;
        char const* cursor;

        inline s32 length() const { return (s32)(end - begin); }
    };

    struct cwstr_t
    {
        wchar_t const* begin;
        wchar_t const* end;
        wchar_t const* cursor;

        inline s32 length() const { return (s32)(end - begin); }
    };

    static inline s32 is_digit(char c) { return (c >= '0' && c <= '9'); }

    // helper functions
    void signed_num_to_string(s64 val, str_t& buff, s32 base);
    void unsigned_num_to_string(u64 val, str_t& buff, s32 base, bool neg=false);
    bool check_integer_specifier(char c);
    bool is_all_zeroes(str_t& buff);
    void to_upper(str_t& str);
    void prepend_ox(str_t& buff, flags_t f);
    void double_to_string(double val, str_t& buff, flags_t f);
    void remove_trailing_zeroes(str_t& buff);

    // parse format
    const char* get_flags(cstr_t const& format, flags_t& f);
    const char* get_width(cstr_t& format, flags_t& f, va_list_t& va, s32& va_idx);
    const char* get_precision(cstr_t& format, flags_t& f, va_list_t& va, s32& va_idx);
    const char* get_length(cstr_t const& format, flags_t& f);

    // parse values
    void handle_value(flags_t f, str_t& buff, va_list_t& va, s32& va_idx);
    void parse_int(flags_t f, str_t& buff, va_list_t& va, s32& va_idx);
    void parse_unsigned(flags_t f, str_t& buff, va_list_t& va, s32& va_idx);
    void parse_octal(flags_t f, str_t& buff, va_list_t& va, s32& va_idx);
    void parse_hex(flags_t f, str_t& buff, va_list_t& va, s32& va_idx);
    void parse_char(flags_t f, str_t& buff, va_list_t& va, s32& va_idx);
    void parse_string(flags_t f, str_t& buff, va_list_t& va, s32& va_idx);
    void parse_pointer(flags_t f, str_t& buff, va_list_t& va, s32& va_idx);
    void parse_float(flags_t f, str_t& buff, va_list_t& va, s32& va_idx);

    // format values
    void format_precision(str_t& buff, flags_t f);
    void format_flags(str_t& buff, flags_t f);
    void format_wchar(flags_t f, str_t& buff, wchar_t w_c);
    void format_char(flags_t f, str_t& buff, char c);
    void format_string(flags_t f, str_t& buff, cstr_t const& str);
    void format_wide_string(flags_t f, str_t& buff, cwstr_t const& wstr);

    s32 sprintf(char* str_begin, char const* str_end, const char* format_begin, const char* format_end, va_list_t& va_list)
    {
        str_t  str = {str_begin, str_begin, str_end};
        cstr_t fmt = {format_begin, format_begin, format_end};

        s32     va_idx = 0;
        flags_t flags  = {0};
        while (fmt.cursor < fmt.end)
        {
            if (*fmt.cursor != '%')
            {
                *str.end++ = *fmt.cursor++;
                continue;
            }
            else
            {
                fmt.cursor++;
                flags.reset();
            }

            fmt.cursor = get_flags(fmt, flags);
            fmt.cursor = get_width(fmt, flags, va_list, va_idx);
            fmt.cursor = get_precision(fmt, flags, va_list, va_idx);
            fmt.cursor = get_length(fmt, flags);

            flags.specifier = *fmt.cursor;
            fmt.cursor++;

            str.str = str.end;
            handle_value(flags, str, va_list, va_idx);

            if (flags.specifier == 'n')
            {
                s32* ret = va_list[va_idx++];
                *ret     = str.end - str_begin;
            }
        }

        *str.end = '\0';
        return str.end - str_begin;
    }

    const char* get_flags(cstr_t const& format, flags_t& f)
    {
        const char* cursor = format.cursor;
        while (true)
        {
            switch (*cursor)
            {
                case '0': f.zero = true; break;
                case '-': f.minus = true; break;
                case '+': f.plus = true; break;
                case ' ': f.space = true; break;
                case '#': f.hash = true; break;
                default: return cursor;
            }
            cursor++;
        }
        return cursor;
    }

    const char* get_width(cstr_t const& format, flags_t& f, va_list_t& va, s32& va_idx)
    {
        const char* cursor = format.cursor;
        if (*cursor == '*')
        {
            cursor++;
            f.width = va[va_idx];
        }
        else if (is_digit(*cursor))
        {
            s32 value = 0;
            while (is_digit(*cursor))
            {
                value *= 10;
                value += *cursor - '0';
                cursor++;
            }
            f.width = value;
        }
        return cursor;
    }

    const char* get_precision(cstr_t const& format, flags_t& f, va_list_t& va, s32& va_idx)
    {
        const char* cursor = format.cursor;
        if (*cursor == '.')
        {
            f.is_precision_set = true;
            cursor++;
        }

        if (*cursor == '*')
        {
            cursor++;
            f.precision = va[va_idx];
        }
        if (is_digit(*cursor))
        {
            s32 value = 0;
            while (is_digit(*cursor))
            {
                value *= 10;
                value += *cursor - '0';
                cursor++;
            }
            f.precision = value;
        }
        return cursor;
    }

    const char* get_length(cstr_t const& format, flags_t& f)
    {
        const char* cursor = format.cursor;
        switch (*cursor)
        {
            case 'h': f.length = 'h'; return cursor + 1;
            case 'l': f.length = 'l'; return cursor + 1;
            case 'L': f.length = 'L'; return cursor + 1;
        }
        return cursor;
    }

    void handle_value(flags_t f, str_t& buff, va_list_t& va, s32& va_idx)
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
            *buff.end++ = '%';
        else if (f.specifier == 'c')
            parse_char(f, buff, va, va_idx);
        else if (f.specifier == 's')
            parse_string(f, buff, va, va_idx);
        else if (f.specifier == 'p')
            parse_pointer(f, buff, va, va_idx);
        else if (f.specifier == 'f' || (f.specifier == 'e' || f.specifier == 'E') || (f.specifier == 'g' || f.specifier == 'G'))
            parse_float(f, buff, va, va_idx);

        if (f.specifier == 'G' || f.specifier == 'E' || f.specifier == 'X')
            to_upper(buff);
    }

    void parse_int(flags_t f, str_t& buff, va_list_t& va, s32& va_idx)
    {
        s64 val = va[va_idx];
        switch (f.length)
        {
            case 0: val = (s32)val; break;
            case 'h': val = (s16)val;
        }
        signed_num_to_string(val, buff, 10);
        format_precision(buff, f);
        format_flags(buff, f);
    }

    void signed_num_to_string(s64 val, str_t& buff, s32 base)
    {
        bool neg = val < 0 ? 1 : 0;
        val      = neg ? -val : val;
        unsigned_num_to_string((u64)val, buff, base, neg);
    }

    void unsigned_num_to_string(u64 val, str_t& buff, s32 base, bool neg)
    {
        if (val == 0)
        {
            if (neg)
                *buff.end++ = '-';
            *buff.end++ = '0';
            return;
        }

        char* iter = buff.end;
        if (base == 10)
        {
            while (val != 0)
            {
                const u64 q = val / 10;
                *iter++     = "0123456789"[val - q * 10];
                val         = q;
            }
        }
        else if (base == 16)
        {
            while (val != 0)
            {
                *iter++ = "0123456789abcdef"[val & 15];
                val >>= 4;
            }
        }
        else if (base == 8)
        {
            while (val != 0)
            {
                *iter++ = "01234567"[val & 7];
                val >>= 3;
            }
        }
        else if (base == 2)
        {
            while (val != 0)
            {
                *iter++ = (val & 1) ? '1' : '0';
                val >>= 1;
            }
        }

        if (neg)
            *iter++ = '-';

        internal::str_reverse(buff.end, iter);
        buff.end = iter;
    }

    void format_precision(str_t& buff, flags_t f)
    {
        const char sign_char = '-';
        const s32  sign      = *buff.str == '-' ? 1 : 0;
        const s32  len       = buff.length() - sign;

        const s32 fill_cnt = (f.precision > len) ? f.precision - len : 0;

        // shift string to the right
        for (s32 i = len + sign; i >= sign; i--)
        {
            buff.str[i + fill_cnt] = buff.str[i];
        }

        // add sign
        buff.str[0] = sign_char; // if sign is not set, this will be overwritten

        // fill with zeros
        for (s32 i = sign; i < sign + fill_cnt; i++)
        {
            buff.str[i] = '0';
        }

        buff.end += fill_cnt;

        if (f.is_precision_set && f.precision == 0 && check_integer_specifier(f.specifier) && buff.str[0] == '0')
        {
            // ?
            buff.str    = buff.end;
            buff.str[0] = '\0';
        }
    }

    bool check_integer_specifier(char c)
    {
        switch (c)
        {
            case 'd':
            case 'i':
            case 'o':
            case 'u':
            case 'x':
            case 'X': return true;
        }
        return false;
    }

    void format_flags(str_t& buff, flags_t f)
    {
        s32  sign_len  = 0;
        char sign_char = 0;
        if (f.plus && f.specifier != 'u')
        {
            if (buff.str[0] != '-')
            {
                sign_char = '+';
                sign_len  = 1;
            }
        }
        else if (f.space && buff.str[0] != '-' && f.specifier != 'u')
        {
            sign_char = ' ';
            sign_len  = 1;
        }

        s32 const len = buff.length();
        if (f.width > (len + sign_len))
        {
            const s32  fill_cnt  = (f.width - len);
            const char fill_char = (!f.minus) ? (f.zero ? '0' : ' ') : ' ';

            // make space for insertion (in-place)
            for (s32 i = len; i >= 0; i--)
                buff.str[i + fill_cnt + sign_len] = buff.str[i];

            // insert sign
            if (sign_len > 0)
                buff.str[0] = sign_char;

            // fill with fill_char
            for (s32 i = 0; i < fill_cnt; i++)
                buff.str[sign_len + i] = fill_char;

            buff.end += fill_cnt + sign_len;
        }
        else
        {
            for (s32 i = len; i >= 0; i--)
                buff.str[sign_len + i] = buff.str[i];

            // insert sign
            if (sign_len > 0)
                buff.str[0] = sign_char;

            buff.end += sign_len;
        }
    }

    void parse_unsigned(flags_t f, str_t& buff, va_list_t& va, s32& va_idx)
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

    void parse_octal(flags_t f, str_t& buff, va_list_t& va, s32& va_idx)
    {
        if (f.hash)
        {
            buff.str++;
            buff.end++;
        }
        *buff.end = '0';
        signed_num_to_string((s64)va[va_idx], buff, 8);
        if (f.hash)
        {
            buff.str--;
        }
        format_precision(buff, f);
        format_flags(buff, f);
    }

    void to_upper(str_t& buff)
    {
        char* cursor = buff.str;
        while (cursor < buff.end)
        {
            if (*cursor >= 'a' && *cursor <= 'z')
                *cursor = *cursor - 'a' + 'A';
            cursor++;
        }
    }

    bool is_all_zeroes(str_t& buff)
    {
        const char* cursor = buff.str;
        while (cursor < buff.end)
        {
            if (*cursor != '0')
                return false;
            cursor++;
        }
        return true;
    }

    void parse_hex(flags_t f, str_t& buff, va_list_t& va, s32& va_idx)
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

    void prepend_ox(str_t& buff, flags_t f)
    {
        if (!is_all_zeroes(buff) || f.specifier == 'p')
        {
            for (s32 i = buff.length(); i >= 0; i--)
                buff.str[i + 2] = buff.str[i];
            buff.str[0] = '0';
            buff.str[1] = 'x';
            buff.end += 2;
        }
    }

    void parse_char(flags_t f, str_t& buff, va_list_t& va, s32& va_idx)
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

    void format_wchar(flags_t f, str_t& buff, wchar_t w_c)
    {
        if (!f.minus && f.width)
        {
            for (s32 i = 0; i < f.width - 1; i++)
                *buff.end++ = ' ';
            *buff.end++ = (char)w_c;
        }
        else if (f.width)
        {
            *buff.end++ = (char)w_c;
            for (s32 i = buff.length(); i < f.width; i++)
                *buff.end++ = ' ';
        }
        else
        {
            *buff.end++ = (char)w_c;
        }
    }

    void format_char(flags_t f, str_t& buff, char c)
    {
        if (!f.minus && f.width)
        {
            for (s32 i = 0; i < (f.width - 1); i++)
                *buff.end++ = ' ';
            *buff.end++ = c;
        }
        else if (f.width)
        {
            *buff.end++ = c;
            for (s32 i = 1; i < f.width; i++)
                *buff.end++ = ' ';
        }
        else
        {
            *buff.end++ = c;
        }
    }

    void parse_string(flags_t f, str_t& buff, va_list_t& va, s32& va_idx)
    {
        if (f.length == 'l')
        {
            const wchar_t* wstr = va[va_idx];
            cwstr_t        str  = {wstr, wstr, wstr + internal::wstr_len(wstr)};
            format_wide_string(f, buff, str);
        }
        else
        {
            const char* cstr = va[va_idx];
            cstr_t      str  = {cstr, cstr, cstr + internal::str_len(cstr)};
            format_string(f, buff, str);
        }
    }

    void format_string(flags_t f, str_t& buff, cstr_t const& str)
    {
        const s32 len   = f.is_precision_set ? (f.precision > str.length() ? str.length() : f.precision) : str.length();
        const s32 shift = f.width - len;
        if (f.minus && shift > 0)
        {
            for (s32 i = 0; i < len; i++)
                *buff.end++ = str.begin[i];
            for (s32 i = 0; i < shift; i++)
                *buff.end++ = ' ';
        }
        else
        {
            for (s32 i = 0; i < shift; i++)
                *buff.end++ = ' ';
            for (s32 i = 0; i < len; i++)
                *buff.end++ = str.begin[i];
        }
    }

    void format_wide_string(flags_t f, str_t& buff, cwstr_t const& str)
    {
        const s32 len   = f.is_precision_set ? (f.precision > str.length() ? str.length() : f.precision) : str.length();
        const s32 shift = f.width - len;
        if (f.minus && shift > 0)
        {
            for (s32 i = 0; i < len; i++)
                *buff.end++ = str.begin[i];
            for (s32 i = 0; i < shift; i++)
                *buff.end++ = ' ';
        }
        else
        {
            for (s32 i = 0; i < shift; i++)
                *buff.end++ = ' ';
            for (s32 i = 0; i < len; i++)
                *buff.end++ = str.begin[i];
        }
    }

    void parse_pointer(flags_t f, str_t& buff, va_list_t& va, s32& va_idx)
    {
        unsigned_num_to_string((u64)va[va_idx], buff, 16);
        format_precision(buff, f);
        prepend_ox(buff, f);
        format_flags(buff, f);
    }

    void parse_float(flags_t f, str_t& buff, va_list_t& va, s32& va_idx)
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

    void double_to_string(double val, str_t& buff, flags_t f) {}

    void remove_trailing_zeroes(str_t& buff)
    {
        char* cursor = buff.end - 1;
        while (cursor >= buff.str)
        {
            if (*cursor != '0') // could be a dot or a non-zero character
            {
                // set the cut point at the dot or if we are pointing at a non-zero character put the cut one character after it
                char* cut = *cursor == '.' ? cursor : cursor + 1;
                // now iterate further backwards until we find a dot or the beginning of the string
                while (cursor >= buff.str)
                {
                    if (*cursor == '.') // ok, we do have a dot so we can cut the string
                    {
                        *cut     = '\0';
                        buff.end = cut; // set the new end of the string
                        break;
                    }
                    cursor--;
                }
                break;
            }
            cursor--;
        }
    }

}; // namespace ncore
