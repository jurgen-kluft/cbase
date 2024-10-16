#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "ccore/c_debug.h"
#include "cbase/c_dconv.h"
#include "cbase/c_integer.h"
#include "cbase/c_console.h"
#include "cbase/c_memory.h"
#include "cbase/c_va_list.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"

namespace ncore
{
    namespace utf
    {
        static const u32 s_invalid     = 0x0000ffff;
        static const u32 s_replacement = 0x0000fffd;

        enum EUnicode
        {
            UTF16_BMP_END = 0xFFFF,                       // The last codepoint of the Basic Multilingual Plane, which is the part of Unicode that
                                                          // UTF-16 can encode without surrogates
            UNICODE_MAX                      = 0x10FFFF,  // The highest valid Unicode codepoint
            UTF16_INVALID_CODEPOINT          = 0xFFFD,    // The codepoint that is used to replace invalid encodings
            UTF16_GENERIC_SURROGATE_VALUE    = 0xD800,    // If a character, masked with UTF16_GENERIC_SURROGATE_MASK, matches this value, it is a surrogate.
            UTF16_GENERIC_SURROGATE_MASK     = 0xF800,    // The mask to apply to a character before testing it against UTF16_GENERIC_SURROGATE_VALUE
            UTF16_HIGH_SURROGATE_VALUE       = 0xD800,    // If a character, masked with UTF16_SURROGATE_MASK, matches this value, it is a high surrogate.
            UTF16_LOW_SURROGATE_VALUE        = 0xDC00,    // If a character, masked with UTF16_SURROGATE_MASK, matches this value, it is a low surrogate.
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

        namespace reading
        {
            // UTF sequence sizes
            static inline s8 utf8_octet_count(uchar32 c)
            {
                // handle characters outside the valid range
                if (c >= 0x0000d800 && c <= 0x0000dfff)
                    return 0;
                if (c < 0x00000080)
                    return 1;
                if (c < 0x00000800)
                    return 2;
                if (c < 0x00010000)
                    return 3;
                if (c < 0x00200000)
                    return 4;
                if (c < 0x04000000)
                    return 5;
                if (c < 0x80000000)
                    return 6;
                return 0;
            }

            static inline s8 utf8_octet_count(utf8::rune c)
            {
                if (c.r < 0x80)
                    return 1;  // 0xxxxxxx
                else if (c.r < 0xc0)
                    return -1;  // 10xxxxxx -- in the middle of a multibyte character, counts as one invalid character.
                else if (c.r < 0xe0)
                    return 2;  // 110xxxxx
                else if (c.r < 0xf0)
                    return 3;  // 1110xxxx
                else if (c.r < 0xf8)
                    return 4;  // 11110xxx
                else if (c.r < 0xfc)
                    return 5;  // 111110xx
                else if (c.r < 0xfe)
                    return 6;  // 1111110x
                return -1;     // invalid sequence.  dunno how many bytes to skip so skip one.
            }

            static inline s8 utf16_octet_count(utf16::rune c)
            {
                if (c.r < 0xd800)
                    return 1;
                else if (c.r < 0xdc00)
                    return 2;
                return 1;
            }

            // read

            // Read one ascii character from an ascii string and return it as a utf-32 character.
            static inline uchar32 read_forward(ascii::pcrune bos, u32& str, u32 end) { return str < end ? (uchar32)bos[str++] : '\0'; }

            // Read one ucs-2 character from a ucs-2 string and return it as a utf-32 character.
            static inline uchar32 read_forward(ucs2::pcrune bos, u32& str, u32 end) { return str < end ? (uchar32)bos[str++].r : '\0'; }

            // Mask can be computed as (0x7f >> leadingOnes)
            // Size based on the number of leading 1 bits
            static const u32 c_utf8_sizes = (0 << 28) | (6 << 24) | (5 << 20) | (4 << 16) | (3 << 12) | (2 << 8) | (0 << 4) | 1;
            static const u32 c_minChar[]  = {0, 0x00000000, 0x00000080, 0x00000800, 0x00010000, 0x00200000, 0x04000000};

            // Read one utf-8 character from a utf-8 string and return it as a utf-32 character.
            static uchar32 read_forward(utf8::pcrune data, u32& str, u32 end)
            {
                data = data + str;

                // compute character encoding length, checking for overlong sequences (i.e. characters that don't use the shortest possible encoding).
                u8       value       = data[0].r;
                s8 const leadingOnes = math::countLeadingZeros((u8)(~value));
                u8 const size        = (c_utf8_sizes >> (leadingOnes << 2)) & 0xF;

                if (size == 0)
                {
                    ++str;
                    return s_invalid;
                }

                if (str + size > end)
                {
                    str = end;
                    return s_invalid;
                }

                u32 c = value & ((u32)0x7f >> leadingOnes);
                for (u32 i = 1; i < size; i++)
                {
                    value = data[i].r;
                    if ((value & 0xc0) != 0x80)
                    {
                        // check that all bytes after the first have the pattern 10xxxxxx.
                        // truncated sequences are treated as a single malformed character.
                        str += i;
                        return s_invalid;
                    }
                    c = (c << 6) | (u32)(value & 0x3f);
                }

                str += size;  // update parameters

                // check for characters that didn't use the smallest possible encoding
                if (c < c_minChar[size])
                    return s_invalid;

                // check for characters not in ISO-10646
                if ((c >= 0x0000d800 && c <= 0x0000dfff) || (c >= 0x0000fffe && c <= 0x0000ffff))
                    return s_invalid;

                return c;
            }

            // Read one utf-16 character from a utf-16 string and return it as a utf-32 character.
            static inline uchar32 read_forward(utf16::pcrune bos, u32& str, u32 end)
            {
                u16 high = bos[str++].r;

                // BMP character
                if ((high & UTF16_GENERIC_SURROGATE_MASK) != UTF16_GENERIC_SURROGATE_VALUE)
                    return high;

                // Unmatched low surrogate, invalid
                if ((high & UTF16_SURROGATE_MASK) != UTF16_HIGH_SURROGATE_VALUE)
                    return UTF16_INVALID_CODEPOINT;

                if (str >= end)
                    return UTF16_INVALID_CODEPOINT;

                u16 low = bos[str].r;

                // Unmatched high surrogate, invalid
                if ((low & UTF16_SURROGATE_MASK) != UTF16_LOW_SURROGATE_VALUE)
                    return UTF16_INVALID_CODEPOINT;

                // Two correctly matched surrogates, increase index to indicate we've consumed
                // two characters
                str++;

                // The high bits of the codepoint are the value bits of the high surrogate
                // The low bits of the codepoint are the value bits of the low surrogate
                uchar32 result = high & UTF16_SURROGATE_CODEPOINT_MASK;
                result <<= UTF16_SURROGATE_CODEPOINT_BITS;
                result |= low & UTF16_SURROGATE_CODEPOINT_MASK;
                result += UTF16_SURROGATE_CODEPOINT_OFFSET;

                // And if all else fails, it's valid
                return result;
            }

            // Read one utf-32 character from a utf-32 string and return it as a utf-32 character.
            static inline uchar32 read_forward(utf32::pcrune bos, u32& str, u32 end) { return str < end ? bos[str++].r : utf32::TERMINATOR; }

            static inline uchar32 read_backward(ascii::pcrune bos, u32& str, u32 begin) { return str > begin ? (uchar32)bos[--str] : '\0'; }

            static inline uchar32 read_backward(ucs2::pcrune bos, u32& str, u32 begin) { return '\0'; }

            static inline uchar32 read_backward(utf8::pcrune data, u32& str, u32 begin) { return '\0'; }

            static inline uchar32 read_backward(utf16::pcrune bos, u32& str, u32 begin)
            {
                {
                    uchar16 const c = str > begin ? bos[--str].r : 0;
                    if (c < 0xdc00)
                        return c;

                    {
                        uchar16 const c2 = str > begin ? bos[--str].r : 0;
                        if (c2 < 0xd800)
                            return c2;
                        str++;
                    }
                }
                return '\0';
            }

            static inline uchar32 read_backward(utf32::pcrune bos, u32& str, u32 begin) { return str > begin ? bos[--str].r : utf32::TERMINATOR; }
        }  // namespace reading

        namespace writing
        {
            // write

            // Write one ascii character to an ascii string.
            static inline void write_forward(ascii::prune bos, u32& str, uchar32 c)
            {
                if (c > 0x7f)
                {
                    bos[str++] = '?';
                }
                else
                {
                    bos[str++] = (u8)c;
                }
            }

            // Represents a UTF-8 bit pattern that can be set or verified
            typedef struct
            {
                uchar8 mask;   // The mask that should be applied to the character before testing it
                uchar8 value;  // The value that the character should be tested against after applying the mask
            } utf8_pattern;

            // The patterns for leading bytes of a UTF-8 codepoint encoding
            // Each pattern represents the leading byte for a character encoded with N UTF-8 bytes,
            // where N is the index + 1
            static const utf8_pattern s_utf8_leading_bytes[] = {
              {0x80, 0x00},  // 0xxxxxxx
              {0xE0, 0xC0},  // 110xxxxx
              {0xF0, 0xE0},  // 1110xxxx
              {0xF8, 0xF0}   // 11110xxx
            };

            // Write one utf-8 character to a utf-8 string.
            static inline void write_forward(utf8::prune data, u32& str, uchar32 c)
            {
                const s32 size = (c < UTF8_1_MAX) ? 1 : (c < UTF8_2_MAX) ? 2 : (c < UTF8_3_MAX) ? 3 : (c < UTF8_4_MAX) ? 4 : 0;

                // TODO Not enough space left on the string ?

                // Write the continuation bytes in reverse order first
                switch (size)
                {
                    case 4: data[str + 3].r = (c & ~UTF8_CONTINUATION_MASK) | UTF8_CONTINUATION_VALUE; c >>= UTF8_CONTINUATION_CODEPOINT_BITS;
                    case 3: data[str + 2].r = (c & ~UTF8_CONTINUATION_MASK) | UTF8_CONTINUATION_VALUE; c >>= UTF8_CONTINUATION_CODEPOINT_BITS;
                    case 2:
                        data[str + 1].r = (c & ~UTF8_CONTINUATION_MASK) | UTF8_CONTINUATION_VALUE;
                        c >>= UTF8_CONTINUATION_CODEPOINT_BITS;
                        break;
                    default: data[str].r = '?'; break;
                }

                // Write the leading byte
                utf8_pattern const pattern = s_utf8_leading_bytes[size - 1];
                data[str].r                = (c & ~(pattern.mask)) | pattern.value;
            }

            static inline void write_forward(ucs2::prune data, u32& str, uchar32 c)
            {
                if (c < 0x10000)
                {
                    data[str++].r = (u16)c;
                }
                else
                {
                    data[str++].r = 0xfffd;
                }
            }

            static inline void write_forward(utf16::prune data, u32& str, uchar32 c)
            {
                if (c < 0x10000)
                {
                    data[str++].r = (u16)c;
                }
                else if (c <= UNICODE_MAX)
                {
                    c -= UTF16_SURROGATE_CODEPOINT_OFFSET;
                    data[str++].r = (u16)((c >> UTF16_SURROGATE_CODEPOINT_BITS) + UTF16_HIGH_SURROGATE_VALUE);
                    data[str++].r = (u16)((c & UTF16_SURROGATE_CODEPOINT_MASK) + UTF16_LOW_SURROGATE_VALUE);
                }
                else
                {
                    data[str++].r = UTF16_INVALID_CODEPOINT;
                }
            }

            static inline void write_forward(utf32::prune data, u32& str, uchar32 c) { data[str++].r = c; }

        }  // namespace writing

        s32 convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = inStr[inCursor++];
                if (c == ascii::TERMINATOR)
                    break;
                outStr[outCursor++].r = (u8)c;
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = inStr[inCursor++];
                if (c == ascii::TERMINATOR)
                    break;
                outStr[outCursor++].r = (uchar16)c;
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = inStr[inCursor++];
                if (c == ascii::TERMINATOR)
                    break;
                outStr[outCursor++].r = (uchar16)c;
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(ascii::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = inStr[inCursor++];
                if (c == ascii::TERMINATOR)
                    break;
                outStr[outCursor++].r = c;
            }
            return outCursor - oldOutCursor;
        }

        // ucs2 -> ascii, utf8, utf16, utf32
        s32 convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == ucs2::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == ucs2::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == ucs2::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(ucs2::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == ucs2::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }

        // utf8 -> ascii, ucs2, utf16, utf32
        s32 convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf8::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf8::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf8::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(utf8::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf8::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }

        // utf16 -> ascii, ucs2, utf8, utf32
        s32 convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf16::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf16::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf16::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(utf16::pcrune inStr, u32& inCursor, u32 inStrEnd, utf32::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf16::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }

        // utf32 -> ascii, ucs2, utf8, utf16
        s32 convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, ascii::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf32::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, ucs2::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf32::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, utf8::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf32::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }
        s32 convert(utf32::pcrune inStr, u32& inCursor, u32 inStrEnd, utf16::prune outStr, u32& outCursor, u32 outStrEnd)
        {
            u32 oldOutCursor = outCursor;
            while (inCursor < inStrEnd && outCursor < outStrEnd)
            {
                uchar32 c = reading::read_forward(inStr, inCursor, inStrEnd);
                if (c == utf32::TERMINATOR)
                    break;
                writing::write_forward(outStr, outCursor, c);
            }
            return outCursor - oldOutCursor;
        }

        static inline uchar32 read_forward(crunes_t const& str, u32& cursor)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return reading::read_forward(str.m_ascii, cursor, str.m_end);
                case ucs2::TYPE: return reading::read_forward(str.m_ucs2, cursor, str.m_end);
                case utf8::TYPE: return reading::read_forward(str.m_utf8, cursor, str.m_end);
                case utf16::TYPE: return reading::read_forward(str.m_utf16, cursor, str.m_end);
                case utf32::TYPE: return reading::read_forward(str.m_utf32, cursor, str.m_end);
            }
            return cEOS;
        }

        static inline uchar32 read_backward(crunes_t const& str, u32& cursor)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return reading::read_backward(str.m_ascii, cursor, str.m_str);
                case ucs2::TYPE: return reading::read_backward(str.m_ucs2, cursor, str.m_str);
                case utf8::TYPE: return reading::read_backward(str.m_utf8, cursor, str.m_str);
                case utf16::TYPE: return reading::read_backward(str.m_utf16, cursor, str.m_str);
                case utf32::TYPE: return reading::read_backward(str.m_utf32, cursor, str.m_str);
            }
            return cEOS;
        }

        static inline uchar32 read_forward(runes_t const& str, u32& cursor)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return reading::read_forward(str.m_ascii, cursor, str.m_end);
                case ucs2::TYPE: return reading::read_forward(str.m_ucs2, cursor, str.m_end);
                case utf8::TYPE: return reading::read_forward(str.m_utf8, cursor, str.m_end);
                case utf16::TYPE: return reading::read_forward(str.m_utf16, cursor, str.m_end);
                case utf32::TYPE: return reading::read_forward(str.m_utf32, cursor, str.m_end);
            }
            return cEOS;
        }

        static inline uchar32 read_backward(runes_t const& str, u32& cursor)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return reading::read_backward(str.m_ascii, cursor, str.m_str);
                case ucs2::TYPE: return reading::read_backward(str.m_ucs2, cursor, str.m_str);
                case utf8::TYPE: return reading::read_backward(str.m_utf8, cursor, str.m_str);
                case utf16::TYPE: return reading::read_backward(str.m_utf16, cursor, str.m_str);
                case utf32::TYPE: return reading::read_backward(str.m_utf32, cursor, str.m_str);
            }
            return cEOS;
        }

        static inline s32 skip_forward(ascii::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            cursor += (u32)count;
            if (cursor >= end)
            {
                count  = (s32)end - (s32)(cursor - (u32)count);
                cursor = end;
            }
            return count;
        }

        static s32 skip_backward(ascii::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            if ((cursor - count) < begin)
            {
                count  = (s32)cursor - (s32)begin;
                cursor = begin;
            }
            else
            {
                cursor -= (u32)count;
            }
            return count;
        }

        static s32 skip_forward(ucs2::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            u32 const n = (u32)-count;
            cursor -= n;
            if (cursor < begin)
            {
                count  = (s32)begin - (s32)(cursor + n);
                cursor = begin;
            }
            return count;
        }

        static s32 skip_backward(ucs2::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            if ((cursor - count) < begin)
            {
                count  = (s32)cursor - (s32)begin;
                cursor = begin;
            }
            else
            {
                cursor -= (u32)count;
            }
            return count;
        }

        static s32 skip_forward(utf8::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            ASSERT(cursor >= begin);
            ASSERT(cursor <= end);

            utf8::pcrune ptr = str + cursor;
            s32          c   = count;
            while (c > 0 && (cursor < end))
            {
                if (((*ptr).r & 0xC0) == 0xC0)
                {
                    ptr++;
                    c--;
                    while ((cursor < end) && ((*ptr).r & 0xC0) == 0x80)
                    {
                        ptr++;
                    }
                }
                else
                {
                    ptr++;
                    c--;
                }
            }
            return count - c;
        }

        static s32 skip_backward(utf8::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            ASSERT(cursor >= begin);
            ASSERT(cursor <= end);

            utf8::pcrune ptr = str + cursor;
            s32          c   = 0;
            while (cursor > begin && c < count)
            {
                ptr--;
                cursor--;
                c++;
                while ((cursor > begin) && ((*ptr).r & 0xC0) == 0x80)
                {
                    ptr--;
                    cursor--;
                }
            }
            return c;
        }

        static s32 skip_forward(utf16::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            s32         skipcount = 0;
            utf16::rune c         = str[cursor];
            while (c.r != utf16::TERMINATOR && (cursor < end) && count > 0)
            {
                u32 const l = reading::utf16_octet_count(c);
                if ((cursor + l) <= end)
                {
                    if (l == 1)
                        c = str[cursor];
                    cursor += l;
                }
                skipcount++;
                count -= 1;
            }
            return skipcount;
        }

        static s32 skip_backward(utf16::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            ASSERT(cursor >= begin);
            ASSERT(cursor <= end);

            // TODO

            return 0;
        }

        static s32 skip_forward(utf32::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            cursor += (u32)count;
            if (cursor >= end)
            {
                count  = (s32)end - (s32)(cursor - (u32)count);
                cursor = end;
            }
            return count;
        }

        static s32 skip_backward(utf32::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            if ((cursor - count) < begin)
            {
                count  = (s32)cursor - (s32)begin;
                cursor = begin;
            }
            else
            {
                cursor -= (u32)count;
            }
            return count;
        }

        static ascii::pcrune endof(ascii::pcrune str, ascii::pcrune eos)
        {
            if (str == nullptr)
                return nullptr;

            ascii::pcrune end = str;
            if (eos == nullptr)
            {
                while (*end != ascii::TERMINATOR)
                    end++;
            }
            else
            {
                while (*end != ascii::TERMINATOR && end < eos)
                    end++;
            }
            return end;
        }

        static ucs2::pcrune endof(ucs2::pcrune str, ucs2::pcrune eos)
        {
            if (str == nullptr)
                return nullptr;

            ucs2::pcrune end = str;
            if (eos == nullptr)
            {
                while (end->r != ucs2::TERMINATOR)
                    end++;
            }
            else
            {
                while (end < eos && end->r != ucs2::TERMINATOR)
                    end++;
            }
            return end;
        }

        static utf8::pcrune endof(utf8::pcrune str, utf8::pcrune eos)
        {
            if (str == nullptr)
                return nullptr;

            utf8::pcrune end = str;
            if (eos == nullptr)
            {
                while (end->r != 0)
                {
                    if (((*end).r & 0xC0) == 0xC0)
                    {
                        end++;
                        while (((*end).r & 0xC0) == 0x80)
                        {
                            end++;
                        }
                    }
                    else
                    {
                        end++;
                    }
                }
            }
            else
            {
                while (end < eos && end->r != 0)
                {
                    if (((*end).r & 0xC0) == 0xC0)
                    {
                        end++;
                        while ((end < eos) && ((*end).r & 0xC0) == 0x80)
                        {
                            end++;
                        }
                    }
                    else
                    {
                        end++;
                    }
                }
            }
            return end;
        }

        static utf16::pcrune endof(utf16::pcrune str, utf16::pcrune eos)
        {
            if (str == nullptr)
                return nullptr;

            utf16::pcrune end = str;
            if (eos == nullptr)
            {
                while (end->r != utf16::TERMINATOR)
                {
                    utf16::rune c = *end;
                    u32 const   l = reading::utf16_octet_count(c);
                    end += l;
                }
            }
            else
            {
                while (end < eos && end->r != utf16::TERMINATOR)
                {
                    utf16::rune c = *end;
                    u32 const   l = reading::utf16_octet_count(c);
                    end += l;
                }
            }
            return end;
        }

        static utf32::pcrune endof(utf32::pcrune str, utf32::pcrune eos)
        {
            if (str == nullptr)
                return nullptr;

            utf32::pcrune end = str;
            if (eos == nullptr)
            {
                while (end->r != utf32::TERMINATOR)
                    end++;
            }
            else
            {
                while (end < eos && end->r != utf32::TERMINATOR)
                    end++;
            }
            return end;
        }

        // ####### WRITE ########

        static bool write(uchar32 c, ascii::prune bos, u32& cursor, u32 end)
        {
            if (cursor < end)
            {
                bos[cursor++] = (uchar)((c > 0x7f) ? '?' : c);
                return true;
            }
            return false;
        }

        static bool write(uchar32 c, ucs2::prune bos, u32& cursor, u32 end)
        {
            if (cursor < end)
            {
                // ucs-4 (utf-32) to ucs-2 (utf-16) conversion
                if (c < 0x10000)
                {
                    bos[cursor++].r = (uchar16)c;
                }
                else
                {
                    bos[cursor++].r = (uchar16)s_replacement;
                }
                return true;
            }
            return false;
        }

        static bool write(uchar32 cp, utf8::prune bos, u32& cursor, u32 end)
        {
            if (cp < 0x80)
            {  // one octet
                if (cursor < end)
                {
                    bos[cursor++].r = static_cast<uchar8>(cp);
                    return true;
                }
            }
            else if (cp < 0x800)
            {  // two octets
                if ((cursor + 1) < end)
                {
                    bos[cursor++].r = static_cast<uchar8>((cp >> 6) | 0xc0);
                    bos[cursor++].r = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            else if (cp < 0x10000)
            {  // three octets
                if ((cursor + 2) < end)
                {
                    bos[cursor++].r = static_cast<uchar8>((cp >> 12) | 0xe0);
                    bos[cursor++].r = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
                    bos[cursor++].r = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            else
            {  // four octets
                if ((cursor + 3) < end)
                {
                    bos[cursor++].r = static_cast<uchar8>((cp >> 18) | 0xf0);
                    bos[cursor++].r = static_cast<uchar8>(((cp >> 12) & 0x3f) | 0x80);
                    bos[cursor++].r = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
                    bos[cursor++].r = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            return false;
        }

        static bool write(uchar32 rune, utf16::prune bos, u32& cursor, u32 end)
        {
            s32 const len = (rune < 0xd800) ? 1 : (rune < 0xe000) ? 0 : (rune < 0x010000) ? 1 : (rune < 0x110000) ? 2 : 0;
            if (len > 0)
            {
                if (len == 1 && cursor < end)
                {
                    bos[cursor++].r = (uchar16)rune;
                    return true;
                }
                else if ((cursor + 1) < end)
                {
                    // 20-bit intermediate value
                    u32 const iv    = rune - 0x10000;
                    bos[cursor++].r = static_cast<uchar16>((iv >> 10) + 0xd800);
                    bos[cursor++].r = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
                    return true;
                }
            }
            return false;
        }

        static bool write(uchar32 c, utf32::prune bos, u32& cursor, u32 end)
        {
            if (cursor < end)
            {
                bos[cursor++].r = c;
                return true;
            }
            return false;
        }

        static inline bool write(uchar32 c, runes_t const& dst, u32& cursor)
        {
            switch (dst.m_type)
            {
                case ascii::TYPE: return write(c, dst.m_ascii, cursor, dst.m_eos);
                case ucs2::TYPE: return write(c, dst.m_ucs2, cursor, dst.m_eos);
                case utf8::TYPE: return write(c, dst.m_utf8, cursor, dst.m_eos);
                case utf16::TYPE: return write(c, dst.m_utf16, cursor, dst.m_eos);
                case utf32::TYPE: return write(c, dst.m_utf32, cursor, dst.m_eos);
            }
            return false;
        }

    }  // namespace utf

    namespace ascii
    {
        s32 strlen(pcrune str, pcrune* end, pcrune eos)
        {
            pcrune iter = utf::endof(str, eos);
            if (end != nullptr)
                *end = iter;
            return (s32)(iter - str);
        }

        s32 compare(pcrune str1, pcrune str2, pcrune end1, pcrune end2)
        {
            if (end1 == nullptr && end2 == nullptr)
            {
                while (*str1 != TERMINATOR && *str2 != TERMINATOR)
                {
                    if (*str1 != *str2)
                        return *str1 - *str2;
                    str1++;
                    str2++;
                }
                return *str1 - *str2;
            }
            else if (end1 == nullptr)
            {
                while (str2 < end2 && (*str1 != TERMINATOR && *str2 != TERMINATOR))
                {
                    if (*str1 != *str2)
                        return *str1 - *str2;
                    str1++;
                    str2++;
                }
                return *str1 - *str2;
            }
            else if (end2 == nullptr)
            {
                while (str1 < end1 && (*str1 != TERMINATOR && *str2 != TERMINATOR))
                {
                    if (*str1 != *str2)
                        return *str1 - *str2;
                    str1++;
                    str2++;
                }
                return *str1 - *str2;
            }
            else
            {
                while (str1 < end1 && str2 < end2 && (*str1 != TERMINATOR && *str2 != TERMINATOR))
                {
                    if (*str1 != *str2)
                        return *str1 - *str2;
                    str1++;
                    str2++;
                }
                return *str1 - *str2;
            }
        }

        s32 compare(pcrune left, pcrune right) { return compare(left, right, nullptr, nullptr); }

        void reverse(char* str, char* end)
        {
            // Reverse work buffer
            char* head = str;
            char* tail = end - 1;
            while (head < tail)
            {
                char t = *head;
                *head  = *tail;
                *tail  = t;
                head += 1;
                tail -= 1;
            }
        }

        static s32 divmod10(u32 value, u8& remainder)
        {
            const u32 q = (value / 10);
            remainder   = (u8)(value - (q * 10));
            return (s32)(q);
        }

        static s32 divmod10(s32 value, s8& remainder)
        {
            const s32 q = (value / 10);
            remainder   = (s8)(value - (q * 10));
            return q;
        }

        char* utoa(u32 val, char* cursor, char const* end, s32 base, bool octzero, bool lowercase)
        {
            ASSERT(cursor != nullptr && end != nullptr);

            if ((end - cursor) < 10)
                return cursor;

            uchar32 c;
            s32     sval;
            u8      mod;
            s8      smod;

            char* w = cursor + 10;

            switch (base)
            {
                case 10:
                    // On many machines, unsigned arithmetic is harder than
                    // signed arithmetic, so we do at most one unsigned mod and
                    // divide; this is sufficient to reduce the range of
                    // the incoming value to where signed arithmetic works.

                    if (val > ((~(u32)0) >> 1))
                    {
                        sval = divmod10(val, mod);
                        c    = (uchar32)nrunes::to_dec_char(mod);
                        w -= 1;
                        *w = (char)c;
                    }
                    else
                    {
                        sval = (s32)val;
                    }

                    do
                    {
                        sval = divmod10(sval, smod);
                        c    = (uchar32)nrunes::to_dec_char((u8)smod);
                        w -= 1;
                        *w = (char)c;
                    } while (sval != 0);

                    break;

                case 8:
                    do
                    {
                        c = (uchar32)nrunes::to_dec_char(val & 7);
                        w -= 1;
                        *w = (char)c;
                        val >>= 3;
                    } while (val);
                    if (octzero && c != '0')
                    {
                        *w-- = '0';
                    }
                    break;

                case 16:
                    do
                    {
                        w -= 1;
                        *w = nrunes::to_hex_char(val & 15, lowercase);
                        val >>= 4;
                    } while (val);

                    break;

                default:
                    /* oops */
                    break;
            }

            // The conversion might not use all 10 characters, so we need to
            // move the characters down to the bottom of the buffer.
            const char* we = cursor + 10;
            while (w < we)
                *cursor++ = *w++;
            return cursor;
        }

        char* itoa(s32 val, char* cursor, char const* end, s32 base, bool octzero, bool lowercase)
        {
            ASSERT(cursor != nullptr && end != nullptr);

            if ((end - cursor) < 11)
                return cursor;

            if (val < 0)
            {
                *cursor++ = '-';
                val       = -val;
            }
            return utoa((u32)val, cursor, end, base, octzero, lowercase);
        }

        static s64 divmod10(u64 value, u8& remainder)
        {
            const u64 q = (value / 10);
            remainder   = (u8)(value - (q * 10));
            return (s64)(q);
        }

        static s64 divmod10(s64 value, s8& remainder)
        {
            const s64 q = (value / 10);
            remainder   = (s8)(value - (q * 10));
            return q;
        }

        char* utoa(u64 val, char* cursor, char const* end, s32 base, bool octzero, bool lowercase)
        {
            uchar32 c;
            s64     sval;
            s8      smod;
            u8      mod;

            if ((end - cursor) < 20)
                return cursor;

            char* w = cursor + 20;

            switch (base)
            {
                case 10:
                    // On many machines, unsigned arithmetic is harder than
                    // signed arithmetic, so we do at most one unsigned mod and
                    // divide; this is sufficient to reduce the range of
                    // the incoming value to where signed arithmetic works.

                    if (val > ((~(u64)0) >> 1))
                    {
                        sval = divmod10(val, mod);
                        c    = (uchar32)nrunes::to_dec_char(mod);
                        *--w = (char)c;
                    }
                    else
                    {
                        sval = (s64)val;
                    }

                    do
                    {
                        sval = divmod10(sval, smod);
                        c    = (uchar32)nrunes::to_dec_char((u8)smod);
                        *--w = (char)c;
                    } while (sval != 0);

                    break;

                case 8:
                    do
                    {
                        c    = (uchar32)nrunes::to_dec_char(val & 7);
                        *--w = (char)c;
                        val >>= 3;
                    } while (val);

                    if (octzero && c != '0')
                    {
                        c    = '0';
                        *--w = (char)c;
                    }

                    break;

                case 16:
                    do
                    {
                        c    = (uchar32)nrunes::to_hex_char((u8)val, lowercase);
                        *--w = (char)c;
                        val >>= 4;
                    } while (val);

                    break;

                default:
                    /* oops */
                    break;
            }

            // The conversion might not use all 10 characters, so we need to
            // move the characters down to the bottom of the buffer.
            const char* we = cursor + 20;
            while (w < we)
                *cursor++ = *w++;

            return cursor;
        }

        char* itoa(s64 val, char* cursor, char const* end, s32 base, bool octzero, bool lowercase)
        {
            ASSERT(cursor != nullptr && end != nullptr);

            if ((end - cursor) < 11)
                return cursor;

            if (val < 0)
            {
                *cursor++ = '-';
                val       = -val;
            }
            return utoa((u64)val, cursor, end, base, octzero, lowercase);
        }

        char* ftoa(f32 val, char* cursor, char const* end, bool lowercase)
        {
            s32       bufsize = (s32)(end - cursor);
            u32 const flags   = lowercase ? 0 : DoubleConvert::FLAG_UPPERCASE;
            const s32 len     = dconvstr_print(&cursor, &bufsize, val, 'g', flags, 0, DoubleConvert::DEFAULT_PRECISION);
            return cursor + len;
        }

        char* dtoa(f64 val, char* cursor, char const* end, bool lowercase)
        {
            s32       bufsize = (s32)(end - cursor);
            u32 const flags   = lowercase ? 0 : DoubleConvert::FLAG_UPPERCASE;
            const s32 len     = dconvstr_print(&cursor, &bufsize, val, 'g', flags, 0, DoubleConvert::DEFAULT_PRECISION);
            return cursor + len;
        }

        char* btoa(bool val, char* cursor, char const* end, s8 caseType)
        {
            const char* boolstr;
            switch (caseType)
            {
                case TrueFalse | LowerCase: boolstr = (val ? "true" : "false"); break;
                case TrueFalse | CamelCase: boolstr = (val ? "True" : "False"); break;
                case TrueFalse | UpperCase: boolstr = (val ? "TRUE" : "FALSE"); break;
                case YesNo | LowerCase: boolstr = (val ? "yes" : "no"); break;
                case YesNo | CamelCase: boolstr = (val ? "Yes" : "No"); break;
                case YesNo | UpperCase: boolstr = (val ? "YES" : "NO"); break;
                default: ASSERT(false); return cursor;
            }
            while (*boolstr != '\0' && cursor < end)
                *cursor++ = *boolstr++;
            return cursor;
        }

    }  // namespace ascii

    namespace ucs2
    {
        s32 compare(pcrune str1, u32 len1, pcrune str2, u32 len2)
        {
            if (len1 == 0 && len2 == 0)
                return 0;
            else if (len1 == 0)
                return -1;
            else if (len2 == 0)
                return 1;

            u32 cursor1 = 0;
            u32 cursor2 = 0;
            while (cursor1 < len1 && cursor2 < len2)
            {
                uchar32 c1 = utf::reading::read_forward(str1, cursor1, len1);
                uchar32 c2 = utf::reading::read_forward(str2, cursor2, len2);
                if (c1 == c2)
                {
                    if (c1 == cEOS)
                        break;
                }
                else
                {
                    if (c1 < c2)
                        return -1;
                    else
                        return 1;
                }
            }
            return 0;
        }

        s32 strlen(pcrune str, pcrune& end, pcrune eos)
        {
            pcrune iter = utf::endof(str, eos);
            return (s32)(iter - str);
        }
    }  // namespace ucs2

    namespace utf8
    {
        s32 compare(pcrune str1, u32 len1, pcrune str2, u32 len2)
        {
            if (len1 == 0 && len2 == 0)
                return 0;
            else if (len1 == 0)
                return -1;
            else if (len2 == 0)
                return 1;

            u32 cursor1 = 0;
            u32 cursor2 = 0;
            while (cursor1 < len1 && cursor2 < len2)
            {
                uchar32 c1 = utf::reading::read_forward(str1, cursor1, len1);
                uchar32 c2 = utf::reading::read_forward(str2, cursor2, len2);
                if (c1 == c2)
                {
                    if (c1 == cEOS)
                        break;
                }
                else
                {
                    if (c1 < c2)
                        return -1;
                    else
                        return 1;
                }
            }
            return 0;
        }

        s32 strlen(pcrune str, pcrune& end, pcrune eos)
        {
            pcrune iter = utf::endof(str, eos);
            return (s32)(iter - str);
        }
    }  // namespace utf8

    namespace utf16
    {
        s32 strlen(pcrune str, pcrune& end, pcrune eos)
        {
            pcrune iter = utf::endof(str, eos);
            return (s32)(iter - str);
        }
    }  // namespace utf16

    namespace utf32
    {
        s32 strlen(pcrune str, pcrune& end, pcrune eos)
        {
            s32 chars = 0;
            end       = str;
            if (eos == nullptr)
            {
                while (end->r != utf32::TERMINATOR)
                {
                    end++;
                    chars++;
                }
            }
            else
            {
                while (end < eos && end->r != utf32::TERMINATOR)
                {
                    end++;
                    chars++;
                }
            }
            return chars;
        }
    }  // namespace utf32

    namespace nrunes
    {
        static u32     get_begin(runes_t const& str);
        static u32     get_end(runes_t const& str);
        static u32     get_eos(runes_t const& str);
        static runes_t select(runes_t const& str, u32 from, u32 to);
        static bool    backwards(runes_t const& str, u32& cursor, s32 step = 1);
        static bool    write(runes_t const& str, u32& cursor, uchar32 c);

        static u32         get_begin(crunes_t const& str);
        static u32         get_end(crunes_t const& str);
        static crunes_t    select(crunes_t const& str, u32 from, u32 to);
        static bool        forwards(crunes_t const& str, u32& cursor, s32 step = 1);
        static bool        backwards(crunes_t const& str, u32& cursor, s32 step = 1);
        static bool        is_valid(crunes_t const& str, u32 const& cursor);
        static inline bool is_in(crunes_t const& str, crunes_t const& selection) { return (selection.m_ascii == str.m_ascii && selection.m_str >= str.m_str && selection.m_end <= str.m_end); }

        static inline runes_t crunes_to_runes(runes_t const& str, crunes_t const& sel)
        {
            runes_t r(str);
            r.m_str  = sel.m_str;
            r.m_end  = sel.m_end;
            r.m_eos  = sel.m_eos;
            r.m_type = sel.m_type;
            return r;
        }

        static inline runes_t  nothing_found(runes_t const& str) { return make_runes(str.m_ascii, str.m_str, str.m_str, str.m_eos, str.m_type); }
        static inline crunes_t nothing_found(crunes_t const& str) { return make_crunes(str.m_ascii, str.m_str, str.m_str, str.m_eos, str.m_type); }

        // Find

        crunes_t find(crunes_t const& _str, uchar32 _c, bool _casesensitive)
        {
            u32 const end  = _str.m_end;
            u32       iter = _str.m_str;
            while (iter < end)
            {
                u32 const     cursor = iter;
                uchar32 const c      = utf::read_forward(_str, iter);
                if (iter == cursor)
                    return nothing_found(_str);

                if (_casesensitive)
                {
                    if (c == _c)
                        return select(_str, cursor, iter);
                }
                else
                {
                    if (is_equalfold(c, _c))
                        return select(_str, cursor, iter);
                }
            }
            return nothing_found(_str);
        }

        runes_t find(runes_t const& _str, uchar32 _c, bool _casesensitive)
        {
            crunes_t str  = make_crunes(_str);
            crunes_t csel = find(str, _c, _casesensitive);
            return crunes_to_runes(_str, csel);
        }

        crunes_t findLast(crunes_t const& _str, uchar32 _c, bool _casesensitive)
        {
            u32 const begin = _str.m_str;
            u32       iter  = _str.m_end;
            while (iter > begin)
            {
                u32 const     end   = iter;
                uchar32 const c     = utf::read_backward(_str, iter);
                bool const    equal = _casesensitive ? is_equal(c, _c) : is_equalfold(c, _c);
                if (equal)
                {
                    return select(_str, iter, end);
                }
            }
            return nothing_found(_str);
        }

        runes_t findLast(runes_t const& _str, uchar32 _c, bool _casesensitive)
        {
            crunes_t str  = make_crunes(_str);
            crunes_t csel = findLast(str, _c, _casesensitive);
            return crunes_to_runes(_str, csel);
        }

        crunes_t find(crunes_t const& _str, crunes_t const& _find, bool _casesensitive)
        {
            if (is_empty(_str) || is_empty(_find))
                return nothing_found(_str);

            u32       str_iter = get_begin(_str);
            u32 const str_end  = get_end(_str);

            u32 const find_iter = get_begin(_find);
            u32 const find_end  = get_end(_find);

            while (str_iter < str_end)
            {
                u32  str_iter2  = str_iter;
                u32  find_iter2 = find_iter;
                bool equal      = true;
                while (str_iter2 < str_end && find_iter2 < find_end)
                {
                    u32           str_cursor = str_iter2;
                    uchar32 const c1         = utf::read_forward(_str, str_cursor);
                    uchar32 const c2         = utf::read_forward(_find, find_iter2);
                    if (_casesensitive ? !is_equal(c1, c2) : !is_equalfold(c1, c2))
                    {
                        equal = false;
                        break;
                    }
                    str_iter2 = str_cursor;
                }
                if (equal)
                {
                    return select(_str, str_iter, str_iter2);
                }
                uchar32 const c = utf::read_forward(_str, str_iter);
            }
            return nothing_found(_str);
        }

        runes_t find(runes_t const& _str, crunes_t const& _find, bool _casesensitive)
        {
            crunes_t str  = make_crunes(_str);
            crunes_t csel = find(str, _find, _casesensitive);
            if (!is_empty(csel))
            {
                runes_t sel = crunes_to_runes(_str, csel);
                return sel;
            }
            return nothing_found(_str);
        }

        crunes_t findLast(crunes_t const& _str, crunes_t const& _find, bool _casesensitive)
        {
            if (is_empty(_str) || is_empty(_find))
                return nothing_found(_str);

            u32 const str_begin = get_begin(_str);
            u32       str_iter  = get_end(_str);

            u32 const find_begin = get_begin(_find);
            u32 const find_end   = get_end(_find);

            while (str_iter > str_begin)
            {
                u32 find_iter2 = find_end;

                // Remember the end of the 'str' string

                // First see if we can find the first character of the 'find' string in the 'str' string
                u32           str_end = str_iter;
                uchar32 const f       = utf::read_backward(_find, find_iter2);
                bool          equal   = false;
                while (str_iter > str_begin)
                {
                    str_end         = str_iter;
                    uchar32 const c = utf::read_backward(_str, str_iter);
                    equal           = _casesensitive ? is_equal(c, f) : is_equalfold(c, f);
                    if (equal)
                        break;
                }

                // We did not find the first character, so we can stop here
                if (!equal)
                    return nothing_found(_str);

                // We did find the first character, now see if the rest of the 'find' string is following in the 'str' string
                u32 str_iter2 = str_iter;
                equal         = true;
                while (str_iter2 > str_begin && find_iter2 > find_begin)
                {
                    uchar32 const c1 = utf::read_backward(_str, str_iter2);
                    uchar32 const c2 = utf::read_backward(_find, find_iter2);
                    equal            = _casesensitive ? !is_equal(c1, c2) : !is_equalfold(c1, c2);
                    if (!equal)
                        break;
                }
                if (equal)
                    return select(_str, str_iter2, str_end);

                // We did not find the 'find' string, so we need to continue searching
                utf::read_backward(_str, str_iter);
            }
            return nothing_found(_str);
        }

        runes_t findLast(runes_t const& _str, crunes_t const& _find, bool _casesensitive)
        {
            crunes_t str  = make_crunes(_str);
            crunes_t csel = findLast(str, _find, _casesensitive);
            if (!is_empty(csel))
            {
                runes_t sel = crunes_to_runes(_str, csel);
                return sel;
            }
            return nothing_found(_str);
        }

        crunes_t findOneOf(crunes_t const& _str, crunes_t const& _any, bool _casesensitive)
        {
            u32 iter = get_begin(_str);
            u32 end  = get_end(_str);
            while (iter < end)
            {
                u32 const     cursor = iter;
                uchar32 const c      = utf::read_forward(_str, iter);

                // See if this char can be found in the 'set'
                crunes_t found = find(_any, c, _casesensitive);
                if (!is_empty(found))
                    return select(_str, cursor, iter);
            }
            return nothing_found(_str);
        }

        runes_t findOneOf(runes_t const& _str, crunes_t const& _any, bool _casesensitive)
        {
            crunes_t str  = make_crunes(_str);
            crunes_t csel = findOneOf(str, _any, _casesensitive);
            if (!is_empty(csel))
            {
                runes_t sel = crunes_to_runes(_str, csel);
                return sel;
            }
            return nothing_found(_str);
        }

        // ------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------

        bool contains(crunes_t const& _str, uchar32 _c, bool _case_sensitive)
        {
            u32 const end  = _str.m_end;
            u32       iter = _str.m_str;
            while (iter < end)
            {
                u32           cursor = iter;
                uchar32 const c      = utf::read_forward(_str, iter);
                if (iter == cursor)
                    return false;

                if (_case_sensitive)
                {
                    if (c == _c)
                        return true;
                }
                else
                {
                    if (is_equalfold(c, _c))
                        return true;
                }
            }
            return false;
        }

        bool contains(runes_t const& _str, uchar32 _c, bool case_sensitive)
        {
            crunes_t str = make_crunes(_str);
            return contains(str, _c, case_sensitive);
        }

        // ------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------
        crunes_t findSelectUntil(const crunes_t& _str, uchar32 _c, bool _casesensitive)
        {
            utf32::rune dst_runes[2];
            dst_runes[0].r = _c;
            dst_runes[1].r = 0;
            crunes_t _find = make_crunes(dst_runes, 0, 1, 1);
            return findSelectUntil(_str, _find, _casesensitive);
        }

        crunes_t findSelectUntil(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t sel = find(_str, _find, _casesensitive);
            if (is_empty(sel))
                return nothing_found(_str);
            return make_crunes(_str.m_ascii, _str.m_str, sel.m_str, _str.m_eos, _str.m_type);
        }

        crunes_t findLastSelectUntil(const crunes_t& _str, uchar32 _c, bool _casesensitive)
        {
            utf32::rune dst_runes[2];
            dst_runes[0].r = _c;
            dst_runes[1].r = 0;
            crunes_t _find = make_crunes(dst_runes, 0, 1, 1);
            return findLastSelectUntil(_str, _find, _casesensitive);
        }

        crunes_t findLastSelectUntil(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t sel = findLast(_str, _find, _casesensitive);
            if (is_empty(sel))
                return nothing_found(_str);
            return make_crunes(_str.m_ascii, _str.m_str, sel.m_str, _str.m_eos, _str.m_type);
        }

        crunes_t findSelectUntilIncluded(const crunes_t& _str, uchar32 _c, bool _casesensitive)
        {
            utf32::rune dst_runes[2];
            dst_runes[0].r = _c;
            dst_runes[1].r = 0;
            crunes_t _find = make_crunes(dst_runes, 0, 1, 1);
            return findSelectUntilIncluded(_str, _find, _casesensitive);
        }

        crunes_t findSelectUntilIncluded(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t sel = find(_str, _find, _casesensitive);
            if (is_empty(sel))
                return nothing_found(_str);
            return make_crunes(_str.m_ascii, _str.m_str, sel.m_end, _str.m_eos, _str.m_type);
        }

        crunes_t findSelectUntilIncludedAbortAtOneOf(const crunes_t& _str, const crunes_t& _find, const crunes_t& _abortAnyOneOf, bool _casesensitive)
        {
            if (is_empty(_str) || is_empty(_find))
                return nothing_found(_str);

            u32       str_iter = get_begin(_str);
            u32 const str_end  = get_end(_str);

            u32 const find_iter = get_begin(_find);
            u32 const find_end  = get_end(_find);

            while (str_iter < str_end)
            {
                u32           str_iter2 = str_iter;
                uchar32 const c         = utf::read_forward(_str, str_iter2);
                if (contains(_abortAnyOneOf, c, _casesensitive))
                    return nothing_found(_str);

                str_iter2       = str_iter;
                u32  find_iter2 = find_iter;
                bool equal      = true;
                while (str_iter2 < str_end && find_iter2 < find_end)
                {
                    u32           str_cursor = str_iter2;
                    uchar32 const c1         = utf::read_forward(_str, str_cursor);
                    uchar32 const c2         = utf::read_forward(_find, find_iter2);
                    if (_casesensitive ? !is_equal(c1, c2) : !is_equalfold(c1, c2))
                    {
                        equal = false;
                        break;
                    }
                    str_iter2 = str_cursor;
                }
                if (equal)
                {
                    return select(_str, str_iter, str_iter2);
                }
                utf::read_forward(_str, str_iter);
            }
            return nothing_found(_str);
        }

        crunes_t findLastSelectUntilIncluded(const crunes_t& _str, uchar32 _c, bool _casesensitive)
        {
            utf32::rune dst_runes[2];
            dst_runes[0].r = _c;
            dst_runes[1].r = 0;
            crunes_t _find = make_crunes(dst_runes, 0, 1, 1);
            return findLastSelectUntilIncluded(_str, _find, _casesensitive);
        }

        crunes_t findLastSelectUntilIncluded(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t sel = findLast(_str, _find, _casesensitive);
            if (is_empty(sel))
                return sel;
            return make_crunes(_str.m_ascii, _str.m_str, sel.m_end, _str.m_eos, _str.m_type);
        }

        crunes_t selectAfterSelection(const crunes_t& _str, const crunes_t& _sel)
        {
            if (!is_empty(_sel))
            {
                crunes_t sel;
                sel.m_ascii = _str.m_ascii;
                sel.m_eos   = _str.m_eos;
                sel.m_end   = _sel.m_end;
                sel.m_end   = _str.m_end;
                return sel;
            }
            return _sel;
        }

        crunes_t findSelectAfter(const crunes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t sel = findSelectUntilIncluded(_str, _find, _casesensitive);
            return selectAfterSelection(_str, sel);
        }

        crunes_t findSelectAfter(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t sel = findSelectUntilIncluded(_str, _find, _casesensitive);
            return selectAfterSelection(_str, sel);
        }

        crunes_t findLastSelectAfter(const crunes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t sel = findLastSelectUntilIncluded(_str, _find, _casesensitive);
            return selectAfterSelection(_str, sel);
        }

        crunes_t findLastSelectAfter(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t sel = findLastSelectUntilIncluded(_str, _find, _casesensitive);
            return selectAfterSelection(_str, sel);
        }

        // -------------------------------------------------------------------------------
        // expand
        bool selectMoreRight(crunes_t& inStr, uchar32 inChar)
        {
            u32           cursor = get_end(inStr);
            uchar32 const c      = utf::read_forward(inStr, cursor);
            if (cursor == get_end(inStr))
                return false;
            if (c == inChar)
            {
                inStr.m_end = cursor;
                return true;
            }
            return false;
        }

        // -------------------------------------------------------------------------------
        // select
        crunes_t selectFromToInclude(const crunes_t& inStr, crunes_t const& from, crunes_t const& to)
        {
            if (is_empty(from) || is_empty(to))
                return nothing_found(inStr);
            if (!is_in(inStr, from) || !is_in(inStr, to))
                return nothing_found(inStr);
            return select(inStr, from.m_str, to.m_end);
        }

        crunes_t selectBetween(const crunes_t& _str, uchar32 _left, uchar32 _right)
        {
            crunes_t l = find(_str, _right);
            if (!is_empty(l))
            {
                crunes_t after = selectAfterExclude(_str, l);
                crunes_t r     = findLast(after, _left);
                if (!is_empty(l))
                {
                    return make_crunes(_str.m_ascii, l.m_end, r.m_str, _str.m_eos, _str.m_type);
                }
            }
            return nothing_found(_str);
        }

        crunes_t selectNextBetween(const crunes_t& _str, const crunes_t& _selection, uchar32 _left, uchar32 _right)
        {
            crunes_t str;
            str.m_ascii = _str.m_ascii;
            str.m_eos   = _str.m_eos;
            str.m_end   = _str.m_end;
            str.m_str   = _selection.m_end;
            return selectBetween(str, _left, _right);
        }

        crunes_t selectBetweenLast(const crunes_t& _str, uchar32 _left, uchar32 _right)
        {
            crunes_t r = findLast(_str, _right);
            if (!is_empty(r))
            {
                crunes_t before = selectBeforeExclude(_str, r);
                crunes_t l      = findLast(before, _left);
                if (!is_empty(l))
                {
                    return make_crunes(_str.m_ascii, l.m_end, r.m_str, _str.m_eos, _str.m_type);
                }
            }
            return nothing_found(_str);
        }

        crunes_t selectPreviousBetween(const crunes_t& _str, const crunes_t& _selection, uchar32 _left, uchar32 _right)
        {
            crunes_t str;
            str.m_ascii = _str.m_ascii;
            str.m_eos   = _str.m_eos;
            str.m_str   = _str.m_str;
            str.m_end   = _selection.m_str;
            str.m_type  = _str.m_type;
            return selectBetweenLast(str, _left, _right);
        }

        crunes_t selectBeforeExclude(const crunes_t& _str, const crunes_t& _selection)
        {
            crunes_t str;
            str.m_ascii = _str.m_ascii;
            str.m_str   = _str.m_str;
            str.m_end   = _selection.m_str;
            str.m_eos   = _str.m_eos;
            str.m_type  = _str.m_type;
            return str;
        }

        crunes_t selectBeforeInclude(const crunes_t& _str, const crunes_t& _selection)
        {
            crunes_t str;
            str.m_ascii = _str.m_ascii;
            str.m_eos   = _str.m_eos;
            str.m_ascii = _str.m_ascii;
            str.m_end   = _selection.m_end;
            str.m_type  = _str.m_type;
            return str;
        }

        crunes_t selectAfterExclude(const crunes_t& _str, const crunes_t& _selection)
        {
            crunes_t sel;
            sel.m_ascii = _str.m_ascii;
            sel.m_str   = _selection.m_end;
            sel.m_end   = _str.m_end;
            sel.m_eos   = _str.m_eos;
            sel.m_type  = _str.m_type;
            return sel;
        }

        crunes_t selectAfterInclude(const crunes_t& _str, const crunes_t& _selection)
        {
            crunes_t sel;
            sel.m_ascii = _str.m_ascii;
            sel.m_eos   = _str.m_eos;
            sel.m_ascii = _selection.m_ascii;
            sel.m_end   = _str.m_end;
            sel.m_type  = _str.m_type;
            return sel;
        }

        crunes_t selectOverlap(const crunes_t& _lstr, const crunes_t& _rstr)
        {
            // Case A:                              Case B:
            // lllllllllllll              OR               lllllllllllll
            //          rrrrrrrrrrrr                rrrrrrrrrrrr
            //
            // Case C:                              Case D:
            // llllllllllllllllllll       OR               llllllll
            //      rrrrrrrrrr                      rrrrrrrrrrrrrrrrrrrrr
            //
            u32 lb = get_begin(_lstr);
            u32 le = get_end(_lstr);

            u32 const rb = get_begin(_rstr);
            u32 const re = get_end(_rstr);

            // Check for no overlap
            if (le <= rb || re <= lb)
                return nothing_found(_lstr);

            // Ok, there is overlap, determine the begin and end of the overlap
            if (lb < rb)
                lb = rb;
            if (le > re)
                le = re;
            return select(_lstr, lb, le);
        }

        s32 compare(crunes_t const& _lstr, crunes_t const& _rstr, bool _casesensitive)
        {
            if (is_empty(_lstr) && is_empty(_rstr))
                return 0;
            else if (is_empty(_lstr))
                return -1;
            else if (is_empty(_rstr))
                return 1;

            u32     lcursor = get_begin(_lstr);
            u32     rcursor = get_begin(_rstr);
            u32     lend    = get_end(_lstr);
            u32     rend    = get_end(_rstr);
            uchar32 lc, rc;
            if (_casesensitive)
            {
                do
                {
                    lc = utf::read_forward(_lstr, lcursor);
                    rc = utf::read_forward(_rstr, rcursor);
                } while (lc == rc && lcursor < lend && rcursor < rend);
            }
            else
            {
                do
                {
                    lc = utf::read_forward(_lstr, lcursor);
                    rc = utf::read_forward(_rstr, rcursor);
                    lc = to_lower(lc);
                    rc = to_lower(rc);
                } while (lc == rc && lcursor < lend && rcursor < rend);
            }

            if (lcursor < lend && rcursor == rend)
                return 1;
            if (lcursor == lend && rcursor < rend)
                return -1;

            if (lc < rc)
                return -1;
            else if (lc > rc)
                return 1;

            return 0;
        }

        s32 compare(crunes_t const& _lstr, runes_t const& _rstr, bool _casesensitive)
        {
            crunes_t rstr = make_crunes(_rstr);
            return compare(_lstr, rstr, _casesensitive);
        }

        s32 compare(runes_t const& _lstr, crunes_t const& _rstr, bool _casesensitive)
        {
            crunes_t lstr = make_crunes(_lstr);
            return compare(lstr, _rstr, _casesensitive);
        }

        // ------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------

        runes_t findSelectUntil(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findSelectUntil(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectUntil(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findLastSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectUntil(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findLastSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findSelectUntilIncluded(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findSelectUntilIncluded(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectUntilIncluded(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findLastSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectUntilIncluded(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findLastSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t selectAfterSelection(const runes_t& _str, const runes_t& _sel)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t csel  = make_crunes(_sel);
            crunes_t found = selectAfterSelection(cstr, csel);
            return crunes_to_runes(_str, found);
        }

        runes_t findSelectAfter(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findSelectAfter(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectAfter(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findLastSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectAfter(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = findLastSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t selectBetween(const runes_t& _str, uchar32 _left, uchar32 _right)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = selectBetween(cstr, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes_t selectNextBetween(const runes_t& _str, const runes_t& _selection, uchar32 _left, uchar32 _right)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t csel  = make_crunes(_selection);
            crunes_t found = selectNextBetween(cstr, csel, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes_t selectBetweenLast(const runes_t& _str, uchar32 _left, uchar32 _right)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t found = selectBetweenLast(cstr, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes_t selectPreviousBetween(const runes_t& _str, const runes_t& _selection, uchar32 _left, uchar32 _right)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t csel  = make_crunes(_selection);
            crunes_t found = selectPreviousBetween(cstr, csel, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes_t selectBeforeExclude(const runes_t& _str, const runes_t& _selection)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t csel  = make_crunes(_selection);
            crunes_t found = selectBeforeExclude(cstr, csel);
            return crunes_to_runes(_str, found);
        }

        runes_t selectBeforeInclude(const runes_t& _str, const runes_t& _selection)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t csel  = make_crunes(_selection);
            crunes_t found = selectBeforeInclude(cstr, csel);
            return crunes_to_runes(_str, found);
        }

        runes_t selectAfterExclude(const runes_t& _str, const runes_t& _selection)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t csel  = make_crunes(_selection);
            crunes_t found = selectAfterExclude(cstr, csel);
            return crunes_to_runes(_str, found);
        }

        runes_t selectAfterInclude(const runes_t& _str, const runes_t& _selection)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t csel  = make_crunes(_selection);
            crunes_t found = selectAfterInclude(cstr, csel);
            return crunes_to_runes(_str, found);
        }

        runes_t selectOverlap(const runes_t& _str, const runes_t& _rstr)
        {
            crunes_t cstr  = make_crunes(_str);
            crunes_t crstr = make_crunes(_rstr);
            crunes_t found = selectOverlap(cstr, crstr);
            return crunes_to_runes(_str, found);
        }

        s32 compare(runes_t const& _lstr, runes_t const& _rstr, bool _casesensitive)
        {
            crunes_t clstr = make_crunes(_lstr);
            crunes_t crstr = make_crunes(_rstr);
            return compare(clstr, crstr, _casesensitive);
        }

        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------

        crunes_t parse(crunes_t const& _str, bool& value)
        {
            crunes_t    str          = _str;
            ascii::rune format_str[] = {'%', 'b', ascii::TERMINATOR};
            crunes_t    format       = make_crunes(format_str, 0, 2, 2);
            sscanf(str, format, va_r_t(&value));
            return str;
        }

        crunes_t parse(crunes_t const& _str, s8& value, s32 base)
        {
            s64      value64;
            crunes_t adv = parse(_str, value64, base);
            value        = (s8)value64;
            return adv;
        }

        crunes_t parse(crunes_t const& _str, s16& value, s32 base)
        {
            s64      value64;
            crunes_t adv = parse(_str, value64, base);
            value        = (s16)value64;
            return adv;
        }

        crunes_t parse(crunes_t const& _str, s32& value, s32 base)
        {
            s64      value64;
            crunes_t adv = parse(_str, value64, base);
            value        = (s32)value64;
            return adv;
        }

        crunes_t parse(crunes_t const& _str, u8& value, s32 base)
        {
            u64      value64;
            crunes_t adv = parse(_str, value64, base);
            value        = (u8)value64;
            return adv;
        }

        crunes_t parse(crunes_t const& _str, u16& value, s32 base)
        {
            u64      value64;
            crunes_t adv = parse(_str, value64, base);
            value        = (u16)value64;
            return adv;
        }

        crunes_t parse(crunes_t const& _str, u32& value, s32 base)
        {
            u64      value64;
            crunes_t adv = parse(_str, value64, base);
            value        = (u16)value64;
            return adv;
        }

        crunes_t parse(crunes_t const& _str, s64& value, s32 base)
        {
            crunes_t    str          = _str;
            ascii::rune format_str[] = {'%', 'd', ascii::TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes_t format = make_crunes(format_str, 0, 2, 2);
            sscanf(str, format, va_r_t(&value));
            return str;
        }

        crunes_t parse(crunes_t const& _str, u64& value, s32 base)
        {
            crunes_t    str          = _str;
            ascii::rune format_str[] = {'%', 'd', ascii::TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes_t format = make_crunes(format_str, 0, 2, 2);
            sscanf(str, format, va_r_t(&value));
            return str;
        }

        crunes_t parse(crunes_t const& _str, f32& value)
        {
            crunes_t    str          = _str;
            ascii::rune format_str[] = {'%', 'f', ascii::TERMINATOR};
            crunes_t    format       = make_crunes(format_str, 0, 2, 2);
            sscanf(str, format, va_r_t(&value));
            return str;
        }

        crunes_t parse(crunes_t const& _str, f64& value)
        {
            crunes_t    str          = _str;
            ascii::rune format_str[] = {'%', 'f', ascii::TERMINATOR};
            crunes_t    format       = make_crunes(format_str, 0, 2, 2);
            sscanf(str, format, va_r_t(&value));
            return str;
        }

        //------------------------------------------------------------------------------
        static uchar32 const sWhitespaceChars[]    = {' ', '\t', '\n', '\r', 0};
        static u32 const     sWhitespaceCharsCount = 4;

        //------------------------------------------------------------------------------
        bool is_decimal(crunes_t const& _str)
        {
            u32       iter = get_begin(_str);
            u32 const end  = get_end(_str);
            while (iter < end)
            {
                u32 const     cursor = iter;
                uchar32 const c      = utf::read_forward(_str, iter);
                if ((c == '-' || c == '+') && cursor > _str.m_str)
                    return false;
                if (c < '0' || c > '9')
                    return false;
            }
            return true;
        }

        //------------------------------------------------------------------------------
        bool is_hexadecimal(crunes_t const& _str, bool with_prefix)
        {
            u32       iter = get_begin(_str);
            u32 const end  = get_end(_str);

            uchar32 p = 0;
            uchar32 c = 0;
            s32     n = 0;
            while (iter < end)
            {
                p = c;
                c = utf::read_forward(_str, iter);
                if (c == 'x')
                {
                    if (p != '0' || n != 1)
                        return false;
                }
                else if ((c < '0' || c > '9') && (c < 'a' || c > 'f') && (c < 'A' || c > 'F'))
                {
                    return false;
                }
                n++;
            }
            return true;
        }

        //------------------------------------------------------------------------------
        bool is_float(crunes_t const& _str)
        {
            ascii::rune f32chars_str[] = {'E', 'e', '.', '#', 'Q', 'N', 'A', 'B', 'I', 'F', 0};
            crunes_t    f32chars       = make_crunes(f32chars_str, 0, 10, 10);

            u32 iter = get_begin(_str);
            u32 end  = get_end(_str);
            while (iter < end)
            {
                uchar32 c = utf::read_forward(_str, iter);
                if (c >= '0' && c <= '9')
                    continue;
                if (!contains(f32chars, c))
                    return false;
            }
            return true;
        }

        //------------------------------------------------------------------------------
        bool is_GUID(crunes_t const& _str)
        {
            u32 iter = get_begin(_str);
            u32 end  = get_end(_str);

            // Does it have only 0-9, a-f, A-F characters
            s32 n = 0;
            s32 l = 0;
            while (iter < end)
            {
                uchar32 const c = utf::read_forward(_str, iter);
                if (c >= '0' && c <= '9')
                {
                    n++;
                    continue;
                }
                if (c >= 'A' && c <= 'F')
                {
                    n++;
                    continue;
                }
                if (c >= 'a' && c <= 'f')
                {
                    n++;
                    continue;
                }
                if (c == ':')
                {
                    l++;
                    if (l > 3)
                        return false;
                    continue;
                }
            }
            // the length of the string must be 17 characters long (4*8 + 3 ':')
            return (n == (4 * 8) && l == 3);
        }

        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------

        void to_string(runes_t& str, s32 val, s32 base)
        {
            ascii::rune format_str[] = {'%', 'd', ascii::TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes_t format = make_crunes(format_str, 0, 2, 2);
            sprintf(str, format, va_t(val));
        }

        void to_string(runes_t& str, u32 val, s32 base)
        {
            ascii::rune format_str[] = {'%', 'u', ascii::TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'u'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes_t format = make_crunes(format_str, 0, 2, 2);
            sprintf(str, format, va_t(val));
        }

        // *cFHbRXV[N]WWWR6FETY+

        void to_string(runes_t& str, s64 val, s32 base)
        {
            ascii::rune format_str[] = {'%', 'd', ascii::TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes_t format = make_crunes(format_str, 0, 2, 2);
            sprintf(str, format, va_t(val));
        }

        void to_string(runes_t& str, u64 val, s32 base)
        {
            ascii::rune format_str[] = {'%', 'd', ascii::TERMINATOR};
            switch (base)
            {
                case 16: format_str[1] = 'x'; break;
                case 10: format_str[1] = 'd'; break;
                case 8: format_str[1] = 'o'; break;
            };
            crunes_t format = make_crunes(format_str, 0, 2, 2);
            sprintf(str, format, va_t(val));
        }

        void to_string(runes_t& str, f32 val, s32 numFractionalDigits)
        {
            ascii::rune format_str[] = {'%', '.', '0', '2', 'f', ascii::TERMINATOR};
            if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
            {
                format_str[2] = (ascii::rune)('0' + numFractionalDigits / 10);
                format_str[3] = (ascii::rune)('0' + numFractionalDigits % 10);
            }
            crunes_t format = make_crunes(format_str, 0, 5, 5);
            sprintf(str, format, va_t(val));
        }

        void to_string(runes_t& str, f64 val, s32 numFractionalDigits)
        {
            ascii::rune format_str[] = {'%', '.', '0', '2', 'f', ascii::TERMINATOR};
            if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
            {
                format_str[2] = (ascii::rune)('0' + numFractionalDigits / 10);
                format_str[3] = (ascii::rune)('0' + numFractionalDigits % 10);
            }
            crunes_t format = make_crunes(format_str, 0, 5, 5);
            sprintf(str, format, va_t(val));
        }

        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------

        bool is_upper(crunes_t const& _str)
        {
            u32 iter = get_begin(_str);
            u32 end  = get_end(_str);
            while (iter < end)
            {
                uchar32 c = utf::read_forward(_str, iter);
                if (is_lower(c))
                    return false;
            }
            return true;
        }

        bool is_lower(crunes_t const& _str)
        {
            u32 iter = get_begin(_str);
            u32 end  = get_end(_str);
            while (iter < end)
            {
                uchar32 c = utf::read_forward(_str, iter);
                if (is_upper(c))
                    return false;
            }
            return true;
        }

        bool is_capitalized(crunes_t const& _str)
        {
            bool b    = true;
            u32  iter = get_begin(_str);
            u32  end  = get_end(_str);
            while (iter < end)
            {
                uchar32 c = utf::read_forward(_str, iter);
                if (is_upper(c) != b)
                    return false;
                b = (c == ' ');
            }
            return true;
        }

        bool is_quoted(crunes_t const& str, uchar32 quote) { return is_delimited(str, quote, quote); }

        bool is_delimited(crunes_t const& _str, uchar32 delimit_left, uchar32 delimit_right)
        {
            uchar32 first = first_char(_str);
            if (first == delimit_left)
            {
                uchar32 last = last_char(_str);
                return last == delimit_right;
            }
            return false;
        }

        /**
         * Assumption: Letters A-Z and a-z are contiguous in the character set.
         * This is true for ASCII and UniCode.  (Not so for EBCDIC!)
         */
        void to_upper(runes_t& _str)
        {
            u32 iter = get_begin(_str);
            u32 end  = get_end(_str);
            while (iter < end)
            {
                u32     cursor = iter;
                uchar32 c      = utf::read_forward(_str, iter);
                if ((c >= 'a') && (c <= 'z'))
                {
                    c += (uchar32)('A' - 'a');
                    write(_str, cursor, c);
                }
            }
        }

        /**
         * Assumption: Letters A-Z and a-z are contiguous in the character set.
         * This is true for ASCII and UniCode.  (Not so for EBCDIC!)
         */
        void to_lower(runes_t& _str)
        {
            u32 iter = get_begin(_str);
            u32 end  = get_end(_str);
            while (iter < end)
            {
                u32     cursor = iter;
                uchar32 c      = utf::read_forward(_str, iter);
                if ((c >= 'A') && (c <= 'Z'))
                {
                    c += (uchar32)('a' - 'A');
                    write(_str, cursor, c);
                }
            }
        }

        // ------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------

        bool starts_with(crunes_t const& _str, uchar32 start_char)
        {
            u32     iter = get_begin(_str);
            uchar32 c    = utf::read_forward(_str, iter);
            return start_char == c;
        }

        bool starts_with(crunes_t const& _str, crunes_t const& _start, bool _casesensitive)
        {
            u32       liter = get_begin(_str);
            u32 const lend  = get_end(_str);
            u32       riter = get_begin(_start);
            u32 const rend  = get_end(_start);
            while (liter < lend && riter < rend)
            {
                uchar32 const lc = utf::read_forward(_str, liter);
                uchar32 const rc = utf::read_forward(_start, riter);
                if (_casesensitive)
                {
                    if (lc != rc)
                        return false;
                }
                else
                {
                    if (to_lower(lc) != to_lower(rc))
                        return false;
                }
            }
            return liter <= lend && riter == rend;
        }

        bool ends_with(crunes_t const& _str, uchar32 end_char)
        {
            u32     iter = get_end(_str);
            uchar32 c    = utf::read_backward(_str, iter);
            return end_char == c;
        }

        bool ends_with(crunes_t const& _str, crunes_t const& _end)
        {
            u32       liter = get_end(_str);
            u32 const lend  = get_begin(_str);
            u32       riter = get_end(_end);
            u32 const rend  = get_begin(_end);
            while (liter > lend && riter > rend)
            {
                uchar32 const lc = utf::read_backward(_str, liter);
                uchar32 const rc = utf::read_backward(_end, riter);
                if (lc != rc)
                    return false;
            }
            return liter == lend && riter == rend;
        }

        uchar32 first_char(crunes_t const& _str)
        {
            u32     iter = get_begin(_str);
            uchar32 c    = utf::read_forward(_str, iter);
            return c;
        }

        uchar32 last_char(crunes_t const& _str)
        {
            u32     iter = get_end(_str);
            uchar32 c    = utf::read_backward(_str, iter);
            return c;
        }

        // ------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------

        void removeSelection(runes_t& str, crunes_t const& selection)
        {
            //@note: treated as a mem move
            u32       dst = selection.m_str;
            u32       src = selection.m_end;
            const u32 end = str.m_end;
            while (src < end)
            {
                str.m_ascii[dst++] = str.m_ascii[src++];
            }
            str.m_end = dst;
        }

        void keepOnlySelection(runes_t& str, crunes_t const& keep)
        {
            //@note: treated as a mem move
            u32       dst = str.m_str;
            u32       src = keep.m_str;
            u32 const end = keep.m_end;
            while (src < end)
            {
                // *dst++ = *src++;
                str.m_ascii[dst++] = str.m_ascii[src++];
            }
            str.m_end = dst;
        }

        void replaceSelection(runes_t& _str, crunes_t const& _selection, crunes_t const& _replace)
        {
            ASSERT(_str.m_type == _selection.m_type);
            ASSERT(_str.m_type == _replace.m_type);

            runes_t str = crunes_to_runes(_str, _selection);

            // NOTE!! : The logic here is based on memory copy and size in bytes, we do not consider number of characters

            u32 const selected_len = (u32)(str.m_end - str.m_str);
            u32 const replace_len  = (u32)(_replace.m_end - _replace.m_str);

            u32 end;
            if (selected_len < replace_len)
            {
                // Move, expanding
                u32 move_len = replace_len - selected_len;
                if (move_len > (str.m_eos - str.m_end))
                    move_len = (str.m_eos - str.m_end);

                // No matter what, push out anything at the end!
                u32 dst = (str.m_end + (str.m_eos - str.m_end) - 1);
                u32 src = (dst - move_len);
                while (dst > str.m_end)
                {
                    //*dst-- = *src--;
                    str.m_ascii[dst--] = str.m_ascii[src--];
                }

                end = _str.m_end + move_len;  // Update str_end
            }
            else if (selected_len > replace_len)
            {
                // Move, shrinking
                u32 move_len = selected_len - replace_len;
                u32 dst      = str.m_end - move_len;
                u32 src      = str.m_end;
                while (src < str.m_eos)
                {
                    //*dst++ = *src++;
                    str.m_ascii[dst++] = str.m_ascii[src++];
                }

                end = _str.m_end - move_len;  // Update str_end
            }
            else
            {
                end = _str.m_end;
            }

            // Replace
            u32 src     = _replace.m_str;
            u32 src_end = _replace.m_str + replace_len;
            u32 dst     = str.m_str;
            while (src < src_end)
            {
                //*dst++ = *src++;
                str.m_ascii[dst++] = _replace.m_ascii[src++];
            }
            _str.m_end = end;
        }

        void findReplace(runes_t& _str, uchar32 _find, uchar32 _replace, bool _casesensitive)
        {
            utf32::rune find_runes[2];
            find_runes[0].r = _find;
            find_runes[1].r = 0;
            crunes_t find   = make_crunes(find_runes, 0, 1, 1);

            utf32::rune replace_runes[2];
            replace_runes[0].r = _replace;
            replace_runes[1].r = 0;
            crunes_t replace   = make_crunes(replace_runes, 0, 1, 1);

            findReplace(_str, find, replace, _casesensitive);
        }

        void findReplace(runes_t& _str, crunes_t const& _find, crunes_t const& _replace, bool _casesensitive)
        {
            runes_t selected = find(_str, _find, _casesensitive);
            if (is_empty(selected))
                return;
            crunes_t cselected = make_crunes(selected);
            replaceSelection(_str, cselected, _replace);
        }

        // TODO implementations of 'insert'
        void insert(runes_t& str, crunes_t const& insert) {}
        void insert(runes_t& str, crunes_t const& selection, crunes_t const& insert) {}

        void trim(runes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset    = make_crunes(charseta, &charseta[2]);
            trimLeft(str, charset);
            trimRight(str, charset);
        }

        void trimLeft(runes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset    = make_crunes(charseta, &charseta[2]);
            trimLeft(str, charset);
        }

        void trimRight(runes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset    = make_crunes(charseta, &charseta[2]);
            trimRight(str, charset);
        }

        void trim(runes_t& str, uchar32 _c)
        {
            utf32::rune charseta[] = {_c, cEOS};
            crunes_t    charset    = make_crunes(charseta, &charseta[1]);
            trimLeft(str, charset);
            trimRight(str, charset);
        }

        void trimLeft(runes_t& str, uchar32 _c)
        {
            utf32::rune charset[] = {_c, cEOS};
            crunes_t    ccharset  = make_crunes(charset, 0, 1, 1);
            trimLeft(str, ccharset);
        }

        void trimRight(runes_t& str, uchar32 _c)
        {
            utf32::rune charset[2] = {_c, cEOS};
            crunes_t    ccharset   = make_crunes(charset, 0, 1, 1);
            trimLeft(str, ccharset);
        }

        void trim(runes_t& str, crunes_t const& _charset)
        {
            trimLeft(str, _charset);
            trimRight(str, _charset);
        }

        void trimLeft(runes_t& _str, crunes_t const& _charset)
        {
            bool      trim = true;
            u32       iter = get_begin(_str);
            u32 const end  = get_end(_str);
            u32 const cend = get_end(_charset);
            u32       l    = get_begin(_str);
            while (iter < end && trim)
            {
                uchar32 c1    = utf::read_forward(_str, iter);
                u32     citer = get_begin(_charset);
                trim          = false;
                while (citer < cend)
                {
                    uchar32 c2 = utf::read_forward(_charset, citer);
                    if (c1 == c2)
                    {
                        trim = true;
                        l    = iter;
                        break;
                    }
                }
            }
            _str = select(_str, l, end);
        }

        void trimRight(runes_t& _str, crunes_t const& _charset)
        {
            u32       iter  = get_end(_str);
            u32 const begin = get_begin(_str);
            u32       r     = iter;
            while (iter > begin)
            {
                uchar32 const c1 = utf::read_backward(_str, iter);
                if (!contains(_charset, c1))
                    break;
                r = iter;
            }
            _str = select(_str, begin, r);
        }

        void trimQuotes(runes_t& str)
        {
            utf32::rune charseta[] = {'\'', '"', cEOS};
            crunes_t    charset    = make_crunes(charseta, 0, 2, 2);
            trim(str, charset);
        }

        void trimQuotes(runes_t& str, uchar32 quote)
        {
            utf32::rune charseta[] = {quote, cEOS};
            crunes_t    charset    = make_crunes(charseta, 0, 1, 1);
            trim(str, charset);
        }

        void trimDelimiters(runes_t& str, uchar32 _left, uchar32 _right)
        {
            trimLeft(str, _left);
            trimRight(str, _right);
        }

        void trim(crunes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset    = make_crunes(charseta, 0, 2, 2);
            trimLeft(str, charset);
            trimRight(str, charset);
        }

        void trimLeft(crunes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset    = make_crunes(charseta, 0, 2, 2);
            trimLeft(str, charset);
        }

        void trimRight(crunes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset    = make_crunes(charseta, 0, 2, 2);
            trimRight(str, charset);
        }

        void trim(crunes_t& str, uchar32 _c)
        {
            utf32::rune charseta[] = {_c, cEOS};
            crunes_t    charset    = make_crunes(charseta, 0, 1, 1);
            trimLeft(str, charset);
            trimRight(str, charset);
        }

        void trimLeft(crunes_t& str, uchar32 _c)
        {
            utf32::rune charseta[] = {_c, cEOS};
            crunes_t    charset    = make_crunes(charseta, 0, 1, 1);
            trimLeft(str, charset);
        }

        void trimRight(crunes_t& str, uchar32 _c)
        {
            utf32::rune charseta[2] = {_c, cEOS};
            crunes_t    charset     = make_crunes(charseta, 0, 1, 1);
            trimLeft(str, charset);
        }

        void trim(crunes_t& str, crunes_t const& _charset)
        {
            trimLeft(str, _charset);
            trimRight(str, _charset);
        }

        void trimLeft(crunes_t& _str, crunes_t const& _charset)
        {
            u32       iter = get_begin(_str);
            u32 const end  = get_end(_str);
            u32       l    = iter;
            while (iter < end)
            {
                uchar32 c1 = utf::read_forward(_str, iter);
                if (!contains(_charset, c1))
                    break;
                l = iter;
            }
            _str = select(_str, l, end);
        }

        void trimRight(crunes_t& _str, crunes_t const& _charset)
        {
            u32       iter  = get_end(_str);
            u32 const begin = get_begin(_str);
            u32       r     = iter;
            while (iter > begin)
            {
                uchar32 c1 = utf::read_backward(_str, iter);
                if (!contains(_charset, c1))
                    break;
                r = iter;
            }
            _str = select(_str, begin, r);
        }

        void trimQuotes(crunes_t& str)
        {
            utf32::rune charseta[] = {'\'', '"', cEOS};
            crunes_t    charset    = make_crunes(charseta, 0, 2, 2);
            trim(str, charset);
        }

        void trimQuotes(crunes_t& str, uchar32 quote)
        {
            utf32::rune charseta[] = {quote, cEOS};
            crunes_t    charset    = make_crunes(charseta, 0, 1, 1);
            trim(str, charset);
        }

        void trimDelimiters(crunes_t& str, uchar32 _left, uchar32 _right)
        {
            trimLeft(str, _left);
            trimRight(str, _right);
        }

        // ------------------------------------------------------------------------------
        // reading
        uchar32 read(crunes_t const& str, u32& cursor) { return utf::read_forward(str, cursor); }

        bool copy(crunes_t const& _src, runes_t& _dst)
        {
            u32 siter  = get_begin(_src);
            u32 send   = get_end(_src);
            u32 dbegin = get_begin(_dst);
            u32 diter  = get_begin(_dst);
            u32 dend   = get_eos(_dst);
            while (siter < send)
            {
                if (diter >= dend)
                {
                    _dst = select(_dst, dbegin, dend);
                    return false;
                }
                uchar32 const c = utf::read_forward(_src, siter);
                if (c == 0)
                    break;
                write(_dst, diter, c);
            }
            _dst = select(_dst, dbegin, diter);
            return true;
        }

        bool concatenate(runes_t& _dst, crunes_t const& _con)
        {
            u32       siter  = get_begin(_con);
            u32 const send   = get_end(_con);
            u32       dbegin = get_begin(_dst);
            u32       diter  = get_end(_dst);
            u32 const dend   = get_eos(_dst);
            while (siter < send)
            {
                if (diter >= dend)
                    return false;
                uchar32 const c = utf::read_forward(_con, siter);
                write(_dst, diter, c);
            }
            _dst = select(_dst, dbegin, diter);
            return true;
        }

        bool concatenate(runes_t& str, const crunes_t& concat1, const crunes_t& concat2)
        {
            concatenate(str, concat1);
            concatenate(str, concat2);
            return true;
        }

        // ---------------------------------------------------------------------------------------------------------------------------
        // ---------------------------------------------------------------------------------------------------------------------------
        // ---------------------------------------------------------------------------------------------------------------------------

        // ---------------------------------------------------------------------------------------------------------------------------
        // ---------------------------------------------------------------------------------------------------------------------------
        // u32 functions
        inline u32 get_begin(runes_t const& str) { return str.m_str; }
        inline u32 get_end(runes_t const& str) { return str.m_end; }
        inline u32 get_eos(runes_t const& str) { return str.m_eos; }

        static runes_t select(runes_t const& str, u32 from, u32 to)
        {
            runes_t r;
            r.m_type  = str.m_type;
            r.m_ascii = str.m_ascii;
            r.m_str   = from;
            r.m_end   = to;
            r.m_eos   = str.m_eos;
            return r;
        }

        bool backwards(runes_t const& str, u32& cursor, s32 step)
        {
            ASSERT(step > 0);
            switch (str.m_type)
            {
                case ascii::TYPE: return utf::skip_backward(str.m_ascii, cursor, str.m_str, str.m_end, step) == step;
                case ucs2::TYPE: return utf::skip_backward(str.m_ucs2, cursor, str.m_str, str.m_end, step) == step;
                case utf8::TYPE: return utf::skip_backward(str.m_utf8, cursor, str.m_str, str.m_end, step) == step;
                case utf16::TYPE: return utf::skip_backward(str.m_utf16, cursor, str.m_str, str.m_end, step) == step;
                case utf32::TYPE: return utf::skip_backward(str.m_utf32, cursor, str.m_str, str.m_end, step) == step;
            }
            return false;
        }

        bool write(runes_t const& str, u32& cursor, uchar32 c)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return utf::write(c, str.m_ascii, cursor, str.m_eos); break;
                case ucs2::TYPE: return utf::write(c, str.m_ucs2, cursor, str.m_eos); break;
                case utf16::TYPE: return utf::write(c, str.m_utf16, cursor, str.m_eos); break;
                case utf32::TYPE: return utf::write(c, str.m_utf32, cursor, str.m_eos); break;
                default: return false;
            }
            return true;
        }

        // ---------------------------------------------------------------------------------------------------------------------------
        // ---------------------------------------------------------------------------------------------------------------------------
        // u32 functions
        static inline u32 get_begin(crunes_t const& str) { return (str.m_str); }
        static inline u32 get_end(crunes_t const& str) { return (str.m_end); }

        static crunes_t select(crunes_t const& str, u32 from, u32 to)
        {
            crunes_t r;
            r.m_type  = str.m_type;
            r.m_ascii = str.m_ascii;
            r.m_eos   = str.m_eos;
            r.m_str   = from;
            r.m_end   = to;
            return r;
        }

        static bool forwards(crunes_t const& str, u32& cursor, s32 step)
        {
            ASSERT(step > 0);
            s32 n = 0;
            switch (str.m_type)
            {
                case ascii::TYPE: n = utf::skip_forward(str.m_ascii, cursor, str.m_str, str.m_end, step); break;
                case ucs2::TYPE: n = utf::skip_forward(str.m_ucs2, cursor, str.m_str, str.m_end, step); break;
                case utf8::TYPE: n = utf::skip_forward(str.m_utf8, cursor, str.m_str, str.m_end, step); break;
                case utf16::TYPE: n = utf::skip_forward(str.m_utf16, cursor, str.m_str, str.m_end, step); break;
                case utf32::TYPE: n = utf::skip_forward(str.m_utf32, cursor, str.m_str, str.m_end, step); break;
            }
            return n > 0;
        }
        bool backwards(crunes_t const& str, u32& cursor, s32 step)
        {
            ASSERT(step > 0);
            s32 n = 0;
            switch (str.m_type)
            {
                case ascii::TYPE: n = utf::skip_backward(str.m_ascii, cursor, str.m_str, str.m_end, step); break;
                case ucs2::TYPE: n = utf::skip_backward(str.m_ucs2, cursor, str.m_str, str.m_end, step); break;
                case utf8::TYPE: n = utf::skip_backward(str.m_utf8, cursor, str.m_str, str.m_end, step); break;
                case utf16::TYPE: n = utf::skip_backward(str.m_utf16, cursor, str.m_str, str.m_end, step); break;
                case utf32::TYPE: n = utf::skip_backward(str.m_utf32, cursor, str.m_str, str.m_end, step); break;
            }
            return n > 0;
        }

        static inline bool is_valid(crunes_t const& str, u32 const& cursor) { return cursor >= str.m_str && cursor < str.m_end; }
    }  // namespace nrunes

    // ------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------
    runes_t make_runes()
    {
        runes_t r;
        r.m_ascii = nullptr;
        r.m_str   = 0;
        r.m_end   = 0;
        r.m_eos   = 0;
        r.m_type  = ascii::TYPE;
        return r;
    }

    runes_t make_runes(runes_t const& other)
    {
        runes_t r;
        r.m_ascii = other.m_ascii;
        r.m_str   = other.m_str;
        r.m_end   = other.m_end;
        r.m_eos   = other.m_eos;
        r.m_type  = other.m_type;
        return r;
    }

    runes_t make_runes(ascii::prune _str, ascii::prune _end, u32 _type)
    {
        runes_t r;
        r.m_ascii = _str;
        r.m_str   = 0;
        r.m_end   = (u32)(_end - _str);
        r.m_eos   = (u32)(_end - _str);
        r.m_type  = _type;
        return r;
    }

    runes_t make_runes(ascii::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        runes_t r;
        r.m_ascii = _bos;
        r.m_str   = _str;
        r.m_end   = _end;
        r.m_eos   = _eos;
        r.m_type  = _type;
        return r;
    }

    runes_t make_runes(ucs2::prune _str, ucs2::prune _end, u32 _type)
    {
        runes_t r;
        r.m_ucs2 = _str;
        r.m_str  = 0;
        r.m_end  = (u32)(_end - _str);
        r.m_eos  = (u32)(_end - _str);
        r.m_type = _type;
        return r;
    }

    runes_t make_runes(ucs2::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        runes_t r;
        r.m_ucs2 = _bos;
        r.m_str  = _str;
        r.m_end  = _end;
        r.m_eos  = _eos;
        r.m_type = _type;
        return r;
    }

    runes_t make_runes(utf8::prune _str, utf8::prune _end, u32 _type)
    {
        runes_t r;
        r.m_utf8 = _str;
        r.m_str  = 0;
        r.m_eos  = (u32)(_end - _str);
        r.m_end  = (u32)(_end - _str);
        r.m_type = _type;
        return r;
    }

    runes_t make_runes(utf8::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        runes_t r;
        r.m_utf8 = _bos;
        r.m_str  = _str;
        r.m_end  = _end;
        r.m_eos  = _eos;
        r.m_type = _type;
        return r;
    }

    runes_t make_runes(utf16::prune _str, utf16::prune _end, u32 _type)
    {
        runes_t r;
        r.m_utf16 = _str;
        r.m_str   = 0;
        r.m_eos   = (u32)(_end - _str);
        r.m_end   = (u32)(_end - _str);
        r.m_type  = _type;
        return r;
    }

    runes_t make_runes(utf16::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        runes_t r;
        r.m_utf16 = _bos;
        r.m_str   = _str;
        r.m_end   = _end;
        r.m_eos   = _eos;
        r.m_type  = _type;
        return r;
    }

    runes_t make_runes(utf32::prune _str, utf32::prune _end, u32 _type)
    {
        runes_t r;
        r.m_utf32 = _str;
        r.m_str   = 0;
        r.m_eos   = (u32)(_end - _str);
        r.m_end   = (u32)(_end - _str);
        r.m_type  = _type;
        return r;
    }

    runes_t make_runes(utf32::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        runes_t r;
        r.m_utf32 = _bos;
        r.m_str   = _str;
        r.m_end   = _end;
        r.m_eos   = _eos;
        r.m_type  = _type;
        return r;
    }

    // ------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------
    static const char* sNullAsciiString = "\0\0\0\0";

    crunes_t make_crunes()
    {
        crunes_t c;
        c.m_ascii = sNullAsciiString;
        c.m_str   = 0;
        c.m_end   = 0;
        c.m_eos   = 0;
        c.m_type  = ascii::TYPE;
        return c;
    }

    crunes_t make_crunes(runes_t const& _str)
    {
        crunes_t c;
        c.m_ascii = _str.m_ascii;
        c.m_str   = _str.m_str;
        c.m_end   = _str.m_end;
        c.m_eos   = _str.m_eos;
        c.m_type  = _str.m_type;
        return c;
    }

    crunes_t make_crunes(crunes_t const& _str)
    {
        crunes_t c;
        c.m_ascii = _str.m_ascii;
        c.m_str   = _str.m_str;
        c.m_end   = _str.m_end;
        c.m_eos   = _str.m_eos;
        c.m_type  = _str.m_type;
        return c;
    }

    crunes_t make_crunes(crunes_t const& _str, u32 from, u32 to)
    {
        ASSERT(from >= _str.m_str && from <= _str.m_eos);
        ASSERT(to >= _str.m_str && to <= _str.m_eos);
        ASSERT(from <= to);
        crunes_t c;
        c.m_ascii = _str.m_ascii;
        c.m_str   = from;
        c.m_end   = to;
        c.m_eos   = _str.m_eos;
        c.m_type  = _str.m_type;
        return c;
    }

    crunes_t make_crunes(ascii::pcrune _bos, u32 _str, u32 _end, u32 _eos, u8 _type)
    {
        crunes_t c;
        c.m_ascii = _bos;
        c.m_str   = _str;
        c.m_end   = _end;
        c.m_eos   = _eos;
        c.m_type  = _type;
        return c;
    }

    crunes_t make_crunes(ascii::pcrune _str)
    {
        ascii::pcrune end = utf::endof(_str, nullptr);
        crunes_t      c;
        c.m_ascii = _str;
        c.m_str   = 0;
        c.m_end   = (u32)(end - _str);
        c.m_eos   = c.m_end;
        c.m_type  = ascii::TYPE;
        return c;
    }

    crunes_t make_crunes(ascii::pcrune _str, ascii::pcrune _end)
    {
        crunes_t c;
        c.m_ascii = _str;
        c.m_str   = 0;
        c.m_end   = (u32)(_end - _str);
        c.m_eos   = (u32)(_end - _str);
        c.m_type  = ascii::TYPE;
        return c;
    }

    crunes_t make_crunes(ascii::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        crunes_t c;
        c.m_ascii = _bos;
        c.m_str   = _str;
        c.m_end   = _end;
        c.m_eos   = _eos;
        c.m_type  = ascii::TYPE;
        return c;
    }

    crunes_t make_crunes(ucs2::pcrune _str)
    {
        ucs2::pcrune end = utf::endof(_str, nullptr);
        crunes_t     c;
        c.m_ucs2 = _str;
        c.m_str  = 0;
        c.m_end  = (u32)(end - _str);
        c.m_eos  = c.m_end;
        c.m_type = ucs2::TYPE;
        return c;
    }

    crunes_t make_crunes(ucs2::pcrune _str, ucs2::pcrune _end)
    {
        crunes_t c;
        c.m_ucs2 = _str;
        c.m_str  = 0;
        c.m_end  = (u32)(_end - _str);
        c.m_eos  = (u32)(_end - _str);
        c.m_type = ucs2::TYPE;
        return c;
    }

    crunes_t make_crunes(ucs2::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        crunes_t c;
        c.m_ucs2 = _bos;
        c.m_str  = _str;
        c.m_end  = _end;
        c.m_eos  = _eos;
        c.m_type = ucs2::TYPE;
        return c;
    }

    crunes_t make_crunes(utf8::pcrune _str)
    {
        utf8::pcrune end = utf::endof(_str, nullptr);
        crunes_t     c;
        c.m_utf8 = _str;
        c.m_str  = 0;
        c.m_end  = (u32)(end - _str);
        c.m_eos  = c.m_end;
        c.m_type = utf8::TYPE;
        return c;
    }
    crunes_t make_crunes(utf8::pcrune _str, utf8::pcrune _end)
    {
        crunes_t c;
        c.m_utf8 = _str;
        c.m_str  = 0;
        c.m_end  = (u32)(_end - _str);
        c.m_eos  = (u32)(_end - _str);
        c.m_type = utf8::TYPE;
        return c;
    }

    crunes_t make_crunes(utf8::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        crunes_t c;
        c.m_utf8 = _bos;
        c.m_str  = _str;
        c.m_end  = _end;
        c.m_eos  = _eos;
        c.m_type = utf8::TYPE;
        return c;
    }

    crunes_t make_crunes(utf16::pcrune _str)
    {
        utf16::pcrune end = utf::endof(_str, nullptr);
        crunes_t      c;
        c.m_utf16 = _str;
        c.m_str   = 0;
        c.m_end   = (u32)(end - _str);
        c.m_eos   = c.m_end;
        c.m_type  = utf16::TYPE;
        return c;
    }

    crunes_t make_crunes(utf16::pcrune _str, utf16::pcrune _end)
    {
        crunes_t c;
        c.m_utf16 = _str;
        c.m_str   = 0;
        c.m_end   = (u32)(_end - _str);
        c.m_eos   = (u32)(_end - _str);
        c.m_type  = utf16::TYPE;
        return c;
    }

    crunes_t make_crunes(utf16::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        crunes_t c;
        c.m_utf16 = _bos;
        c.m_str   = _str;
        c.m_end   = _end;
        c.m_eos   = _eos;
        c.m_type  = utf16::TYPE;
        return c;
    }

    crunes_t make_crunes(utf32::pcrune _str)
    {
        crunes_t c;
        c.m_utf32         = _str;
        c.m_str           = 0;
        utf32::pcrune end = utf::endof(_str, nullptr);
        c.m_end           = (u32)(end - _str);
        c.m_eos           = c.m_end;
        c.m_type          = utf32::TYPE;
        return c;
    }

    crunes_t make_crunes(utf32::pcrune _str, utf32::pcrune _end)
    {
        crunes_t c;
        c.m_utf32 = _str;
        c.m_str   = 0;
        c.m_eos   = (u32)(_end - _str);
        c.m_end   = (u32)(_end - _str);
        c.m_type  = utf32::TYPE;
        return c;
    }

    crunes_t make_crunes(utf32::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        crunes_t c;
        c.m_utf32 = _bos;
        c.m_str   = _str;
        c.m_end   = _end;
        c.m_eos   = _eos;
        c.m_type  = utf32::TYPE;
        return c;
    }

    namespace nrunes
    {
        // ------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------
        reader_t::reader_t() {}

        reader_t::reader_t(ascii::pcrune str)
        {
            ascii::pcrune str_end = str;
            while (*str_end != '\0')
                str_end++;
            u32 const len = (u32)(str_end - str);
            m_runes       = make_crunes(str, 0, len, len);
            m_cursor      = 0;
        }
        reader_t::reader_t(ascii::pcrune str, u32 len)
        {
            m_runes  = make_crunes(str, 0, len, len);
            m_cursor = 0;
        }
        reader_t::reader_t(ascii::pcrune str, ascii::pcrune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = make_crunes(str, 0, len, len);
            m_cursor      = 0;
        }
        reader_t::reader_t(utf8::pcrune str, utf8::pcrune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = make_crunes(str, 0, len, len);
            m_cursor      = 0;
        }
        reader_t::reader_t(utf16::pcrune str, utf16::pcrune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = make_crunes(str, 0, len, len);
            m_cursor      = 0;
        }
        reader_t::reader_t(utf32::pcrune str, utf32::pcrune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = make_crunes(str, 0, len, len);
            m_cursor      = 0;
        }
        reader_t::reader_t(crunes_t const& runes)
            : m_runes(runes)
        {
            m_cursor = m_runes.m_str;
        }

        crunes_t reader_t::get_source() const
        {
            crunes_t str(m_runes);
            return str;
        }

        crunes_t reader_t::get_current() const
        {
            crunes_t str(m_runes);
            str.m_str = m_cursor;
            return str;
        }

        bool reader_t::at_end() const { return m_cursor >= m_runes.m_end; }
        bool reader_t::vvalid() const { return m_cursor >= m_runes.m_str && m_cursor < m_runes.m_end; }

        void reader_t::vreset() { m_cursor = m_runes.m_str; }

        bool reader_t::vpeek(s32 n, uchar32& c) const
        {
            if (!at_end() && n == 0)
            {
                u32 next = m_cursor;
                switch (m_runes.m_type)
                {
                    case ascii::TYPE: c = m_runes.m_ascii[m_cursor] & 0xff; return true;
                    case ucs2::TYPE: c = utf::reading::read_forward(m_runes.m_ucs2, next, m_runes.m_end); return true;
                    case utf8::TYPE: c = utf::reading::read_forward(m_runes.m_utf8, next, m_runes.m_end); return true;
                    case utf16::TYPE: c = utf::reading::read_forward(m_runes.m_utf16, next, m_runes.m_end); return true;
                    case utf32::TYPE: c = m_runes.m_utf32[m_cursor].r; return true;
                }
            }
            else
            {
                // n > 0
                // TODO IMPLEMENTATION
            }

            c = '\0';
            return false;
        }

        bool reader_t::vread(uchar32& c)
        {
            if (!at_end())
            {
                switch (m_runes.m_type)
                {
                    case ascii::TYPE: c = m_runes.m_ascii[m_cursor++] & 0xff; break;
                    case ucs2::TYPE: c = utf::reading::read_forward(m_runes.m_ucs2, m_cursor, m_runes.m_end); break;
                    case utf8::TYPE: c = utf::reading::read_forward(m_runes.m_utf8, m_cursor, m_runes.m_end); break;
                    case utf16::TYPE: c = utf::reading::read_forward(m_runes.m_utf16, m_cursor, m_runes.m_end); break;
                    case utf32::TYPE: c = m_runes.m_utf32[m_cursor++].r; break;
                    default: ASSERT(false); break;
                }
                return true;
            }
            c = '\0';
            return false;
        }

        bool reader_t::vread_line(runes_t& line)
        {
            // TODO IMPLEMENTATION
            return false;
        }

        bool reader_t::vview_line(crunes_t& line)
        {
            if (at_end())
                return false;

            u32 next    = m_cursor;
            line.m_type = m_runes.m_type;
            switch (m_runes.m_type)
            {
                case ascii::TYPE:
                    line.m_ascii = m_runes.m_ascii;
                    line.m_str   = m_cursor;
                    line.m_eos   = m_runes.m_eos;
                    line.m_end   = m_runes.m_end;
                    while (!at_end() && line.m_ascii[m_cursor] != '\n')
                        m_cursor++;
                    if (!at_end())
                        m_cursor++;
                    line.m_end = m_cursor;
                    break;
                case ucs2::TYPE:
                    line.m_ucs2 = m_runes.m_ucs2;
                    line.m_str  = m_cursor;
                    line.m_eos  = m_runes.m_eos;
                    line.m_end  = m_runes.m_end;
                    while (!at_end() && utf::reading::read_forward(m_runes.m_ucs2, next, m_runes.m_end) != '\n')
                        m_cursor = next;
                    if (!at_end())
                        utf::reading::read_forward(m_runes.m_ucs2, m_cursor, m_runes.m_end);
                    line.m_end = m_cursor;
                    break;
                case utf8::TYPE:
                    line.m_utf8 = m_runes.m_utf8;
                    line.m_str  = m_cursor;
                    line.m_eos  = m_runes.m_eos;
                    line.m_end  = m_runes.m_end;
                    while (!at_end() && utf::reading::read_forward(m_runes.m_utf8, next, m_runes.m_end) != '\n')
                        m_cursor = next;
                    if (!at_end())
                        utf::reading::read_forward(m_runes.m_utf8, m_cursor, m_runes.m_end);
                    line.m_end = m_cursor;
                    break;
                case utf16::TYPE:
                    line.m_utf16 = m_runes.m_utf16;
                    line.m_str   = m_cursor;
                    line.m_eos   = m_runes.m_eos;
                    line.m_end   = m_runes.m_end;
                    while (!at_end() && utf::reading::read_forward(m_runes.m_utf16, next, m_runes.m_end) != '\n')
                        m_cursor = next;
                    if (!at_end())
                        utf::reading::read_forward(m_runes.m_utf16, m_cursor, m_runes.m_end);
                    line.m_end = m_cursor;
                    break;
                case utf32::TYPE:
                    line.m_utf32 = m_runes.m_utf32;
                    line.m_str   = m_cursor;
                    line.m_eos   = m_runes.m_eos;
                    line.m_end   = m_runes.m_end;
                    while (!at_end() && line.m_utf32[m_cursor].r != '\n')
                        m_cursor++;
                    if (!at_end())
                        m_cursor++;
                    line.m_end = m_cursor;
                    break;
                default: ASSERT(false); break;
            }

            return true;
        }

        void reader_t::vskip(s32 c)
        {
            switch (m_runes.m_type)
            {
                case ascii::TYPE:
                    while (c > 0 && !at_end())
                    {
                        m_cursor += 1;
                        c -= 1;
                    }
                    break;

                case ucs2::TYPE:
                    while (c > 0 && !at_end())
                    {
                        m_cursor += 1;
                        c -= 1;
                    }
                    break;

                case utf8::TYPE: utf::skip_forward(m_runes.m_utf8, m_cursor, m_runes.m_str, m_runes.m_end, c); break;
                case utf16::TYPE: utf::skip_forward(m_runes.m_utf16, m_cursor, m_runes.m_str, m_runes.m_end, c); break;

                case utf32::TYPE:
                    while (c > 0 && !at_end())
                    {
                        m_cursor += 1;
                        c -= 1;
                    }
                    break;
                default: ASSERT(false); break;
            }
        }

        inline static bool s_contains_char(uchar32 const* chars, u32 num_chars, uchar32 c)
        {
            for (u32 i = 0; i < num_chars; i++)
            {
                if (chars[i] == c)
                    return true;
            }
            return false;
        }

        s32 reader_t::vskip_any(uchar32 const* chars, u32 num_chars)
        {
            s32     skipped = -1;
            uchar32 c       = 0;
            switch (m_runes.m_type)
            {
                case ascii::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        skipped += 1;
                        uchar32 c = m_runes.m_ascii[m_cursor] & 0xff;
                        if (!s_contains_char(chars, num_chars, c))
                            break;
                        m_cursor += 1;
                    }
                    break;
                case ucs2::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        u32 next = m_cursor;
                        skipped += 1;
                        c = utf::reading::read_forward(m_runes.m_ucs2, next, m_runes.m_end);
                        if (!s_contains_char(chars, num_chars, c))
                            break;
                        m_cursor += 1;
                    }
                    break;
                case utf8::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        u32 next = m_cursor;
                        skipped += 1;
                        c = utf::reading::read_forward(m_runes.m_utf8, next, m_runes.m_end);
                        if (!s_contains_char(chars, num_chars, c))
                            break;
                        m_cursor = next;
                    }
                    break;
                case utf16::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        u32 next = m_cursor;
                        skipped += 1;
                        c = utf::reading::read_forward(m_runes.m_utf16, next, m_runes.m_end);
                        if (!s_contains_char(chars, num_chars, c))
                            break;
                        m_cursor = next;
                    }
                    break;
                case utf32::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        skipped += 1;
                        c = m_runes.m_utf32[m_cursor].r;
                        if (!s_contains_char(chars, num_chars, c))
                            break;
                        m_cursor += 1;
                    }
                    break;
                default: ASSERT(false); break;
            }
            return skipped;
        }

        s32 reader_t::vskip_until_one_of(uchar32 const* chars, u32 num_chars)
        {
            s32     skipped = -1;
            uchar32 c       = 0;
            switch (m_runes.m_type)
            {
                case ascii::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        skipped += 1;
                        uchar32 c = m_runes.m_ascii[m_cursor] & 0xff;
                        if (s_contains_char(chars, num_chars, c))
                            return skipped;
                        m_cursor += 1;
                    }
                    break;
                case ucs2::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        u32 next = m_cursor;
                        skipped += 1;
                        c = utf::reading::read_forward(m_runes.m_ucs2, next, m_runes.m_end);
                        if (s_contains_char(chars, num_chars, c))
                            return skipped;
                        m_cursor = next;
                    }
                    break;
                case utf8::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        u32 next = m_cursor;
                        skipped += 1;
                        c = utf::reading::read_forward(m_runes.m_utf8, next, m_runes.m_end);
                        if (s_contains_char(chars, num_chars, c))
                            return skipped;
                        m_cursor = next;
                    }
                    break;
                case utf16::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        u32 next = m_cursor;
                        skipped += 1;
                        c = utf::reading::read_forward(m_runes.m_utf16, next, m_runes.m_end);
                        if (s_contains_char(chars, num_chars, c))
                            return skipped;
                        m_cursor = next;
                    }
                    break;
                case utf32::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        skipped += 1;
                        c = m_runes.m_utf32[m_cursor].r;
                        if (s_contains_char(chars, num_chars, c))
                            return skipped;
                        m_cursor += 1;
                    }
                    break;
                default: ASSERT(false); break;
            }
            return -1;
        }

        reader_t reader_t::select(u32 const& from, u32 to) const
        {
            reader_t reader;
            if (from >= reader.m_runes.m_str && to <= reader.m_runes.m_eos)
            {
                reader.m_runes.m_ascii = m_runes.m_ascii;
                reader.m_runes.m_eos   = m_runes.m_eos;
                reader.m_runes.m_str   = from;
                reader.m_runes.m_end   = to;
                reader.m_runes.m_type  = m_runes.m_type;
                reader.m_cursor        = from;
            }
            return reader;
        }

        writer_t::writer_t(ascii::prune str, ascii::prune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = make_runes(str, 0, len, len, ascii::TYPE);
            m_cursor      = 0;
            m_count       = 0;
        }
        writer_t::writer_t(ucs2::prune str, ucs2::prune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = make_runes(str, 0, len, len, ucs2::TYPE);
            m_cursor      = 0;
            m_count       = 0;
        }
        writer_t::writer_t(utf8::prune str, utf8::prune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = make_runes(str, 0, len, len, utf8::TYPE);
            m_cursor      = 0;
            m_count       = 0;
        }
        writer_t::writer_t(utf16::prune str, utf16::prune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = make_runes(str, 0, len, len, utf16::TYPE);
            m_cursor      = 0;
            m_count       = 0;
        }
        writer_t::writer_t(utf32::prune str, utf32::prune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = make_runes(str, 0, len, len, utf32::TYPE);
            m_cursor      = 0;
            m_count       = 0;
        }
        writer_t::writer_t(runes_t const& runes)
            : m_runes(runes)
        {
            m_cursor = m_runes.m_str;
            m_count  = 0;
        }

        runes_t writer_t::get_destination() const
        {
            runes_t str(m_runes);
            return str;
        }

        runes_t writer_t::get_current() const
        {
            runes_t str(m_runes);
            str.m_end = m_cursor;
            return str;
        }

        void writer_t::reset()
        {
            m_cursor = m_runes.m_str;
            m_count  = 0;
        }

        s32 writer_t::count() const { return m_count; }

        static bool at_end(u32 p, runes_t const& str) { return p >= str.m_eos; }

        s32 writer_t::vwrite(uchar32 c)
        {
            utf32::rune str[2] = {c, 0};
            crunes_t    cstr   = make_crunes(str, 0, 1, 1);
            return vwrite(cstr);
        }

        s32 writer_t::vwrite(const char* str)
        {
            uchar32 c;
            s32     count = m_count;
            switch (m_runes.m_type)
            {
                case ascii::TYPE:
                    while (*str != '\0' && !at_end(m_cursor, m_runes))
                    {
                        m_runes.m_ascii[m_cursor++] = (ascii::rune)*str;
                        m_count += 1;
                        str++;
                    }
                case ucs2::TYPE:
                    while (*str != '\0' && !at_end(m_cursor, m_runes))
                    {
                        m_runes.m_ucs2[m_cursor++].r = *str;
                        m_count += 1;
                        str++;
                    }
                case utf8::TYPE:
                    while (*str != '\0' && !at_end(m_cursor, m_runes))
                    {
                        m_runes.m_utf8[m_cursor++].r = *str;
                        m_count += 1;
                        str++;
                    }
                case utf16::TYPE:
                    while (*str != '\0' && !at_end(m_cursor, m_runes))
                    {
                        m_runes.m_utf16[m_cursor++].r = *str;
                        m_count += 1;
                        str++;
                    }
                case utf32::TYPE:
                    while (*str != '\0' && !at_end(m_cursor, m_runes))
                    {
                        m_runes.m_utf32[m_cursor].r = *str;
                        m_cursor += 1;
                        m_count += 1;
                        str++;
                    }
                default: ASSERT(false); break;
            }
            return m_count - count;
        }

        s32 writer_t::vwrite(const char* str, const char* end)
        {
            crunes_t cstr = make_crunes(str, end);
            return vwrite(cstr);
        }

        s32 writer_t::vwrite(crunes_t const& str)
        {
            if (!at_end(m_cursor, m_runes))
            {
                uchar32  c;
                reader_t reader(str);
                s32      count = m_count;
                switch (m_runes.m_type)
                {
                    case ascii::TYPE:
                        while (reader.read(c) && !at_end(m_cursor, m_runes))
                        {
                            m_runes.m_ascii[m_cursor++] = (ascii::rune)c;
                            m_count += 1;
                        }
                        return m_count - count;
                    case ucs2::TYPE:
                        while (reader.read(c) && !at_end(m_cursor, m_runes))
                        {
                            m_runes.m_ucs2[m_cursor++].r = c;
                            m_count += 1;
                        }
                    case utf8::TYPE:
                        while (reader.read(c) && utf::write(c, m_runes.m_utf8, m_cursor, m_runes.m_eos))
                        {
                            m_count += 1;
                        }
                    case utf16::TYPE:
                        while (reader.read(c) && utf::write(c, m_runes.m_utf16, m_cursor, m_runes.m_eos))
                        {
                            m_count += 1;
                        }
                    case utf32::TYPE:
                        while (reader.read(c) && !at_end(m_cursor, m_runes))
                        {
                            m_runes.m_utf32[m_cursor].r = c;
                            m_cursor += 1;
                            m_count += 1;
                        }
                    default: ASSERT(false); break;
                }
                return m_count - count;
            }
            return 0;
        }

        void writer_t::vflush() {}

    }  // namespace nrunes
}  // namespace ncore
