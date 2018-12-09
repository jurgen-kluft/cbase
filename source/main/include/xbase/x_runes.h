#ifndef __XBASE_RUNES_H__
#define __XBASE_RUNES_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//==============================================================================
// INCLUDES
//==============================================================================

namespace xcore
{
#undef printf

    namespace ascii
    {
        typedef char        rune;
        typedef char*       prune;
        typedef const char* pcrune;
        struct runes
        {
            inline runes() : m_str(nullptr), m_end(nullptr), m_eos(nullptr) {}
            inline runes(prune _str, prune _end, pcrune _eos) : m_str(_str), m_end(_end), m_eos(_eos) {}
            inline runes(runes const& other) : m_str(other.m_str), m_end(other.m_end), m_eos(other.m_eos) {}
            s32  size() const { return (s32)(m_end - m_str); }
            s32  cap() const { return (s32)(m_eos - m_str); }
            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                if (m_str != nullptr)
                {
                    m_end    = m_str;
                    m_end[0] = '\0';
                }
            }
            runes operator=(runes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                m_eos = other.m_eos;
                return *this;
            }
            prune  m_str;
            prune  m_end;
            pcrune m_eos;
        };
        struct crunes
        {
            inline crunes() : m_str(nullptr), m_end(nullptr) {}
            inline crunes(pcrune _str) : m_str(_str), m_end(_str)
            {
                while (*m_end != '\0')
                    ++m_end;
            }
            inline crunes(pcrune _str, pcrune _end) : m_str(_str), m_end(_end)
            {
                if (m_end == nullptr)
                {
                    m_end = m_str;
                    while (*m_end != '\0')
                        ++m_end;
                }
            }
            inline crunes(runes const& other) : m_str(other.m_str), m_end(other.m_end) {}
            inline crunes(crunes const& other) : m_str(other.m_str), m_end(other.m_end) {}
            s32  size() const { return (s32)(m_end - m_str); }
            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                m_str = "";
                m_end = m_str;
            }
            crunes& operator=(crunes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                return *this;
            }
            pcrune m_str;
            pcrune m_end;
        };

        template <s32 L> class runez : public runes
        {
        public:
            enum
            {
                SIZE = L + 1
            };
            rune m_str[SIZE];
            inline runez() : runes(m_str, m_str, &m_str[SIZE - 1]) { m_str[SIZE - 1] = 0; }
            void reset()
            {
                m_end    = m_str;
                m_str[0] = '\0';
            }
        };

        class alloc
        {
        public:
            virtual runes allocate(s32 len, s32 cap) = 0;
            virtual void  deallocate(runes& slice)   = 0;
        };
    } // namespace ascii

    namespace utf8
    {
        typedef uchar8        rune;
        typedef uchar8*       prune;
        typedef const uchar8* pcrune;
        struct runes
        {
            inline runes() : m_str(nullptr), m_end(nullptr), m_eos(nullptr) {}
            inline runes(prune _str, prune _end, pcrune _eos) : m_str(_str), m_end(_end), m_eos(_eos) {}
            inline runes(runes const& other) : m_str(other.m_str), m_end(other.m_end), m_eos(other.m_eos) {}
            s32  size() const { return (s32)(m_end - m_str); }
            s32  cap() const { return (s32)(m_eos - m_str); }
            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                if (m_str != nullptr)
                {
                    m_end    = m_str;
                    m_end[0] = '\0';
                }
            }
            runes& operator=(runes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                m_eos = other.m_eos;
                return *this;
            }
            prune  m_str;
            prune  m_end;
            pcrune m_eos;
        };
        struct crunes
        {
            inline crunes() : m_str(nullptr), m_end(nullptr) {}
            inline crunes(pcrune _str, pcrune _end) : m_str(_str), m_end(_end) {}
            inline crunes(runes const& other) : m_str(other.m_str), m_end(other.m_end) {}
            inline crunes(crunes const& other) : m_str(other.m_str), m_end(other.m_end) {}
            s32  size() const { return (s32)(m_end - m_str); }
            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                m_str = (pcrune) "";
                m_end = m_str;
            }
            crunes& operator=(crunes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                return *this;
            }
            pcrune m_str;
            pcrune m_end;
        };
        template <s32 L> class runez : public runes
        {
        public:
            enum
            {
                SIZE = L + 1
            };
            rune m_str[SIZE];
            inline runez() : runes(m_str, m_str, &m_str[SIZE - 1]) { m_str[SIZE - 1] = 0; }
        };

        class alloc
        {
        public:
            virtual runes allocate(s32 len, s32 cap) = 0;
            virtual void  deallocate(runes& slice)   = 0;
        };
    } // namespace utf8

    namespace utf16
    {
        typedef uchar16        rune;
        typedef uchar16*       prune;
        typedef const uchar16* pcrune;
        struct runes
        {
            inline runes() : m_str(nullptr), m_end(nullptr), m_eos(nullptr) {}
            inline runes(prune _str, prune _end, pcrune _eos) : m_str(_str), m_end(_end), m_eos(_eos) {}
            inline runes(runes const& other) : m_str(other.m_str), m_end(other.m_end), m_eos(other.m_eos) {}

            s32  size() const { return (s32)(m_end - m_str); }
            s32  cap() const { return (s32)(m_eos - m_str); }
            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                if (m_str != nullptr)
                {
                    m_end    = m_str;
                    m_end[0] = '\0';
                }
            }

            runes& operator=(runes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                m_eos = other.m_eos;
                return *this;
            }

            bool operator==(runes const& other) const
            {
                pcrune s1 = other.m_str;
                pcrune s2 = m_str;
                while (s1 < other.m_end && s2 < m_end)
                {
                    if (*s1++ != *s2++)
                        return false;
                }
                return (s1 == other.m_end && s2 == m_end);
            }

            prune  m_str;
            prune  m_end;
            pcrune m_eos;
        };
        struct crunes
        {
            inline crunes() : m_str(nullptr), m_end(nullptr) {}
            inline crunes(pcrune _str, pcrune _end) : m_str(_str), m_end(_end) {}
            inline crunes(runes const& other) : m_str(other.m_str), m_end(other.m_end) {}
            inline crunes(crunes const& other) : m_str(other.m_str), m_end(other.m_end) {}
            s32  size() const { return (s32)(m_end - m_str); }
            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                m_str = (pcrune) "\0\0\0\0";
                m_end = m_str;
            }

            crunes& operator=(crunes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                return *this;
            }
            pcrune m_str;
            pcrune m_end;
        };
        template <s32 L> class runez : public runes
        {
        public:
            enum
            {
                SIZE = L + 1
            };
            rune m_str[SIZE];
            inline runez() : runes(m_str, m_str, &m_str[SIZE - 1]) { m_str[SIZE - 1] = 0; }
            inline runez(const char* str) : runes(m_str, m_str, &m_str[SIZE - 1])
            {
                m_str[SIZE - 1] = 0;
                while (*str != '\0' && m_str < m_eos)
                {
                    *m_end++ = *str;
                }
                *m_end = '\0';
            }
        };

        class alloc
        {
        public:
            virtual runes allocate(s32 len, s32 cap) = 0;
            virtual void  deallocate(runes& slice)   = 0;
        };
    } // namespace utf16

    namespace utf32
    {
        typedef uchar32        rune;
        typedef uchar32*       prune;
        typedef const uchar32* pcrune;
        struct runes
        {
            inline runes() : m_str(nullptr), m_end(nullptr), m_eos(nullptr) {}
            inline runes(prune _str, prune _end, pcrune _eos) : m_str(_str), m_end(_end), m_eos(_eos) {}
            inline runes(runes const& other) : m_str(other.m_str), m_end(other.m_end), m_eos(other.m_eos) {}

            s32  size() const { return (s32)(m_end - m_str); }
            s32  cap() const { return (s32)(m_eos - m_str); }
            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                if (m_str != nullptr)
                {
                    m_end    = m_str;
                    m_end[0] = '\0';
                }
            }

            runes& operator=(runes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                m_eos = other.m_eos;
                return *this;
            }
            prune  m_str;
            prune  m_end;
            pcrune m_eos;
        };
        struct crunes
        {
            inline crunes() : m_str(nullptr), m_end(nullptr) {}
            inline crunes(pcrune _str, pcrune _end) : m_str(_str), m_end(_end) {}
            inline crunes(runes const& other) : m_str(other.m_str), m_end(other.m_end) {}
            inline crunes(crunes const& other) : m_str(other.m_str), m_end(other.m_end) {}

            s32  size() const { return (s32)(m_end - m_str); }
            bool is_empty() const { return m_str == m_end; }
            void clear()
            {
                m_str = (pcrune) "\0\0\0\0";
                m_end = m_str;
            }

            crunes& operator=(crunes const& other)
            {
                m_str = other.m_str;
                m_end = other.m_end;
                return *this;
            }
            pcrune m_str;
            pcrune m_end;
        };
        template <s32 L> class runez : public runes
        {
        public:
            enum
            {
                SIZE = L + 1
            };
            rune m_str[SIZE];
            inline runez() : runes(m_str, m_str, &m_str[SIZE - 1]) { m_str[SIZE - 1] = 0; }
        };

        class alloc
        {
        public:
            virtual runes allocate(s32 len, s32 cap) = 0;
            virtual void  deallocate(runes& slice)   = 0;
        };
    } // namespace utf32
} // namespace xcore

namespace xcore
{
    // NOTE:
    //   UTF string manipulation is the easiest with UTF-32 since it is the only UTF format that
    //   has a fixed rune size and thus is easier to index directly than UTF-8 or UTF-16.
    //

    // ASCII, UTF-8, UTF-16, UTF-32 conversions
    namespace utf
    {
        enum ETermType
        {
            TERMINATOR_NONE  = 0,
            TERMINATOR_WRITE = 1
        };

        uchar32 peek(ascii::crunes const& str);
        uchar32 peek(utf8::crunes const& str);
        uchar32 peek(utf16::crunes const& str);
        uchar32 peek(utf32::crunes const& str);

        uchar32 read(ascii::rune const*& str, ascii::rune const* end);
        uchar32 read(utf8::rune const*& str, utf8::rune const* end);
        uchar32 read(utf16::rune const*& str, utf16::rune const* end);
        uchar32 read(utf32::rune const*& str, utf32::rune const* end);

        uchar32 read(ascii::rune*& str, ascii::rune const* end);
        uchar32 read(utf8::rune*& str, utf8::rune const* end);
        uchar32 read(utf16::rune*& str, utf16::rune const* end);
        uchar32 read(utf32::rune*& str, utf32::rune const* end);

        uchar32 read(ascii::crunes& str);
        uchar32 read(utf8::crunes& str);
        uchar32 read(utf16::crunes& str);
        uchar32 read(utf32::crunes& str);

        void write(uchar32 c, uchar*& str, const uchar* end);
        void write(uchar32 c, uchar8*& str, const uchar8* end);
        void write(uchar32 c, uchar16*& str, const uchar16* end);
        void write(uchar32 c, uchar32*& str, const uchar32* end);

        bool write(uchar32 c, ascii::runes& str);
        bool write(uchar32 c, utf8::runes& str);
        bool write(uchar32 c, utf16::runes& str);
        bool write(uchar32 c, utf32::runes& str);

        void copy(ascii::crunes const& sstr, ascii::runes& dstr, ETermType term_type = TERMINATOR_WRITE);
        void copy(utf32::crunes const& sstr, ascii::runes& dstr, ETermType term_type = TERMINATOR_WRITE);

        void copy(ascii::crunes const& sstr, utf32::runes& dstr, ETermType term_type = TERMINATOR_WRITE);
        void copy(utf32::crunes const& sstr, utf32::runes& dstr, ETermType term_type = TERMINATOR_WRITE);

        s32 size(uchar32 c);

        bool is_eos(uchar const* str);
        bool is_eos(uchar8 const* str);
        bool is_eos(uchar16 const* str);
        bool is_eos(uchar32 const* str);

        bool can_read(ascii::runes const& str);
        bool can_read(utf8::runes const& str);
        bool can_read(utf16::runes const& str);
        bool can_read(utf32::runes const& str);
        bool can_read(ascii::crunes const& str);
        bool can_read(utf8::crunes const& str);
        bool can_read(utf16::crunes const& str);
        bool can_read(utf32::crunes const& str);

        bool can_write(ascii::runes const& str);
        bool can_write(utf8::runes const& str);
        bool can_write(utf16::runes const& str);
        bool can_write(utf32::runes const& str);

        bool read_is_crln(ascii::runes const& str);
        bool read_is_crln(utf8::runes const& str);
        bool read_is_crln(utf16::runes const& str);
        bool read_is_crln(utf32::runes const& str);
        bool read_is_crln(ascii::crunes const& str);
        bool read_is_crln(utf8::crunes const& str);
        bool read_is_crln(utf16::crunes const& str);
        bool read_is_crln(utf32::crunes const& str);
    }; // namespace utf

    namespace ascii
    {
        inline s32 size(crunes const& _str) { return _str.size(); }

        inline prune endof(prune str, pcrune eos)
        {
            if (eos == nullptr)
            {
                prune end = str;
                while (*end != '\0')
                    end++;
                return end;
            }
            else
            {
                prune end = str;
                while (*end != '\0' && end < eos)
                    end++;
                return end;
            }
        }
        inline pcrune endof(pcrune str, pcrune eos)
        {
            if (eos == nullptr)
            {
                pcrune end = str;
                while (*end != '\0')
                    end++;
                return end;
            }
            else
            {
                pcrune end = str;
                while (*end != '\0' && end < eos)
                    end++;
                return end;
            }
        }

        inline s32 len(pcrune str, pcrune end)
        {
            end = endof(str, end);
            return s32(end - str);
        }

        crunes find(crunes const& _str, uchar32 _c, bool inCaseSensitive = true);
        runes  find(runes const& _str, uchar32 _c, bool inCaseSensitive = true);
        crunes findLast(crunes const& _str, uchar32 _c, bool inCaseSensitive = true);
        runes  findLast(runes const& _str, uchar32 _c, bool inCaseSensitive = true);

        crunes find(crunes const& str, crunes const& find, bool inCaseSensitive = true);
        runes  find(runes const& str, crunes const& find, bool inCaseSensitive = true);
        crunes findLast(crunes const& str, crunes const& find, bool inCaseSensitive = true);
        runes  findLast(runes const& str, crunes const& find, bool inCaseSensitive = true);

        crunes findOneOf(crunes const& str, crunes const& set, bool inCaseSensitive = true);
        runes  findOneOf(runes const& str, crunes const& set, bool inCaseSensitive = true);

        runes findSelectUntil(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectUntil(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectUntil(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectUntil(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findSelectUntilIncluded(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectUntilIncluded(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectUntilIncluded(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectUntilIncluded(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        runes findSelectAfter(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectAfter(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectAfter(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectAfter(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        runes selectBetween(const runes& inStr, uchar32 inLeft, uchar32 inRight);
        runes selectNextBetween(const runes& inStr, const runes& inSelection, uchar32 inLeft, uchar32 inRight);

        runes selectBetweenLast(const runes& inStr, uchar32 inLeft, uchar32 inRight);
        runes selectPreviousBetween(const runes& inStr, const runes& inSelection, uchar32 inLeft, uchar32 inRight);

        runes selectUntilEndExcludeSelection(const runes& inStr, const runes& inSelection);
        runes selectUntilEndIncludeSelection(const runes& inStr, const runes& inSelection);

        s32 compare(crunes const& str1, crunes const& str2, bool inCaseSensitive = true);

        crunes parse(crunes const& str, bool& value);
        crunes parse(crunes const& str, s32& value, s32 base = 10);
        crunes parse(crunes const& str, u32& value, s32 base = 10);
        crunes parse(crunes const& str, s64& value, s32 base = 10);
        crunes parse(crunes const& str, u64& value, s32 base = 10);
        crunes parse(crunes const& str, f32& value);
        crunes parse(crunes const& str, f64& value);

        bool is_decimal(crunes const& str);
        bool is_hexadecimal(crunes const& str, bool with_prefix = false);
        bool is_float(crunes const& str);
        bool is_GUID(crunes const& str);

        void to_string(runes& str, s32 val, s32 base = 10);
        void to_string(runes& str, u32 val, s32 base = 10);
        void to_string(runes& str, s64 val, s32 base = 10);
        void to_string(runes& str, u64 val, s32 base = 10);
        void to_string(runes& str, f32 val, s32 num_fractional_digits = 4);
        void to_string(runes& str, f64 val, s32 num_fractional_digits = 4);

        inline bool is_space(uchar32 c) { return ((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
        inline bool is_upper(uchar32 c) { return ((c >= 'A') && (c <= 'Z')); }
        inline bool is_lower(uchar32 c) { return ((c >= 'a') && (c <= 'z')); }
        inline bool is_alpha(uchar32 c) { return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
        inline bool is_digit(uchar32 c) { return ((c >= '0') && (c <= '9')); }
        inline bool is_hexa(uchar32 c)
        {
            return (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9')));
        }

        void           to_upper(runes& str);
        void           to_lower(runes& str);
        inline uchar32 to_upper(uchar32 c) { return ((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }
        inline uchar32 to_lower(uchar32 c) { return ((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }
        inline u32     to_digit(uchar32 c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
        inline u32     to_number(uchar32 c)
        {
            if (is_digit(c))
                return to_digit(c);
            else if (c >= 'A' && c <= 'F')
                return (c - 'A' + 10);
            else if (c >= 'a' && c <= 'f')
                return (c - 'a' + 10);
            else
                return (c);
        }

        inline bool is_equal(uchar32 a, uchar32 b) { return (a == b); }

        inline bool is_equalfold(uchar32 a, uchar32 b)
        {
            a = to_lower(a);
            b = to_lower(b);
            return (a == b);
        }

        bool        is_upper(crunes const& str);
        bool        is_lower(crunes const& str);
        bool        is_capitalized(crunes const& str);
        bool        is_delimited(crunes const& str, uchar32 delimit_left = '\"', uchar32 delimit_right = '\"');
        inline bool is_quoted(crunes const& str, uchar32 quote = '\"') { return is_delimited(str, quote, quote); }

        bool    starts_with(crunes const& str, uchar32 start);
        bool    starts_with(crunes const& str, crunes const& start);
        bool    ends_with(crunes const& str, uchar32 end_char);
        bool    ends_with(crunes const& str, crunes const& end);
        uchar32 first_char(crunes const& str);
        uchar32 last_char(crunes const& str);

        void removeSelection(runes& str, crunes const& selection);
        void keepOnlySelection(runes& str, crunes const& keep);

        void replaceSelection(runes& str, crunes const& selection, crunes const& replace);
        void replaceSelection(runes& str, crunes const& selection, crunes const& replace, alloc* allocator, s32 size_alignment);

        void findReplace(runes& str, uchar32 find, uchar32 replace, bool inCaseSensitive = true);
        void findReplace(runes& str, crunes const& find, crunes const& replace, bool inCaseSensitive = true);

        void insert(runes& str, crunes const& insert);
        void insert(runes& str, crunes const& insert, alloc* allocator, s32 size_alignment);

        void insert(runes& str, crunes const& selection, crunes const& insert);
        void insert(runes& str, crunes const& selection, crunes const& insert, alloc* allocator, s32 size_alignment);

        void trim(runes&);                               // Trim whitespace from left and right side of xstring
        void trimLeft(runes&);                           // Trim whitespace from left side of xstring
        void trimRight(runes&);                          // Trim whitespace from right side of xstring
        void trim(runes&, uchar32 inChar);               // Trim characters in <inCharSet> from left and right side of xstring
        void trimLeft(runes&, uchar32 inChar);           // Trim character <inChar> from left side of xstring
        void trimRight(runes&, uchar32 inChar);          // Trim character <inChar> from right side of xstring
        void trim(runes&, crunes const& inCharSet);      // Trim characters in <inCharSet> from left and right side of xstring
        void trimLeft(runes&, crunes const& inCharSet);  // Trim characters in <inCharSet> from left side of xstring
        void trimRight(runes&, crunes const& inCharSet); // Trim characters in <inCharSet> from right side of xstring
        void trimQuotes(runes&);                         // Trim double quotes from left and right side of xstring
        void trimQuotes(runes&, uchar32 quote);          // Trim double quotes from left and right side of xstring
        void trimDelimiters(runes&, uchar32 inLeft, uchar32 inRight); // Trim delimiters from left and right side of xstring

        void copy(const crunes& src, runes& dst);
        void copy(const crunes& src, runes& dst, alloc* allocator, s32 size_alignment);

        void concatenate(runes& str, const crunes& concat);
        void concatenate(runes& str, const crunes& concat, alloc* allocator, s32 size_alignment);
        void concatenate(runes& str, const crunes& concat1, const crunes& concat2, alloc* allocator, s32 size_alignment);
    } // namespace ascii

    inline bool operator==(const ascii::crunes& lhs, const ascii::crunes& rhs) { return ascii::compare(lhs, rhs) == 0; }

    namespace utf32
    {
        s32 len(pcrune str, pcrune end);

        prune  endof(prune str, pcrune end);
        pcrune endof(pcrune str, pcrune end);

        crunes find(crunes const& _str, uchar32 _c, bool inCaseSensitive = true);
        runes  find(runes const& _str, uchar32 _c, bool inCaseSensitive = true);
        crunes findLast(crunes const& _str, uchar32 _c, bool inCaseSensitive = true);
        runes  findLast(runes const& _str, uchar32 _c, bool inCaseSensitive = true);

        crunes find(crunes const& str, crunes const& find, bool inCaseSensitive = true);
        runes  find(runes const& str, crunes const& find, bool inCaseSensitive = true);
        crunes findLast(crunes const& str, crunes const& find, bool inCaseSensitive = true);
        runes  findLast(runes const& str, crunes const& find, bool inCaseSensitive = true);

        crunes findOneOf(crunes const& str, crunes const& set, bool inCaseSensitive = true);
        runes  findOneOf(runes const& str, crunes const& set, bool inCaseSensitive = true);

        runes findSelectUntil(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectUntil(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectUntil(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectUntil(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findSelectUntilIncluded(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectUntilIncluded(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectUntilIncluded(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectUntilIncluded(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        runes findSelectAfter(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findSelectAfter(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);
        runes findLastSelectAfter(const runes& inStr, uchar32 inFind, bool inCaseSensitive = true);
        runes findLastSelectAfter(const runes& inStr, const crunes& inFind, bool inCaseSensitive = true);

        runes selectBetween(const runes& inStr, uchar32 inLeft, uchar32 inRight);
        runes selectNextBetween(const runes& inStr, const runes& inSelection, uchar32 inLeft, uchar32 inRight);

        runes selectBetweenLast(const runes& inStr, uchar32 inLeft, uchar32 inRight);
        runes selectPreviousBetween(const runes& inStr, const runes& inSelection, uchar32 inLeft, uchar32 inRight);

        runes selectUntilEndExcludeSelection(const runes& inStr, const runes& inSelection);
        runes selectUntilEndIncludeSelection(const runes& inStr, const runes& inSelection);

        s32 compare(crunes const& str1, crunes const& str2, bool inCaseSensitive = true);

        crunes parse(crunes const& str, bool& value);
        crunes parse(crunes const& str, s32& value, s32 base = 10);
        crunes parse(crunes const& str, u32& value, s32 base = 10);
        crunes parse(crunes const& str, s64& value, s32 base = 10);
        crunes parse(crunes const& str, u64& value, s32 base = 10);
        crunes parse(crunes const& str, f32& value);
        crunes parse(crunes const& str, f64& value);

        bool is_decimal(crunes const& str);
        bool is_hexadecimal(crunes const& str, bool with_prefix = false);
        bool is_float(crunes const& str);
        bool is_GUID(crunes const& str);

        void to_string(runes& str, s32 val, s32 base = 10);
        void to_string(runes& str, u32 val, s32 base = 10);
        void to_string(runes& str, s64 val, s32 base = 10);
        void to_string(runes& str, u64 val, s32 base = 10);
        void to_string(runes& str, f32 val, s32 num_fractional_digits = 4);
        void to_string(runes& str, f64 val, s32 num_fractional_digits = 4);

        inline bool is_space(uchar32 c) { return ((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
        inline bool is_upper(uchar32 c) { return ((c >= 'A') && (c <= 'Z')); }
        inline bool is_lower(uchar32 c) { return ((c >= 'a') && (c <= 'z')); }
        inline bool is_alpha(uchar32 c) { return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
        inline bool is_digit(uchar32 c) { return ((c >= '0') && (c <= '9')); }
        inline bool is_hexa(uchar32 c)
        {
            return (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9')));
        }

        void           to_upper(runes& str);
        void           to_lower(runes& str);
        inline uchar32 to_upper(uchar32 c) { return ((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }
        inline uchar32 to_lower(uchar32 c) { return ((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }
        inline u32     to_digit(uchar32 c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
        inline u32     to_number(uchar32 c)
        {
            if (is_digit(c))
                return to_digit(c);
            else if (c >= 'A' && c <= 'F')
                return (c - 'A' + 10);
            else if (c >= 'a' && c <= 'f')
                return (c - 'a' + 10);
            else
                return (c);
        }

        inline bool is_equal(uchar32 a, uchar32 b) { return (a == b); }

        inline bool is_equalfold(uchar32 a, uchar32 b)
        {
            a = to_lower(a);
            b = to_lower(b);
            return (a == b);
        }

        bool        is_upper(crunes const& str);
        bool        is_lower(crunes const& str);
        bool        is_capitalized(crunes const& str);
        bool        is_delimited(crunes const& str, uchar32 delimit_left = '\"', uchar32 delimit_right = '\"');
        inline bool is_quoted(crunes const& str, uchar32 quote = '\"') { return is_delimited(str, quote, quote); }

        bool    starts_with(crunes const& str, uchar32 start);
        bool    starts_with(crunes const& str, crunes const& start);
        bool    ends_with(crunes const& str, uchar32 end_char);
        bool    ends_with(crunes const& str, crunes const& end);
        uchar32 first_char(crunes const& str);
        uchar32 last_char(crunes const& str);

        void removeSelection(runes& str, crunes const& selection);
        void keepOnlySelection(runes& str, crunes const& keep);

        void replaceSelection(runes& str, crunes const& selection, crunes const& replace);
        void replaceSelection(runes& str, crunes const& selection, crunes const& replace, alloc* allocator, s32 size_alignment);

        void findReplace(runes& str, uchar32 find, uchar32 replace, bool inCaseSensitive = true);
        void findReplace(runes& str, crunes const& find, crunes const& replace, bool inCaseSensitive = true);

        void insert(runes& str, crunes const& insert);
        void insert(runes& str, crunes const& insert, alloc* allocator, s32 size_alignment);

        void insert(runes& str, crunes const& selection, crunes const& insert);
        void insert(runes& str, crunes const& selection, crunes const& insert, alloc* allocator, s32 size_alignment);

        void trim(runes&);                               // Trim whitespace from left and right side of xstring
        void trimLeft(runes&);                           // Trim whitespace from left side of xstring
        void trimRight(runes&);                          // Trim whitespace from right side of xstring
        void trim(runes&, uchar32 inChar);               // Trim characters in <inCharSet> from left and right side of xstring
        void trimLeft(runes&, uchar32 inChar);           // Trim character <inChar> from left side of xstring
        void trimRight(runes&, uchar32 inChar);          // Trim character <inChar> from right side of xstring
        void trim(runes&, crunes const& inCharSet);      // Trim characters in <inCharSet> from left and right side of xstring
        void trimLeft(runes&, crunes const& inCharSet);  // Trim characters in <inCharSet> from left side of xstring
        void trimRight(runes&, crunes const& inCharSet); // Trim characters in <inCharSet> from right side of xstring
        void trimQuotes(runes&);                         // Trim double quotes from left and right side of xstring
        void trimQuotes(runes&, uchar32 quote);          // Trim double quotes from left and right side of xstring
        void trimDelimiters(runes&, uchar32 inLeft, uchar32 inRight); // Trim delimiters from left and right side of xstring

        void copy(const crunes& src, runes& dst);
        void copy(const crunes& src, runes& dst, alloc* allocator, s32 size_alignment);

        void concatenate(runes& str, const crunes& concat);
        void concatenate(runes& str, const crunes& concat, alloc* allocator, s32 size_alignment);
        void concatenate(runes& str, const crunes& concat1, const crunes& concat2, alloc* allocator, s32 size_alignment);

    }; // namespace utf32

}; // namespace xcore

#endif ///< __XBASE_RUNES_H__
