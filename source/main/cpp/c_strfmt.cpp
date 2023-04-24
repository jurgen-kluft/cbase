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
#define USF_CPP14_CONSTEXPR constexpr
#define USF_ALWAYS_INLINE inline

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

        USF_CPP14_CONSTEXPR BasicStringView() noexcept = delete;

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
            , m_end{str + internal::CharTraits::length(str)}
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

    template <typename CharT> class ArgFormat
    {
    public:
        using iterator       = CharT*;
        using const_iterator = const CharT*;

        enum class Align : u8
        {
            kNone    = (0U << 1U),
            kLeft    = (1U << 1U),
            kRight   = (2U << 1U),
            kCenter  = (3U << 1U),
            kNumeric = (4U << 1U)
        };

        enum class Sign : u8
        {
            kNone  = (0U << 4U),
            kMinus = (1U << 4U),
            kPlus  = (2U << 4U),
            kSpace = (3U << 4U)
        };

        enum class Type : u8
        {
            kNone,
            kChar,
            kIntegerDec,
            kIntegerHex,
            kIntegerOct,
            kIntegerBin,
            kPointer,
            kFloatFixed,
            kFloatScientific,
            kFloatGeneral,
            kString,
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
                m_index = static_cast<int8_t>(parse_positive_small_int(it, arg_count));
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

                bool fill_zero = false;

                // Parse fill zero flag
                if (*it == '0')
                {
                    fill_zero = true;
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

                    m_precision = static_cast<int8_t>(parse_positive_small_int(it, 127));
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
        // PRIVATE DEFINITIONS
        // --------------------------------------------------------------------

        enum Flags : u8
        {
            kNone = (0U << 0U),

            kEmpty = (1U << 0U),

            kAlignNone    = (0U << 1U),
            kAlignLeft    = (1U << 1U),
            kAlignRight   = (2U << 1U),
            kAlignCenter  = (3U << 1U),
            kAlignNumeric = (4U << 1U),
            kAlignBitmask = (7U << 1U),

            kSignNone    = (0U << 4U),
            kSignMinus   = (1U << 4U),
            kSignPlus    = (2U << 4U),
            kSignSpace   = (3U << 4U),
            kSignBitmask = (3U << 4U),

            kHash = (1U << 6U),

            kUppercase = (1U << 7U)
        };

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

                // Check for overflow
                USF_ENFORCE(value <= max_value, std::runtime_error);
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

        CharT  m_fill_char = ' ';
        Type   m_type      = Type::kNone;
        u8     m_flags     = Flags::kEmpty;
        u8     m_width     = 0;
        int8_t m_precision = -1;
        int8_t m_index     = -1;
    };

    static void Test()
    {
        BasicStringView s("=+ 10.5d");
        ArgFormat<char> f(s, 2);
    }

} // namespace ncore