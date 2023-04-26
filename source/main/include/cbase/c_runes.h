#ifndef __CBASE_RUNES_V2_H__
#define __CBASE_RUNES_V2_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

// TODO

// Mirror all functions from main runes_t and crunes_t in:
// - ascii runes_t and crunes_t
// - utf-8
// - utf-16
// - utf-32

// The result is that we can also use them separately

namespace ncore
{
#undef printf

    const uchar32 cEOS = 0x00; // \0, end of string
    const uchar32 cEOF = 0x05; // end of file
    const uchar32 cEOL = 0x0A; // \n, end of line
    const uchar32 cCR  = 0x0D; // \r, carriage return
    const uchar32 cTAB = 0x09; // \t, tab

    namespace ascii
    {
        typedef char        rune;
        typedef rune*       prune;
        typedef const rune* pcrune;
        static const rune   TERMINATOR   = '\0';
        static const u32    TYPE         = 1;
        static const u64    EMPTY_STRING = 0;

        struct runes_t
        {
            runes_t()
                : m_bos(nullptr)
                , m_str(0)
                , m_end(0)
                , m_eos(0)
                , m_flags(TYPE)
            {
            }

            prune m_bos;   // begin of string
            u32   m_str;   // &m_bos[m_end] -> string
            u32   m_end;   // &m_bos[m_end], does not necessarily point at TERMINATOR
            u32   m_eos;   // &m_bos[m_end], end of string, points to a TERMINATOR
            u32   m_flags; // type (ascii, utf-8, utf-16, utf-32)
        };

        struct crunes_t
        {
            crunes_t()
                : m_bos((pcrune)&EMPTY_STRING)
                , m_str(0)
                , m_end(0)
                , m_eos(0)
                , m_flags(TYPE)
            {
            }
            crunes_t(pcrune str, u32 len)
                : m_bos(str)
                , m_str(0)
                , m_end(len)
                , m_eos(len)
                , m_flags(TYPE)
            {
            }
            crunes_t(pcrune str, pcrune end)
                : m_bos(str)
                , m_str(0)
                , m_end((u32)(end - str))
                , m_eos((u32)(end - str))
                , m_flags(TYPE)
            {
            }

            pcrune m_bos;   // begin of string
            u32    m_str;   // &m_bos[m_end] -> string
            u32    m_end;   // &m_bos[m_end], does not necessarily point at TERMINATOR
            u32    m_eos;   // &m_bos[m_end], end of string, points to a TERMINATOR
            u32    m_flags; // type (ascii, utf-8, utf-16, utf-32)
        };

        // return length of string in bytes (for ascii this also means number of characters)
        s32 strlen(pcrune str, pcrune eos = nullptr);

        s32 compare(pcrune left, pcrune right);
        s32 compare(pcrune str1, pcrune str2, pcrune end1, pcrune end2);

        void reverse(char* str, char* end);

        // return false when there is not enough space in the output
        // would need at least 10 characters for u32 and 20 for u64
        bool asStr(u32 val, char const* str, char*& cursor, char const* end, s32 base, bool octzero, bool lowercase);
        bool asStr(s32 val, char const* str, char*& cursor, char const* end, s32 base, bool octzero, bool lowercase);
        bool asStr(u64 val, char const* str, char*& cursor, char const* end, s32 base, bool octzero, bool lowercase);
        bool asStr(s64 val, char const* str, char*& cursor, char const* end, s32 base, bool octzero, bool lowercase);

    } // namespace ascii

    namespace utf8
    {
        typedef u8          rune;
        typedef rune*       prune;
        typedef const rune* pcrune;
        static const u32    TYPE         = 2;
        static const rune   TERMINATOR   = '\0';
        static const u64    EMPTY_STRING = 0;

        struct runes_t
        {
            runes_t()
                : m_bos(nullptr)
                , m_str(0)
                , m_end(0)
                , m_eos(0)
                , m_flags(TYPE)
            {
            }

            prune m_bos;   // begin of string
            u32   m_str;   // &m_bos[m_end] -> string
            u32   m_end;   // &m_bos[m_end], does not necessarily point at TERMINATOR
            u32   m_eos;   // &m_bos[m_end], end of string, points to a TERMINATOR
            u32   m_flags; // type (ascii, utf-8, utf-16, utf-32)
        };

        struct crunes_t
        {
            crunes_t()
                : m_bos((pcrune)&EMPTY_STRING)
                , m_str(0)
                , m_end(0)
                , m_eos(0)
                , m_flags(TYPE)
            {
            }
            crunes_t(pcrune str, pcrune end)
                : m_bos(str)
                , m_str(0)
                , m_end((u32)(end - str))
                , m_eos((u32)(end - str))
                , m_flags(TYPE)
            {
            }

            pcrune m_bos;   // begin of string
            u32    m_str;   // &m_bos[m_end] -> string
            u32    m_end;   // &m_bos[m_end], does not necessarily point at TERMINATOR
            u32    m_eos;   // &m_bos[m_end], end of string, points to a TERMINATOR
            u32    m_flags; // type (ascii, utf-8, utf-16, utf-32)
        };

        // return length of string in bytes (for utf8 this means number of bytes and NOT characters)
        s32 strlen(pcrune str, pcrune eos = nullptr);
    } // namespace utf8

    namespace utf16
    {
        typedef u16         rune;
        typedef rune*       prune;
        typedef const rune* pcrune;
        static const u32    TYPE         = 4;
        static const rune   TERMINATOR   = '\0';
        static const u64    EMPTY_STRING = 0;

        struct runes_t
        {
            runes_t()
                : m_bos(nullptr)
                , m_str(0)
                , m_end(0)
                , m_eos(0)
                , m_flags(TYPE)
            {
            }

            prune m_bos;   // begin of string
            u32   m_str;   // &m_bos[m_end] -> string
            u32   m_end;   // &m_bos[m_end], does not necessarily point at TERMINATOR
            u32   m_eos;   // &m_bos[m_end], end of string, points to a TERMINATOR
            u32   m_flags; // type (ascii, utf-8, utf-16, utf-32)
        };

        struct crunes_t
        {
            crunes_t()
                : m_bos((pcrune)&EMPTY_STRING)
                , m_str(0)
                , m_end(0)
                , m_eos(0)
                , m_flags(TYPE)
            {
            }
            crunes_t(pcrune str, pcrune end)
                : m_bos(str)
                , m_str(0)
                , m_end((u32)(end - str))
                , m_eos((u32)(end - str))
                , m_flags(TYPE)
            {
            }

            pcrune m_bos;   // begin of string
            u32    m_str;   // &m_bos[m_end] -> string
            u32    m_end;   // &m_bos[m_end], does not necessarily point at TERMINATOR
            u32    m_eos;   // &m_bos[m_end], end of string, points to a TERMINATOR
            u32    m_flags; // type (ascii, utf-8, utf-16, utf-32)
        };

    } // namespace utf16

    namespace utf32
    {
        typedef u32         rune;
        typedef rune*       prune;
        typedef const rune* pcrune;
        static const u32    TYPE         = 8;
        static const rune   TERMINATOR   = '\0';
        static const u64    EMPTY_STRING = 0;

        struct runes_t
        {
            runes_t()
                : m_bos(nullptr)
                , m_str(0)
                , m_end(0)
                , m_eos(0)
                , m_flags(TYPE)
            {
            }

            prune m_bos;   // begin of string
            u32   m_str;   // &m_bos[m_end] -> string
            u32   m_end;   // &m_bos[m_end], does not necessarily point at TERMINATOR
            u32   m_eos;   // &m_bos[m_end], end of string, points to a TERMINATOR
            u32   m_flags; // type (ascii, utf-8, utf-16, utf-32)
        };

        struct crunes_t
        {
            crunes_t()
                : m_bos((pcrune)&EMPTY_STRING)
                , m_str(0)
                , m_end(0)
                , m_eos(0)
                , m_flags(TYPE)
            {
            }
            crunes_t(pcrune str, pcrune end)
                : m_bos(str)
                , m_str(0)
                , m_end((u32)(end - str))
                , m_eos((u32)(end - str))
                , m_flags(TYPE)
            {
            }

            pcrune m_bos;   // begin of string
            u32    m_str;   // &m_bos[m_end] -> string
            u32    m_end;   // &m_bos[m_end], does not necessarily point at TERMINATOR
            u32    m_eos;   // &m_bos[m_end], end of string, points to a TERMINATOR
            u32    m_flags; // type (ascii, utf-8, utf-16, utf-32)
        };

        u32 strlen(pcrune str, pcrune eos = nullptr);
    } // namespace utf32

    struct crunes_t;
    struct runes_t
    {
        runes_t();
        runes_t(runes_t const& other);

        runes_t(ascii::prune _str, ascii::prune _end, u32 _type = ascii::TYPE);
        runes_t(ascii::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = ascii::TYPE);

        runes_t(utf8::prune _str, utf8::prune _end, u32 _type = utf8::TYPE);
        runes_t(utf8::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = utf8::TYPE);

        runes_t(utf16::prune _str, utf16::prune _end, u32 _type = utf16::TYPE);
        runes_t(utf16::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = utf16::TYPE);

        runes_t(utf32::prune _str, utf32::prune _end, u32 _type = utf32::TYPE);
        runes_t(utf32::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = utf32::TYPE);

        s32  size() const;
        s32  cap() const;
        bool is_empty() const;
        bool is_valid() const;
        bool is_nil() const;
        void reset();
        void clear();
        void term();

        uchar32 peek() const;
        uchar32 read();
        bool    write(uchar32 c);

        bool scan(u32& cursor, const crunes_t& until_chars, uchar32& encountered) const; // scan until we reach one of the 'chars'
        void skip(u32& cursor, const crunes_t& skip_chars) const;                        // skip until we reach a character not part of 'chars'

        runes_t& operator+=(const ascii::crunes_t& str);
        runes_t& operator+=(const utf32::crunes_t& str);
        runes_t& operator+=(ascii::rune c);
        runes_t& operator+=(utf32::rune c);
        runes_t& operator=(runes_t const& other);

        union
        {
            ascii::runes_t m_ascii;
            utf8::runes_t  m_utf8;
            utf16::runes_t m_utf16;
            utf32::runes_t m_utf32;
        };

        inline u32  get_type() const { return m_ascii.m_flags & 3; }
        inline bool is_ascii() const { return get_type() == ascii::TYPE; }
        inline bool is_utf8() const {  return get_type() == utf8::TYPE; }
        inline bool is_utf16() const { return get_type() == utf16::TYPE; }
        inline bool is_utf32() const { return get_type() == utf32::TYPE; }
    };

    struct crunes_t
    {
        crunes_t();
        crunes_t(ascii::crunes_t const& _str);
        crunes_t(ascii::pcrune _bos);
        crunes_t(ascii::pcrune _bos, ascii::pcrune _end);
        crunes_t(ascii::pcrune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = ascii::TYPE);

        crunes_t(utf8::crunes_t const& _str);
        crunes_t(utf8::pcrune _str);
        crunes_t(utf8::pcrune _str, utf8::pcrune _end);
        crunes_t(utf8::pcrune _bos, u32 _str, u32 _end, u32 _eos);

        crunes_t(utf16::crunes_t const& _str);
        crunes_t(utf16::pcrune _str);
        crunes_t(utf16::pcrune _str, utf16::pcrune _end);
        crunes_t(utf16::pcrune _bos, u32 _str, u32 _end, u32 _eos);

        crunes_t(utf32::crunes_t const& _str);
        crunes_t(utf32::pcrune _str);
        crunes_t(utf32::pcrune _str, utf32::pcrune _end);
        crunes_t(utf32::pcrune _bos, u32 _str, u32 _end, u32 _eos);

        crunes_t(runes_t const& other);
        crunes_t(crunes_t const& other);
        crunes_t(crunes_t const& other, u32 from, u32 to);

        s32  size() const;
        bool is_valid() const;
        bool is_empty() const;
        bool is_nil() const;
        void reset();
        void clear();

        uchar32 peek() const;
        uchar32 read();

        bool scan(u32& cursor, const crunes_t& until_chars, uchar32& encountered) const; // scan until we reach one of the 'chars'
        void skip(u32& cursor, const crunes_t& skip_chars) const;                        // skip until we reach a character not part of 'chars'
        bool contains(uchar32 c) const;

        crunes_t& operator=(crunes_t const& other);

        union
        {
            ascii::crunes_t m_ascii;
            utf8::crunes_t  m_utf8;
            utf16::crunes_t m_utf16;
            utf32::crunes_t m_utf32;
        };

        inline u32  get_type() const { return m_ascii.m_flags & 3; }
        inline bool is_ascii() const { return get_type() == ascii::TYPE; }
        inline bool is_utf8() const { return get_type() == utf8::TYPE; }
        inline bool is_utf16() const { return get_type() == utf16::TYPE; }
        inline bool is_utf32() const { return get_type() == utf32::TYPE; }
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
    crunes_t find(crunes_t const& _str, uchar32 _c, bool case_sensitive = true);
    runes_t  find(runes_t const& _str, uchar32 _c, bool case_sensitive = true);
    crunes_t findLast(crunes_t const& _str, uchar32 _c, bool case_sensitive = true);
    runes_t  findLast(runes_t const& _str, uchar32 _c, bool case_sensitive = true);
    crunes_t find(crunes_t const& str, crunes_t const& find, bool case_sensitive = true);
    runes_t  find(runes_t const& str, crunes_t const& find, bool case_sensitive = true);
    crunes_t findLast(crunes_t const& str, crunes_t const& find, bool case_sensitive = true);
    runes_t  findLast(runes_t const& str, crunes_t const& find, bool case_sensitive = true);
    crunes_t findOneOf(crunes_t const& str, crunes_t const& set, bool case_sensitive = true);
    runes_t  findOneOf(runes_t const& str, crunes_t const& set, bool case_sensitive = true);

    crunes_t findSelectUntil(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    crunes_t findSelectUntil(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
    crunes_t findLastSelectUntil(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    crunes_t findLastSelectUntil(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
    crunes_t findSelectUntilIncluded(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    crunes_t findSelectUntilIncluded(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
    crunes_t findLastSelectUntilIncluded(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    crunes_t findLastSelectUntilIncluded(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
    crunes_t findSelectAfter(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    crunes_t findSelectAfter(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
    crunes_t findLastSelectAfter(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    crunes_t findLastSelectAfter(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);

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

    runes_t findSelectUntil(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    runes_t findSelectUntil(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
    runes_t findLastSelectUntil(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    runes_t findLastSelectUntil(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
    runes_t findSelectUntilIncluded(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    runes_t findSelectUntilIncluded(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
    runes_t findLastSelectUntilIncluded(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    runes_t findLastSelectUntilIncluded(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
    runes_t findSelectAfter(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    runes_t findSelectAfter(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
    runes_t findLastSelectAfter(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
    runes_t findLastSelectAfter(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);

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
    s32 compare(crunes_t const& str1, crunes_t const& str2, bool case_sensitive = true);
    s32 compare(runes_t const& str1, runes_t const& str2, bool case_sensitive = true);

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

    void to_string(runes_t& str, s32 val, s32 base = 10);
    void to_string(runes_t& str, u32 val, s32 base = 10);
    void to_string(runes_t& str, s64 val, s32 base = 10);
    void to_string(runes_t& str, u64 val, s32 base = 10);
    void to_string(runes_t& str, f32 val, s32 num_fractional_digits = 4);
    void to_string(runes_t& str, f64 val, s32 num_fractional_digits = 4);

    // -------------------------------------------------------------------------------
    // filters
    bool is_decimal(crunes_t const& str);
    bool is_hexadecimal(crunes_t const& str, bool with_prefix = false);
    bool is_float(crunes_t const& str);
    bool is_GUID(crunes_t const& str);
    void to_upper(runes_t& str);
    void to_lower(runes_t& str);

    inline bool    is_space(uchar32 c) { return ((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
    inline bool    is_whitespace(uchar32 c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }
    inline bool    is_upper(uchar32 c) { return ((c >= 'A') && (c <= 'Z')); }
    inline bool    is_lower(uchar32 c) { return ((c >= 'a') && (c <= 'z')); }
    inline bool    is_alpha(uchar32 c) { return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
    inline bool    is_digit(uchar32 c) { return ((c >= '0') && (c <= '9')); }
    inline bool    is_hexa(uchar32 c) { return (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }
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
    inline char to_dec_char(u8 val) { return "0123456789??????"[val & 0xf]; }
    inline char to_hex_char(u8 val, bool lowercase) { return (lowercase) ? "0123456789abcdef"[val & 0xf] : "0123456789ABCDEF"[val & 0xf]; }
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
    void findReplace(runes_t& str, uchar32 find, uchar32 replace, bool case_sensitive = true);
    void findReplace(runes_t& str, crunes_t const& find, crunes_t const& replace, bool case_sensitive = true);
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
            SIZE = L
        };
        T m_run[SIZE];
        inline runez_t()
            : runes_t(m_run, 0, 0, SIZE - 1)
        {
        }
        inline runez_t(uchar32 c)
            : runes_t(m_run, 0, 1, SIZE - 1)
        {
            m_run[0] = c;
            m_run[1] = 0;
        }
        inline runez_t(const char* _str)
            : runes_t(m_run, 0, 0, SIZE - 1)
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
            SIZE = L
        };
        T m_run[SIZE];

        inline crunez_t(uchar32 c)
            : crunes_t(m_run, 0, 1, SIZE - 1)
        {
            m_run[0] = c;
            m_run[1] = 0;
        }
    };

    // -------------------------------------------------------------------------------
    // runes reader and writer
    class irunes_reader_t
    {
    public:
        void    reset() { vreset(); }
        bool    valid() const { return vvalid(); }
        uchar32 peek(s32 n = 0) const { return vpeek(n); }
        bool    read(uchar32& c) { return vread(c); }
        bool    read_line(runes_t& line) { return vread_line(line); }
        bool    view_line(crunes_t& line) { return vview_line(line); }
        void    skip(s32 c = 1) { vskip(c); }

        inline uchar32 read()
        {
            uchar32 c;
            vread(c);
            return c;
        }

    protected:
        virtual void    vreset()                   = 0;
        virtual bool    vvalid() const             = 0;
        virtual uchar32 vpeek(s32 n) const         = 0;
        virtual bool    vread(uchar32& c)          = 0;
        virtual bool    vread_line(runes_t& line)  = 0;
        virtual bool    vview_line(crunes_t& line) = 0;
        virtual void    vskip(s32 c)               = 0;
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

        runes_reader_t select(u32 const& from, u32 to) const;
        u32            get_cursor() const { return m_cursor; }
        void           set_cursor(u32 const& c) { m_cursor = c; }

        crunes_t get_source() const;
        crunes_t get_current() const;
        bool     at_end() const;

    protected:
        virtual bool    vvalid() const;
        virtual void    vreset();
        virtual uchar32 vpeek(s32 n) const;
        virtual bool    vread(uchar32& c);
        virtual bool    vread_line(runes_t& line);
        virtual bool    vview_line(crunes_t& line);
        virtual void    vskip(s32 c);

        crunes_t m_runes;
        u32      m_cursor;
    };

    class irunes_writer_t
    {
    public:
        bool write(uchar32 c) { return vwrite(c); }
        bool write(const char* str, const char* end) { return vwrite(str, end); }
        bool write(crunes_t const& str) { return vwrite(str); }
        bool writeln(crunes_t const& str)
        {
            write(str);
            writeln();
        }
        bool writeln()
        {
            const char* eos = "\n";
            return vwrite(eos, eos + 1);
        }
        void flush() { vflush(); }

    protected:
        virtual ~irunes_writer_t() {}
        virtual bool vwrite(uchar32 c)                        = 0;
        virtual bool vwrite(const char* str, const char* end) = 0;
        virtual bool vwrite(crunes_t const& str)              = 0;
        virtual void vflush()                                 = 0;
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
        virtual bool vwrite(const char* str, const char* end);
        virtual bool vwrite(crunes_t const& str);
        virtual void vflush();

        runes_t m_runes;
        u32     m_cursor;
        s32     m_count;
    };

}; // namespace ncore

#endif ///< __CBASE_RUNES2_H__
