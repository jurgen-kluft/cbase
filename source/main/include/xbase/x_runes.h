#ifndef __XBASE_RUNES_V2_H__
#define __XBASE_RUNES_V2_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
#undef printf

    namespace ascii
    {
        typedef char        rune;
        typedef rune*       prune;
        typedef const rune* pcrune;
        static const rune    TERMINATOR = '\0';
        static const s32     TYPE       = 1;
        struct runes_t
        {
            prune m_bos; // Begin of stream
            prune m_str; // Begin of string
            prune m_end; // End of string (does not necessarily point at TERMINATOR)
            prune m_eos; // End of stream, points to a TERMINATOR
        };
        struct crunes_t
        {
            crunes_t() : m_bos("\0\0\0\0"), m_str(m_bos), m_end(m_bos), m_eos(m_bos) {}
            crunes_t(pcrune str, s32 len) : m_bos(str), m_str(str), m_end(str + len), m_eos(str + len) {}
            crunes_t(pcrune str, pcrune end) : m_bos(str), m_str(str), m_end(end), m_eos(end) {}
            pcrune m_bos, m_str, m_end, m_eos;
        };
        s32 strlen(pcrune str, pcrune eos = nullptr);
    } // namespace ascii
    namespace utf8
    {
        typedef u8        rune;
        typedef rune*       prune;
        typedef const rune* pcrune;
        static const s32      TYPE       = 3;
        static const rune     TERMINATOR = '\0';

        struct runes_t
        {
            prune m_bos, m_str, m_end, m_eos;
        };
        struct crunes_t
        {
            crunes_t() : m_bos((pcrune) "\0\0\0\0"), m_str(m_bos), m_end(m_bos), m_eos(m_bos) {}
            pcrune m_bos, m_str, m_end, m_eos;
        };
    } // namespace utf8
    namespace utf16
    {
        typedef u16        rune;
        typedef rune*       prune;
        typedef const rune* pcrune;
        static const s32       TYPE       = 2;
        static const rune      TERMINATOR = '\0';
        static const u64       EMPTY_STRING = 0;

        struct runes_t
        {
            prune m_bos, m_str, m_end, m_eos;
        };
        struct crunes_t
        {
            crunes_t() : m_bos((pcrune)&EMPTY_STRING), m_str(m_bos), m_end(m_bos), m_eos(m_bos) {}
            pcrune m_bos, m_str, m_end, m_eos;
        };
    } // namespace utf16
    namespace utf32
    {
        typedef u32        rune;
        typedef rune*       prune;
        typedef const rune* pcrune;
        static const s32       TYPE       = 4;
        static const rune      TERMINATOR = '\0';

        struct runes_t
        {
            prune m_bos, m_str, m_end, m_eos;
        };
        struct crunes_t
        {
            crunes_t() : m_bos((pcrune) "\0\0\0\0"), m_str(m_bos), m_end(m_bos), m_eos(m_bos) {}
            crunes_t(pcrune str, pcrune end) : m_bos(str), m_str(str), m_end(end), m_eos(end) {}
            pcrune m_bos, m_str, m_end, m_eos;
        };
    } // namespace utf32

    struct crunes_t;
    struct runes_t
    {
        struct ptr_t
        {
			ptr_t() : m_ptr() {}
			ptr_t(ascii::prune ptr) : m_ptr(ptr) {}
			ptr_t(utf8::prune ptr) : m_ptr(ptr) {}
			ptr_t(utf16::prune ptr) : m_ptr(ptr) {}
			ptr_t(utf32::prune ptr) : m_ptr(ptr) {}

            ptr_t& operator=(const ptr_t& other)
            {
                m_ptr.m_ascii = other.m_ptr.m_ascii;
                return *this;
            }
            bool operator<(const ptr_t& other) const { return m_ptr.m_ascii < other.m_ptr.m_ascii; }
            bool operator>(const ptr_t& other) const { return m_ptr.m_ascii > other.m_ptr.m_ascii; }
            bool operator<=(const ptr_t& other) const { return m_ptr.m_ascii <= other.m_ptr.m_ascii; }
            bool operator>=(const ptr_t& other) const { return m_ptr.m_ascii >= other.m_ptr.m_ascii; }
            bool operator==(const ptr_t& other) const { return m_ptr.m_ascii == other.m_ptr.m_ascii; }
            bool operator!=(const ptr_t& other) const { return m_ptr.m_ascii != other.m_ptr.m_ascii; }
            union runeptr
            {
                inline runeptr() : m_ascii(nullptr) {}
				inline runeptr(ascii::prune ptr) : m_ascii(ptr) {}
				inline runeptr(utf8::prune ptr) : m_utf8(ptr) {}
				inline runeptr(utf16::prune ptr) : m_utf16(ptr) {}
				inline runeptr(utf32::prune ptr) : m_utf32(ptr) {}

                ascii::prune m_ascii;
                utf8::prune  m_utf8;
                utf16::prune m_utf16;
                utf32::prune m_utf32;
            };
            runeptr m_ptr;
        };

        runes_t();
        runes_t(runes_t const& other);

        runes_t(ascii::prune _str, ascii::prune _end, s32 _type = ascii::TYPE);
        runes_t(ascii::prune _str, ascii::prune _end, ascii::prune _eos, s32 _type = ascii::TYPE);
        runes_t(ascii::prune _bos, ascii::prune _str, ascii::prune _end, ascii::prune _eos, s32 _type = ascii::TYPE);

		runes_t(utf8::prune _str, utf8::prune _end, s32 _type = utf8::TYPE);
		runes_t(utf8::prune _str, utf8::prune _end, utf8::prune _eos, s32 _type = utf8::TYPE);
		runes_t(utf8::prune _bos, utf8::prune _str, utf8::prune _end, utf8::prune _eos, s32 _type = utf8::TYPE);

        runes_t(utf16::prune _str, utf16::prune _end, s32 _type = utf16::TYPE);
        runes_t(utf16::prune _str, utf16::prune _end, utf16::prune _eos, s32 _type = utf16::TYPE);
        runes_t(utf16::prune _bos, utf16::prune _str, utf16::prune _end, utf16::prune _eos, s32 _type = utf16::TYPE);

        runes_t(utf32::prune _str, utf32::prune _end, s32 _type = utf32::TYPE);
        runes_t(utf32::prune _str, utf32::prune _end, utf32::prune _eos, s32 _type = utf32::TYPE);
        runes_t(utf32::prune _bos, utf32::prune _str, utf32::prune _end, utf32::prune _eos, s32 _type = utf32::TYPE);

        ascii::prune str();
        utf16::prune str16();
        utf32::prune str32();
        ascii::prune end();
        utf16::prune end16();
        utf32::prune end32();

        s32  size() const;
        s32  cap() const;
        bool is_empty() const;
        bool is_valid() const;
        bool is_nil() const;
        void reset();
        void clear();
        void term();

        runes_t& operator+=(const ascii::crunes_t& str);
        runes_t& operator+=(const utf32::crunes_t& str);
        runes_t& operator+=(ascii::rune c);
        runes_t& operator+=(utf32::rune c);
        runes_t& operator=(runes_t const& other);

        union urunes_t
        {
            inline urunes_t() : m_ascii() {}
            ascii::runes_t m_ascii;
            utf8::runes_t  m_utf8;
            utf16::runes_t m_utf16;
            utf32::runes_t m_utf32;
        };
        urunes_t m_runes;
        s32      m_type;
    };

    struct crunes_t
    {
        struct ptr_t
        {
            ptr_t() : m_ptr() {}
            ptr_t(ascii::pcrune ptr) : m_ptr(ptr) {}
            ptr_t(utf8::pcrune ptr) : m_ptr(ptr) {}
            ptr_t(utf16::pcrune ptr) : m_ptr(ptr) {}
            ptr_t(utf32::pcrune ptr) : m_ptr(ptr) {}

            ptr_t& operator=(const ptr_t& other)
            {
                m_ptr.m_ascii = other.m_ptr.m_ascii;
                return *this;
            }
            bool operator<(const ptr_t& other) const { return m_ptr.m_ascii < other.m_ptr.m_ascii; }
            bool operator>(const ptr_t& other) const { return m_ptr.m_ascii > other.m_ptr.m_ascii; }
            bool operator<=(const ptr_t& other) const { return m_ptr.m_ascii <= other.m_ptr.m_ascii; }
            bool operator>=(const ptr_t& other) const { return m_ptr.m_ascii >= other.m_ptr.m_ascii; }
            bool operator==(const ptr_t& other) const { return m_ptr.m_ascii == other.m_ptr.m_ascii; }
            bool operator!=(const ptr_t& other) const { return m_ptr.m_ascii != other.m_ptr.m_ascii; }

            ptr_t furthest(const ptr_t& other) const { return other.m_ptr.m_ascii > m_ptr.m_ascii ? ptr_t(other.m_ptr.m_ascii) : ptr_t(m_ptr.m_ascii); }
            union runeptr
            {
				inline runeptr() : m_ascii(nullptr) {}
				inline runeptr(ascii::pcrune ptr) : m_ascii(ptr) {}
                inline runeptr(utf8::pcrune ptr) : m_utf8(ptr) {}
                inline runeptr(utf16::pcrune ptr) : m_utf16(ptr) {}
                inline runeptr(utf32::pcrune ptr) : m_utf32(ptr) {}
                ascii::pcrune m_ascii;
                utf8::pcrune  m_utf8;
                utf16::pcrune m_utf16;
                utf32::pcrune m_utf32;
            };
            runeptr m_ptr;
        };

        crunes_t();
        crunes_t(ascii::crunes_t const& _str);
        crunes_t(ascii::pcrune _str);
        crunes_t(ascii::pcrune _str, u32 len);
        crunes_t(ascii::pcrune _str, ascii::pcrune _end);
        crunes_t(ascii::pcrune _str, ascii::pcrune _end, ascii::pcrune _eos, s32 _type = ascii::TYPE);
        crunes_t(ascii::pcrune _bos, ascii::pcrune _str, ascii::pcrune _end, ascii::pcrune _eos, s32 _type = ascii::TYPE);

        crunes_t(utf8::crunes_t const& _str);
        crunes_t(utf8::pcrune _str);
        crunes_t(utf8::pcrune _str, u32 len);
        crunes_t(utf8::pcrune _str, utf8::pcrune _end);
        crunes_t(utf8::pcrune _str, utf8::pcrune _end, utf8::pcrune _eos);
        crunes_t(utf8::pcrune _bos, utf8::pcrune _str, utf8::pcrune _end, utf8::pcrune _eos);

        crunes_t(utf16::crunes_t const& _str);
        crunes_t(utf16::pcrune _str);
        crunes_t(utf16::pcrune _str, u32 len);
        crunes_t(utf16::pcrune _str, utf16::pcrune _end);
        crunes_t(utf16::pcrune _str, utf16::pcrune _end, utf16::pcrune _eos);
        crunes_t(utf16::pcrune _bos, utf16::pcrune _str, utf16::pcrune _end, utf16::pcrune _eos);

        crunes_t(utf32::crunes_t const& _str);
        crunes_t(utf32::pcrune _str);
        crunes_t(utf32::pcrune _str, u32 len);
        crunes_t(utf32::pcrune _str, utf32::pcrune _end);
        crunes_t(utf32::pcrune _str, utf32::pcrune _end, utf32::pcrune _eos);
        crunes_t(utf32::pcrune _bos, utf32::pcrune _str, utf32::pcrune _end, utf32::pcrune _eos);

        crunes_t(runes_t const& other);
        crunes_t(crunes_t const& other);
        crunes_t(crunes_t const& other, ptr_t const& from, ptr_t const& to);

        ascii::pcrune str() const;
        utf16::pcrune str16() const;
        utf32::pcrune str32() const;
        ascii::pcrune end() const;
        utf16::pcrune end16() const;
        utf32::pcrune end32() const;

        s32  size() const;
        bool is_valid() const;
        bool is_empty() const;
        bool is_nil() const;
        void reset();
        void clear();

        crunes_t& operator=(crunes_t const& other);

        union urunes_t
        {
            inline urunes_t() : m_ascii() {}
            ascii::crunes_t m_ascii;
            utf8::crunes_t  m_utf8;
            utf16::crunes_t m_utf16;
            utf32::crunes_t m_utf32;
        };
        urunes_t m_runes;
        s32      m_type;
    };

    class runes_alloc_t
    {
    public:
        virtual ~runes_alloc_t() {}
        virtual runes_t allocate(s32 len, s32 cap, s32 type) = 0;
        virtual void    deallocate(runes_t& slice_t)         = 0;
    };

    // -------------------------------------------------------------------------------
    // search functions
    crunes_t find(crunes_t const& _str, uchar32 _c, bool inCaseSensitive = true);
    runes_t  find(runes_t const& _str, uchar32 _c, bool inCaseSensitive = true);
    crunes_t findLast(crunes_t const& _str, uchar32 _c, bool inCaseSensitive = true);
    runes_t  findLast(runes_t const& _str, uchar32 _c, bool inCaseSensitive = true);
    crunes_t find(crunes_t const& str, crunes_t const& find, bool inCaseSensitive = true);
    runes_t  find(runes_t const& str, crunes_t const& find, bool inCaseSensitive = true);
    crunes_t findLast(crunes_t const& str, crunes_t const& find, bool inCaseSensitive = true);
    runes_t  findLast(runes_t const& str, crunes_t const& find, bool inCaseSensitive = true);
    crunes_t findOneOf(crunes_t const& str, crunes_t const& set, bool inCaseSensitive = true);
    runes_t  findOneOf(runes_t const& str, crunes_t const& set, bool inCaseSensitive = true);

    crunes_t findSelectUntil(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    crunes_t findSelectUntil(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
    crunes_t findLastSelectUntil(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    crunes_t findLastSelectUntil(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
    crunes_t findSelectUntilIncluded(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    crunes_t findSelectUntilIncluded(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
    crunes_t findLastSelectUntilIncluded(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    crunes_t findLastSelectUntilIncluded(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
    crunes_t findSelectAfter(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    crunes_t findSelectAfter(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
    crunes_t findLastSelectAfter(const crunes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    crunes_t findLastSelectAfter(const crunes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);

    // -------------------------------------------------------------------------------
    // search and select text between delimiters
    // e.g. selectBetween(str, '<', '>');
    crunes_t selectBetween(const crunes_t& inStr, uchar32 inLeft, uchar32 inRight);
    crunes_t selectNextBetween(const crunes_t& inStr, const crunes_t& inSelection, uchar32 inLeft, uchar32 inRight);
    crunes_t selectBetweenLast(const crunes_t& inStr, uchar32 inLeft, uchar32 inRight);
    crunes_t selectPreviousBetween(const crunes_t& inStr, const crunes_t& inSelection, uchar32 inLeft, uchar32 inRight);

    // -------------------------------------------------------------------------------
    // select before after
    crunes_t selectBeforeExclude(const crunes_t& inStr, const crunes_t& inSelection);
    crunes_t selectBeforeInclude(const crunes_t& inStr, const crunes_t& inSelection);
    crunes_t selectAfterExclude(const crunes_t& inStr, const crunes_t& inSelection);
    crunes_t selectAfterInclude(const crunes_t& inStr, const crunes_t& inSelection);
    crunes_t selectOverlap(const crunes_t& inStr, const crunes_t& inRight);

    // -------------------------------------------------------------------------------

    runes_t findSelectUntil(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    runes_t findSelectUntil(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
    runes_t findLastSelectUntil(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    runes_t findLastSelectUntil(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
    runes_t findSelectUntilIncluded(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    runes_t findSelectUntilIncluded(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
    runes_t findLastSelectUntilIncluded(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    runes_t findLastSelectUntilIncluded(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
    runes_t findSelectAfter(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    runes_t findSelectAfter(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);
    runes_t findLastSelectAfter(const runes_t& inStr, uchar32 inFind, bool inCaseSensitive = true);
    runes_t findLastSelectAfter(const runes_t& inStr, const crunes_t& inFind, bool inCaseSensitive = true);

    // -------------------------------------------------------------------------------
    // search and select
    runes_t selectBetween(const runes_t& inStr, uchar32 inLeft, uchar32 inRight);
    runes_t selectNextBetween(const runes_t& inStr, const runes_t& inSelection, uchar32 inLeft, uchar32 inRight);
    runes_t selectBetweenLast(const runes_t& inStr, uchar32 inLeft, uchar32 inRight);
    runes_t selectPreviousBetween(const runes_t& inStr, const runes_t& inSelection, uchar32 inLeft, uchar32 inRight);
    runes_t selectBeforeExclude(const runes_t& inStr, const runes_t& inSelection);
    runes_t selectBeforeInclude(const runes_t& inStr, const runes_t& inSelection);
    runes_t selectAfterExclude(const runes_t& inStr, const runes_t& inSelection);
    runes_t selectAfterInclude(const runes_t& inStr, const runes_t& inSelection);
    runes_t selectOverlap(const runes_t& inStr, const runes_t& inRight);

    // -------------------------------------------------------------------------------
    // compare
    s32 compare(crunes_t const& str1, crunes_t const& str2, bool inCaseSensitive = true);
    s32 compare(runes_t const& str1, runes_t const& str2, bool inCaseSensitive = true);

    // -------------------------------------------------------------------------------
    // parse/from_string, to_string
    crunes_t parse(crunes_t const& str, bool& value);
    crunes_t parse(crunes_t const& str, s8& value, s32 base = 10);
    crunes_t parse(crunes_t const& str, s16& value, s32 base = 10);
    crunes_t parse(crunes_t const& str, s32& value, s32 base = 10);
    crunes_t parse(crunes_t const& str, s64& value, s32 base = 10);
    crunes_t parse(crunes_t const& str, u8& value, s32 base = 10);
    crunes_t parse(crunes_t const& str, u16& value, s32 base = 10);
    crunes_t parse(crunes_t const& str, u32& value, s32 base = 10);
    crunes_t parse(crunes_t const& str, u64& value, s32 base = 10);
    crunes_t parse(crunes_t const& str, f32& value);
    crunes_t parse(crunes_t const& str, f64& value);
    
    void     to_string(runes_t& str, s32 val, s32 base = 10);
    void     to_string(runes_t& str, u32 val, s32 base = 10);
    void     to_string(runes_t& str, s64 val, s32 base = 10);
    void     to_string(runes_t& str, u64 val, s32 base = 10);
    void     to_string(runes_t& str, f32 val, s32 num_fractional_digits = 4);
    void     to_string(runes_t& str, f64 val, s32 num_fractional_digits = 4);

    // -------------------------------------------------------------------------------
    // filters
    bool           is_decimal(crunes_t const& str);
    bool           is_hexadecimal(crunes_t const& str, bool with_prefix = false);
    bool           is_float(crunes_t const& str);
    bool           is_GUID(crunes_t const& str);
    inline bool    is_space(uchar32 c) { return ((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
    inline bool    is_upper(uchar32 c) { return ((c >= 'A') && (c <= 'Z')); }
    inline bool    is_lower(uchar32 c) { return ((c >= 'a') && (c <= 'z')); }
    inline bool    is_alpha(uchar32 c) { return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
    inline bool    is_digit(uchar32 c) { return ((c >= '0') && (c <= '9')); }
    inline bool    is_hexa(uchar32 c) { return (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }
    void           to_upper(runes_t& str);
    void           to_lower(runes_t& str);
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
        return 0;
    }

    inline bool is_equal(uchar32 a, uchar32 b) { return (a == b); }
    inline bool is_equalfold(uchar32 a, uchar32 b) { return (to_lower(a) == to_lower(b)); }

    bool    is_upper(crunes_t const& str);
    bool    is_lower(crunes_t const& str);
    bool    is_capitalized(crunes_t const& str);
    bool    is_delimited(crunes_t const& str, uchar32 delimit_left = '\"', uchar32 delimit_right = '\"');
    bool    is_quoted(crunes_t const& str, uchar32 quote = '\"');
    bool    starts_with(crunes_t const& str, uchar32 start);
    bool    starts_with(crunes_t const& str, crunes_t const& start);
    bool    ends_with(crunes_t const& str, uchar32 end_char);
    bool    ends_with(crunes_t const& str, crunes_t const& end);
    uchar32 first_char(crunes_t const& str);
    uchar32 last_char(crunes_t const& str);

    // -------------------------------------------------------------------------------
    // modifiers
    void removeSelection(runes_t& str, crunes_t const& sel);
    void keepOnlySelection(runes_t& str, crunes_t const& sel);
    void replaceSelection(runes_t& str, crunes_t const& sel, crunes_t const& replace);
    void replaceSelection(runes_t& str, crunes_t const& sel, crunes_t const& replace, runes_alloc_t* allocator, s32 size_alignment);
    void findReplace(runes_t& str, uchar32 find, uchar32 replace, bool inCaseSensitive = true);
    void findReplace(runes_t& str, crunes_t const& find, crunes_t const& replace, bool inCaseSensitive = true);
    void insert(runes_t& str, crunes_t const& insert);
    void insert(runes_t& str, crunes_t const& insert, runes_alloc_t* allocator, s32 size_alignment);
    void insert(runes_t& str, crunes_t const& sel, crunes_t const& insert);
    void insert(runes_t& str, crunes_t const& sel, crunes_t const& insert, runes_alloc_t* allocator, s32 size_alignment);

    runes_t expand(runes_t& str, runes_t const& sel);
    runes_t expand(runes_t& str, crunes_t const& sel);

    void trim(runes_t&);                                            // Trim whitespace from left and right side
    void trimLeft(runes_t&);                                        // Trim whitespace from left side
    void trimRight(runes_t&);                                       // Trim whitespace from right side
    void trim(runes_t&, uchar32 inChar);                            // Trim characters in <inCharSet> from left and right side
    void trimLeft(runes_t&, uchar32 inChar);                        // Trim character <inChar> from left side
    void trimRight(runes_t&, uchar32 inChar);                       // Trim character <inChar> from right side
    void trim(runes_t&, crunes_t const& inCharSet);                 // Trim characters in <inCharSet> from left and right side
    void trimLeft(runes_t&, crunes_t const& inCharSet);             // Trim characters in <inCharSet> from left side
    void trimRight(runes_t&, crunes_t const& inCharSet);            // Trim characters in <inCharSet> from right side
    void trimQuotes(runes_t&);                                      // Trim double quotes from left and right side
    void trimQuotes(runes_t&, uchar32 quote);                       // Trim double quotes from left and right side
    void trimDelimiters(runes_t&, uchar32 inLeft, uchar32 inRight); // Trim delimiters from left and right side

    void trim(crunes_t&);                                            // Trim whitespace from left and right side
    void trimLeft(crunes_t&);                                        // Trim whitespace from left side
    void trimRight(crunes_t&);                                       // Trim whitespace from right side
    void trim(crunes_t&, uchar32 inChar);                            // Trim characters in <inCharSet> from left and right side
    void trimLeft(crunes_t&, uchar32 inChar);                        // Trim character <inChar> from left side
    void trimRight(crunes_t&, uchar32 inChar);                       // Trim character <inChar> from right side
    void trim(crunes_t&, crunes_t const& inCharSet);                 // Trim characters in <inCharSet> from left and right side
    void trimLeft(crunes_t&, crunes_t const& inCharSet);             // Trim characters in <inCharSet> from left side
    void trimRight(crunes_t&, crunes_t const& inCharSet);            // Trim characters in <inCharSet> from right side
    void trimQuotes(crunes_t&);                                      // Trim double quotes from left and right side
    void trimQuotes(crunes_t&, uchar32 quote);                       // Trim double quotes from left and right side
    void trimDelimiters(crunes_t&, uchar32 inLeft, uchar32 inRight); // Trim delimiters from left and right side

    void copy(const crunes_t& src, runes_t& dst);
    void copy(const crunes_t& src, runes_t& dst, runes_alloc_t* allocator, s32 size_alignment = 8);

    void concatenate(runes_t& str, const crunes_t& concat);
    void concatenate(runes_t& str, const crunes_t& concat, runes_alloc_t* allocator, s32 size_alignment);
    void concatenate(runes_t& str, const crunes_t& concat1, const crunes_t& concat2, runes_alloc_t* allocator, s32 size_alignment);

    // -------------------------------------------------------------------------------
    // global operators for comparison
    inline bool operator==(const crunes_t& lhs, const crunes_t& rhs) { return compare(lhs, rhs) == 0; }
    inline bool operator!=(const crunes_t& lhs, const crunes_t& rhs) { return compare(lhs, rhs) != 0; }
    inline bool operator==(const runes_t& lhs, const runes_t& rhs) { return compare(lhs, rhs) == 0; }
    inline bool operator!=(const runes_t& lhs, const runes_t& rhs) { return compare(lhs, rhs) != 0; }


    // -------------------------------------------------------------------------------
    // helpers for inline sized runes_t
    template <typename T, s32 L> class runez_t : public runes_t
    {
    public:
        enum
        {
            SIZE = L + 1
        };
        T m_run[SIZE];
        inline runez_t() : runes_t(m_run, m_run, &m_run[SIZE - 1]) {}
        inline runez_t(uchar32 c) : runes_t(m_run, &m_run[1])
        {
            m_run[0] = c;
            m_run[1] = 0;
        }
        inline runez_t(const char* _str) : runes_t(m_run, m_run, m_run, &m_run[SIZE - 1])
        {
            crunes_t str((ascii::pcrune)_str);
            concatenate(*this, str);
            term();
        }
    };

    template <typename T, s32 L> class crunez_t : public crunes_t
    {
    public:
        enum
        {
            SIZE = L + 1
        };
        T m_run[SIZE];

        inline crunez_t(uchar32 c) : crunes_t(m_run, &m_run[1])
        {
            m_run[0] = c;
            m_run[1] = 0;
        }

        inline crunez_t(const char* str) : crunes_t(m_run, m_run, m_run, &m_run[SIZE - 1])
        {
            runes_t run(m_run, m_run, m_run, &m_run[SIZE - 1]);
            crunes_t cstr((ascii::pcrune)str, (ascii::pcrune)str);
            concatenate(run, cstr);
            run.term();
            m_runes.m_ascii.m_end = run.m_runes.m_ascii.m_end;
        }
    };

    // -------------------------------------------------------------------------------
    // runes reader and writer
	class irunes_reader_t
	{
	public:

		void             reset() { vreset(); }
		bool             valid() const { return vvalid(); }
		uchar32          peek() const { return vpeek(); }
        bool             read(uchar32& c) { return vread(c); }
		inline uchar32   read() { uchar32 c; vread(c); return c; }
        inline void      skip(s32 c = 1) { vskip(c); }

    protected:
		virtual void     vreset() = 0;
		virtual bool     vvalid() const = 0;
		virtual uchar32  vpeek() const = 0;
		virtual bool     vread(uchar32& c) = 0;
		virtual void     vskip(s32 c = 1) = 0;
	};

	class runes_reader_t : public irunes_reader_t
    {
    public:
        runes_reader_t();
        runes_reader_t(ascii::pcrune str);
        runes_reader_t(ascii::pcrune str, u32 len);
		runes_reader_t(ascii::pcrune str, ascii::pcrune str_end);
		runes_reader_t(utf8::pcrune str, utf8::pcrune str_end);
		runes_reader_t(utf16::pcrune str, utf16::pcrune str_end);
		runes_reader_t(utf32::pcrune str, utf32::pcrune str_end);
        runes_reader_t(crunes_t const& runes);

		runes_reader_t  select(crunes_t::ptr_t const& from, crunes_t::ptr_t to) const;
        crunes_t::ptr_t get_cursor() const { return m_cursor; }
        void            set_cursor(crunes_t::ptr_t const& c) { m_cursor = c; }

        crunes_t get_source() const;
        crunes_t get_current() const;
        bool     at_end() const;
        
    protected:
        virtual bool     vvalid() const;
        virtual void     vreset();
        virtual uchar32  vpeek() const;
		virtual bool     vread(uchar32& c);
		virtual void     vskip(s32 c = 1);

        crunes_t        m_runes;
        crunes_t::ptr_t m_cursor;
    };

    class irunes_writer_t
    {
    public:
        bool write(uchar32 c) { return vwrite(c); }
        bool write(crunes_t const& str) { return vwrite(str); }
        void flush() { vflush(); }

    protected:
        virtual bool vwrite(uchar32 c) = 0;
        virtual bool vwrite(crunes_t const& str) = 0;
        virtual void vflush() = 0;
    };

    class runes_writer_t : public irunes_writer_t
    {
    public:
		runes_writer_t(ascii::prune str, ascii::prune str_end);
		runes_writer_t(utf8::prune str, utf8::prune str_end);
		runes_writer_t(utf16::prune str, utf16::prune str_end);
		runes_writer_t(utf32::prune str, utf32::prune str_end);
		runes_writer_t(runes_t const& runes);

        runes_t get_destination() const;
        runes_t get_current() const;

        void reset();
        s32  count() const;

    protected:
        virtual bool vwrite(uchar32 c);
        virtual bool vwrite(crunes_t const& str);
        virtual void vflush();

        runes_t        m_runes;
        runes_t::ptr_t m_cursor;
        s32            m_count;
    };

}; // namespace xcore

#endif ///< __XBASE_RUNES2_H__
