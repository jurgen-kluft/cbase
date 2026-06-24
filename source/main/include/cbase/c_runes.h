#ifndef __CBASE_RUNES_V2_H__
#define __CBASE_RUNES_V2_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_runes.h"

namespace ncore
{
    struct str_t
    {
        union
        {
            ascii::pcrune m_const_ascii;
            utf8::pcrune  m_const_utf8;
            ucs2::pcrune  m_const_ucs2;
            utf16::pcrune m_const_utf16;
            utf32::pcrune m_const_utf32;
            ascii::prune  m_ascii;
            utf8::prune   m_utf8;
            ucs2::prune   m_ucs2;
            utf16::prune  m_utf16;
            utf32::prune  m_utf32;
        };
        u32 m_str;   // ptr[m_str] is the first character
        u32 m_end;   // ptr[m_end] is one past the last character
        u32 m_eos;   // ptr[m_eos] is the end of the string but always points to a terminator
        u8  m_type;  // type of string (ascii, ucs2, utf8, utf16, utf32)
    };

    namespace ascii
    {
        char* ftoa(f32 val, char* cursor, char const* end, bool lowercase = true);
        char* dtoa(f64 val, char* cursor, char const* end, bool lowercase = true);
    }  // namespace ascii

    namespace ascii
    {
        str_t make_const_runes(pcrune _bos);
        str_t make_const_runes(pcrune _bos, pcrune _end);

        str_t make_const_runes(pcrune _bos, u32 _str, u32 _end, u32 _eos);
        str_t make_const_runes(pcrune _bos, u32 _str, u32 _end, u32 _eos, u8 _type);

        str_t make_runes(prune _str, prune _end, u32 _type = ascii::TYPE);
        str_t make_runes(prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = ascii::TYPE);
    }  // namespace ascii

    namespace ucs2
    {
        str_t make_const_runes(pcrune _bos);
        str_t make_const_runes(pcrune _bos, pcrune _end);

        str_t make_const_runes(pcrune _bos, u32 _str, u32 _end, u32 _eos);
        str_t make_const_runes(pcrune _bos, u32 _str, u32 _end, u32 _eos, u8 _type);

        str_t make_runes(prune _str, prune _end, u32 _type = ucs2::TYPE);
        str_t make_runes(prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = ucs2::TYPE);
    }  // namespace ucs2
    namespace utf8
    {
        str_t make_const_runes(pcrune _bos);
        str_t make_const_runes(pcrune _bos, pcrune _end);

        str_t make_const_runes(pcrune _bos, u32 _str, u32 _end, u32 _eos);
        str_t make_const_runes(pcrune _bos, u32 _str, u32 _end, u32 _eos, u8 _type);

        str_t make_runes(prune _str, prune _end, u32 _type = utf8::TYPE);
        str_t make_runes(prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = utf8::TYPE);
    }  // namespace utf8
    namespace utf16
    {
        str_t make_const_runes(pcrune _bos);
        str_t make_const_runes(pcrune _bos, pcrune _end);

        str_t make_const_runes(pcrune _bos, u32 _str, u32 _end, u32 _eos);
        str_t make_const_runes(pcrune _bos, u32 _str, u32 _end, u32 _eos, u8 _type);

        str_t make_runes(prune _str, prune _end, u32 _type = utf16::TYPE);
        str_t make_runes(prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = utf16::TYPE);
    }  // namespace utf16
    namespace utf32
    {
        str_t make_const_runes(pcrune _bos);
        str_t make_const_runes(pcrune _bos, pcrune _end);

        str_t make_const_runes(pcrune _bos, u32 _str, u32 _end, u32 _eos);
        str_t make_const_runes(pcrune _bos, u32 _str, u32 _end, u32 _eos, u8 _type);

        str_t make_runes(prune _str, prune _end, u32 _type = utf32::TYPE);
        str_t make_runes(prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type = utf32::TYPE);
    }  // namespace utf32

    str_t make_runes();
    str_t make_runes(str_t const& other);
    str_t make_runes(str_t const& other, u32 from, u32 to);

    inline bool is_empty(str_t const& str) { return str.m_str == str.m_end || str.m_ascii == nullptr; }
    inline void reset(str_t& str) { str.m_end = str.m_str; }

    namespace utf
    {
        enum EUnicode
        {
            UTF_MAX_CODEPOINT                = 0x10FFFF,  // The highest valid Unicode codepoint
            UTF16_BMP_END                    = 0xFFFF,    // Basic Multilingual Plane, that part of Unicode that UTF-16 can encode without surrogates
            UTF16_INVALID_CODEPOINT          = 0xFFFD,    // The codepoint that is used to replace invalid encodings
            UTF16_GENERIC_SURROGATE_VALUE    = 0xD800,    // If a character, masked with UTF16_GENERIC_SURROGATE_MASK, matches this value, it is a surrogate.
            UTF16_GENERIC_SURROGATE_MASK     = 0xF800,    // The mask to apply to a character before testing it against UTF16_GENERIC_SURROGATE_VALUE
            UTF16_HIGH_SURROGATE_VALUE       = 0xD800,    // If a character, masked with UTF16_SURROGATE_MASK, matches this value, it is a high surrogate.
            UTF16_HIGH_SURROGATE_MIN         = 0xD800,    // The minimum value of a high surrogate
            UTF16_HIGH_SURROGATE_MAX         = 0xDBFF,    // The maximum value of a high surrogate
            UTF16_LOW_SURROGATE_VALUE        = 0xDC00,    // If a character, masked with UTF16_SURROGATE_MASK, matches this value, it is a low surrogate.
            UTF16_MIN_SURROGATE              = 0xD800,    // The minimum value of a surrogate
            UTF16_MAX_SURROGATE              = 0xDFFF,    // The maximum value of a surrogate
            UTF16_SURROGATE_MASK             = 0xFC00,    // The mask to apply to a character before testing it against UTF16_HIGH_SURROGATE_VALUE or UTF16_LOW_SURROGATE_VALUE
            UTF16_SURROGATE_CODEPOINT_OFFSET = 0x10000,   // The value that is subtracted from a codepoint before encoding it in a surrogate pair
            UTF16_SURROGATE_CODEPOINT_MASK   = 0x03FF,    // A mask that can be applied to a surrogate to extract the codepoint value contained in it
            UTF16_SURROGATE_CODEPOINT_BITS   = 10,        // The number of bits of UTF16_SURROGATE_CODEPOINT_MASK
            UTF8_1_MAX                       = 0x7F,      // The highest codepoint that can be encoded with 1 byte in UTF-8
            UTF8_2_MAX                       = 0x7FF,     // The highest codepoint that can be encoded with 2 bytes in UTF-8
            UTF8_3_MAX                       = 0xFFFF,    // The highest codepoint that can be encoded with 3 bytes in UTF-8
            UTF8_4_MAX                       = 0x10FFFF,  // The highest codepoint that can be encoded with 4 bytes in UTF-8
            UTF8_CONTINUATION_VALUE          = 0x80,      // If a character, masked with UTF8_CONTINUATION_MASK, matches this value, it is a UTF-8 continuation byte
            UTF8_CONTINUATION_MASK           = 0xC0,      // The mask to a apply to a character before testing it against UTF8_CONTINUATION_VALUE
            UTF8_CONTINUATION_CODEPOINT_BITS = 6          // The number of bits of a codepoint that are contained in a UTF-8 continuation byte
        };
    }

    // -------------------------------------------------------------------------------
    // conversion (note: cursor and end indices are byte based indices)
    // Warning: string terminators are NOT copied to the output string
    // ascii -> ucs2, utf8, utf16, utf32
    namespace ascii
    {
        void convert(ascii::pcrune inStr, u32& inCursor, ascii::prune outStr, u32& outCursor, u32 outStrEnd);
        void convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& outCursor, u32 outStrEnd);

        namespace to_utf8
        {
            void convert(ascii::pcrune inStr, u32& inCursor, utf8::prune outStr, u32& outCursor, u32 outStrEnd);
        }
        namespace to_ucs2
        {
            void convert(ascii::pcrune inStr, u32& inCursor, ucs2::prune outStr, u32& outCursor, u32 outStrEnd);
        }
        namespace to_utf16
        {
            void convert(ascii::pcrune inStr, u32& inCursor, utf16::prune outStr, u32& outCursor, u32 outStrEnd);
        }
        namespace to_utf32
        {
            void convert(ascii::pcrune inStr, u32& inCursor, utf32::prune outStr, u32& outCursor, u32 outStrEnd);
        }

        namespace to_utf8
        {
            void convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& outCursor, u32 outStrEnd);
        }
        namespace to_ucs2
        {
            void convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& outCursor, u32 outStrEnd);
        }
        namespace to_utf16
        {
            void convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& outCursor, u32 outStrEnd);
        }
        namespace to_utf32
        {
            void convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& outCursor, u32 outStrEnd);
        }
    }  // namespace ascii

    // ucs2 -> ascii, utf8, utf16, utf32
    namespace ucs2
    {
        void convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& cursor, u32 outStrEnd);

        namespace to_ascii
        {
            void convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_utf8
        {
            void convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_utf16
        {
            void convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_utf32
        {
            void convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& cursor, u32 outStrEnd);
        }
    }  // namespace ucs2

    // utf8 -> ascii, ucs2, utf16, utf32
    namespace utf8
    {
        void convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& cursor, u32 outStrEnd);

        namespace to_ascii
        {
            void convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_ucs2
        {
            void convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_utf16
        {
            void convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_utf32
        {
            void convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& cursor, u32 outStrEnd);
        }
    }  // namespace utf8

    // utf16 -> ascii, ucs2, utf8, utf32
    namespace utf16
    {
        void convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& cursor, u32 outStrEnd);

        namespace to_ascii
        {
            void convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_utf8
        {
            void convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_ucs2
        {
            void convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_utf32
        {
            void convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& cursor, u32 outStrEnd);
        }
    }  // namespace utf16

    // utf32 -> ascii, ucs2, utf8, utf16
    namespace utf32
    {
        void convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& cursor, u32 outStrEnd);

        namespace to_ascii
        {
            void convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_ucs2
        {
            void convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_utf8
        {
            void convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& cursor, u32 outStrEnd);
        }
        namespace to_utf16
        {
            void convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& cursor, u32 outStrEnd);
        }
    }  // namespace utf32

    // -------------------------------------------------------------------------------
    // contains
    bool contains(str_t const& _str, uchar32 _c, bool case_sensitive = true);

    // -------------------------------------------------------------------------------
    // expand
    bool selectMoreRight(str_t& inStr, uchar32 inChar);

    // -------------------------------------------------------------------------------
    // select
    // e.g. selectBetween(str, '<', '>');
    str_t selectFromToInclude(const str_t& inStr, str_t const& inFrom, str_t const& inTo);
    str_t selectBetween(const str_t& inStr, uchar32 inLeft, uchar32 inRight);
    str_t selectNextBetween(const str_t& inStr, const str_t& inSelection, uchar32 inLeft, uchar32 inRight);
    str_t selectBetweenLast(const str_t& inStr, uchar32 inLeft, uchar32 inRight);
    str_t selectPreviousBetween(const str_t& inStr, const str_t& inSelection, uchar32 inLeft, uchar32 inRight);

    // -------------------------------------------------------------------------------
    // select left and right of
    bool selectLeftAndRightOf(const str_t& inStr, uchar32 inPivot, str_t& outLeft, str_t& outRight);

    // -------------------------------------------------------------------------------
    // select before after
    str_t selectBeforeExclude(const str_t& inStr, const str_t& inSelection);
    str_t selectBeforeInclude(const str_t& inStr, const str_t& inSelection);
    str_t selectAfterExclude(const str_t& inStr, const str_t& inSelection);
    str_t selectAfterInclude(const str_t& inStr, const str_t& inSelection);
    str_t selectOverlap(const str_t& inStr, const str_t& inRight);

    // -------------------------------------------------------------------------------
    // find and select
    str_t find(str_t const& _str, uchar32 _c, bool case_sensitive = true);
    str_t findLast(str_t const& _str, uchar32 _c, bool case_sensitive = true);

    str_t find(str_t const& str, str_t const& find, bool case_sensitive = true);
    str_t findLast(str_t const& str, str_t const& find, bool case_sensitive = true);
    str_t findOneOf(str_t const& str, str_t const& set, bool case_sensitive = true);

    str_t findSelectUntil(const str_t& inStr, const str_t& inFind, bool case_sensitive = true);
    str_t findLastSelectUntil(const str_t& inStr, const str_t& inFind, bool case_sensitive = true);
    str_t findSelectUntilIncluded(const str_t& inStr, const str_t& inFind, bool case_sensitive = true);
    str_t findLastSelectUntilIncluded(const str_t& inStr, const str_t& inFind, bool case_sensitive = true);
    str_t findSelectUntilIncludedAbortAtOneOf(const str_t& inStr, const str_t& inFind, const str_t& inAbortAny, bool case_sensitive = true);
    str_t findSelectAfter(const str_t& inStr, const str_t& inFind, bool case_sensitive = true);
    str_t findLastSelectAfter(const str_t& inStr, const str_t& inFind, bool case_sensitive = true);

    str_t findSelectUntil(const str_t& inStr, uchar32 inFind, bool case_sensitive = true);
    str_t findLastSelectUntil(const str_t& inStr, uchar32 inFind, bool case_sensitive = true);
    str_t findSelectUntilIncluded(const str_t& inStr, uchar32 inFind, bool case_sensitive = true);
    str_t findLastSelectUntilIncluded(const str_t& inStr, uchar32 inFind, bool case_sensitive = true);
    str_t findSelectAfter(const str_t& inStr, uchar32 inFind, bool case_sensitive = true);
    str_t findLastSelectAfter(const str_t& inStr, uchar32 inFind, bool case_sensitive = true);

    // -------------------------------------------------------------------------------
    // compare
    s32 compare(str_t const& str1, str_t const& str2, bool case_sensitive = true);

    // -------------------------------------------------------------------------------
    // parse/from_string, to_string
    str_t parse(str_t const& str, bool& value);
    str_t parse(str_t const& str, s8& value, s32 base = 10);
    str_t parse(str_t const& str, s16& value, s32 base = 10);
    str_t parse(str_t const& str, s32& value, s32 base = 10);
    str_t parse(str_t const& str, s64& value, s32 base = 10);
    str_t parse(str_t const& str, u8& value, s32 base = 10);
    str_t parse(str_t const& str, u16& value, s32 base = 10);
    str_t parse(str_t const& str, u32& value, s32 base = 10);
    str_t parse(str_t const& str, u64& value, s32 base = 10);
    str_t parse(str_t const& str, f32& value);
    str_t parse(str_t const& str, f64& value);

    // -------------------------------------------------------------------------------
    u64 parse_mac(str_t const& str);

    // -------------------------------------------------------------------------------
    // integer and float value to string
    void to_string(str_t& str, s32 val, s32 base = 10);
    void to_string(str_t& str, u32 val, s32 base = 10);
    void to_string(str_t& str, s64 val, s32 base = 10);
    void to_string(str_t& str, u64 val, s32 base = 10);
    void to_string(str_t& str, f32 val, s32 num_fractional_digits = 4);
    void to_string(str_t& str, f64 val, s32 num_fractional_digits = 4);

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
    bool is_decimal(str_t const& str);
    bool is_hexadecimal(str_t const& str, bool with_prefix = false);
    bool is_float(str_t const& str);
    bool is_GUID(str_t const& str);
    void to_upper(str_t& str);
    void to_lower(str_t& str);
    bool is_upper(str_t const& str);
    bool is_lower(str_t const& str);
    bool is_capitalized(str_t const& str);
    bool is_delimited(str_t const& str, uchar32 delimit_left = '\"', uchar32 delimit_right = '\"');
    bool is_quoted(str_t const& str, uchar32 quote = '\"');
    bool starts_with(str_t const& str, uchar32 start);
    bool starts_with(str_t const& str, str_t const& start, bool case_sensitive = true);
    bool ends_with(str_t const& str, uchar32 end_char);
    bool ends_with(str_t const& str, str_t const& end);

    // -------------------------------------------------------------------------------
    // first and last character
    uchar32 first_char(str_t const& str);
    uchar32 last_char(str_t const& str);

    // -------------------------------------------------------------------------------
    // modifiers
    void removeSelection(str_t& str, str_t const& sel);
    void keepOnlySelection(str_t& str, str_t const& sel);
    void replaceSelection(str_t& str, str_t const& sel, str_t const& replace);
    void findReplace(str_t& str, uchar32 find, uchar32 replace, bool case_sensitive = true);
    void findReplace(str_t& str, str_t const& find, str_t const& replace, bool case_sensitive = true);
    void insert(str_t& str, str_t const& insert);
    void insert(str_t& str, str_t const& sel, str_t const& insert);

    // -------------------------------------------------------------------------------
    // trim
    void trim(str_t&);                                             // Trim whitespace from left and right side
    void trimLeft(str_t&);                                         // Trim whitespace from left side
    void trimRight(str_t&);                                        // Trim whitespace from right side
    void trim(str_t&, uchar32 inChar);                             // Trim character <inChar> from left and right side
    void trimLeft(str_t&, uchar32 inChar);                         // Trim character <inChar> from left side
    void trimRight(str_t&, uchar32 inChar);                        // Trim character <inChar> from right side
    void trim(str_t&, str_t const& inCharSet);                     // Trim characters in <inCharSet> from left and right side
    void trimLeft(str_t&, str_t const& inCharSet);                 // Trim characters in <inCharSet> from left side
    void trimRight(str_t&, str_t const& inCharSet);                // Trim characters in <inCharSet> from right side
    void trimQuotes(str_t&);                                       // Trim double quotes from left and right side
    void trimQuotes(str_t&, uchar32 quote);                        // Trim double quotes from left and right side
    void trimDelimiters(str_t&, uchar32 inLeft, uchar32 inRight);  // Trim delimiters from left and right side

    // -------------------------------------------------------------------------------
    // reading
    uchar32 read(str_t const& str, u32& cursor);

    // -------------------------------------------------------------------------------
    // copy and concatenate
    bool copy(const str_t& src, str_t& dst);
    bool concatenate(str_t& str, const str_t& concat);
    bool concatenate(str_t& str, const str_t& concat1, const str_t& concat2);

    // -------------------------------------------------------------------------------
    // runes reader and writer
    class ireader_t
    {
    public:
        void    reset() { vreset(); }
        bool    valid() const { return vvalid(); }
        uchar32 peek() const { return vpeek(0); }
        uchar32 peekn(u32 n = 0) const { return vpeek(n); }
        uchar32 read() { return vread(); }
        str_t   read(u32 n) { return vread(n); }
        str_t   view(u32 n = 0xFFFFFFFF) const { return vview(n); }
        void    skip(u32 c = 1) { vskip(c); }
        bool    end() const { return vend(); }

    protected:
        virtual void    vreset()           = 0;
        virtual bool    vvalid() const     = 0;
        virtual uchar32 vpeek(u32 n) const = 0;
        virtual uchar32 vread()            = 0;
        virtual str_t   vread(u32 n)       = 0;
        virtual str_t   vview(u32 n) const = 0;
        virtual void    vskip(u32 c)       = 0;
        virtual bool    vend() const       = 0;
    };

    void skip_any(ireader_t* reader, const char* chars, u32 count);
    s32  skip_until_one_of(ireader_t* reader, const char* chars, u32 count);
    void skip_whitespace(ireader_t* reader);

    str_t read_line(ireader_t* reader);

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
        reader_t(str_t const& runes);

        reader_t reader(u32 from, u32 to) const;

        u32  get_cursor() const { return m_cursor; }
        void set_cursor(u32 const& c) { m_cursor = c; }

        str_t get_source() const;
        str_t get_current() const;
        bool  at_end() const;

    protected:
        virtual void    vreset();
        virtual bool    vvalid() const;
        virtual uchar32 vpeek(u32 n) const;
        virtual uchar32 vread();
        virtual str_t   vread(u32 n);
        virtual str_t   vview(u32 n) const;
        virtual void    vskip(u32 c);
        virtual bool    vend() const;

        str_t m_runes;
        u32   m_cursor;
    };

    class iwriter_t
    {
    public:
        s32  write(uchar32 c) { return vwrite(c); }
        s32  write(const char* str) { return vwrite(str); }
        s32  write(const char* str, const char* end) { return vwrite(str, end); }
        s32  write(str_t const& str) { return vwrite(str); }
        s32  writeln(str_t const& str) { return write(str) + writeln(); }
        bool writeln() { return vwrite("\n"); }
        void flush() { vflush(); }

    protected:
        virtual ~iwriter_t() {}
        virtual s32  vwrite(uchar32 c)                        = 0;
        virtual s32  vwrite(const char* str)                  = 0;
        virtual s32  vwrite(const char* str, const char* end) = 0;
        virtual s32  vwrite(str_t const& str)                 = 0;
        virtual void vflush()                                 = 0;
    };

    class writer_t : public iwriter_t
    {
    public:
        writer_t(str_t const& runes);

        void reset();
        s32  count() const;

        str_t get_destination() const;
        str_t get_current() const;

    protected:
        virtual s32  vwrite(uchar32 c);
        virtual s32  vwrite(const char* str);
        virtual s32  vwrite(const char* str, const char* end);
        virtual s32  vwrite(str_t const& str);
        virtual void vflush();

        str_t m_runes;
        u32   m_cursor;
        s32   m_count;
    };

    // -------------------------------------------------------------------------------
    // global operators for comparison
    inline bool operator==(const str_t& lhs, const str_t& rhs) { return compare(lhs, rhs) == 0; }
    inline bool operator!=(const str_t& lhs, const str_t& rhs) { return compare(lhs, rhs) != 0; }
};  // namespace ncore

#endif  ///< __CBASE_RUNES2_H__
