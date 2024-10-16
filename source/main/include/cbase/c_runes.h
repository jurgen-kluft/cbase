#ifndef __CBASE_RUNES_V2_H__
#define __CBASE_RUNES_V2_H__
#include "ccore/c_target.h"
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

    const uchar32 cEOS = 0x00;  // \0, end of string
    const uchar32 cEOF = 0x05;  // end of file
    const uchar32 cEOL = 0x0A;  // \n, end of line
    const uchar32 cCR  = 0x0D;  // \r, carriage return
    const uchar32 cTAB = 0x09;  // \t, tab

    namespace ascii
    {
        typedef char        rune;
        typedef rune*       prune;
        typedef const rune* pcrune;
        static const char   TERMINATOR   = '\0';
        static const u8     TYPE         = 1;
        static const u64    EMPTY_STRING = 0;

        void reverse(char* str, char* end);

        // return false when there is not enough space in the output
        // would need at least 10 characters for u32 and 20 for u64
        char* utoa(u32 val, char* cursor, char const* end, s32 base, bool octzero = false, bool lowercase = true);
        char* itoa(s32 val, char* cursor, char const* end, s32 base, bool octzero = false, bool lowercase = true);
        char* utoa(u64 val, char* cursor, char const* end, s32 base, bool octzero = false, bool lowercase = true);
        char* itoa(s64 val, char* cursor, char const* end, s32 base, bool octzero = false, bool lowercase = true);
        char* ftoa(f32 val, char* cursor, char const* end, bool lowercase = true);
        char* dtoa(f64 val, char* cursor, char const* end, bool lowercase = true);
        enum EBoolTypes
        {
            TrueFalse = 0,
            YesNo     = 1,
            LowerCase = 0,
            UpperCase = 2,
            CamelCase = 4,
        };
        char* btoa(bool val, char* cursor, char const* end, s8 caseType = TrueFalse | LowerCase);

        s32 compare(pcrune left, pcrune right);
        s32 compare(pcrune str1, pcrune str2, pcrune end1, pcrune end2);
        s32 strlen(pcrune str, pcrune* end = nullptr, pcrune eos = nullptr);
    }  // namespace ascii

    namespace ucs2
    {
        static const u8      TYPE         = 8;
        static const uchar16 TERMINATOR   = {'\0'};
        static const u64     EMPTY_STRING = 0;

        struct rune
        {
            uchar16 r;
        };
        typedef rune*       prune;
        typedef const rune* pcrune;

        s32 compare(pcrune str1, u32 len1, pcrune str2, u32 len2);
        s32 strlen(pcrune str, pcrune& end, pcrune eos);  // return length of string in runes

    }  // namespace ucs2

    namespace utf8
    {
        static const u8  TYPE         = 16;
        static const u8  TERMINATOR   = '\0';
        static const u64 EMPTY_STRING = 0;
        struct rune
        {
            u8 r;
        };
        typedef rune*       prune;
        typedef const rune* pcrune;

        s32 compare(pcrune str1, u32 len1, pcrune str2, u32 len2);
        s32 strlen(pcrune str, pcrune& end, pcrune eos);  // return length of string in runes

    }  // namespace utf8

    namespace utf16
    {
        static const u8      TYPE         = 2;
        static const uchar16 TERMINATOR   = '\0';
        static const u64     EMPTY_STRING = 0;
        struct rune
        {
            uchar16 r;
        };
        typedef rune*       prune;
        typedef const rune* pcrune;

        s32 compare(pcrune str1, u32 len1, pcrune str2, u32 len2);
        s32 strlen(pcrune str, pcrune& end, pcrune eos);

    }  // namespace utf16

    namespace utf32
    {
        static const u8      TYPE         = 4;
        static const uchar32 TERMINATOR   = '\0';
        static const u64     EMPTY_STRING = 0;
        struct rune
        {
            uchar32 r;
        };
        typedef rune*       prune;
        typedef const rune* pcrune;

        s32 compare(pcrune str1, u32 len1, pcrune str2, u32 len2);
        s32 strlen(pcrune str, pcrune& end, pcrune eos);
    }  // namespace utf32

    struct crunes_t;
    struct runes_t
    {
        union
        {
            ascii::prune m_ascii;
            ucs2::prune  m_ucs2;
            utf8::prune  m_utf8;
            utf16::prune m_utf16;
            utf32::prune m_utf32;
        };
        u32 m_str;   // ptr[m_str] is the first character
        u32 m_end;   // ptr[m_end] is one past the last character
        u32 m_eos;   // ptr[m_eos] is the end of the string but always points to a terminator
        u8  m_type;  // type of string (ascii, ucs2, utf8, utf16, utf32)
    };

    runes_t make_runes();
    runes_t make_runes(runes_t const& other);

    runes_t make_runes(ascii::prune _str, ascii::prune _end, u32 _type = ascii::TYPE);
    runes_t make_runes(ascii::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = ascii::TYPE);

    runes_t make_runes(ucs2::prune _str, ucs2::prune _end, u32 _type = ucs2::TYPE);
    runes_t make_runes(ucs2::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = ucs2::TYPE);

    runes_t make_runes(utf8::prune _str, utf8::prune _end, u32 _type = utf8::TYPE);
    runes_t make_runes(utf8::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = utf8::TYPE);

    runes_t make_runes(utf16::prune _str, utf16::prune _end, u32 _type = utf16::TYPE);
    runes_t make_runes(utf16::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = utf16::TYPE);

    runes_t make_runes(utf32::prune _str, utf32::prune _end, u32 _type = utf32::TYPE);
    runes_t make_runes(utf32::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = utf32::TYPE);

    inline void reset(runes_t& str) { str.m_str = str.m_end; }
    inline bool is_empty(runes_t const& str) { return str.m_str == str.m_end || str.m_ascii == nullptr; }

    struct crunes_t
    {
        union
        {  // ptr
            ascii::pcrune m_ascii;
            ucs2::pcrune  m_ucs2;
            utf8::pcrune  m_utf8;
            utf16::pcrune m_utf16;
            utf32::pcrune m_utf32;
        };
        u32 m_str;   // ptr[m_str] is the first character
        u32 m_end;   // ptr[m_end] is one past the last character
        u32 m_eos;   // ptr[m_eos] is the end of the string but always points to a terminator
        u8  m_type;  // type of string (ascii, ucs2, utf8, utf16, utf32)
    };

    crunes_t make_crunes();

    crunes_t make_crunes(ascii::pcrune _bos, u32 _str, u32 _end, u32 _eos, u8 _type);

    crunes_t make_crunes(ascii::pcrune _bos);
    crunes_t make_crunes(ascii::pcrune _bos, ascii::pcrune _end);
    crunes_t make_crunes(ascii::pcrune _bos, u32 _str, u32 _end, u32 _eos);

    crunes_t make_crunes(ucs2::pcrune _bos);
    crunes_t make_crunes(ucs2::pcrune _bos, ucs2::pcrune _end);
    crunes_t make_crunes(ucs2::pcrune _bos, u32 _str, u32 _end, u32 _eos);

    crunes_t make_crunes(utf8::pcrune _str);
    crunes_t make_crunes(utf8::pcrune _str, utf8::pcrune _end);
    crunes_t make_crunes(utf8::pcrune _bos, u32 _str, u32 _end, u32 _eos);

    crunes_t make_crunes(utf16::pcrune _str);
    crunes_t make_crunes(utf16::pcrune _str, utf16::pcrune _end);
    crunes_t make_crunes(utf16::pcrune _bos, u32 _str, u32 _end, u32 _eos);

    crunes_t make_crunes(utf32::pcrune _str);
    crunes_t make_crunes(utf32::pcrune _str, utf32::pcrune _end);
    crunes_t make_crunes(utf32::pcrune _bos, u32 _str, u32 _end, u32 _eos);

    crunes_t make_crunes(runes_t const& other);
    crunes_t make_crunes(runes_t const& other, u32 from, u32 to);
    crunes_t make_crunes(crunes_t const& other);
    crunes_t make_crunes(crunes_t const& other, u32 from, u32 to);

    inline bool is_empty(crunes_t const& str) { return str.m_str == str.m_end || str.m_ascii == nullptr; }

    namespace nrunes
    {
        // -------------------------------------------------------------------------------
        // conversion (note: cursor and end indices are byte based indices)
        // Warning: string terminators are NOT copied to the output string
        // ascii -> ucs2, utf8, utf16, utf32
        s32 convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& outCursor, u32 outStrEnd);
        s32 convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& outCursor, u32 outStrEnd);
        s32 convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& outCursor, u32 outStrEnd);
        s32 convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& outCursor, u32 outStrEnd);
        // ucs2 -> ascii, utf8, utf16, utf32
        s32 convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& cursor, u32 outStrEnd);
        // utf8 -> ascii, ucs2, utf16, utf32
        s32 convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& cursor, u32 outStrEnd);
        // utf16 -> ascii, ucs2, utf8, utf32
        s32 convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& cursor, u32 outStrEnd);
        // utf32 -> ascii, ucs2, utf8, utf16
        s32 convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& cursor, u32 outStrEnd);
        s32 convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& cursor, u32 outStrEnd);

        // -------------------------------------------------------------------------------
        // contains
        bool contains(crunes_t const& _str, uchar32 _c, bool case_sensitive = true);
        bool contains(runes_t const& _str, uchar32 _c, bool case_sensitive = true);

        // -------------------------------------------------------------------------------
        // find
        crunes_t find(crunes_t const& str, crunes_t const& find, bool case_sensitive = true);
        runes_t  find(runes_t const& str, crunes_t const& find, bool case_sensitive = true);
        crunes_t findLast(crunes_t const& str, crunes_t const& find, bool case_sensitive = true);
        runes_t  findLast(runes_t const& str, crunes_t const& find, bool case_sensitive = true);
        crunes_t findOneOf(crunes_t const& str, crunes_t const& set, bool case_sensitive = true);
        runes_t  findOneOf(runes_t const& str, crunes_t const& set, bool case_sensitive = true);

        crunes_t find(crunes_t const& _str, uchar32 _c, bool case_sensitive = true);
        crunes_t findLast(crunes_t const& _str, uchar32 _c, bool case_sensitive = true);
        runes_t  find(runes_t const& _str, uchar32 _c, bool case_sensitive = true);
        runes_t  findLast(runes_t const& _str, uchar32 _c, bool case_sensitive = true);

        crunes_t findSelectUntil(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
        crunes_t findLastSelectUntil(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
        crunes_t findSelectUntilIncluded(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
        crunes_t findSelectUntilIncludedAbortAtOneOf(const crunes_t& inStr, const crunes_t& inFind, const crunes_t& inAbortAny, bool case_sensitive = true);
        crunes_t findLastSelectUntilIncluded(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
        crunes_t findSelectAfter(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
        crunes_t findLastSelectAfter(const crunes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);

        crunes_t findSelectUntil(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
        crunes_t findLastSelectUntil(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
        crunes_t findSelectUntilIncluded(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
        crunes_t findLastSelectUntilIncluded(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
        crunes_t findSelectAfter(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);
        crunes_t findLastSelectAfter(const crunes_t& inStr, uchar32 inFind, bool case_sensitive = true);

        // -------------------------------------------------------------------------------
        // expand
        bool selectMoreRight(crunes_t& inStr, uchar32 inChar);

        // -------------------------------------------------------------------------------
        // select
        // e.g. selectBetween(str, '<', '>');
        crunes_t selectFromToInclude(const crunes_t& inStr, crunes_t const& inFrom, crunes_t const& inTo);
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
        // find and select
        runes_t findSelectUntil(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
        runes_t findLastSelectUntil(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
        runes_t findSelectUntilIncluded(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
        runes_t findLastSelectUntilIncluded(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
        runes_t findSelectAfter(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);
        runes_t findLastSelectAfter(const runes_t& inStr, const crunes_t& inFind, bool case_sensitive = true);

        runes_t findSelectUntil(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
        runes_t findLastSelectUntil(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
        runes_t findSelectUntilIncluded(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
        runes_t findLastSelectUntilIncluded(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
        runes_t findSelectAfter(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);
        runes_t findLastSelectAfter(const runes_t& inStr, uchar32 inFind, bool case_sensitive = true);

        // -------------------------------------------------------------------------------
        // select
        runes_t selectBetween(const runes_t& inStr, uchar32 inLeft, uchar32 inRight);
        runes_t selectBetweenLast(const runes_t& inStr, uchar32 inLeft, uchar32 inRight);
        runes_t selectNextBetween(const runes_t& inStr, const runes_t& inSelection, uchar32 inLeft, uchar32 inRight);
        runes_t selectPreviousBetween(const runes_t& inStr, const runes_t& inSelection, uchar32 inLeft, uchar32 inRight);
        runes_t selectBeforeExclude(const runes_t& inStr, const runes_t& inSelection);
        runes_t selectBeforeInclude(const runes_t& inStr, const runes_t& inSelection);
        runes_t selectAfterExclude(const runes_t& inStr, const runes_t& inSelection);
        runes_t selectAfterInclude(const runes_t& inStr, const runes_t& inSelection);
        runes_t selectOverlap(const runes_t& inStr, const runes_t& inRight);

        // -------------------------------------------------------------------------------
        // compare
        s32 compare(crunes_t const& str1, crunes_t const& str2, bool case_sensitive = true);
        s32 compare(crunes_t const& str1, runes_t const& str2, bool case_sensitive = true);
        s32 compare(runes_t const& str1, runes_t const& str2, bool case_sensitive = true);
        s32 compare(runes_t const& str1, crunes_t const& str2, bool case_sensitive = true);

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

        // -------------------------------------------------------------------------------
        // integer and float value to string
        void to_string(runes_t& str, s32 val, s32 base = 10);
        void to_string(runes_t& str, u32 val, s32 base = 10);
        void to_string(runes_t& str, s64 val, s32 base = 10);
        void to_string(runes_t& str, u64 val, s32 base = 10);
        void to_string(runes_t& str, f32 val, s32 num_fractional_digits = 4);
        void to_string(runes_t& str, f64 val, s32 num_fractional_digits = 4);

        // -------------------------------------------------------------------------------
        // filters
        inline bool    is_space(uchar32 c) { return ((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
        inline bool    is_whitespace(uchar32 c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }
        inline bool    is_upper(uchar32 c) { return ((c >= 'A') && (c <= 'Z')); }
        inline bool    is_lower(uchar32 c) { return ((c >= 'a') && (c <= 'z')); }
        inline bool    is_alpha(uchar32 c) { return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
        inline bool    is_digit(uchar32 c) { return ((c >= '0') && (c <= '9')); }
        inline bool    is_hexa(uchar32 c) { return (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }
        inline uchar32 to_upper(uchar32 c) { return ((c >= 'a') && (c <= 'z')) ? c + (uchar32)('A' - 'a') : c; }
        inline uchar32 to_lower(uchar32 c) { return ((c >= 'A') && (c <= 'Z')) ? c + (uchar32)('a' - 'A') : c; }
        inline u32     to_digit(uchar32 c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
        inline u32     hex_to_number(uchar32 c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : ((c >= 'A') && (c <= 'F')) ? (c - 'A' + 10) : ((c >= 'a') && (c <= 'f')) ? (c - 'a' + 10) : 0; }
        inline char    to_dec_char(u8 val) { return "0123456789??????"[val & 0xf]; }
        inline char    to_hex_char(u8 val, bool lowercase) { return (lowercase) ? "0123456789abcdef"[val & 0xf] : "0123456789ABCDEF"[val & 0xf]; }
        inline bool    is_equal(uchar32 a, uchar32 b) { return (a == b); }
        inline bool    is_equalfold(uchar32 a, uchar32 b) { return (to_lower(a) == to_lower(b)); }

        // -------------------------------------------------------------------------------
        // checks
        bool is_decimal(crunes_t const& str);
        bool is_hexadecimal(crunes_t const& str, bool with_prefix = false);
        bool is_float(crunes_t const& str);
        bool is_GUID(crunes_t const& str);
        void to_upper(runes_t& str);
        void to_lower(runes_t& str);
        bool is_upper(crunes_t const& str);
        bool is_lower(crunes_t const& str);
        bool is_capitalized(crunes_t const& str);
        bool is_delimited(crunes_t const& str, uchar32 delimit_left = '\"', uchar32 delimit_right = '\"');
        bool is_quoted(crunes_t const& str, uchar32 quote = '\"');
        bool starts_with(crunes_t const& str, uchar32 start);
        bool starts_with(crunes_t const& str, crunes_t const& start, bool case_sensitive = true);
        bool ends_with(crunes_t const& str, uchar32 end_char);
        bool ends_with(crunes_t const& str, crunes_t const& end);

        // -------------------------------------------------------------------------------
        // first and last character
        uchar32 first_char(crunes_t const& str);
        uchar32 last_char(crunes_t const& str);

        // -------------------------------------------------------------------------------
        // modifiers
        void removeSelection(runes_t& str, crunes_t const& sel);
        void keepOnlySelection(runes_t& str, crunes_t const& sel);
        void replaceSelection(runes_t& str, crunes_t const& sel, crunes_t const& replace);
        void findReplace(runes_t& str, uchar32 find, uchar32 replace, bool case_sensitive = true);
        void findReplace(runes_t& str, crunes_t const& find, crunes_t const& replace, bool case_sensitive = true);
        void insert(runes_t& str, crunes_t const& insert);
        void insert(runes_t& str, crunes_t const& sel, crunes_t const& insert);

        // -------------------------------------------------------------------------------
        // trim
        void trim(runes_t&);                                             // Trim whitespace from left and right side
        void trimLeft(runes_t&);                                         // Trim whitespace from left side
        void trimRight(runes_t&);                                        // Trim whitespace from right side
        void trim(runes_t&, uchar32 inChar);                             // Trim characters in <inCharSet> from left and right side
        void trimLeft(runes_t&, uchar32 inChar);                         // Trim character <inChar> from left side
        void trimRight(runes_t&, uchar32 inChar);                        // Trim character <inChar> from right side
        void trim(runes_t&, crunes_t const& inCharSet);                  // Trim characters in <inCharSet> from left and right side
        void trimLeft(runes_t&, crunes_t const& inCharSet);              // Trim characters in <inCharSet> from left side
        void trimRight(runes_t&, crunes_t const& inCharSet);             // Trim characters in <inCharSet> from right side
        void trimQuotes(runes_t&);                                       // Trim double quotes from left and right side
        void trimQuotes(runes_t&, uchar32 quote);                        // Trim double quotes from left and right side
        void trimDelimiters(runes_t&, uchar32 inLeft, uchar32 inRight);  // Trim delimiters from left and right side

        void trim(crunes_t&);                                             // Trim whitespace from left and right side
        void trimLeft(crunes_t&);                                         // Trim whitespace from left side
        void trimRight(crunes_t&);                                        // Trim whitespace from right side
        void trim(crunes_t&, uchar32 inChar);                             // Trim characters in <inCharSet> from left and right side
        void trimLeft(crunes_t&, uchar32 inChar);                         // Trim character <inChar> from left side
        void trimRight(crunes_t&, uchar32 inChar);                        // Trim character <inChar> from right side
        void trim(crunes_t&, crunes_t const& inCharSet);                  // Trim characters in <inCharSet> from left and right side
        void trimLeft(crunes_t&, crunes_t const& inCharSet);              // Trim characters in <inCharSet> from left side
        void trimRight(crunes_t&, crunes_t const& inCharSet);             // Trim characters in <inCharSet> from right side
        void trimQuotes(crunes_t&);                                       // Trim double quotes from left and right side
        void trimQuotes(crunes_t&, uchar32 quote);                        // Trim double quotes from left and right side
        void trimDelimiters(crunes_t&, uchar32 inLeft, uchar32 inRight);  // Trim delimiters from left and right side

        // -------------------------------------------------------------------------------
        // reading
        uchar32 read(crunes_t const& str, u32& cursor);

        // -------------------------------------------------------------------------------
        // copy and concatenate
        bool copy(const crunes_t& src, runes_t& dst);
        bool concatenate(runes_t& str, const crunes_t& concat);
        bool concatenate(runes_t& str, const crunes_t& concat1, const crunes_t& concat2);

        // -------------------------------------------------------------------------------
        // runes reader and writer
        class ireader_t
        {
        public:
            void    reset() { vreset(); }
            bool    valid() const { return vvalid(); }
            uchar32 peek() const
            {
                uchar32 c;
                vpeek(0, c);
                return c;
            }
            bool peek(uchar32& c) const { return vpeek(0, c); }
            bool peek(s32 n, uchar32& c) const { return vpeek(n, c); }
            bool read(uchar32& c) { return vread(c); }
            bool read_line(runes_t& line) { return vread_line(line); }
            bool view_line(crunes_t& line) { return vview_line(line); }
            void skip(s32 c = 1) { vskip(c); }
            s32  skip_any(uchar32 const* chars, u32 num_chars) { return vskip_any(chars, num_chars); }
            s32  skip_until_one_of(uchar32 const* chars, u32 num_chars) { return vskip_until_one_of(chars, num_chars); }

            inline uchar32 read()
            {
                uchar32 c;
                vread(c);
                return c;
            }

        protected:
            virtual void vreset()                                                = 0;
            virtual bool vvalid() const                                          = 0;
            virtual bool vpeek(s32 n, uchar32& c) const                          = 0;
            virtual bool vread(uchar32& c)                                       = 0;
            virtual bool vread_line(runes_t& line)                               = 0;
            virtual bool vview_line(crunes_t& line)                              = 0;
            virtual void vskip(s32 c)                                            = 0;
            virtual s32  vskip_any(uchar32 const* chars, u32 num_chars)          = 0;  // return -1 if end of string is reached, otherwise return the number of characters skipped
            virtual s32  vskip_until_one_of(uchar32 const* chars, u32 num_chars) = 0;  // return -1 if end of string is reached, otherwise return the number of characters skipped
        };

        class reader_t : public ireader_t
        {
        public:
            reader_t();
            reader_t(ascii::pcrune str);
            reader_t(ascii::pcrune str, u32 len);
            reader_t(ascii::pcrune str, ascii::pcrune str_end);
            reader_t(utf8::pcrune str, utf8::pcrune str_end);
            reader_t(utf16::pcrune str, utf16::pcrune str_end);
            reader_t(utf32::pcrune str, utf32::pcrune str_end);
            reader_t(crunes_t const& runes);

            reader_t select(u32 const& from, u32 to) const;
            u32      get_cursor() const { return m_cursor; }
            void     set_cursor(u32 const& c) { m_cursor = c; }

            crunes_t get_source() const;
            crunes_t get_current() const;
            bool     at_end() const;

        protected:
            virtual bool vvalid() const;
            virtual void vreset();
            virtual bool vpeek(s32 n, uchar32& c) const;
            virtual bool vread(uchar32& c);
            virtual bool vread_line(runes_t& line);
            virtual bool vview_line(crunes_t& line);
            virtual void vskip(s32 c);
            virtual s32  vskip_any(uchar32 const* chars, u32 num_chars);
            virtual s32  vskip_until_one_of(uchar32 const* chars, u32 num_chars);

            crunes_t m_runes;
            u32      m_cursor;
        };

        class iwriter_t
        {
        public:
            s32 write(uchar32 c) { return vwrite(c); }
            s32 write(const char* str);
            s32 write(const char* str, const char* end) { return vwrite(str, end); }
            s32 write(runes_t const& str)
            {
                crunes_t cstr = make_crunes(str);
                return vwrite(cstr);
            }
            s32  write(crunes_t const& str) { return vwrite(str); }
            s32  writeln(runes_t const& str) { return write(str) + writeln(); }
            s32  writeln(crunes_t const& str) { return write(str) + writeln(); }
            bool writeln()
            {
                const char* eos = "\n";
                return vwrite(eos, eos + 1);
            }
            void flush() { vflush(); }

        protected:
            virtual ~iwriter_t() {}
            virtual s32  vwrite(uchar32 c)                        = 0;
            virtual s32  vwrite(const char* str)                  = 0;
            virtual s32  vwrite(const char* str, const char* end) = 0;
            virtual s32  vwrite(crunes_t const& str)              = 0;
            virtual void vflush()                                 = 0;
        };

        class writer_t : public iwriter_t
        {
        public:
            writer_t(ascii::prune str, ascii::prune str_end);
            writer_t(ucs2::prune str, ucs2::prune str_end);
            writer_t(utf8::prune str, utf8::prune str_end);
            writer_t(utf16::prune str, utf16::prune str_end);
            writer_t(utf32::prune str, utf32::prune str_end);
            writer_t(runes_t const& runes);

            runes_t get_destination() const;
            runes_t get_current() const;

            void reset();
            s32  count() const;

        protected:
            virtual s32  vwrite(uchar32 c);
            virtual s32  vwrite(const char* str);
            virtual s32  vwrite(const char* str, const char* end);
            virtual s32  vwrite(crunes_t const& str);
            virtual void vflush();

            runes_t m_runes;
            u32     m_cursor;
            s32     m_count;
        };

    }  // namespace nrunes

    // -------------------------------------------------------------------------------
    // global operators for comparison
    inline bool operator==(const crunes_t& lhs, const crunes_t& rhs) { return nrunes::compare(lhs, rhs) == 0; }
    inline bool operator!=(const crunes_t& lhs, const crunes_t& rhs) { return nrunes::compare(lhs, rhs) != 0; }
    inline bool operator==(const runes_t& lhs, const runes_t& rhs) { return nrunes::compare(lhs, rhs) == 0; }
    inline bool operator!=(const runes_t& lhs, const runes_t& rhs) { return nrunes::compare(lhs, rhs) != 0; }
};  // namespace ncore

#endif  ///< __CBASE_RUNES2_H__
