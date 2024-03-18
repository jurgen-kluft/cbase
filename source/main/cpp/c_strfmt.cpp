// https://github.com/hparracho/usflib/tree/master

#include "cbase/c_float.h"
#include "cbase/c_double.h"
#include "cbase/c_dconv.h"
#include "cbase/c_integer.h"
#include "cbase/c_strfmt.h"
#include "cbase/c_runes.h"

namespace ncore
{
    namespace fmt
    {
#define USF_CONTRACT_VIOLATION(except) static_cast<void>(0)
#define USF_FALLTHROUGH                /*fall through*/
#define FMT_CHECK(cond, except)        ((!!(cond)) ? static_cast<void>(0) : USF_CONTRACT_VIOLATION(except))

        constexpr char digits_hex_uppercase[]{"0123456789ABCDEF"};
        constexpr char digits_hex_lowercase[]{"0123456789abcdef"};

        class cstrspan_t
        {
            friend class strspan_t;

            enum
            {
                Type_Ascii,
                Type_Utf8,
                Type_Utf16,
                Type_Utf32
            } m_type;

        public:
            cstrspan_t() noexcept                  = delete;
            cstrspan_t(const cstrspan_t&) noexcept = default;
            cstrspan_t(cstrspan_t&&) noexcept      = default;
            cstrspan_t(const char* str) noexcept
                : m_type(Type_Ascii)
                , m_begin{str}
                , m_end{str}
            {
                m_end += compute_length();
                FMT_CHECK(m_begin <= m_end, std::runtime_error);
            }
            cstrspan_t(const char* str, const uint_t length)
                : m_type(Type_Ascii)
                , m_begin{str}
                , m_end{str + length}
            {
                FMT_CHECK(length >= 0, std::runtime_error);
            }
            cstrspan_t(const char* first, const char* last)
                : m_type(Type_Ascii)
                , m_begin{first}
                , m_end{last}
            {
                if (m_end == nullptr)
                {
                    m_end = m_begin + compute_length();
                }
                FMT_CHECK(m_begin <= m_end, std::runtime_error);
            }

            // -------- ASSIGNMENT ------------------------------------------------

            cstrspan_t& operator=(const cstrspan_t&) noexcept = default;
            cstrspan_t& operator=(cstrspan_t&&) noexcept      = default;

            inline uchar32 operator*() const noexcept { return peek(); }

            uint_t length() const noexcept { return m_end - m_begin; }
            bool   at_end() const noexcept { return m_begin >= m_end; }

            uchar32 peek(s32 n = 0) const noexcept;
            uchar32 read() noexcept;
            void    skip(s32 n = 1) noexcept;

        protected:
            uint_t compute_length() const noexcept
            {
                // this is different for each character encoding
                const char* iter = m_begin;
                while (*iter)
                {
                    ++iter;
                }
                return iter - m_begin;
            }

            // --------------------------------------------------------------------
            // PRIVATE MEMBER VARIABLES
            // --------------------------------------------------------------------
            const char* m_begin;
            const char* m_end;
        };

        uchar32 cstrspan_t::peek(s32 n) const noexcept
        {
            if (m_type == Type_Ascii)
            {
                if ((m_begin + n) < m_end)
                {
                    return static_cast<uchar32>(*(m_begin + n));
                }
            }
            return '\0';
        }

        uchar32 cstrspan_t::read() noexcept
        {
            if (m_type == Type_Ascii)
            {
                if (m_begin < m_end)
                {
                    return *m_begin++;
                }
            }
            return '\0';
        }

        void cstrspan_t::skip(s32 n) noexcept
        {
            if (m_type == Type_Ascii)
            {
                if ((m_begin + n) < m_end)
                {
                    m_begin += n;
                }
                else
                {
                    m_begin = m_end;
                }
            }
        }

        strspan_t::strspan_t(char* first, char* last)
            : m_type(Type_Ascii)
            , m_begin{first}
            , m_end{last}
        {
        }

        void strspan_t::skip(const uint_t n)
        {
            if (m_type == Type_Ascii)
            {
                m_begin += n;
            }
        }

        void strspan_t::write(uchar32 c)
        {
            if (m_type == Type_Ascii)
            {
                if (c < 0x80)
                {
                    *m_begin++ = static_cast<char>(c);
                }
                else
                {
                    *m_begin++ = '?';
                }
            }
        }

        void strspan_t::write(cstrspan_t const& str)
        {
            if (m_type == Type_Ascii)
            {
                if (str.m_type == cstrspan_t::Type_Ascii)
                {
                    const char* src = static_cast<const char*>(str.m_begin);
                    while (src != static_cast<const char*>(str.m_end))
                    {
                        *m_begin++ = *src++;
                    }
                }
            }
        }

        class CharTraits
        {
        public:
            // --------------------------------------------------------------------
            // PUBLIC STATIC FUNCTIONS
            // --------------------------------------------------------------------
            inline static void assign(char* dst, char ch, ptr_t count) noexcept
            {
                while ((count--) > 0)
                {
                    *dst++ = (static_cast<char>(ch));
                }
            }

            inline static void assign(strspan_t& dst, char ch, ptr_t count) noexcept
            {
                while ((count--) > 0)
                {
                    dst.write(static_cast<char>(ch));
                }
            }

            inline static void copy(strspan_t& dst, const char* src, ptr_t count) noexcept
            {
                while ((count--) > 0)
                {
                    dst.write(static_cast<char>(*src++));
                }
            }

            inline static void copy(strspan_t& dst, const cstrspan_t& str, ptr_t count) noexcept
            {
                cstrspan_t src(str);
                while ((count--) > 0)
                {
                    uchar32 c = src.read();
                    dst.write(c);
                }
            }

            inline static ptr_t length(const char* str) noexcept
            {
                const char* str_begin = str;
                while (*str != char{})
                {
                    ++str;
                }
                return str - str_begin;
            }
        };

        constexpr u32 pow10_uint32_lut[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

        constexpr u64 pow10_uint64_lut[] = {1,           10,           100,           1000,           10000,           100000,           1000000,           10000000,           100000000,           1000000000,
                                            10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000, 1000000000000000, 10000000000000000, 100000000000000000, 1000000000000000000, 10000000000000000000U};

        class Integer
        {
        public:
            enum  // 64 bit
            {
                kMaxDigitsDec     = 20 + 1,
                kMaxDigitsHex     = 16 + 1,
                kMaxDigitsOct     = 22 + 1,
                kMaxDigitsBin     = 64 + 1,
                kMaxDigitsPointer = kMaxDigitsHex,
            };

            // -------- POWERS OF 10 ----------------------------------------------
            static u32 pow10_uint32(const s32 index) noexcept
            {
                ASSERT(index >= 0 && index < 10);
                return pow10_uint32_lut[index];
            }

            static u64 pow10_uint64(const s32 index) noexcept
            {
                ASSERT(index >= 0 && index < 20);
                return pow10_uint64_lut[index];
            }

            // -------- COUNT DIGITS ----------------------------------------------
            static s32 count_digits_dec(const u64 n) noexcept
            {
                if (n < 10)
                    return 1;

                const s32 length = sizeof(pow10_uint64_lut) / sizeof(pow10_uint64_lut[0]);
                s32       index  = 0;
                s32       left   = 0;
                s32       right  = length - 1;
                while (left <= right)
                {
                    index = (left + right) / 2;
                    if (n < pow10_uint64_lut[index])
                    {
                        ASSERT(index > 0);
                        if (n >= pow10_uint64_lut[index - 1])
                        {
                            return index;
                        }
                        right = index - 1;
                    }
                    else
                    {
                        left = index + 1;
                    }
                }
                return index + 1;
            }

            static s32 count_digits_dec(const u32 n) noexcept { return count_digits_dec((u64)n); }

            static s32 count_digits_bin(u64 n) noexcept
            {
                s8 c = 0;
                if (n > 0xFFFFFFFFUL)
                {
                    n >>= 32;
                    c += 32;
                }
                while (n > 0xFF)
                {
                    n >>= 8;
                    c += 8;
                }
                if (n > 0xF)
                {
                    n >>= 4;
                    c += 4;
                }
                if (n > 0x3)
                {
                    n >>= 2;
                    c += 2;
                }
                if (n > 0x1)
                {
                    n >>= 1;
                    c += 1;
                }
                c += (s8)(n);
                return c == 0 ? 1 : c;
            }

            static s32 count_digits_bin(const u32 n) noexcept { return count_digits_bin((u64)n); }

            static s32 count_digits_oct(u64 n) noexcept
            {
                s8 c = 0;
                while (n > 0xFFF)
                {
                    c += 4;
                    n >>= 12;
                }
                while (n != 0)
                {
                    ++c;
                    n >>= 3;
                }
                return c == 0 ? 1 : c;
            }

            static s32 count_digits_oct(u32 n) noexcept { return count_digits_oct((u64)n); }

            static inline s32 count_digits_hex(u64 n) noexcept
            {
                s8 c = 0;
                while (n > 0xFFFF)
                {
                    n >>= 16;
                    c += 4;
                }
                if (n > 0xFF)
                {
                    n >>= 8;
                    c += 2;
                }
                if (n > 0xF)
                {
                    n >>= 4;
                    c += 1;
                }
                c += (n > 0 ? 1 : 0);
                return c == 0 ? 1 : c;
            }

            static inline s32 count_digits_hex(u32 n) noexcept { return count_digits_hex((u64)n); }

            // -------- DIVIDE BY 10 WITH REMAINDER-------------------------------
            static u32 moddiv10(const u32 n, s8& mod) noexcept
            {
                const u32 div = (n / 10);
                mod           = static_cast<s8>(n - (div * 10));
                return div;
            }

            static u64 moddiv10(const u64 n, s8& mod) noexcept
            {
                const u64 div = (n / 10);
                mod           = static_cast<s8>(n - (div * 10));
                return div;
            }

            // -------- CONVERTERS ------------------------------------------------
            // The following converters write the value from back to front.
            // It is assumed that the pointer `dst` is already placed at the
            // position after the last character. The pointer position is
            // calculated using the corresponding count_digits_xxx() functions.

            // Example:
            // value ->  1234
            // array -> [........]
            // dst   ->      ^

            // -------- DECIMAL CONVERSION ----------------------------------------
            static void convert_dec(char* dst, u32 value) noexcept
            {
                u64 tmp = value;
                convert_dec(dst, tmp);
            }

            static void convert_dec(char* dst, u64 value) noexcept
            {
                s8 mod = 0;
                do
                {
                    value    = moddiv10(value, mod);
                    *(--dst) = nrunes::to_dec_char(mod);
                } while (value);
            }

            // -------- BINARY CONVERSION -----------------------------------------
            static void convert_bin(char* dst, u64 value) noexcept
            {
                do
                {
                    *(--dst) = static_cast<char>('0' + (value & 0x1));
                    value >>= 1U;
                } while (value);
            }
            static void convert_bin(char* dst, u32 value) noexcept
            {
                u64 tmp = value;
                convert_bin(dst, tmp);
            }

            // -------- OCTAL CONVERSION ------------------------------------------
            static void convert_oct(char* dst, u64 value) noexcept
            {
                do
                {
                    *(--dst) = static_cast<char>('0' + (value & 0x7));
                    value >>= 3U;
                } while (value);
            }
            static void convert_oct(char* dst, u32 value) noexcept
            {
                u64 tmp = value;
                convert_oct(dst, tmp);
            }

            // -------- HEXADECIMAL CONVERSION ------------------------------------
            static void convert_hex(char* dst, u64 value, const bool uppercase) noexcept
            {
                do
                {
                    *(--dst) = nrunes::to_hex_char((u8)value, !uppercase);
                    value >>= 4U;
                } while (value);
            }
            static void convert_hex(char* dst, u32 value, const bool uppercase) noexcept
            {
                u64 tmp = value;
                convert_hex(dst, tmp, uppercase);
            }
        };

        class Float
        {
        public:
            // --------------------------------------------------------------------
            // PUBLIC STATIC FUNCTIONS
            // --------------------------------------------------------------------

            static s32 convert(char* const significand, char* const significandEnd, s32& exponent, double value, const bool format_fixed, const s32 precision) noexcept
            {
                u64 ipart = 0;
                u64 fpart = 0;

                s32 ipart_digits = 0;
                s32 fpart_digits = 0;

                s32 fpart_padding = 0;

                if (value < 1)
                {
                    // Negative exponent

                    value *= 1e19;

                    fpart        = static_cast<u64>(value);
                    fpart_digits = Integer::count_digits_dec(fpart);

                    exponent = fpart_digits - 20;

                    fpart_padding = -exponent - 1;

                    // if(!format_fixed && precision > 19 - fpart_padding)
                    if (fpart_padding > 14 - precision)
                    {
                        fpart        = static_cast<u64>(value * static_cast<double>(Integer::pow10_uint64(fpart_padding)));
                        fpart_digits = Integer::count_digits_dec(fpart);
                    }
                }
                else
                {
                    // Positive exponent

                    ipart        = static_cast<u64>(value);
                    ipart_digits = Integer::count_digits_dec(ipart);

                    fpart        = static_cast<u64>((value - static_cast<double>(ipart)) * 1e14);
                    fpart_digits = Integer::count_digits_dec(fpart);

                    exponent = ipart_digits - 1;

                    fpart_padding = 14 - fpart_digits;
                }

                const auto round_index = 1 + precision + (format_fixed ? exponent : 0);

                if (round_index < 0)
                {
                    // Specified precision higher than converted value.
                    // Should print all zeros. Bail!
                    significand[0] = '0';
                    exponent       = 0;
                    return 1;
                }

                char* it = significand;

                if (ipart != 0)
                {
                    it += ipart_digits;
                    Integer::convert_dec(it, ipart);
                }

                if (fpart != 0)
                {
                    if (ipart != 0)
                    {
                        CharTraits::assign(it, '0', fpart_padding);
                    }

                    it += fpart_digits;
                    Integer::convert_dec(it, fpart);
                }

                const auto significand_size = remove_trailing_zeros(significand, it);

                if (significand_size <= round_index)
                {
                    // Rounding not needed. Bail!
                    return significand_size;
                }

                // Round to the specified precision.
                return round(significand, significand_size, exponent, format_fixed, round_index);
            }

        private:
            static s32 round(char* const significand, const s32 significand_size, s32& exponent, const bool format_fixed, const s32 round_index) noexcept
            {
                char* it = significand + round_index;

                bool round_up = false;

                if (round_index == significand_size - 1)
                {
                    // Round the last digit of the significand buffer.
                    // It can simultaneously be the first one if the
                    // significant buffer has only one digit.

                    const bool prev_digit_odd = (round_index > 0) ? (('0' - *(it - 1)) & 1) != 0 : false;

                    if (*it > '5' || (*it == '5' && prev_digit_odd))
                    {
                        // Round up if digit is:
                        // 1) greater than 5
                        //    e.g. 2.6 -> 3
                        // 2) exactly 5 and previous digit is odd
                        //    e.g. 2.5 -> 2
                        //    e.g. 3.5 -> 3
                        round_up = true;
                    }
                }
                else if (*it >= '5')
                {
                    // Round any digit except the last one. Since the trailing zeros were
                    // removed, we only need to test if the digit is at least '5' because it
                    // is granted that other non-zero digits are present after this position.
                    round_up = true;
                }

                if (round_up)
                {
                    bool carry = false;

                    if (round_index > 0)
                    {
                        --it;

                        do
                        {
                            if (*it < '9')
                            {
                                carry = false;
                                ++(*it);
                            }
                            else
                            {
                                carry = true;
                                *it   = '0';
                            }
                        } while (--it >= significand && carry);
                    }
                    else
                    {
                        carry = true;
                    }

                    // Buffer termination is not necessary since the caller functions
                    // rely on the returned size and not on null terminator.

                    if (carry)
                    {
                        significand[0] = '1';
                        ++exponent;
                        return 1;
                    }
                }
                else if (round_index == 0)
                {
                    significand[0] = '0';
                    exponent       = 0;
                    return 1;
                }

                // Do not remove the trailing zeros if format is fixed.
                if (format_fixed)
                {
                    return round_index;
                }

                return remove_trailing_zeros(significand, significand + round_index);
            }

            // Evaluates the range [first, last), truncates all the trailing zeros and return the
            // new range size. Keeps always at least 1 element of the range (even if it is zero).
            static s32 remove_trailing_zeros(const char* const first, const char* last) noexcept
            {
                while ((last - 1) > first && *(last - 1) == '0')
                {
                    --last;
                }

                // Buffer termination is not really necessary since the caller
                // functions rely on the returned size and not on null terminator.

                return static_cast<s32>(last - first);
            }
        };

        // Writes the alignment (sign, prefix and fill before) for any
        // argument type. Returns the fill counter to write after argument.
        s32 state_t::write_alignment(strspan_t& it, s32 characters, const bool negative) const
        {
            characters += sign_width(negative) + prefix_width();

            s32 fill_after = 0;

            if (width() <= characters)
            {
                // the 'width' is not enough to write the 'characters'
                write_sign(it, negative);
                write_prefix(it);
            }
            else
            {
                s32         fill_count = width() - characters;
                const Align al         = align();
                if (al == Align::kLeft)
                {
                    fill_after = fill_count;
                }
                else if (al == Align::kCenter)
                {
                    fill_after = fill_count - (fill_count / 2);
                    fill_count /= 2;
                }

                if (al != Align::kLeft && al != Align::kNumeric)
                {
                    // None (default right), Right or Center alignment
                    CharTraits::assign(it, fill_char(), fill_count);
                }

                write_sign(it, negative);
                write_prefix(it);

                if (al == Align::kNumeric)
                {
                    CharTraits::assign(it, fill_char(), fill_count);
                }
            }

            return fill_after;
        }

        void state_t::write_sign(strspan_t& it, const bool negative) const noexcept
        {
            if (negative)
            {
                it.write('-');
                return;
            }
            const Sign s = sign();
            switch (s.value)
            {
                case Sign::kPlus: it.write('+'); break;
                case Sign::kSpace: it.write(' '); break;
            }
        }

        void state_t::write_prefix(strspan_t& it) const noexcept
        {
            // Alternative format is valid for hexadecimal (including
            // pointers), octal, binary and all floating point types.
            if (hash() && !type_is_float())
            {
                it.write('0');

                if (type_is_integer_bin())
                {
                    it.write(uppercase() ? 'B' : 'b');
                }
                else if (type_is_integer_hex() || type_is_pointer())
                {
                    it.write(uppercase() ? 'X' : 'x');
                }
            }
        }

        class format_t
        {
        public:
            using iterator       = char*;
            using const_iterator = const char*;

            // --------------------------------------------------------------------
            // PUBLIC MEMBER FUNCTIONS
            // --------------------------------------------------------------------

            format_t(cstrspan_t& fmt, const s64 arg_count)
            {
                cstrspan_t it = fmt;

                // Parse argument index
                if (*it >= '0' && *it <= '9')
                {
                    // Index limited to `arg_count` value.
                    state.m_index = static_cast<s8>(parse_positive_small_int(it, arg_count));
                }

                if (*it == ':' && (it.peek(1) != '}'))
                {
                    // A format spec is expected next...

                    // Remove the empty format flag
                    state.m_flags = Flags::None;

                    // Advance ':' character
                    it.skip(1);

                    // Try to parse alignment flag at second character of format spec.
                    state.m_flags = parse_align_flag(it.peek(1));

                    if (state.m_flags != Flags::None)
                    {
                        // Alignment flag present at second character of format spec.
                        // Should also have a fill character at the first character.

                        // The fill character can be any character except '{' or '}'.
                        FMT_CHECK(*it != '{' && *it != '}', std::runtime_error);

                        state.m_fill_char = *it;
                        it.skip(2);
                    }
                    else
                    {
                        // Alignment flag not present at the second character of format spec.
                        // Try to parse the alignment flag at the first character instead...
                        state.m_flags = parse_align_flag(*it);

                        if (state.m_flags != Flags::None)
                        {
                            it.skip(1);
                        }
                    }

                    // Parse sign flag
                    switch (*it)
                    {
                        case '-':
                            state.m_flags.value |= Flags::SignMinus;
                            it.skip(1);
                            break;
                        case '+':
                            state.m_flags.value |= Flags::SignPlus;
                            it.skip(1);
                            break;
                        case ' ':
                            state.m_flags.value |= Flags::SignSpace;
                            it.skip(1);
                            break;
                        default: break;
                    }

                    // Parse hash flag
                    if (*it == '#')
                    {
                        state.m_flags.value |= Flags::Hash;
                        it.skip(1);
                    }

                    // Parse fill zero flag
                    const bool fill_zero = *it == '0';
                    if (fill_zero)
                    {
                        it.skip(1);
                    }

                    // Parse width
                    if (*it >= '0' && *it <= '9')
                    {
                        // Limit width to 255 characters
                        state.m_width = parse_positive_small_int(it, 255);
                    }

                    // Parse precision
                    if (*it == '.')
                    {
                        it.skip(1);

                        // Check for a missing/invalid precision specifier.
                        FMT_CHECK(*it >= '0' && *it <= '9', std::runtime_error);

                        state.m_precision = static_cast<s8>(parse_positive_small_int(it, 127));
                    }

                    // Parse type
                    if (*it != '}')
                    {
                        uchar c = it.read();
                        switch (c)
                        {
                            case 'c': state.m_type = Type::kChar; break;
                            case 'd': state.m_type = Type::kIntegerDec; break;
                            case 'X': state.m_flags |= Flags::Uppercase; USF_FALLTHROUGH;
                            case 'x': state.m_type = Type::kIntegerHex; break;
                            case 'o': state.m_type = Type::kIntegerOct; break;
                            case 'B': state.m_flags |= Flags::Uppercase; USF_FALLTHROUGH;
                            case 'b': state.m_type = Type::kIntegerBin; break;
                            case 'P': state.m_flags |= Flags::Uppercase; USF_FALLTHROUGH;
                            case 'p': state.m_type = Type::kPointer; break;
                            case 'F': state.m_flags |= Flags::Uppercase; USF_FALLTHROUGH;
                            case 'f': state.m_type = Type::kFloatFixed; break;
                            case 'E': state.m_flags |= Flags::Uppercase; USF_FALLTHROUGH;
                            case 'e': state.m_type = Type::kFloatScientific; break;
                            case 'G': state.m_flags |= Flags::Uppercase; USF_FALLTHROUGH;
                            case 'g': state.m_type = Type::kFloatGeneral; break;
                            case 's': state.m_type = Type::kString; break;
                            default: state.m_type = Type::kInvalid; break;
                        }

                        FMT_CHECK(state.m_type != Type::kInvalid, std::runtime_error);
                    }

                    // Validate and adjust the read format spec!
                    if (fill_zero)
                    {
                        // Fill zero flag has precedence over any other alignment and fill character.
                        state.m_flags.value = ((state.m_flags.value & (~Flags::AlignBitmask)) | Flags::AlignNumeric);
                        state.m_fill_char   = '0';
                    }

                    if (state.align() == Align::kNumeric)
                    {
                        // Numeric alignment are only valid for numeric and pointer types.
                        if (!state.type_is_numeric())
                            state.m_flags.value = (state.m_flags.value & Flags::AlignBitmask) | Flags::AlignDefault;
                    }

                    if (state.sign() != Sign::kNone)
                    {
                        // Sign is only valid for numeric types.
                        if (!state.type_is_numeric())
                            state.m_flags.value &= ~Flags::SignBitmask;
                    }

                    if (state.hash())
                    {
                        // Alternative format is valid for hexadecimal (including pointers), octal, binary and all floating point types.
                        if (!state.type_allow_hash())
                            state.m_flags.value &= ~Flags::Hash;
                    }

                    if (state.m_precision != -1)
                    {
                        // Precision is only valid for floating point and string types.
                        if (!state.type_is_float() && !state.type_is_string())
                            state.m_precision = -1;
                    }
                }

                // Test for unterminated argument format spec.
                FMT_CHECK(!it.at_end() && *it == '}', std::runtime_error);
                it.skip(1);

                fmt = it;
            }

            inline void default_align_left() noexcept
            {
                if ((state.m_flags.value & Flags::AlignBitmask) == Flags::AlignNone)
                {
                    state.m_flags.value |= Flags::AlignLeft;
                }
            }

            // Parses the input as a positive integer that fits into a `u8` type. This
            // function assumes that the first character is a digit and terminates parsing
            // at the presence of the first non-digit character or when value overflows.
            static u8 parse_positive_small_int(cstrspan_t& it, const s64 max_value)
            {
                ASSERT(max_value < 256);

                s64     value = 0;
                uchar32 c     = 0;
                do
                {
                    c     = it.read();
                    value = (value * 10) + static_cast<s32>(c - '0');
                    FMT_CHECK(value <= max_value, std::runtime_error);  // Check for overflow
                    c = it.peek();
                } while (c >= '0' && c <= '9');

                return static_cast<u8>(value);
            }

            static Flags parse_align_flag(const char ch) noexcept
            {
                switch (ch)
                {
                    case '<': return Flags::AlignLeft; break;
                    case '>': return Flags::AlignRight; break;
                    case '^': return Flags::AlignCenter; break;
                    case '=': return Flags::AlignNumeric; break;
                    default: return Flags::None; break;
                }
            }

            state_t state;
        };

        bool state_t::format_string(strspan_t& it, state_t& state, const char* str, const char* end)
        {
            cstrspan_t strview(str, end);
            return format_string(it, state, strview);
        }

        bool state_t::format_string(strspan_t& it, state_t& state, const cstrspan_t& str)
        {
            // Test for argument type / format match
            if (state.type_is_none() || state.type_is_string())
            {
                // TODO Characters and strings align to left by default.
                // default_align_left();

                // If precision is specified use it up to string size.
                const s32 str_length = (state.precision() == -1) ? static_cast<s32>(str.length()) : math::min(static_cast<s32>(state.precision()), static_cast<s32>(str.length()));

                return format_string(it, state, str, str_length, false);
            }
            return false;
        }

        bool state_t::format_string(strspan_t& it, const state_t& state, const cstrspan_t& str, const s32 str_length, const bool negative)
        {
            const s32 fill_after = state.write_alignment(it, str_length, negative);

            CharTraits::copy(it, str, str_length);
            CharTraits::assign(it, state.fill_char(), fill_after);
            return true;
        }

        class argument_t
        {
        public:
            // --------------------------------------------------------------------
            // PUBLIC TYPE ALIASES
            // --------------------------------------------------------------------

            using iterator       = char*;
            using const_iterator = const char*;

            static bool format(encoded_arg_t const& argValue, strspan_t& it, state_t& format)
            {
                switch (argValue.type.value)
                {
                    case TypeId::kBool: return format_bool(it, format, arg_t<bool>::decode(argValue));
                    case TypeId::kChar: return format_char(it, format, arg_t<char>::decode(argValue));
                    case TypeId::kInt8: return format_integer(it, format, arg_t<s8>::decode(argValue));
                    case TypeId::kInt16: return format_integer(it, format, arg_t<s16>::decode(argValue));
                    case TypeId::kInt32: return format_integer(it, format, arg_t<s32>::decode(argValue));
                    case TypeId::kUint8: return format_unsigned_integer(it, format, arg_t<u8>::decode(argValue));
                    case TypeId::kUint16: return format_unsigned_integer(it, format, arg_t<u16>::decode(argValue));
                    case TypeId::kUint32: return format_unsigned_integer(it, format, arg_t<u32>::decode(argValue));
                    case TypeId::kInt64: return format_integer(it, format, arg_t<s64>::decode(argValue));
                    case TypeId::kUint64: return format_unsigned_integer(it, format, arg_t<u64>::decode(argValue));
                    case TypeId::kVoidPointer: return format_pointer(it, format, (int_t)argValue.first.vp);
                    case TypeId::kConstVoidPointer: return format_pointer(it, format, (int_t)argValue.first.cvp);
                    case TypeId::kFloat: return format_float(it, format, arg_t<float>::decode(argValue));
                    case TypeId::kDouble: return format_float(it, format, arg_t<double>::decode(argValue));
                    case TypeId::kString:
                    case TypeId::kConstString:
                    {
                        cstrspan_t const cstr(argValue.first.cstr);
                        return state_t::format_string(it, format, cstr);
                    }
                    case TypeId::kStrSpan:
                    {
						cstrspan_t const cstr(argValue.first.cstr, argValue.second.cstr);
						return state_t::format_string(it, format, cstr);
                    }
                }
                return false;
            }

        private:
            // --------------------------------------------------------------------
            // PRIVATE STATIC FUNCTIONS
            // --------------------------------------------------------------------

            static bool format_bool(strspan_t& it, const state_t& state, const bool value)
            {
                if (state.type_is_none())
                {
                    s32 const  len = value ? 4 : 5;
                    cstrspan_t src(value ? "true" : "false", len);
                    return state_t::format_string(it, state, src, len, false);
                }
                else if (state.type_is_integer())
                {
                    return format_integer(it, state, static_cast<u32>(value));
                }
                else
                {
                    // Argument type / format mismatch
                    return false;
                }
                return true;
            }

            static bool format_char(strspan_t& it, state_t& state, const char value)
            {
                if (state.type_is_none() || state.type_is_char())
                {
                    // TODO Characters and strings align to left by default.
                    // state.default_align_left();

                    const s32 fill_after = state.write_alignment(it, 1, false);
                    it.write(value);
                    CharTraits::assign(it, state.fill_char(), fill_after);
                }
                else if (state.type_is_integer())
                {
                    format_integer(it, state, static_cast<s32>(value));
                }
                else if (state.type_is_float())
                {
                    format_float(it, state, static_cast<f32>(value));
                }
                else
                {
                    return false;
                }
                return true;
            }

            static bool format_integer(strspan_t& it, const state_t& state, const s64 value)
            {
                const bool negative = (value < 0);
                const auto uvalue   = (negative ? -value : value);

                return format_unsigned_integer(it, state, uvalue, negative);
            }

            static bool format_unsigned_integer(strspan_t& it, const state_t& state, const u64 value, const bool negative = false)
            {
                s32 fill_after = 0;

                if (state.type_is_none() || state.type_is_integer_dec())
                {
                    char       buffer[Integer::kMaxDigitsDec];
                    const auto digits = Integer::count_digits_dec(value);
                    fill_after        = state.write_alignment(it, digits, negative);
                    char* dst         = buffer + digits;
                    Integer::convert_dec(dst, value);
                    cstrspan_t src(buffer, digits);
                    it.write(src);
                }
                else if (state.type_is_integer_hex())
                {
                    char       buffer[Integer::kMaxDigitsHex];
                    const auto digits = Integer::count_digits_hex(value);
                    fill_after        = state.write_alignment(it, digits, negative);
                    char* dst         = buffer + digits;
                    Integer::convert_hex(dst, value, state.uppercase());
                    cstrspan_t src(buffer, digits);
                    it.write(src);
                }
                else if (state.type_is_integer_oct())
                {
                    char       buffer[Integer::kMaxDigitsOct];
                    const auto digits = Integer::count_digits_oct(value);
                    fill_after        = state.write_alignment(it, digits, negative);
                    char* dst         = buffer + digits;
                    Integer::convert_oct(dst, value);
                    cstrspan_t src(buffer, digits);
                    it.write(src);
                }
                else if (state.type_is_integer_bin())
                {
                    char       buffer[Integer::kMaxDigitsBin];
                    const auto digits = Integer::count_digits_bin(value);
                    fill_after        = state.write_alignment(it, digits, negative);
                    char* dst         = buffer + digits;
                    Integer::convert_bin(dst, value);
                    cstrspan_t src(buffer, digits);
                    it.write(src);
                }
                else
                {
                    return false;
                }

                CharTraits::assign(it, state.fill_char(), fill_after);
                return true;
            }

            static bool format_pointer(strspan_t& it, const state_t& state, const uint_t value)
            {
                if (state.type_is_none() || state.type_is_pointer())
                {
                    const auto ivalue     = static_cast<u64>(value);
                    const auto digits     = Integer::count_digits_hex(ivalue);
                    const auto fill_after = state.write_alignment(it, digits, false);
                    char       buffer[Integer::kMaxDigitsPointer];
                    char*      dst = buffer + digits;
                    Integer::convert_hex(dst, ivalue, state.uppercase());
                    cstrspan_t src(buffer, digits);
                    it.write(src);
                    CharTraits::assign(it, state.fill_char(), fill_after);
                }
                else
                {
                    // Argument type / format mismatch
                    return false;
                }
                return true;
            }

            static bool format_float(strspan_t& it, const state_t& state, double value)
            {
                char format_char;
                s8   precision;
                if (state.type_is_none())
                {
                    format_char = 'g';
                    precision   = DoubleConvert::DEFAULT_PRECISION;
                }
                else
                {
                    precision   = state.m_precision < 0 ? DoubleConvert::DEFAULT_PRECISION : state.m_precision;
                    format_char = state.type_is_float_scientific() ? 'e' : state.type_is_float_general() ? 'g' : 'f';
                }
                u32 flags = 0;
                if (state.uppercase())
                {
                    flags |= DoubleConvert::FLAG_UPPERCASE;
                }
                if (state.m_width > 0)
                {
                    flags |= DoubleConvert::FLAG_HAVE_WIDTH;
                }

                // s32 dconvstr_print(char** outbuf, s32* outbuf_size,
                //                    double value, s32 format_char, u32 format_flags,
                //                    s32 format_width, s32 format_precision);
                char      outbuffer[64];
                char*     outbuf      = outbuffer;
                const s32 output_cap  = 64;
                s32       outbuf_size = output_cap;
                s32       result      = dconvstr_print(&outbuf, &outbuf_size, value, format_char, flags, state.m_width, precision);
                if (result)
                {
                    CharTraits::copy(it, outbuffer, output_cap - outbuf_size);
                    return true;
                }
                return false;
            }
        };

        void parse_format_string(strspan_t& str, cstrspan_t& fmt)
        {
            while (!fmt.at_end() && !str.at_end())
            {
                uchar32 c = fmt.read();
                if (c == '{')
                {
                    if (fmt.peek() == '{')
                    {
                        // Found '{{' escape character, skip the first and copy the second '{'.
                        str.write(c);
                        fmt.skip();
                    }
                    else
                    {
                        // A type format should follow...
                        break;
                    }
                }
                else if (c == '}')
                {
                    FMT_CHECK(fmt.peek() == '}', std::runtime_error);

                    // Found '}}' escape character, skip the first and copy the second '}'.
                    fmt.skip();
                    str.write(c);
                }
                else
                {
                    // Copy literal text
                    str.write(c);
                }
            }
        }

        bool process(strspan_t& str, cstrspan_t& fmt, const args_t& args)
        {
            // Argument's sequential index
            s32 arg_seq_index = 0;

            parse_format_string(str, fmt);

            bool result = true;
            while (!fmt.at_end())
            {
                // Should be one character after '{
                format_t format(fmt, args.size);

                // Determine which argument index to use, sequential or positional.
                s32 arg_index = format.state.index();

                if (arg_index < 0)
                {
                    FMT_CHECK(arg_seq_index < args.size, std::runtime_error);
                    arg_index = arg_seq_index++;
                }

                if (args.funcs != nullptr && args.funcs[arg_index] != nullptr)
                {
                    args.funcs[arg_index](args.args[arg_index], str, format.state);
                }
                else
                {
                    if (!argument_t::format(args.args[arg_index], str, format.state))
                        result = false;
                }

                parse_format_string(str, fmt);
            }
            return result;
        }

        bool toStr(ascii::prune str, ascii::prune end, ascii::pcrune fmt, args_t const& args)
        {
            strspan_t  str_span(str, end);
            cstrspan_t fmt_view(fmt);
            bool       result = process(str_span, fmt_view, args);
            str_span.write();
            return result;
        }

        static void Test()
        {
            char s[128];
            toStr(s, 128, "Hello {:s}!", "world");

            char str[128];
            toStr(str, 128, "{:14}", false);
            toStr(str, 128, "{0}{1}", false, 100.0f);
            toStr(str, 128, "{0}{1}{2}", false, 100.0f, 10);
            toStr(str, 128, "{0}{1}{2}{3}", false, 100.0f, 10, "test");
            toStr(str, 128, "{0}{1}{2}{3}{4}", false, 100.0f, 10, "test", 'a');
        }

    }  // namespace fmt
}  // namespace ncore