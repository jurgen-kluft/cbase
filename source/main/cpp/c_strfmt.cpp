// https://github.com/hparracho/usflib/tree/master

#include "cbase/c_strfmt.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace ncore
{
    namespace fmt
    {
#define USF_CPP14_CONSTEXPR constexpr
#define USF_ALWAYS_INLINE inline

#define USF_CONTRACT_VIOLATION(except) static_cast<void>(0)
#define USF_FALLTHROUGH /*fall through*/
#define USF_ENFORCE(cond, except) ((!!(cond)) ? static_cast<void>(0) : USF_CONTRACT_VIOLATION(except))

// ----------------------------------------------------------------------------
// Compiler version detection
// ----------------------------------------------------------------------------

// MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 14.1 _MSC_VER >= 1910 (Visual Studio 2017)
#if defined(_MSC_VER) && !defined(__clang__)
#    define USF_COMPILER_MSVC
#    define USF_MSVC_VERSION (_MSC_VER / 10 - 10 * (5 + (_MSC_VER < 1900)))
#    if (USF_MSVC_VERSION < 140)
#        error usflib requires MSVC++ 14.0 or greater
#    endif
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.
#    define USF_CPLUSPLUS (_MSC_VER <= 1900 ? 201103L : _MSVC_LANG)
#endif

        // ----------------------------------------------------------------------------
// Missing intrinsic functions definition for MSVC
// ----------------------------------------------------------------------------
#if defined(USF_COMPILER_MSVC)
#    include <intrin.h>

#    pragma intrinsic(_BitScanReverse, _BitScanReverse64)

        int __builtin_clz(u32 value)
        {
            unsigned long leading_zero = 0;
            return _BitScanReverse(&leading_zero, value) ? static_cast<int>(31 - leading_zero) : 32;
        }

        int __builtin_clzll(u64 value)
        {
            unsigned long leading_zero = 0;
            return _BitScanReverse64(&leading_zero, value) ? static_cast<int>(63 - leading_zero) : 64;
        }
#endif // defined(USF_COMPILER_MSVC)

        constexpr u32 pow10_uint32_lut[]{1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

        constexpr u64 pow10_uint64_lut[]{1,           10,           100,           1000,           10000,           100000,           1000000,           10000000,           100000000,           1000000000,
                                         10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000, 1000000000000000, 10000000000000000, 100000000000000000, 1000000000000000000, 10000000000000000000U};

        constexpr char digits_hex_uppercase[]{"0123456789ABCDEF"};
        constexpr char digits_hex_lowercase[]{"0123456789abcdef"};

        // Both the Integer and Float classes only 'generate' ascii, so why make most
        // of those functions a template?

        // TODO We could pass a 'writer' object to the functions which would reduce code bloat.

        class Integer
        {
        public:
            // --------------------------------------------------------------------
            // PUBLIC STATIC FUNCTIONS
            // --------------------------------------------------------------------

            // -------- POWERS OF 10 ----------------------------------------------
            static USF_CPP14_CONSTEXPR u32 pow10_uint32(const int index) noexcept
            {
                assert(index >= 0 && index < 10);
                return pow10_uint32_lut[index];
            }

            static USF_CPP14_CONSTEXPR u64 pow10_uint64(const int index) noexcept
            {
                assert(index >= 0 && index < 20);
                return pow10_uint64_lut[index];
            }

            // -------- COUNT DIGITS ----------------------------------------------
            // Based on the code from:
            // http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10
            // --------------------- ----------------------------------------------

            static USF_CPP14_CONSTEXPR int count_digits_dec(const u32 n) noexcept
            {
                if (n < 10)
                    return 1;

                // The algorithm below doesn't work when `n` is 0 because:
                // 1. the result of __builtin_clz() is undefined if `n` is 0.
                // 2. the `pow10_uint32_lut` lookup table has the value 1 in
                //    the first element and not a 0 as the algorithm expects.
                // (both cases are covered by the previous if statement or
                //  by the slower commented OR operation below).

                // n = n | 1;

                const int t = (32 - __builtin_clz(n)) * 1233 >> 12;
                return t - (n < pow10_uint32_lut[t]) + 1;
            }

            static USF_CPP14_CONSTEXPR int count_digits_dec(const u64 n) noexcept
            {
                if (n <= std::numeric_limits<u32>::max())
                {
                    return count_digits_dec(static_cast<u32>(n));
                }

                // The algorithm below doesn't work when `n` is 0 because:
                // 1. the result of __builtin_clzll() is undefined if `n` is 0.
                // 2. the `pow10_uint64_lut` lookup table has the value 1 in
                //    the first element and not a 0 as the algorithm expects.
                // (both cases are covered by the previous if statement or
                //  by the slower commented OR operation below).

                // n = n | 1;

                const int t = (64 - __builtin_clzll(n)) * 1233 >> 12;
                return t - (n < pow10_uint64_lut[t]) + 1;
            }

            static USF_CPP14_CONSTEXPR int count_digits_bin(const u32 n) noexcept
            {
                // The result of __builtin_clz() is undefined if `n` is 0.
                return (n < 2) ? 1 : (32 - __builtin_clz(n));
            }

            static USF_CPP14_CONSTEXPR int count_digits_bin(const u64 n) noexcept
            {
                // The result of __builtin_clzll() is undefined if `n` is 0.
                return (n < 2) ? 1 : (64 - __builtin_clzll(n));
            }

            template <typename T, typename std::enable_if<std::numeric_limits<T>::is_integer && std::is_unsigned<T>::value, bool>::type = true> static USF_CPP14_CONSTEXPR int count_digits_oct(T n) noexcept
            {
                int digits = 1;
                while ((n >>= 3U) != 0)
                {
                    ++digits;
                }
                return digits;
            }

            template <typename T, typename std::enable_if<std::numeric_limits<T>::is_integer && std::is_unsigned<T>::value, bool>::type = true> static USF_CPP14_CONSTEXPR int count_digits_hex(T n) noexcept
            {
                int digits = 1;
                while ((n >>= 4U) != 0)
                {
                    ++digits;
                }
                return digits;
            }

            // -------- DIVIDE BY 10 WITH REMAINDER-------------------------------
            static USF_CPP14_CONSTEXPR u32 moddiv10(const u32 n, s8& mod) noexcept
            {
                const u32 div = (n / 10) * 10;
                mod           = static_cast<s8>(n - div);
                return div;
            }

            static USF_CPP14_CONSTEXPR u64 moddiv10(const u64 n, s8& mod) noexcept
            {
                const u64 div = (n / 10) * 10;
                mod           = static_cast<s8>(n - div);
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
            template <typename CharT> static USF_CPP14_CONSTEXPR void convert_dec(CharT* dst, u32 value) noexcept
            {
                u64 tmp = value;
                convert_dec(dst, tmp);
            }

            template <typename CharT> static USF_CPP14_CONSTEXPR void convert_dec(CharT* dst, u64 value) noexcept
            {
                s8 mod;
                do
                {
                    value    = moddiv10(value, mod);
                    *(--dst) = static_cast<CharT>("0123456789??????"[mod & 0xF]);
                } while (value);
            }

            // -------- BINARY CONVERSION -----------------------------------------
            template <typename CharT> static USF_CPP14_CONSTEXPR void convert_bin(CharT* dst, u32 value) noexcept
            {
                u64 tmp = value;
                convert_bin(dst, tmp);
            }

            template <typename CharT> static USF_CPP14_CONSTEXPR void convert_bin(CharT* dst, u64 value) noexcept
            {
                do
                {
                    *(--dst) = static_cast<CharT>('0' + (value & 0x1));
                    value >>= 1U;
                } while (value);
            }

            // -------- OCTAL CONVERSION ------------------------------------------
            template <typename CharT> static USF_CPP14_CONSTEXPR void convert_oct(CharT* dst, u32 value) noexcept
            {
                u64 tmp = value;
                convert_oct(dst, tmp);
            }

            template <typename CharT> static USF_CPP14_CONSTEXPR void convert_oct(CharT* dst, u64 value) noexcept
            {
                do
                {
                    *(--dst) = static_cast<CharT>('0' + (value & 0x7));
                    value >>= 3U;
                } while (value);
            }

            // -------- HEXADECIMAL CONVERSION ------------------------------------
            template <typename CharT> static USF_CPP14_CONSTEXPR void convert_hex(CharT* dst, u32 value, const bool uppercase) noexcept
            {
                u64 tmp = value;
                convert_hex(dst, tmp, uppercase);
            }

            template <typename CharT> static USF_CPP14_CONSTEXPR void convert_hex(CharT* dst, u64 value, const bool uppercase) noexcept
            {
                const char* digits = uppercase ? digits_hex_uppercase : digits_hex_lowercase;
                do
                {
                    *(--dst) = static_cast<CharT>(digits[value & 0xF]);
                    value >>= 4U;
                } while (value);
            }
        };

        class Float
        {
        public:
            // --------------------------------------------------------------------
            // PUBLIC STATIC FUNCTIONS
            // --------------------------------------------------------------------

            template <typename CharT> static USF_CPP14_CONSTEXPR int convert(CharT* const significand, int& exponent, double value, const bool format_fixed, const int precision) noexcept
            {
                u64 ipart = 0;
                u64 fpart = 0;

                int ipart_digits = 0;
                int fpart_digits = 0;

                int fpart_padding = 0;

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

                CharT* it = significand;

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
            // --------------------------------------------------------------------
            // PRIVATE STATIC FUNCTIONS
            // --------------------------------------------------------------------

            template <typename CharT> static USF_CPP14_CONSTEXPR int round(CharT* const significand, const int significand_size, int& exponent, const bool format_fixed, const int round_index) noexcept
            {
                CharT* it = significand + round_index;

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
            template <typename CharT> static USF_CPP14_CONSTEXPR int remove_trailing_zeros(const CharT* const first, const CharT* last) noexcept
            {
                while ((last - 1) > first && *(last - 1) == '0')
                {
                    --last;
                }

                // Buffer termination is not really necessary since the caller
                // functions rely on the returned size and not on null terminator.

                return static_cast<int>(last - first);
            }
        };

        class CharTraits
        {
        public:
            // --------------------------------------------------------------------
            // PUBLIC STATIC FUNCTIONS
            // --------------------------------------------------------------------

            template <typename CharDst, typename CharSrc, typename std::enable_if<std::is_convertible<CharSrc, CharDst>::value, bool>::type = true>
            USF_ALWAYS_INLINE static USF_CPP14_CONSTEXPR void assign(CharDst*& dst, CharSrc ch, std::ptrdiff_t count) noexcept
            {
                while ((count--) > 0)
                {
                    *dst++ = static_cast<CharDst>(ch);
                }
            }

            template <typename CharDst, typename CharSrc, typename std::enable_if<std::is_convertible<CharSrc, CharDst>::value, bool>::type = true>
            USF_ALWAYS_INLINE static USF_CPP14_CONSTEXPR void copy(CharDst*& dst, const CharSrc* src, std::ptrdiff_t count) noexcept
            {
                while ((count--) > 0)
                {
                    *dst++ = static_cast<CharDst>(*src++);
                }
            }

            template <typename CharT> USF_ALWAYS_INLINE static USF_CPP14_CONSTEXPR std::ptrdiff_t length(const CharT* str) noexcept
            {
                const CharT* str_begin = str;
                while (*str != CharT{})
                {
                    ++str;
                }
                return str - str_begin;
            }
        };

        template <typename CharT> class BasicStringSpan
        {
        public:
            // --------------------------------------------------------------------
            // PUBLIC TYPE ALIASES
            // --------------------------------------------------------------------

            using char_type       = CharT;
            using size_type       = std::ptrdiff_t;
            using reference       = char_type&;
            using const_reference = const char_type&;
            using pointer         = char_type*;
            using const_pointer   = const char_type*;
            using iterator        = pointer;
            using const_iterator  = const_pointer;

            // --------------------------------------------------------------------
            // TEMPLATE PARAMETERS VALIDATION
            // --------------------------------------------------------------------

            static_assert(std::is_trivial<CharT>::value && std::is_standard_layout<CharT>::value, "BasicStringSpan: CharT must be a POD type (both trivial and standard-layout).");

            // --------------------------------------------------------------------
            // PUBLIC MEMBER FUNCTIONS
            // --------------------------------------------------------------------

            // -------- CONSTRUCTORS ----------------------------------------------

            USF_CPP14_CONSTEXPR BasicStringSpan() noexcept = delete;

            USF_CPP14_CONSTEXPR BasicStringSpan(const BasicStringSpan&) noexcept = default;
            USF_CPP14_CONSTEXPR BasicStringSpan(BasicStringSpan&&) noexcept      = default;
#if 0
        template <std::size_t N> // N includes the null terminator!
        USF_CPP14_CONSTEXPR BasicStringSpan(char_type (&str)[N]) noexcept
            : m_begin{str}, m_end{str + N - 1}
        {}
#endif
            USF_CPP14_CONSTEXPR BasicStringSpan(pointer str) noexcept
                : m_begin{str}
                , m_end{str + CharTraits::length(str)}
            {
            }

            USF_CPP14_CONSTEXPR BasicStringSpan(pointer str, const size_type str_count)
                : m_begin{str}
                , m_end{str + str_count}
            {
                USF_ENFORCE(str_count >= 0, std::runtime_error);
            }

            USF_CPP14_CONSTEXPR BasicStringSpan(iterator first, iterator last)
                : m_begin{first}
                , m_end{last}
            {
                USF_ENFORCE(first <= last, std::runtime_error);
            }

            template <std::size_t N>
            USF_CPP14_CONSTEXPR BasicStringSpan(std::array<CharT, N>& array) noexcept
                : m_begin{array.begin()}
                , m_end{array.end()}
            {
            }

            // -------- ASSIGNMENT ------------------------------------------------

            USF_CPP14_CONSTEXPR BasicStringSpan& operator=(const BasicStringSpan&) noexcept = default;
            USF_CPP14_CONSTEXPR BasicStringSpan& operator=(BasicStringSpan&&) noexcept      = default;

            // -------- ELEMENT ACCESS --------------------------------------------

            // Returns a reference to the character at specified location `pos`.
            // Bounds checking is performed.
            inline USF_CPP14_CONSTEXPR const_reference at(const size_type pos) const
            {
                USF_ENFORCE(pos >= 0 && pos < size(), std::out_of_range);
                return m_begin[pos];
            }

            inline USF_CPP14_CONSTEXPR reference at(const size_type pos)
            {
                USF_ENFORCE(pos >= 0 && pos < size(), std::out_of_range);
                return m_begin[pos];
            }

            // Returns a reference to the character at specified location `pos`.
            // No bounds checking is performed.
            inline USF_CPP14_CONSTEXPR const_reference operator[](const size_type pos) const noexcept { return m_begin[pos]; }
            inline USF_CPP14_CONSTEXPR reference       operator[](const size_type pos) noexcept { return m_begin[pos]; }

            // Returns reference to the first character of the sequence.
            inline USF_CPP14_CONSTEXPR const_reference front() const noexcept
            {
                assert(!empty());
                return m_begin[0];
            }
            inline USF_CPP14_CONSTEXPR reference front() noexcept
            {
                assert(!empty());
                return m_begin[0];
            }

            // Returns reference to the last character of the sequence.
            inline USF_CPP14_CONSTEXPR const_reference back() const noexcept
            {
                assert(!empty());
                return *(m_end - 1);
            }
            inline USF_CPP14_CONSTEXPR reference back() noexcept
            {
                assert(!empty());
                return *(m_end - 1);
            }

            // Returns a pointer to the beginning of the sequence.
            inline USF_CPP14_CONSTEXPR const_pointer data() const noexcept { return m_begin; }
            inline USF_CPP14_CONSTEXPR pointer       data() noexcept { return m_begin; }

#if defined(USF_STD_BASIC_STRING_VIEW)
            // Conversion to std::basic_string view.
            template <typename Traits = std::char_traits<CharT>> inline USF_CPP14_CONSTEXPR operator USF_STD_BASIC_STRING_VIEW<CharT, Traits>() const noexcept { return USF_STD_BASIC_STRING_VIEW<CharT, Traits>{data(), static_cast<std::size_t>(size())}; }
#endif
            // -------- ITERATORS -------------------------------------------------

            // Returns an iterator to the first character of the sequence.
            inline USF_CPP14_CONSTEXPR const_iterator cbegin() const noexcept { return m_begin; }
            inline USF_CPP14_CONSTEXPR const_iterator begin() const noexcept { return m_begin; }
            inline USF_CPP14_CONSTEXPR iterator       begin() noexcept { return m_begin; }

            // Returns an iterator to the character following the last character of the sequence.
            // This character acts as a placeholder, attempting to access it results in undefined behavior.
            inline USF_CPP14_CONSTEXPR const_iterator cend() const noexcept { return m_end; }
            inline USF_CPP14_CONSTEXPR const_iterator end() const noexcept { return m_end; }
            inline USF_CPP14_CONSTEXPR iterator       end() noexcept { return m_end; }

            // -------- CAPACITY --------------------------------------------------

            // Checks if the sequence has no characters, i.e. whether begin() == end().
            inline USF_CPP14_CONSTEXPR bool empty() const noexcept { return (size() == 0); }

            // Returns the number of characters in the sequence, i.e. the distance between begin() and end().
            inline USF_CPP14_CONSTEXPR size_type size() const noexcept { return static_cast<size_type>(m_end - m_begin); }
            inline USF_CPP14_CONSTEXPR size_type length() const noexcept { return static_cast<size_type>(m_end - m_begin); }

            // -------- MODIFIERS -------------------------------------------------

            // Moves the start of the sequence forward by `n` characters.
            // The behavior is undefined if n > size().
            inline USF_CPP14_CONSTEXPR void remove_prefix(const size_type n) noexcept { m_begin += n; }

            // Moves the end of the sequence back by `n` characters.
            // The behavior is undefined if n > size().
            inline USF_CPP14_CONSTEXPR void remove_suffix(const size_type n) noexcept { m_end -= n; }

        private:
            // --------------------------------------------------------------------
            // PRIVATE MEMBER VARIABLES
            // --------------------------------------------------------------------

            iterator m_begin{nullptr};
            iterator m_end{nullptr};
        };

        using StringSpan  = BasicStringSpan<char>;
        using WStringSpan = BasicStringSpan<wchar_t>;

#if defined(USF_CPP20_CHAR8_T_SUPPORT)
        using U8StringSpan = BasicStringSpan<char8_t>;
#endif
        using U16StringSpan = BasicStringSpan<char16_t>;
        using U32StringSpan = BasicStringSpan<char32_t>;

        using ByteStringSpan = BasicStringSpan<uint8_t>;

        template <typename CharT> class BasicStringView
        {
        public:
            // --------------------------------------------------------------------
            // PUBLIC TYPE ALIASES
            // --------------------------------------------------------------------

            using char_type       = CharT;
            using size_type       = std::ptrdiff_t; // Not std::size_t
            using reference       = char_type&;
            using const_reference = const char_type&;
            using pointer         = char_type*;
            using const_pointer   = const char_type*;
            using iterator        = pointer;
            using const_iterator  = const_pointer;

            // --------------------------------------------------------------------
            // TEMPLATE PARAMETERS VALIDATION
            // --------------------------------------------------------------------

            static_assert(std::is_trivial<CharT>::value && std::is_standard_layout<CharT>::value, "BasicStringView: CharT must be a POD type (both trivial and standard-layout).");

            // --------------------------------------------------------------------
            // PUBLIC MEMBER FUNCTIONS
            // --------------------------------------------------------------------

            // -------- CONSTRUCTORS ----------------------------------------------

            USF_CPP14_CONSTEXPR BasicStringView() noexcept                       = delete;
            USF_CPP14_CONSTEXPR BasicStringView(const BasicStringView&) noexcept = default;
            USF_CPP14_CONSTEXPR BasicStringView(BasicStringView&&) noexcept      = default;
#if 0
        template <std::size_t N> // N includes the null terminator!
        USF_CPP14_CONSTEXPR BasicStringView(const char_type (&str)[N]) noexcept
            : m_begin{str}, m_end{str + N - 1}
        {}
#endif
            USF_CPP14_CONSTEXPR BasicStringView(const_pointer str) noexcept
                : m_begin{str}
                , m_end{str + CharTraits::length(str)}
            {
            }

            USF_CPP14_CONSTEXPR BasicStringView(const_pointer str, const size_type str_count)
                : m_begin{str}
                , m_end{str + str_count}
            {
                USF_ENFORCE(str_count >= 0, std::runtime_error);
            }

            USF_CPP14_CONSTEXPR BasicStringView(const_iterator first, const_iterator last)
                : m_begin{first}
                , m_end{last}
            {
                USF_ENFORCE(first <= last, std::runtime_error);
            }

#if defined(USF_STD_BASIC_STRING_VIEW)
            template <typename Traits = std::char_traits<CharT>>
            USF_CPP14_CONSTEXPR BasicStringView(const USF_STD_BASIC_STRING_VIEW<CharT, Traits> view)
                : m_begin{view.cbegin()}
                , m_end{view.cend()}
            {
            }
#endif
            // -------- ASSIGNMENT ------------------------------------------------

            USF_CPP14_CONSTEXPR BasicStringView& operator=(const BasicStringView&) noexcept = default;
            USF_CPP14_CONSTEXPR BasicStringView& operator=(BasicStringView&&) noexcept      = default;

            // -------- ELEMENT ACCESS --------------------------------------------

            // Returns a reference to the character at specified location `pos`.
            // Bounds checking is performed.
            inline USF_CPP14_CONSTEXPR const_reference at(const size_type pos) const
            {
                USF_ENFORCE(pos >= 0 && pos < size(), std::out_of_range);
                return m_begin[pos];
            }

            // Returns a reference to the character at specified location `pos`.
            // No bounds checking is performed.
            inline USF_CPP14_CONSTEXPR const_reference operator[](const size_type pos) const noexcept { return m_begin[pos]; }

            // Returns reference to the first character of the sequence.
            inline USF_CPP14_CONSTEXPR const_reference front() const noexcept
            {
                assert(!empty());
                return m_begin[0];
            }

            // Returns reference to the last character of the sequence.
            inline USF_CPP14_CONSTEXPR const_reference back() const noexcept
            {
                assert(!empty());
                return *(m_end - 1);
            }

            // Returns a pointer to the beginning of the sequence.
            inline USF_CPP14_CONSTEXPR const_pointer data() const noexcept { return m_begin; }

#if defined(USF_STD_BASIC_STRING_VIEW)
            // Conversion to std::basic_string view.
            template <typename Traits = std::char_traits<CharT>> inline USF_CPP14_CONSTEXPR operator USF_STD_BASIC_STRING_VIEW<CharT, Traits>() const noexcept { return USF_STD_BASIC_STRING_VIEW<CharT, Traits>{data(), static_cast<std::size_t>(size())}; }
#endif
            // -------- ITERATORS -------------------------------------------------

            // Returns an iterator to the first character of the sequence.
            inline USF_CPP14_CONSTEXPR const_iterator cbegin() const noexcept { return m_begin; }
            inline USF_CPP14_CONSTEXPR const_iterator begin() const noexcept { return m_begin; }

            // Returns an iterator to the character following the last character of the sequence.
            // This character acts as a placeholder, attempting to access it results in undefined behavior.
            inline USF_CPP14_CONSTEXPR const_iterator cend() const noexcept { return m_end; }
            inline USF_CPP14_CONSTEXPR const_iterator end() const noexcept { return m_end; }

            // -------- CAPACITY --------------------------------------------------

            // Checks if the sequence has no characters, i.e. whether begin() == end().
            inline USF_CPP14_CONSTEXPR bool empty() const noexcept { return (size() == 0); }

            // Returns the number of characters in the sequence, i.e. the distance between begin() and end().
            inline USF_CPP14_CONSTEXPR size_type size() const noexcept { return static_cast<size_type>(m_end - m_begin); }
            inline USF_CPP14_CONSTEXPR size_type length() const noexcept { return static_cast<size_type>(m_end - m_begin); }

            // -------- MODIFIERS -------------------------------------------------

            // Moves the start of the sequence forward by `n` characters.
            // The behavior is undefined if n > size().
            inline USF_CPP14_CONSTEXPR void remove_prefix(const size_type n) noexcept { m_begin += n; }

            // Moves the end of the sequence back by `n` characters.
            // The behavior is undefined if n > size().
            inline USF_CPP14_CONSTEXPR void remove_suffix(const size_type n) noexcept { m_end -= n; }

        private:
            // --------------------------------------------------------------------
            // PRIVATE MEMBER VARIABLES
            // --------------------------------------------------------------------

            const_iterator m_begin{nullptr};
            const_iterator m_end{nullptr};
        };

        using StringView  = BasicStringView<char>;
        using WStringView = BasicStringView<wchar_t>;

#if defined(USF_CPP20_CHAR8_T_SUPPORT)
        using U8StringView = BasicStringView<char8_t>;
#endif
        using U16StringView = BasicStringView<char16_t>;
        using U32StringView = BasicStringView<char32_t>;

        using ByteStringView = BasicStringView<uint8_t>;

        // This whole class is a template class mainly because of the iterator ???
        // Why not give it an object that can do the character reading ???
        // The same with BasicStringView and BasicStringSpan, there seems to be only
        // an underlying requirement for a char reader and writer ???
        template <typename CharT> class ArgFormat
        {
            enum Flags : u8
            {
                kNone         = (0U << 0U),
                kEmpty        = (1U << 0U),
                kAlignNone    = (0U << 1U),
                kAlignLeft    = (1U << 1U),
                kAlignRight   = (2U << 1U),
                kAlignCenter  = (3U << 1U),
                kAlignNumeric = (4U << 1U),
                kAlignBitmask = kAlignLeft | kAlignRight | kAlignCenter | kAlignNumeric,
                kSignNone     = (0U << 4U),
                kSignMinus    = (1U << 4U),
                kSignPlus     = (2U << 4U),
                kSignSpace    = (3U << 4U),
                kSignBitmask  = kSignMinus | kSignPlus | kSignSpace,
                kHash         = (1U << 6U),
                kUppercase    = (1U << 7U)
            };

        public:
            using iterator       = CharT*;
            using const_iterator = const CharT*;

            enum class Align : u8
            {
                kNone    = kAlignNone,
                kLeft    = kAlignLeft,
                kRight   = kAlignRight,
                kCenter  = kAlignCenter,
                kNumeric = kAlignNumeric
            };

            enum class Sign : u8
            {
                kNone  = kSignNone,
                kMinus = kSignMinus,
                kPlus  = kSignPlus,
                kSpace = kSignSpace
            };

            enum class Type : u8
            {
                kNone,
                kIntegerDec,
                kIntegerHex,
                kIntegerOct,
                kIntegerBin,
                kFloatFixed,
                kFloatScientific,
                kFloatGeneral,
                kPointer,
                kString,
                kChar,
                kInvalid
            };

            // --------------------------------------------------------------------
            // PUBLIC MEMBER FUNCTIONS
            // --------------------------------------------------------------------

            USF_CPP14_CONSTEXPR ArgFormat(BasicStringView<CharT>& fmt, const int arg_count)
            {
                const_iterator it = fmt.cbegin();

                USF_ENFORCE(*it == '{', std::runtime_error);

                // Iterator is placed at "{" character, so advance it.
                ++it;

                // Parse argument index
                if (*it >= '0' && *it <= '9')
                {
                    // Index limited to `arg_count` value.
                    m_index = static_cast<s8>(parse_positive_small_int(it, arg_count));
                }

                if (*it == ':' && *(it + 1) != '}')
                {
                    // A format spec is expected next...

                    // Remove the empty format flag
                    m_flags = Flags::kNone;

                    // Advance ':' character
                    ++it;

                    // Try to parse alignment flag at second character of format spec.
                    m_flags = parse_align_flag(*(it + 1));

                    if (m_flags != Flags::kNone)
                    {
                        // Alignment flag present at second character of format spec.
                        // Should also have a fill character at the first character.

                        // The fill character can be any character except '{' or '}'.
                        USF_ENFORCE(*it != '{' && *it != '}', std::runtime_error);

                        m_fill_char = *it;
                        it += 2;
                    }
                    else
                    {
                        // Alignment flag not present at the second character of format spec.
                        // Try to parse the alignment flag at the first character instead...
                        m_flags = parse_align_flag(*it);

                        if (m_flags != Flags::kNone)
                        {
                            ++it;
                        }
                    }

                    // Parse sign flag
                    switch (*it)
                    {
                        case '-':
                            m_flags |= Flags::kSignMinus;
                            ++it;
                            break;
                        case '+':
                            m_flags |= Flags::kSignPlus;
                            ++it;
                            break;
                        case ' ':
                            m_flags |= Flags::kSignSpace;
                            ++it;
                            break;
                        default: break;
                    }

                    // Parse hash flag
                    if (*it == '#')
                    {
                        m_flags |= Flags::kHash;
                        ++it;
                    }

                    // Parse fill zero flag
                    const bool fill_zero = *it == '0';
                    if (fill_zero)
                    {
                        ++it;
                    }

                    // Parse width
                    if (*it >= '0' && *it <= '9')
                    {
                        // Limit width to 255 characters
                        m_width = parse_positive_small_int(it, 255);
                    }

                    // Parse precision
                    if (*it == '.')
                    {
                        ++it;

                        // Check for a missing/invalid precision specifier.
                        USF_ENFORCE(*it >= '0' && *it <= '9', std::runtime_error);

                        m_precision = static_cast<s8>(parse_positive_small_int(it, 127));
                    }

                    // Parse type
                    if (*it != '}')
                    {
                        switch (*it++)
                        {
                            case 'c': m_type = Type::kChar; break;
                            case 'd': m_type = Type::kIntegerDec; break;
                            case 'X': m_flags |= Flags::kUppercase; USF_FALLTHROUGH;
                            case 'x': m_type = Type::kIntegerHex; break;
                            case 'o': m_type = Type::kIntegerOct; break;
                            case 'B': m_flags |= Flags::kUppercase; USF_FALLTHROUGH;
                            case 'b': m_type = Type::kIntegerBin; break;
                            case 'P': m_flags |= Flags::kUppercase; USF_FALLTHROUGH;
                            case 'p': m_type = Type::kPointer; break;
                            case 'F': m_flags |= Flags::kUppercase; USF_FALLTHROUGH;
                            case 'f': m_type = Type::kFloatFixed; break;
                            case 'E': m_flags |= Flags::kUppercase; USF_FALLTHROUGH;
                            case 'e': m_type = Type::kFloatScientific; break;
                            case 'G': m_flags |= Flags::kUppercase; USF_FALLTHROUGH;
                            case 'g': m_type = Type::kFloatGeneral; break;
                            case 's': m_type = Type::kString; break;
                            default: m_type = Type::kInvalid; break;
                        }

                        USF_ENFORCE(m_type != Type::kInvalid, std::runtime_error);
                    }

                    // Validate the read format spec!

                    if (fill_zero)
                    {
                        // Fill zero flag has precedence over any other alignment and fill character.
                        m_flags     = static_cast<u8>((m_flags & (~Flags::kAlignBitmask)) | Flags::kAlignNumeric);
                        m_fill_char = '0';
                    }

                    if (align() == Align::kNumeric)
                    {
                        // Numeric alignment are only valid for numeric and pointer types.
                        USF_ENFORCE(type_is_numeric() || type_is_pointer(), std::runtime_error);
                    }

                    if (sign() != Sign::kNone)
                    {
                        // Sign is only valid for numeric types.
                        USF_ENFORCE(type_is_numeric(), std::runtime_error);
                    }

                    if (hash())
                    {
                        // Alternative format is valid for hexadecimal (including
                        // pointers), octal, binary and all floating point types.
                        USF_ENFORCE(type_allow_hash(), std::runtime_error);
                    }

                    if (m_precision != -1)
                    {
                        // Precision is only valid for floating point and string types.
                        USF_ENFORCE(type_is_float() || type_is_string(), std::runtime_error);
                    }
                }

                // Test for unterminated argument format spec.
                USF_ENFORCE(it < fmt.cend() && *it++ == '}', std::runtime_error);

                fmt.remove_prefix(it - fmt.cbegin());
            }

            // Writes the alignment (sign, prefix and fill before) for any
            // argument type. Returns the fill counter to write after argument.
            USF_CPP14_CONSTEXPR int write_alignment(iterator& it, const_iterator end, int digits, const bool negative) const
            {
                digits += sign_width(negative) + prefix_width();

                int fill_after = 0;

                if (width() <= digits)
                {
                    USF_ENFORCE(it + digits < end, std::runtime_error);
                    write_sign(it, negative);
                    write_prefix(it);
                }
                else
                {
                    USF_ENFORCE(it + width() < end, std::runtime_error);

                    int fill_count = width() - digits;

                    const Align al = align();

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

            inline constexpr CharT fill_char() const noexcept { return m_fill_char; }
            inline constexpr Type  type() const noexcept { return m_type; }
            inline constexpr int   width() const noexcept { return static_cast<int>(m_width); }
            inline constexpr int   precision() const noexcept { return static_cast<int>(m_precision); }
            inline constexpr int   index() const noexcept { return static_cast<int>(m_index); }

            inline constexpr Align align() const noexcept { return Align(m_flags & Flags::kAlignBitmask); }
            inline constexpr Sign  sign() const noexcept { return Sign(m_flags & Flags::kSignBitmask); }

            inline constexpr bool is_empty() const noexcept { return (m_flags & Flags::kEmpty) != 0; }
            inline constexpr bool hash() const noexcept { return (m_flags & Flags::kHash) != 0; }
            inline constexpr bool uppercase() const noexcept { return (m_flags & Flags::kUppercase) != 0; }

            inline constexpr bool type_is_none() const noexcept { return m_type == Type::kNone; }
            inline constexpr bool type_is_char() const noexcept { return m_type == Type::kChar; }
            inline constexpr bool type_is_string() const noexcept { return m_type == Type::kString; }
            inline constexpr bool type_is_pointer() const noexcept { return m_type == Type::kPointer; }

            inline constexpr bool type_is_integer() const noexcept { return m_type >= Type::kIntegerDec && m_type <= Type::kIntegerBin; }
            inline constexpr bool type_is_float() const noexcept { return m_type >= Type::kFloatFixed && m_type <= Type::kFloatGeneral; }
            inline constexpr bool type_is_numeric() const noexcept { return m_type >= Type::kIntegerDec && m_type <= Type::kFloatGeneral; }

            inline constexpr bool type_is_integer_dec() const noexcept { return m_type == Type::kIntegerDec; }
            inline constexpr bool type_is_integer_hex() const noexcept { return m_type == Type::kIntegerHex; }
            inline constexpr bool type_is_integer_oct() const noexcept { return m_type == Type::kIntegerOct; }
            inline constexpr bool type_is_integer_bin() const noexcept { return m_type == Type::kIntegerBin; }

            inline constexpr bool type_is_float_fixed() const noexcept { return m_type == Type::kFloatFixed; }
            inline constexpr bool type_is_float_scientific() const noexcept { return m_type == Type::kFloatScientific; }
            inline constexpr bool type_is_float_general() const noexcept { return m_type == Type::kFloatGeneral; }

            inline constexpr bool type_allow_hash() const noexcept
            {
                // Alternative format is valid for hexadecimal (including
                // pointers), octal, binary and all floating point types.
                return m_type >= Type::kIntegerHex && m_type <= Type::kFloatGeneral;
            }

            inline USF_CPP14_CONSTEXPR void default_align_left() noexcept
            {
                if ((m_flags & Flags::kAlignBitmask) == Flags::kAlignNone)
                {
                    m_flags |= Flags::kAlignLeft;
                }
            }

        private:
            // --------------------------------------------------------------------
            // PRIVATE MEMBER FUNCTIONS
            // --------------------------------------------------------------------

            inline constexpr int sign_width(const bool negative) const noexcept { return (!negative && sign() <= Sign::kMinus) ? 0 : 1; }

            inline constexpr int prefix_width() const noexcept
            {
                // Alternative format is valid for hexadecimal (including
                // pointers), octal, binary and all floating point types.
                return (!hash() || type_is_float()) ? 0 : type_is_integer_oct() ? 1 : 2;
            }

            USF_CPP14_CONSTEXPR void write_sign(iterator& it, const bool negative) const noexcept
            {
                if (negative)
                {
                    *it++ = '-';
                }
                else
                {
                    const Sign s = sign();

                    if (s != Sign::kNone)
                    {
                        if (s == Sign::kPlus)
                        {
                            *it++ = '+';
                        }
                        else if (s == Sign::kSpace)
                        {
                            *it++ = ' ';
                        }
                    }
                }
            }

            USF_CPP14_CONSTEXPR void write_prefix(iterator& it) const noexcept
            {
                // Alternative format is valid for hexadecimal (including
                // pointers), octal, binary and all floating point types.
                if (hash() && !type_is_float())
                {
                    *it++ = '0';

                    if (type_is_integer_bin())
                    {
                        *it++ = uppercase() ? 'B' : 'b';
                    }
                    else if (type_is_integer_hex() || type_is_pointer())
                    {
                        *it++ = uppercase() ? 'X' : 'x';
                    }
                }
            }

            // --------------------------------------------------------------------
            // PRIVATE STATIC FUNCTIONS
            // --------------------------------------------------------------------

            // Parses the input as a positive integer that fits into a `u8` type. This
            // function assumes that the first character is a digit and terminates parsing
            // at the presence of the first non-digit character or when value overflows.
            static USF_CPP14_CONSTEXPR u8 parse_positive_small_int(const_iterator& it, const int max_value)
            {
                assert(max_value < 256);

                int value = 0;
                do
                {
                    value = (value * 10) + static_cast<int>(*it++ - '0');
                    USF_ENFORCE(value <= max_value, std::runtime_error); // Check for overflow
                } while (*it >= '0' && *it <= '9');

                return static_cast<u8>(value);
            }

            static USF_CPP14_CONSTEXPR u8 parse_align_flag(const CharT ch) noexcept
            {
                switch (ch)
                {
                    case '<': return Flags::kAlignLeft; break;
                    case '>': return Flags::kAlignRight; break;
                    case '^': return Flags::kAlignCenter; break;
                    case '=': return Flags::kAlignNumeric; break;
                    default: return Flags::kNone; break;
                }
            }

            // --------------------------------------------------------------------
            // PRIVATE MEMBER VARIABLES
            // --------------------------------------------------------------------

            CharT m_fill_char = ' ';
            Type  m_type      = Type::kNone;
            u8    m_flags     = Flags::kEmpty;
            u8    m_width     = 0;
            s8    m_precision = -1;
            s8    m_index     = -1;
        };

        // Again, this class seems to be a template class mainly because of the
        // 'format' function
        template <typename CharT> class Argument
        {
        public:
            // --------------------------------------------------------------------
            // PUBLIC TYPE ALIASES
            // --------------------------------------------------------------------

            using iterator       = CharT*;
            using const_iterator = const CharT*;

            using Format = ArgFormat<CharT>;

            // --------------------------------------------------------------------
            // PUBLIC MEMBER FUNCTIONS
            // --------------------------------------------------------------------

            constexpr Argument() = delete;

            constexpr Argument(const bool value) noexcept
                : m_bool(value)
                , m_type_id(TypeId::kBool)
            {
            }

            constexpr Argument(const CharT value) noexcept
                : m_char(value)
                , m_type_id(TypeId::kChar)
            {
            }

            constexpr Argument(const int32_t value) noexcept
                : m_int32(value)
                , m_type_id(TypeId::kInt32)
            {
            }

            constexpr Argument(const u32 value) noexcept
                : m_uint32(value)
                , m_type_id(TypeId::kUint32)
            {
            }

            constexpr Argument(const int64_t value) noexcept
                : m_int64(value)
                , m_type_id(TypeId::kInt64)
            {
            }

            constexpr Argument(const u64 value) noexcept
                : m_uint64(value)
                , m_type_id(TypeId::kUint64)
            {
            }

            constexpr Argument(const void* value) noexcept
                : m_pointer(reinterpret_cast<std::uintptr_t>(value))
                , m_type_id(TypeId::kPointer)
            {
            }

#if !defined(USF_DISABLE_FLOAT_SUPPORT)
            constexpr Argument(const double value) noexcept
                : m_float(value)
                , m_type_id(TypeId::kFloat)
            {
            }
#endif
            constexpr Argument(const BasicStringView<CharT> value) noexcept
                : m_string(value)
                , m_type_id(TypeId::kString)
            {
            }

            USF_CPP14_CONSTEXPR void format(BasicStringSpan<CharT>& dst, Format& format) const
            {
                iterator it = dst.begin();

                switch (m_type_id)
                {
                    case TypeId::kBool: format_bool(it, dst.end(), format, m_bool); break;
                    case TypeId::kChar: format_char(it, dst.end(), format, m_char); break;
                    case TypeId::kInt32: format_integer(it, dst.end(), format, m_int32); break;
                    case TypeId::kUint32: format_integer(it, dst.end(), format, m_uint32); break;
                    case TypeId::kInt64: format_integer(it, dst.end(), format, m_int64); break;
                    case TypeId::kUint64: format_integer(it, dst.end(), format, m_uint64); break;
                    case TypeId::kPointer: format_pointer(it, dst.end(), format, m_pointer); break;
#if !defined(USF_DISABLE_FLOAT_SUPPORT)
                    case TypeId::kFloat: format_float(it, dst.end(), format, m_float); break;
#endif
                    case TypeId::kString: format_string(it, dst.end(), format, m_string); break;
                }

                dst.remove_prefix(it - dst.begin());
            }

        private:
            // --------------------------------------------------------------------
            // PRIVATE STATIC FUNCTIONS
            // --------------------------------------------------------------------

            static USF_CPP14_CONSTEXPR void format_bool(iterator& it, const_iterator end, const Format& format, const bool value)
            {
                if (format.type_is_none())
                {
                    format_string(it, end, format, value ? "true" : "false", value ? 4 : 5);
                }
                else if (format.type_is_integer())
                {
                    format_integer(it, end, format, static_cast<u32>(value));
                }
                else
                {
                    // Argument type / format mismatch
                    USF_CONTRACT_VIOLATION(std::runtime_error);
                }
            }

            static USF_CPP14_CONSTEXPR void format_char(iterator& it, const_iterator end, Format& format, const CharT value)
            {
                if (format.type_is_none() || format.type_is_char())
                {
                    // Characters and strings align to left by default.
                    format.default_align_left();

                    const int fill_after = format.write_alignment(it, end, 1, false);
                    *it++                = value;
                    CharTraits::assign(it, format.fill_char(), fill_after);
                }
                else if (format.type_is_integer())
                {
                    format_integer(it, end, format, static_cast<int32_t>(value));
                }
                else
                {
                    // Argument type / format mismatch
                    USF_CONTRACT_VIOLATION(std::runtime_error);
                }
            }

            template <typename T, typename std::enable_if<std::is_signed<T>::value, bool>::type = true> static USF_CPP14_CONSTEXPR void format_integer(iterator& it, const_iterator end, const Format& format, const T value)
            {
                using unsigned_type = typename std::make_unsigned<T>::type;

                const bool negative = (value < 0);
                const auto uvalue   = static_cast<unsigned_type>(negative ? -value : value);

                format_integer(it, end, format, uvalue, negative);
            }

            template <typename T, typename std::enable_if<std::is_unsigned<T>::value, bool>::type = true> static USF_CPP14_CONSTEXPR void format_integer(iterator& it, const_iterator end, const Format& format, const T value, const bool negative = false)
            {
                int fill_after = 0;

                if (format.type_is_none() || format.type_is_integer_dec())
                {
                    const auto digits = Integer::count_digits_dec(value);
                    fill_after        = format.write_alignment(it, end, digits, negative);
                    it += digits;
                    Integer::convert_dec(it, value);
                }
                else if (format.type_is_integer_hex())
                {
                    const auto digits = Integer::count_digits_hex(value);
                    fill_after        = format.write_alignment(it, end, digits, negative);
                    it += digits;
                    Integer::convert_hex(it, value, format.uppercase());
                }
                else if (format.type_is_integer_oct())
                {
                    const auto digits = Integer::count_digits_oct(value);
                    fill_after        = format.write_alignment(it, end, digits, negative);
                    it += digits;
                    Integer::convert_oct(it, value);
                }
                else if (format.type_is_integer_bin())
                {
                    const auto digits = Integer::count_digits_bin(value);
                    fill_after        = format.write_alignment(it, end, digits, negative);
                    it += digits;
                    Integer::convert_bin(it, value);
                }
                else
                {
                    // Argument type / format mismatch
                    USF_CONTRACT_VIOLATION(std::runtime_error);
                }

                CharTraits::assign(it, format.fill_char(), fill_after);
            }

            static USF_CPP14_CONSTEXPR void format_pointer(iterator& it, const_iterator end, const Format& format, const std::uintptr_t value)
            {
                if (format.type_is_none() || format.type_is_pointer())
                {
#if defined(USF_TARGET_64_BITS)
                    const auto ivalue = static_cast<u64>(value);
#else
                    const auto ivalue = static_cast<u32>(value);
#endif
                    const auto digits     = Integer::count_digits_hex(ivalue);
                    const auto fill_after = format.write_alignment(it, end, digits, false);
                    it += digits;
                    Integer::convert_hex(it, ivalue, format.uppercase());
                    CharTraits::assign(it, format.fill_char(), fill_after);
                }
                else
                {
                    // Argument type / format mismatch
                    USF_CONTRACT_VIOLATION(std::runtime_error);
                }
            }

#if !defined(USF_DISABLE_FLOAT_SUPPORT)
            static USF_CPP14_CONSTEXPR void format_float(iterator& it, const_iterator end, const Format& format, double value)
            {
                // Test for argument type / format match
                USF_ENFORCE(format.type_is_none() || format.type_is_float(), std::runtime_error);

                if (std::isnan(value))
                {
                    format_string(it, end, format, format.uppercase() ? "NAN" : "nan", 3);
                }
                else
                {
                    const bool negative = std::signbit(value);

                    if (std::isinf(value))
                    {
                        format_string(it, end, format, format.uppercase() ? "INF" : "inf", 3, negative);
                    }
                    else
                    {
                        if (negative)
                        {
                            value = -value;
                        }

                        struct fp_t
                        {
                            union
                            {
                                double d;
                                u64    i;
                            };
                        };

                        const fp_t fp_value{{value}};

                        if (fp_value.i == 0)
                        {
                            format_float_zero(it, end, format, negative);
                        }
                        else if (value >= 1E-19 && value <= 1.8446744E19)
                        {
                            int precision = format.precision();

                            if (precision < 0)
                            {
                                precision = 6;
                            }

                            bool format_fixed        = format.type_is_float_fixed();
                            bool significant_figures = false;

                            if (format.type_is_none() || format.type_is_float_general())
                            {
                                // General format
                                significant_figures = true;

                                if (precision > 0)
                                {
                                    --precision;
                                }
                            }

                            CharT significand[36]{}; // 34 characters should be the maximum size needed
                            int   exponent = 0;

                            const auto significand_size = Float::convert(significand, exponent, value, format_fixed, precision);

                            if (significant_figures)
                            {
                                if (exponent >= -4 && exponent <= precision)
                                {
                                    format_fixed = true;
                                }

                                if (!format.hash())
                                {
                                    precision = significand_size - 1;
                                }

                                if (format_fixed)
                                {
                                    precision -= exponent;
                                }
                            }

                            int fill_after = 0;

                            if (format_fixed)
                            {
                                // Fixed point format
                                if (exponent < 0)
                                {
                                    // 0.<0>SIGNIFICAND[0:N]<0>

                                    const int full_digits = precision + 2;
                                    fill_after            = format.write_alignment(it, end, full_digits, negative);

                                    *it++ = '0';
                                    *it++ = '.';

                                    int zero_digits = -exponent - 1;
                                    CharTraits::assign(it, '0', zero_digits);
                                    CharTraits::copy(it, significand, significand_size);

                                    // Padding is needed if conversion function removes trailing zeros.
                                    zero_digits = precision - zero_digits - significand_size;
                                    CharTraits::assign(it, '0', zero_digits);
                                }
                                else
                                {
                                    const int full_digits = exponent + 1 + precision + static_cast<int>(precision > 0 || format.hash());
                                    fill_after            = format.write_alignment(it, end, full_digits, negative);

                                    const int ipart_digits = exponent + 1;

                                    if (ipart_digits >= significand_size)
                                    {
                                        // [SIGNIFICAND]<0><.><0>

                                        CharTraits::copy(it, significand, significand_size);
                                        CharTraits::assign(it, '0', ipart_digits - significand_size);

                                        if (precision > 0 || format.hash())
                                        {
                                            *it++ = '.';
                                        }

                                        if (precision > 0)
                                        {
                                            CharTraits::assign(it, '0', precision);
                                        }
                                    }
                                    else
                                    {
                                        // SIGNIFICAND[0:x].SIGNIFICAND[x:N]<0>

                                        CharTraits::copy(it, significand, ipart_digits);
                                        *it++ = '.';

                                        const int copy_size = significand_size - ipart_digits;
                                        CharTraits::copy(it, significand + ipart_digits, copy_size);

                                        // Padding is needed if conversion function removes trailing zeros.
                                        CharTraits::assign(it, '0', precision - copy_size);
                                    }
                                }
                            }
                            else
                            {
                                // Exponent format
                                // SIGNIFICAND[0:N]<.>eEXP
                                // OR
                                // SIGNIFICAND[0].SIGNIFICAND[1:N]<0>eEXP

                                const int full_digits = 5 + precision + static_cast<int>(precision > 0 || format.hash());
                                fill_after            = format.write_alignment(it, end, full_digits, negative);

                                *it++ = *significand;

                                if (precision > 0 || format.hash())
                                {
                                    *it++ = '.';

                                    const int copy_size = significand_size - 1;
                                    CharTraits::copy(it, significand + 1, copy_size);
                                    CharTraits::assign(it, '0', precision - copy_size);
                                }

                                write_float_exponent(it, exponent, format.uppercase());
                            }

                            CharTraits::assign(it, format.fill_char(), fill_after);

                            // it += sprintf(it, "[%s] Size:%d Exponent:%d Precision:%d Fixed:%d->", significand, significand_size, exponent, precision, int(format_fixed));
                        }
                        else
                        {
                            format_string(it, end, format, format.uppercase() ? "OVF" : "ovf", 3, negative);
                        }
                    }
                }
            }

            static USF_CPP14_CONSTEXPR void write_float_exponent(iterator& it, int exponent, const bool uppercase) noexcept
            {
                *it++ = uppercase ? 'E' : 'e';

                if (exponent < 0)
                {
                    exponent = -exponent;
                    *it++    = '-';
                }
                else
                {
                    *it++ = '+';
                }

                // No point in making a proper integer to string
                // conversion for exponent since we only support [e-19; e19].
                assert(exponent <= 19);

                if (exponent < 10)
                {
                    *it++ = '0';
                    *it++ = static_cast<CharT>('0' + exponent);
                }
                else
                {
                    *it++ = '1';
                    *it++ = static_cast<CharT>('0' + (exponent - 10));
                }
            }

            static USF_CPP14_CONSTEXPR void format_float_zero(iterator& it, const_iterator end, const Format& format, const bool negative)
            {
                int precision = 0;

                if (format.type_is_float_fixed() || format.type_is_float_scientific())
                {
                    precision = format.precision();
                }

                int digits = 1;

                if (precision > 0)
                {
                    digits += precision + 1;
                }

                if (format.type_is_float_scientific())
                {
                    digits += 4;
                }

                const int fill_after = format.write_alignment(it, end, digits, negative);

                *it++ = '0';

                if (precision > 0)
                {
                    *it++ = '.';
                    CharTraits::assign(it, '0', precision);
                }

                if (format.type_is_float_scientific())
                {
                    *it++ = format.uppercase() ? 'E' : 'e';
                    *it++ = '+';
                    *it++ = '0';
                    *it++ = '0';
                }

                CharTraits::assign(it, format.fill_char(), fill_after);
            }
#endif // !defined(USF_DISABLE_FLOAT_SUPPORT)

            static USF_CPP14_CONSTEXPR void format_string(iterator& it, const_iterator end, Format& format, const BasicStringView<CharT>& str)
            {
                // Test for argument type / format match
                USF_ENFORCE(format.type_is_none() || format.type_is_string(), std::runtime_error);

                // Characters and strings align to left by default.
                format.default_align_left();

                // If precision is specified use it up to string size.
                const int str_length = (format.precision() == -1) ? static_cast<int>(str.size()) : std::min(static_cast<int>(format.precision()), static_cast<int>(str.size()));

                format_string(it, end, format, str.data(), str_length);
            }

            template <typename CharSrc, typename std::enable_if<std::is_convertible<CharSrc, CharT>::value, bool>::type = true>
            static USF_CPP14_CONSTEXPR void format_string(iterator& it, const_iterator end, const Format& format, const CharSrc* str, const int str_length, const bool negative = false)
            {
                const int fill_after = format.write_alignment(it, end, str_length, negative);

                CharTraits::copy(it, str, str_length);
                CharTraits::assign(it, format.fill_char(), fill_after);
            }

            // --------------------------------------------------------------------
            // PRIVATE MEMBER VARIABLES
            // --------------------------------------------------------------------

            enum class TypeId
            {
                kBool = 0,
                kChar,
                kInt32,
                kUint32,
                kInt64,
                kUint64,
                kPointer,
#if !defined(USF_DISABLE_FLOAT_SUPPORT)
                kFloat,
#endif
                kString,
                kCustom
            };

            union
            {
                bool           m_bool;
                CharT          m_char;
                int32_t        m_int32;
                u32            m_uint32;
                int64_t        m_int64;
                u64            m_uint64;
                std::uintptr_t m_pointer;
#if !defined(USF_DISABLE_FLOAT_SUPPORT)
                double m_float;
#endif
                BasicStringView<CharT> m_string;
            };

            TypeId m_type_id;
        };

        // Boolean
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const bool arg) { return arg; }

        // Character (char)
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const char arg) { return static_cast<CharT>(arg); }

        // Character (CharT != char)
        template <typename CharT, typename std::enable_if<!std::is_same<CharT, char>::value, bool>::type = true> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const CharT arg) { return arg; }

        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const s8 arg) { return static_cast<int32_t>(arg); }
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const uint8_t arg) { return static_cast<u32>(arg); }
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const int16_t arg) { return static_cast<int32_t>(arg); }
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const uint16_t arg) { return static_cast<u32>(arg); }
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const int arg) { return static_cast<int32_t>(arg); }
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const unsigned int arg) { return static_cast<u32>(arg); }

#if (__LONG_MAX__ != __LONG_LONG_MAX__)

        // 32 bit signed integer
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const long int arg) { return static_cast<int32_t>(arg); }

        // 32 bit unsigned integer
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const unsigned long int arg) { return static_cast<u32>(arg); }

#endif // (__LONG_MAX__ != __LONG_LONG_MAX__)

        // 64 bit signed integer
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const int64_t arg)
        {
            if (arg >= std::numeric_limits<int32_t>::min() && arg <= std::numeric_limits<int32_t>::max())
            {
                return static_cast<int32_t>(arg);
            }

            return arg;
        }

        // 64 bit unsigned integer
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const u64 arg)
        {
            if (arg <= std::numeric_limits<u32>::max())
            {
                return static_cast<u32>(arg);
            }

            return arg;
        }

        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(void* arg) { return arg; }
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const void* arg) { return arg; }

#if !defined(USF_DISABLE_FLOAT_SUPPORT)
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(float arg) { return static_cast<double>(arg); }
        template <typename CharT> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(double arg) { return arg; }
#endif // !defined(USF_DISABLE_FLOAT_SUPPORT)

        // String (convertible to string view)
        template <typename CharT, typename T, typename std::enable_if<std::is_convertible<T, BasicStringView<CharT>>::value, bool>::type = true> inline USF_CPP14_CONSTEXPR Argument<CharT> make_argument(const T& arg) { return BasicStringView<CharT>(arg); }

        template <typename CharT> USF_CPP14_CONSTEXPR void parse_format_string(BasicStringSpan<CharT>& str, BasicStringView<CharT>& fmt)
        {
            CharT*       str_it = str.begin();
            const CharT* fmt_it = fmt.cbegin();

            while (fmt_it < fmt.cend() && str_it < str.end())
            {
                if (*fmt_it == '{')
                {
                    if (*(fmt_it + 1) == '{')
                    {
                        // Found '{{' escape character, skip the first and copy the second '{'.
                        ++fmt_it;
                        *str_it++ = *fmt_it++;
                    }
                    else
                    {
                        // A type format should follow...
                        break;
                    }
                }
                else if (*fmt_it == '}')
                {
                    USF_ENFORCE(*(fmt_it + 1) == '}', std::runtime_error);

                    // Found '}}' escape character, skip the first and copy the second '}'.
                    ++fmt_it;
                    *str_it++ = *fmt_it++;
                }
                else
                {
                    // Copy literal text
                    *str_it++ = *fmt_it++;
                }
            }

            USF_ENFORCE(str_it < str.end(), std::runtime_error);

            str.remove_prefix(str_it - str.begin());
            fmt.remove_prefix(fmt_it - fmt.cbegin());
        }

        template <typename CharT> USF_CPP14_CONSTEXPR void process(BasicStringSpan<CharT>& str, BasicStringView<CharT>& fmt, const Argument<CharT>* const args, const int arg_count)
        {
            // Argument's sequential index
            int arg_seq_index = 0;

            parse_format_string(str, fmt);

            while (!fmt.empty())
            {
                ArgFormat<CharT> format(fmt, arg_count);

                // Determine which argument index to use, sequential or positional.
                int arg_index = format.index();

                if (arg_index < 0)
                {
                    USF_ENFORCE(arg_seq_index < arg_count, std::runtime_error);
                    arg_index = arg_seq_index++;
                }

                args[arg_index].format(str, format);

                parse_format_string(str, fmt);
            }
        }

        template <typename CharT, typename... Args> USF_CPP14_CONSTEXPR BasicStringSpan<CharT> basic_format_to(BasicStringSpan<CharT> str, BasicStringView<CharT> fmt)
        {
            auto str_begin = str.begin();

            parse_format_string(str, fmt);

            USF_ENFORCE(fmt.empty(), std::runtime_error);

#if !defined(USF_DISABLE_STRING_TERMINATION)
            // If not disabled in configuration, null terminate the resulting string.
            str[0] = CharT{};
#endif

            // Return a string span to the resulting string
            return BasicStringSpan<CharT>(str_begin, str.begin());
        }

        template <typename CharT, typename... Args> USF_CPP14_CONSTEXPR BasicStringSpan<CharT> basic_format_to(BasicStringSpan<CharT> str, BasicStringView<CharT> fmt, Args&&... args)
        {
            auto str_begin = str.begin();

            const Argument<CharT> arguments[sizeof...(Args)]{make_argument<CharT>(args)...};

            process(str, fmt, arguments, static_cast<int>(sizeof...(Args)));

#if !defined(USF_DISABLE_STRING_TERMINATION)
            // If not disabled in configuration, null terminate the resulting string.
            str[0] = CharT{};
#endif

            // Return a string span to the resulting string
            return BasicStringSpan<CharT>(str_begin, str.begin());
        }

        template <typename CharT, typename... Args> USF_CPP14_CONSTEXPR CharT* basic_format_to(CharT* str, const std::ptrdiff_t str_count, BasicStringView<CharT> fmt, Args&&... args)
        {
            return basic_format_to(BasicStringSpan<CharT>(str, str_count), fmt, args...).end();
        }

        // There is really NO need to make all of the above using templates, the only 2 'smart' objects you need are:
        //   - string reader
        //   - string writer
        //
        // And we basically already have this with crunes_t and runes_t.
        // 


        // ----------------------------------------------------------------------------
        // Formats a char string
        // ---------------------------------------------------------------------------
        template <typename... Args> USF_CPP14_CONSTEXPR StringSpan format_to(StringSpan str, StringView fmt, Args&&... args) { return basic_format_to(str, fmt, args...); }
        template <typename... Args> USF_CPP14_CONSTEXPR char*      format_to(char* str, const std::ptrdiff_t str_count, StringView fmt, Args&&... args) { return basic_format_to(str, str_count, fmt, args...); }

        static void Test()
        {
            BasicStringView<char> s("=+ 10.5d");
            ArgFormat<char>       f(s, 2);

            char str[128];
            format_to(str, 128, "{:14}", false);
        }

        bool to(ascii::prune str, ascii::prune end, ascii::prune fmt, arg_t a, arg_t b, arg_t c, arg_t d, arg_t e, arg_t f, arg_t g, arg_t h, arg_t i, arg_t j, arg_t k, arg_t l) {}

    } // namespace fmt
} // namespace ncore