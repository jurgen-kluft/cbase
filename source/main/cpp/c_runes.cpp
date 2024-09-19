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
                if (c.value < 0x80)
                    return 1;  // 0xxxxxxx
                else if (c.value < 0xc0)
                    return -1;  // 10xxxxxx -- in the middle of a multibyte character, counts as one invalid character.
                else if (c.value < 0xe0)
                    return 2;  // 110xxxxx
                else if (c.value < 0xf0)
                    return 3;  // 1110xxxx
                else if (c.value < 0xf8)
                    return 4;  // 11110xxx
                else if (c.value < 0xfc)
                    return 5;  // 111110xx
                else if (c.value < 0xfe)
                    return 6;  // 1111110x
                return -1;     // invalid sequence.  dunno how many bytes to skip so skip one.
            }

            static inline s8 utf16_octet_count(utf16::rune c)
            {
                if (c.value < 0xd800)
                    return 1;
                else if (c.value < 0xdc00)
                    return 2;
                return 1;
            }

            // read

            // Read one ascii character from an ascii string and return it as a utf-32 character.
            static inline uchar32 read(ascii::pcrune bos, u32& str, u32 const end)
            {
                if (str < end)
                    return (uchar32)bos[str++];
                return ascii::TERMINATOR;
            }

            // Read one ucs-2 character from a ucs-2 string and return it as a utf-32 character.
            static inline uchar32 read(ucs2::pcrune bos, u32& str, u32 const end)
            {
                if (str < end)
                    return (uchar32)bos[str++].value;
                return ucs2::TERMINATOR.value;
            }

            // Mask can be computed as (0x7f >> leadingOnes)
            // Size based on the number of leading 1 bits
            static const u32 c_utf8_sizes = (0 << 28) | (6 << 24) | (5 << 20) | (4 << 16) | (3 << 12) | (2 << 8) | (0 << 4) | 1;
            static const u32 c_minChar[]  = {0, 0x00000000, 0x00000080, 0x00000800, 0x00010000, 0x00200000, 0x04000000};

            // Read one utf-8 character from a utf-8 string and return it as a utf-32 character.
            static uchar32 read(utf8::pcrune data, u32& str, u32 const end)
            {
                data = data + str;

                // compute character encoding length, checking for overlong sequences (i.e. characters that don't use the shortest possible encoding).
                u8       value       = data[0].value;
                s8 const leadingOnes = math::countLeadingZeros((u8)(~value));
                u8 const size        = (c_utf8_sizes >> (leadingOnes << 2)) & 0xF;

                if (size == 0)
                {
                    ++str;
                    return s_invalid;
                }

                // make sure we have enough data
                if ((str + size) > end)
                {
                    str = end;
                    return s_invalid;
                }

                u32 c = value & ((u32)0x7f >> leadingOnes);
                for (u32 i = 1; i < size; i++)
                {
                    value = data[i].value;
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
            static inline uchar32 read(utf16::pcrune bos, u32& str, u32 const end)
            {
                if (str >= end)
                    return cEOS;

                uchar16 const c = bos[str].value;
                u32 const     l = (c < 0xd800) ? 1 : (c < 0xdc00) ? 2 : 0;

                if ((str + l) <= end)
                {
                    uchar32 out_c = 0;
                    for (u32 i = 0; i < l; i++)
                        out_c = (out_c << 16) | bos[str + i].value;
                    str += l;
                    return out_c;
                }
                str = end;
                return '\0';
            }

            // Read one utf-32 character from a utf-32 string and return it as a utf-32 character.
            static inline uchar32 read(utf32::pcrune bos, u32& str, u32 const end)
            {
                if (str >= end)
                    return cEOS;
                uchar32 c = bos[str].value;
                if (c != cEOS)
                    str++;
                return c;
            }
        }  // namespace reading

        static inline uchar32 read2(crunes_t const& str, u32& cursor)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return reading::read(str.m_ascii, cursor, str.m_end);
                case ucs2::TYPE: return reading::read(str.m_ucs2, cursor, str.m_end);
                case utf8::TYPE: return reading::read(str.m_utf8, cursor, str.m_end);
                case utf16::TYPE: return reading::read(str.m_utf16, cursor, str.m_end);
                case utf32::TYPE: return reading::read(str.m_utf32, cursor, str.m_end);
            }
            return cEOS;
        }

        static inline uchar32 read2(runes_t const& str, u32& cursor)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return reading::read(str.m_ascii, cursor, str.m_end);
                case ucs2::TYPE: return reading::read(str.m_ucs2, cursor, str.m_end);
                case utf8::TYPE: return reading::read(str.m_utf8, cursor, str.m_end);
                case utf16::TYPE: return reading::read(str.m_utf16, cursor, str.m_end);
                case utf32::TYPE: return reading::read(str.m_utf32, cursor, str.m_end);
            }
            return cEOS;
        }

        static s32 skip(ascii::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            if (count < 0)
            {
                u32 const n = (u32)-count;
                cursor -= n;
                if (cursor < begin)
                {
                    count  = (s32)begin - (s32)(cursor + n);
                    cursor = begin;
                }
            }
            else
            {
                cursor += (u32)count;
                if (cursor >= end)
                {
                    count  = (s32)end - (s32)(cursor - (u32)count);
                    cursor = end;
                }
            }
            return count;
        }

        static s32 skip(utf8::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            ASSERT(cursor >= begin);
            ASSERT(cursor <= end);

            // All UTF-8 sequences:
            // - EITHER consist of exactly one octet (byte to you and me) with the top bit clear
            // - OR consist of one octet with the two topmost bits set, followed by one or more octets with bit 7 set and bit 6 clear.
            if (count < 0)
            {
                utf8::pcrune ptr = str + cursor;
                s32          c   = count;
                while (cursor > begin && c < 0)
                {
                    ptr--;
                    cursor--;
                    c++;
                    while ((cursor > begin) && ((*ptr).value & 0xC0) == 0x80)
                    {
                        ptr--;
                        cursor--;
                    }
                }
                return count - c;
            }
            else
            {
                utf8::pcrune ptr = str + cursor;
                s32          c   = count;
                while (c > 0 && (cursor < end))
                {
                    if (((*ptr).value & 0xC0) == 0xC0)
                    {
                        ptr++;
                        c--;
                        while ((cursor < end) && ((*ptr).value & 0xC0) == 0x80)
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
        }

        static s32 skip(utf16::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            ASSERT(cursor >= begin);
            ASSERT(cursor <= end);

            if (count < 0)
            {
                // UTF-16 backwards iteration can be quite slow, since we need to iterate forwards
                u32       forward_cursor = begin;
                const s32 len            = skip(str, forward_cursor, begin, cursor, /* good enough estimate */ (s32)(cursor - begin));
                if (len == 0)
                    return 0;

                // Do it again but now stop at (len + count) (count is negative)
                cursor = begin;
                return skip(str, cursor, begin, cursor, len + count);
            }
            else
            {
                s32         skipcount = 0;
                utf16::rune c         = str[cursor];
                while (!c.isEOS() && (cursor < end) && count > 0)
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
        }

        static s32 skip(utf32::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            if (count < 0)
            {
                u32 const n = (u32)(-count);
                cursor -= n;
                if (cursor < begin)
                {
                    count  = (s32)(cursor + n) - (s32)begin;
                    cursor = begin;
                }
            }
            else
            {
                cursor += (u32)count;
                if (cursor >= end)
                {
                    count  = (s32)end - (s32)(cursor - (u32)count);
                    cursor = end;
                }
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
                while (!end->isEOS())
                    end++;
            }
            else
            {
                while (end < eos && !end->isEOS())
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
                while (!end->isEOS())
                {
                    if (((*end).value & 0xC0) == 0xC0)
                    {
                        end++;
                        while (((*end).value & 0xC0) == 0x80)
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
                while (end < eos && !end->isEOS())
                {
                    if (((*end).value & 0xC0) == 0xC0)
                    {
                        end++;
                        while ((end < eos) && ((*end).value & 0xC0) == 0x80)
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
                while (*end != utf16::TERMINATOR)
                {
                    utf16::rune c = *end;
                    u32 const   l = reading::utf16_octet_count(c);
                    end += l;
                }
            }
            else
            {
                while (*end != utf16::TERMINATOR && end < eos)
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
                while (!end->isEOS())
                    end++;
            }
            else
            {
                while (end < eos && !end->isEOS())
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
                    bos[cursor++].value = (uchar16)c;
                }
                else
                {
                    bos[cursor++].value = (uchar16)s_replacement;
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
                    bos[cursor++].value = static_cast<uchar8>(cp);
                    return true;
                }
            }
            else if (cp < 0x800)
            {  // two octets
                if ((cursor + 1) < end)
                {
                    bos[cursor++].value = static_cast<uchar8>((cp >> 6) | 0xc0);
                    bos[cursor++].value = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            else if (cp < 0x10000)
            {  // three octets
                if ((cursor + 2) < end)
                {
                    bos[cursor++].value = static_cast<uchar8>((cp >> 12) | 0xe0);
                    bos[cursor++].value = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
                    bos[cursor++].value = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            else
            {  // four octets
                if ((cursor + 3) < end)
                {
                    bos[cursor++].value = static_cast<uchar8>((cp >> 18) | 0xf0);
                    bos[cursor++].value = static_cast<uchar8>(((cp >> 12) & 0x3f) | 0x80);
                    bos[cursor++].value = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
                    bos[cursor++].value = static_cast<uchar8>((cp & 0x3f) | 0x80);
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
                    bos[cursor++].value = (uchar16)rune;
                    return true;
                }
                else if ((cursor + 1) < end)
                {
                    // 20-bit intermediate value
                    u32 const iv        = rune - 0x10000;
                    bos[cursor++].value = static_cast<uchar16>((iv >> 10) + 0xd800);
                    bos[cursor++].value = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
                    return true;
                }
            }
            return false;
        }

        static bool write(uchar32 c, utf32::prune bos, u32& cursor, u32 end)
        {
            if (cursor < end)
            {
                bos[cursor++].value = c;
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
                uchar32 c1 = utf::reading::read(str1, cursor1, len1);
                uchar32 c2 = utf::reading::read(str2, cursor2, len2);
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
                uchar32 c1 = utf::reading::read(str1, cursor1, len1);
                uchar32 c2 = utf::reading::read(str2, cursor2, len2);
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
                while (!end->isEOS())
                {
                    end++;
                    chars++;
                }
            }
            else
            {
                while (end < eos && !end->isEOS())
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

        static inline runes_t  nothing_found(runes_t const& str) { return runes_t(str.m_ascii, str.m_str, str.m_str, str.m_eos, str.get_type()); }
        static inline crunes_t nothing_found(crunes_t const& str) { return crunes_t(str.m_ascii, str.m_str, str.m_str, str.m_eos, str.get_type()); }

        // Readers

        class crreader_t
        {
            crunes_t m_str;
            u32      m_cursor;

        public:
            crreader_t(crunes_t const& _str)
                : m_str(_str)
                , m_cursor(get_begin(_str))
            {
            }

            inline uchar32 peek(u32& next_cursor) const
            {
                next_cursor = m_cursor;
                switch (m_str.get_type())
                {
                    case ascii::TYPE: return utf::reading::read(m_str.m_ascii, next_cursor, m_str.m_end);
                    case utf8::TYPE: return utf::reading::read(m_str.m_utf8, next_cursor, m_str.m_end);
                    case utf16::TYPE: return utf::reading::read(m_str.m_utf16, next_cursor, m_str.m_end);
                    case utf32::TYPE: return utf::reading::read(m_str.m_utf32, next_cursor, m_str.m_end);
                }
                return '\0';
            }

            inline uchar32 read()
            {
                switch (m_str.get_type())
                {
                    case ascii::TYPE: return utf::reading::read(m_str.m_ascii, m_cursor, m_str.m_end);
                    case utf8::TYPE: return utf::reading::read(m_str.m_utf8, m_cursor, m_str.m_end);
                    case utf16::TYPE: return utf::reading::read(m_str.m_utf16, m_cursor, m_str.m_end);
                    case utf32::TYPE: return utf::reading::read(m_str.m_utf32, m_cursor, m_str.m_end);
                }
                return '\0';
            }
            inline bool is_end() const { return m_cursor >= get_end(m_str); }
            inline void set_cursor(u32 cursor) { m_cursor = cursor; }

            inline crunes_t select_until() const { return select(m_str, get_begin(m_str), m_cursor); }
            inline crunes_t select_after() const { return select(m_str, m_cursor + 1, get_end(m_str)); }
            inline crunes_t select_current() const { return select(m_str, m_cursor, get_end(m_str)); }
        };

        class rreader_t
        {
            runes_t m_str;
            u32     m_cursor;

        public:
            rreader_t(runes_t const& _str)
                : m_str(_str)
                , m_cursor(get_begin(_str))
            {
            }
            inline uchar32 peek(u32& next_cursor) const
            {
                next_cursor = m_cursor;
                switch (m_str.get_type())
                {
                    case ascii::TYPE: return utf::reading::read(m_str.m_ascii, next_cursor, m_str.m_end);
                    case utf8::TYPE: return utf::reading::read(m_str.m_utf8, next_cursor, m_str.m_end);
                    case utf16::TYPE: return utf::reading::read(m_str.m_utf16, next_cursor, m_str.m_end);
                    case utf32::TYPE: return utf::reading::read(m_str.m_utf32, next_cursor, m_str.m_end);
                }
                return '\0';
            }

            inline uchar32 read()
            {
                switch (m_str.get_type())
                {
                    case ascii::TYPE: return utf::reading::read(m_str.m_ascii, m_cursor, m_str.m_end);
                    case utf8::TYPE: return utf::reading::read(m_str.m_utf8, m_cursor, m_str.m_end);
                    case utf16::TYPE: return utf::reading::read(m_str.m_utf16, m_cursor, m_str.m_end);
                    case utf32::TYPE: return utf::reading::read(m_str.m_utf32, m_cursor, m_str.m_end);
                }
                return '\0';
            }
            inline bool is_end() const { return m_cursor >= get_end(m_str); }
            inline void set_cursor(u32 cursor) { m_cursor = cursor; }

            inline runes_t select_until() const { return select(m_str, get_begin(m_str), m_cursor); }
            inline runes_t select_after() const { return select(m_str, m_cursor + 1, get_end(m_str)); }
            inline runes_t select_current() const { return select(m_str, m_cursor, get_end(m_str)); }
        };

        // Find

        crunes_t find(crunes_t const& _str, uchar32 _c, bool _casesensitive)
        {
            crreader_t reader(_str);
            while (!reader.is_end())
            {
                u32           next_cursor;
                uchar32 const c = reader.peek(next_cursor);
                if (_casesensitive)
                {
                    if (c == _c)
                        return reader.select_current();
                }
                else
                {
                    if (is_equalfold(c, _c))
                        return reader.select_current();
                }
                reader.set_cursor(next_cursor);
            }

            return nothing_found(_str);
        }

        runes_t find(runes_t const& _str, uchar32 _c, bool _casesensitive)
        {
            crunes_t str(_str);
            crunes_t csel = find(str, _c, _casesensitive);
            return crunes_to_runes(_str, csel);
        }

        crunes_t findLast(crunes_t const& _str, uchar32 _c, bool _casesensitive)
        {
            u32 iter  = get_end(_str);
            u32 begin = get_begin(_str);
            while (iter > begin)
            {
                u32 end = iter;
                backwards(_str, iter);

                u32        next;
                uchar32    c     = _str.peek(iter, next);
                bool const equal = _casesensitive ? is_equal(c, _c) : is_equalfold(c, _c);
                if (equal)
                {
                    return select(_str, iter, end);
                }
            }
            return nothing_found(_str);
        }

        runes_t findLast(runes_t const& _str, uchar32 _c, bool _casesensitive)
        {
            crunes_t str(_str);
            crunes_t csel = findLast(str, _c, _casesensitive);
            return crunes_to_runes(_str, csel);
        }

        crunes_t find(crunes_t const& _str, crunes_t const& _find, bool _casesensitive)
        {
            if (_str.is_empty() || _find.is_empty())
                return nothing_found(_str);
            if (_find.size() > _str.size())
                return nothing_found(_str);

            u32 iterb = get_begin(_str);
            u32 itere = iterb;
            forwards(_str, itere, (s32)_find.size());
            u32 const end = get_end(_str);
            while (itere <= end)
            {
                crunes_t sel = select(_str, iterb, itere);
                if (compare(sel, _find, _casesensitive) == 0)
                {
                    return sel;
                }
                forwards(_str, iterb, 1);
                if (!forwards(_str, itere, 1))
                    break;
            }
            return nothing_found(_str);
        }

        runes_t find(runes_t const& _str, crunes_t const& _find, bool _casesensitive)
        {
            crunes_t str(_str);
            crunes_t csel = find(str, _find, _casesensitive);
            if (!csel.is_empty())
            {
                runes_t sel = crunes_to_runes(_str, csel);
                return sel;
            }
            return nothing_found(_str);
        }

        crunes_t findLast(crunes_t const& _str, crunes_t const& _find, bool _casesensitive)
        {
            if (_str.is_empty() || _find.is_empty())
                return nothing_found(_str);
            if (_find.size() > _str.size())
                return nothing_found(_str);

            u32 begin = get_begin(_str);
            u32 itere = get_end(_str);
            u32 iterb = itere;
            backwards(_str, iterb, (s32)_find.size());
            while (iterb >= begin)
            {
                crunes_t  sel = select(_str, iterb, itere);
                s32 const cmp = compare(sel, _find, _casesensitive);
                if (cmp == 0)
                {
                    return sel;
                }
                if (!backwards(_str, iterb, 1))
                    break;
                backwards(_str, itere, 1);
            }
            return nothing_found(_str);
        }

        runes_t findLast(runes_t const& _str, crunes_t const& _find, bool _casesensitive)
        {
            crunes_t str(_str);
            crunes_t csel = findLast(str, _find, _casesensitive);
            if (!csel.is_empty())
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
                u32     next = iter;
                uchar32 c    = _str.peek(iter, next);

                // See if this char can be found in the 'set'
                crunes_t found = find(_any, c, _casesensitive);
                if (!found.is_empty())
                    return select(_str, iter, next);

                iter = next;
            }
            return nothing_found(_str);
        }

        runes_t findOneOf(runes_t const& _str, crunes_t const& _any, bool _casesensitive)
        {
            crunes_t str(_str);
            crunes_t csel = findOneOf(str, _any, _casesensitive);
            if (!csel.is_empty())
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
            crreader_t reader(_str);
            if (_case_sensitive)
            {
                while (!reader.is_end())
                {
                    uchar32 const c = reader.read();
                    if (c == _c)
                        return true;
                }
            }
            else
            {
                while (!reader.is_end())
                {
                    uchar32 const c = reader.read();
                    if (is_equalfold(c, _c))
                        return true;
                }
            }
            return false;
        }

        bool contains(runes_t const& _str, uchar32 _c, bool case_sensitive)
        {
            rreader_t reader(_str);
            if (case_sensitive)
            {
                while (!reader.is_end())
                {
                    uchar32 const c = reader.read();
                    if (c == _c)
                        return true;
                }
            }
            else
            {
                while (!reader.is_end())
                {
                    uchar32 const c = reader.read();
                    if (is_equalfold(c, _c))
                        return true;
                }
            }
            return false;
        }

        // ------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------
        crunes_t findSelectUntil(const crunes_t& _str, uchar32 _c, bool _casesensitive)
        {
            crreader_t reader(_str);
            while (!reader.is_end())
            {
                uchar32 const c = reader.read();
                if (_casesensitive ? is_equal(c, _c) : is_equalfold(c, _c))
                {
                    return reader.select_until();
                }
            }
            return nothing_found(_str);
        }

        crunes_t findSelectUntil(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t sel = find(_str, _find, _casesensitive);
            if (sel.is_empty())
                return nothing_found(_str);
            return crunes_t(_str.m_ascii, _str.m_str, sel.m_str, _str.m_eos, _str.get_type());
        }

        crunes_t findLastSelectUntil(const crunes_t& _str, uchar32 _c, bool _casesensitive)
        {
            utf32::rune dst_runes[2];
            dst_runes[0] = {_c};
            dst_runes[1] = {0};
            crunes_t _find(dst_runes, 0, 1, 1);
            return findLastSelectUntil(_str, _find, _casesensitive);
        }

        crunes_t findLastSelectUntil(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t sel = findLast(_str, _find, _casesensitive);
            if (sel.is_empty())
                return nothing_found(_str);
            return crunes_t(_str.m_ascii, _str.m_str, sel.m_str, _str.m_eos, _str.get_type());
        }

        crunes_t findSelectUntilIncluded(const crunes_t& _str, uchar32 _c, bool _casesensitive)
        {
            utf32::rune dst_runes[2];
            dst_runes[0] = {_c};
            dst_runes[1] = {0};
            crunes_t _find(dst_runes, 0, 1, 1);
            return findSelectUntilIncluded(_str, _find, _casesensitive);
        }

        crunes_t findSelectUntilIncluded(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t sel = find(_str, _find, _casesensitive);
            if (sel.is_empty())
                return nothing_found(_str);
            return crunes_t(_str.m_ascii, _str.m_str, sel.m_end, _str.m_eos, _str.get_type());
        }

        crunes_t findSelectUntilIncludedAbortAtOneOf(const crunes_t& inStr, const crunes_t& inFind, const crunes_t& inAbortAnyOneOf, bool case_sensitive)
        {
            u32       iter = get_begin(inStr);
            u32 const end  = get_end(inStr);
            while (iter < end)
            {
                // search inFind at the current position
                if (starts_with(inStr, inFind))
                    return select(inStr, iter, iter + inFind.size());  // TODO, this is incorrect (size is in runes, not bytes, and if the types are different, this will not work)

                u32           next;
                uchar32 const c = inStr.peek(iter, next);

                // See if this char can be found in the 'set'
                if (contains(inAbortAnyOneOf, c, case_sensitive))
                    break;

                iter = next;
            }
            return nothing_found(inStr);
        }

        crunes_t findLastSelectUntilIncluded(const crunes_t& _str, uchar32 _c, bool _casesensitive)
        {
            utf32::rune dst_runes[2];
            dst_runes[0] = {_c};
            dst_runes[1] = {0};
            crunes_t _find(dst_runes, 0, 1, 1);
            return findLastSelectUntilIncluded(_str, _find, _casesensitive);
        }

        crunes_t findLastSelectUntilIncluded(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t sel = findLast(_str, _find, _casesensitive);
            if (sel.is_empty())
                return sel;
            return crunes_t(_str.m_ascii, _str.m_str, sel.m_end, _str.m_eos, _str.get_type());
        }

        crunes_t selectAfterSelection(const crunes_t& _str, const crunes_t& _sel)
        {
            if (!_sel.is_empty())
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

        crunes_t selectFromToInclude(const crunes_t& inStr, crunes_t const& from, crunes_t const& to)
        {
            if (from.is_empty() || to.is_empty())
                return nothing_found(inStr);
            if (!is_in(inStr, from) || !is_in(inStr, to))
                return nothing_found(inStr);
            return select(inStr, from.m_str, to.m_end);
        }

        crunes_t selectBetween(const crunes_t& _str, uchar32 _left, uchar32 _right)
        {
            crunes_t l = find(_str, _right);
            if (!l.is_empty())
            {
                crunes_t after = selectAfterExclude(_str, l);
                crunes_t r     = findLast(after, _left);
                if (!l.is_empty())
                {
                    return crunes_t(_str.m_ascii, l.m_end, r.m_str, _str.m_eos, _str.get_type());
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
            if (!r.is_empty())
            {
                crunes_t before = selectBeforeExclude(_str, r);
                crunes_t l      = findLast(before, _left);
                if (!l.is_empty())
                {
                    return crunes_t(_str.m_ascii, l.m_end, r.m_str, _str.m_eos, _str.get_type());
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
            if (_lstr.size() < _rstr.size())
                return -1;
            if (_lstr.size() > _rstr.size())
                return 1;
            if (_lstr.size() == 0)
                return 0;

            u32     lcursor = get_begin(_lstr);
            u32     rcursor = get_begin(_rstr);
            u32     lend    = get_end(_lstr);
            u32     rend    = get_end(_rstr);
            uchar32 lc, rc;
            if (_casesensitive)
            {
                do
                {
                    lc = _lstr.read(lcursor);
                    rc = _rstr.read(rcursor);
                } while (lc == rc && lcursor < lend && rcursor < rend);
            }
            else
            {
                do
                {
                    lc = _lstr.read(lcursor);
                    rc = _rstr.read(rcursor);
                    lc = to_lower(lc);
                    rc = to_lower(rc);
                } while (lc == rc && lcursor < lend && rcursor < rend);
            }
            if (lc < rc)
                return -1;
            else if (lc > rc)
                return 1;
            return 0;
        }

        // ------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------

        runes_t findSelectUntil(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findSelectUntil(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectUntil(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findLastSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectUntil(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findLastSelectUntil(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findSelectUntilIncluded(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findSelectUntilIncluded(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectUntilIncluded(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findLastSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectUntilIncluded(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findLastSelectUntilIncluded(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t selectAfterSelection(const runes_t& _str, const runes_t& _sel)
        {
            crunes_t cstr(_str);
            crunes_t found = selectAfterSelection(cstr, _sel);
            return crunes_to_runes(_str, found);
        }

        runes_t findSelectAfter(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findSelectAfter(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectAfter(const runes_t& _str, uchar32 _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findLastSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t findLastSelectAfter(const runes_t& _str, const crunes_t& _find, bool _casesensitive)
        {
            crunes_t cstr(_str);
            crunes_t found = findLastSelectAfter(cstr, _find, _casesensitive);
            return crunes_to_runes(_str, found);
        }

        runes_t selectBetween(const runes_t& _str, uchar32 _left, uchar32 _right)
        {
            crunes_t cstr(_str);
            crunes_t found = selectBetween(cstr, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes_t selectNextBetween(const runes_t& _str, const runes_t& _selection, uchar32 _left, uchar32 _right)
        {
            crunes_t cstr(_str);
            crunes_t found = selectNextBetween(cstr, _selection, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes_t selectBetweenLast(const runes_t& _str, uchar32 _left, uchar32 _right)
        {
            crunes_t cstr(_str);
            crunes_t found = selectBetweenLast(cstr, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes_t selectPreviousBetween(const runes_t& _str, const runes_t& _selection, uchar32 _left, uchar32 _right)
        {
            crunes_t cstr(_str);
            crunes_t found = selectPreviousBetween(cstr, _selection, _left, _right);
            return crunes_to_runes(_str, found);
        }

        runes_t selectBeforeExclude(const runes_t& _str, const runes_t& _selection)
        {
            crunes_t cstr(_str);
            crunes_t found = selectBeforeExclude(cstr, _selection);
            return crunes_to_runes(_str, found);
        }

        runes_t selectBeforeInclude(const runes_t& _str, const runes_t& _selection)
        {
            crunes_t cstr(_str);
            crunes_t found = selectBeforeInclude(cstr, _selection);
            return crunes_to_runes(_str, found);
        }

        runes_t selectAfterExclude(const runes_t& _str, const runes_t& _selection)
        {
            crunes_t cstr(_str);
            crunes_t found = selectAfterExclude(cstr, _selection);
            return crunes_to_runes(_str, found);
        }

        runes_t selectAfterInclude(const runes_t& _str, const runes_t& _selection)
        {
            crunes_t cstr(_str);
            crunes_t found = selectAfterInclude(cstr, _selection);
            return crunes_to_runes(_str, found);
        }

        runes_t selectOverlap(const runes_t& _str, const runes_t& _rstr)
        {
            crunes_t cstr(_str);
            crunes_t found = selectOverlap(cstr, _rstr);
            return crunes_to_runes(_str, found);
        }

        s32 compare(runes_t const& _lstr, runes_t const& _rstr, bool _casesensitive)
        {
            crunes_t clstr(_lstr);
            crunes_t crstr(_rstr);
            return compare(clstr, crstr, _casesensitive);
        }

        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------

        crunes_t parse(crunes_t const& _str, bool& value)
        {
            crunes_t    str          = _str;
            ascii::rune format_str[] = {'%', 'b', ascii::TERMINATOR};
            crunes_t    format(format_str, 0, 2, 2);
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
            crunes_t format(format_str, 0, 2, 2);
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
            crunes_t format(format_str, 0, 2, 2);
            sscanf(str, format, va_r_t(&value));
            return str;
        }

        crunes_t parse(crunes_t const& _str, f32& value)
        {
            crunes_t    str          = _str;
            ascii::rune format_str[] = {'%', 'f', ascii::TERMINATOR};
            crunes_t    format(format_str, 0, 2, 2);
            sscanf(str, format, va_r_t(&value));
            return str;
        }

        crunes_t parse(crunes_t const& _str, f64& value)
        {
            crunes_t    str          = _str;
            ascii::rune format_str[] = {'%', 'f', ascii::TERMINATOR};
            crunes_t    format(format_str, 0, 2, 2);
            sscanf(str, format, va_r_t(&value));
            return str;
        }

        //------------------------------------------------------------------------------
        static uchar32 const sWhitespaceChars[]    = {' ', '\t', '\n', '\r', 0};
        static u32 const     sWhitespaceCharsCount = 4;

        //------------------------------------------------------------------------------
        bool is_decimal(crunes_t const& _str)
        {
            u32 iter = get_begin(_str);
            u32 end  = get_end(_str);
            while (iter < end)
            {
                u32     next;
                uchar32 c = _str.peek(iter, next);
                if (c != '-')
                    break;
                iter = next;
            }
            while (iter < end)
            {
                uchar32 c = _str.read(iter);
                if (c < '0' || c > '9')
                    return false;
            }
            return true;
        }

        //------------------------------------------------------------------------------
        bool is_hexadecimal(crunes_t const& _str, bool with_prefix)
        {
            u32 iter = get_begin(_str);
            u32 end  = get_end(_str);

            uchar32 p;
            s32     pos = 0;
            while (iter < end)
            {
                uchar32 c = _str.read(iter);
                if (c == 'x')
                {
                    if (pos == 1 && with_prefix && p == '0')
                    {
                        // Ok, 0x.... prefix
                    }
                    else
                    {
                        return false;
                    }
                }
                else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
                {
                    // Ok, digit or hex character
                    p = c;
                }
                else
                {
                    return false;
                }
                pos++;
            }
            return true;
        }

        //------------------------------------------------------------------------------
        bool is_float(crunes_t const& _str)
        {
            ascii::rune f32chars_str[] = {'E', 'e', '.', '#', 'Q', 'N', 'A', 'B', 'I', 'F', 0};
            crunes_t    f32chars(f32chars_str, 0, 10, 10);

            u32 iter = get_begin(_str);
            u32 end  = get_end(_str);
            while (iter < end)
            {
                uchar32 c = _str.read(iter);
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
                uchar32 c = _str.read(iter);
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
            crunes_t format(format_str, 0, 2, 2);
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
            crunes_t format(format_str, 0, 2, 2);
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
            crunes_t format(format_str, 0, 2, 2);
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
            crunes_t format(format_str, 0, 2, 2);
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
            crunes_t format(format_str, 0, 5, 5);
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
            crunes_t format(format_str, 0, 5, 5);
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
                uchar32 c = _str.read(iter);
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
                uchar32 c = _str.read(iter);
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
                uchar32 c = _str.read(iter);
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
                u32     next;
                uchar32 c = _str.peek(iter, next);
                if ((c >= 'a') && (c <= 'z'))
                {
                    c += (uchar32)('A' - 'a');
                }
                write(_str, iter, c);
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
                u32     next;
                uchar32 c = _str.peek(iter, next);
                if ((c >= 'A') && (c <= 'Z'))
                {
                    c += (uchar32)('a' - 'A');
                }
                write(_str, iter, c);
            }
        }

        // ------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------

        bool starts_with(crunes_t const& _str, uchar32 start_char)
        {
            u32     next;
            u32     iter = get_begin(_str);
            uchar32 c    = _str.peek(iter, next);
            return start_char == c;
        }

        bool starts_with(crunes_t const& _str, crunes_t const& _start, bool _casesensitive)
        {
            if (_start.size() > _str.size())
                return false;

            u32 liter = get_begin(_str);
            u32 lend  = get_end(_str);
            u32 riter = get_begin(_start);
            u32 rend  = get_end(_start);
            while (liter < lend && riter < rend)
            {
                uchar32 lc = _str.read(liter);
                uchar32 rc = _start.read(riter);
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
            u32 iter = get_end(_str);
            u32 last = iter;
            backwards(_str, last, 1);
            if (is_valid(_str, last))
            {
                uchar32 c = _str.read(last);
                return end_char == c;
            }
            return false;
        }

        bool ends_with(crunes_t const& _str, crunes_t const& _end)
        {
            if (_end.size() > _str.size())
                return false;

            u32 lend  = get_end(_str);
            u32 liter = lend;
            backwards(_str, liter, (s32)_end.size());
            u32 rend  = get_end(_end);
            u32 riter = get_begin(_end);

            while (liter < lend && riter < rend)
            {
                uchar32 lc = _str.read(liter);
                uchar32 rc = _end.read(riter);
                if (lc != rc)
                    return false;
            }
            return liter == lend && riter == rend;
        }

        uchar32 first_char(crunes_t const& _str)
        {
            u32     next;
            u32     iter = get_begin(_str);
            uchar32 c    = _str.peek(iter, next);
            return c;
        }

        uchar32 last_char(crunes_t const& _str)
        {
            u32 iter = get_end(_str);
            u32 last = iter;
            backwards(_str, last, 1);
            u32     next;
            uchar32 c = _str.peek(last, next);
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
            ASSERT(_str.get_type() == _selection.get_type());
            ASSERT(_str.get_type() == _replace.get_type());

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
            find_runes[0] = {_find};
            find_runes[1] = {0};
            crunes_t find(find_runes, 0, 1, 1);

            utf32::rune replace_runes[2];
            replace_runes[0] = {_replace};
            replace_runes[1] = {0};
            runes_t replace(replace_runes, 0, 1, 1);

            findReplace(_str, find, replace, _casesensitive);
        }

        void findReplace(runes_t& _str, crunes_t const& _find, crunes_t const& _replace, bool _casesensitive)
        {
            runes_t selected = find(_str, _find, _casesensitive);
            if (selected.is_empty())
                return;
            replaceSelection(_str, selected, _replace);
        }

        // TODO implementations of 'insert'
        void insert(runes_t& str, crunes_t const& insert) {}
        void insert(runes_t& str, crunes_t const& selection, crunes_t const& insert) {}

        void trim(runes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset(charseta, &charseta[2]);
            trimLeft(str, charseta);
            trimRight(str, charseta);
        }

        void trimLeft(runes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset(charseta, &charseta[2]);
            trimLeft(str, charseta);
        }

        void trimRight(runes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset(charseta, &charseta[2]);
            trimRight(str, charseta);
        }

        void trim(runes_t& str, uchar32 _c)
        {
            utf32::rune charseta[] = {_c, cEOS};
            crunes_t    charset(charseta, &charseta[1]);
            trimLeft(str, charseta);
            trimRight(str, charseta);
        }

        void trimLeft(runes_t& str, uchar32 _c)
        {
            utf32::rune charset[] = {_c, cEOS};
            trimLeft(str, charset);
        }

        void trimRight(runes_t& str, uchar32 _c)
        {
            utf32::rune charset[2] = {_c, cEOS};
            trimLeft(str, charset);
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
                uchar32 c1    = _str.read(iter);
                u32     citer = get_begin(_charset);
                trim          = false;
                while (citer < cend)
                {
                    uchar32 c2 = _charset.read(citer);
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
            bool      trim  = true;
            u32       iter  = get_end(_str);
            u32 const begin = get_begin(_str);
            u32 const cend  = get_end(_charset);
            u32       r     = iter;
            while (iter > begin && trim)
            {
                trim = false;
                backwards(_str, iter, 1);
                u32           next;
                uchar32 const c1    = _str.peek(iter, next);
                u32           citer = get_begin(_charset);
                while (citer < cend)
                {
                    uchar32 const c2 = _charset.read(citer);
                    if (c1 == c2)
                    {
                        trim = true;
                        r    = iter;
                        break;
                    }
                }
            }
            _str = select(_str, begin, r);
        }

        void trimQuotes(runes_t& str)
        {
            utf32::rune charseta[] = {'\'', '"', cEOS};
            crunes_t    charset(charseta, 0, 2, 2);
            trim(str, charseta);
        }

        void trimQuotes(runes_t& str, uchar32 quote)
        {
            utf32::rune charseta[] = {quote, cEOS};
            crunes_t    charset(charseta, 0, 1, 1);
            trim(str, charseta);
        }

        void trimDelimiters(runes_t& str, uchar32 _left, uchar32 _right)
        {
            trimLeft(str, _left);
            trimRight(str, _right);
        }

        void trim(crunes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset(charseta, 0, 2, 2);
            trimLeft(str, charseta);
            trimRight(str, charseta);
        }

        void trimLeft(crunes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset(charseta, 0, 2, 2);
            trimLeft(str, charseta);
        }

        void trimRight(crunes_t& str)
        {
            utf32::rune charseta[] = {' ', '\t', cEOS};
            crunes_t    charset(charseta, 0, 2, 2);
            trimRight(str, charseta);
        }

        void trim(crunes_t& str, uchar32 _c)
        {
            utf32::rune charseta[] = {_c, cEOS};
            crunes_t    charset(charseta, 0, 1, 1);
            trimLeft(str, charseta);
            trimRight(str, charseta);
        }

        void trimLeft(crunes_t& str, uchar32 _c)
        {
            utf32::rune charseta[] = {_c, cEOS};
            crunes_t    charset(charseta, 0, 1, 1);
            trimLeft(str, charseta);
        }

        void trimRight(crunes_t& str, uchar32 _c)
        {
            utf32::rune charseta[2] = {_c, cEOS};
            crunes_t    charset(charseta, 0, 1, 1);
            trimLeft(str, charset);
        }

        void trim(crunes_t& str, crunes_t const& _charset)
        {
            trimLeft(str, _charset);
            trimRight(str, _charset);
        }

        void trimLeft(crunes_t& _str, crunes_t const& _charset)
        {
            bool      trim = true;
            u32       iter = get_begin(_str);
            u32 const end  = get_end(_str);
            u32 const cend = get_end(_charset);
            u32       l    = get_begin(_str);
            while (iter < end && trim)
            {
                uchar32 c1    = _str.read(iter);
                u32     citer = get_begin(_charset);
                trim          = false;
                while (citer < cend)
                {
                    uchar32 c2 = _charset.read(citer);
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

        void trimRight(crunes_t& _str, crunes_t const& _charset)
        {
            bool      trim  = true;
            u32       iter  = get_end(_str);
            u32 const begin = get_begin(_str);
            u32 const cend  = get_end(_charset);
            u32       r     = iter;
            while (iter > begin && trim)
            {
                backwards(_str, iter, 1);
                u32     next;
                uchar32 c1    = _str.peek(iter, next);
                u32     citer = get_begin(_charset);
                trim          = false;
                while (citer < cend)
                {
                    uchar32 c2 = _charset.read(citer);
                    if (c1 == c2)
                    {
                        trim = true;
                        r    = iter;
                        break;
                    }
                }
            }
            _str = select(_str, begin, r);
        }

        void trimQuotes(crunes_t& str)
        {
            utf32::rune charseta[] = {'\'', '"', cEOS};
            crunes_t    charset(charseta, 0, 2, 2);
            trim(str, charseta);
        }

        void trimQuotes(crunes_t& str, uchar32 quote)
        {
            utf32::rune charseta[] = {quote, cEOS};
            crunes_t    charset(charseta, 0, 1, 1);
            trim(str, charseta);
        }

        void trimDelimiters(crunes_t& str, uchar32 _left, uchar32 _right)
        {
            trimLeft(str, _left);
            trimRight(str, _right);
        }

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
                uchar32 c = _src.read(siter);
                if (c == 0)
                    break;
                write(_dst, diter, c);
            }
            _dst = select(_dst, dbegin, diter);
            return true;
        }

        bool concatenate(runes_t& _dst, crunes_t const& _con)
        {
            u32 siter  = get_begin(_con);
            u32 send   = get_end(_con);
            u32 dbegin = get_begin(_dst);
            u32 diter  = get_end(_dst);
            u32 dend   = get_eos(_dst);
            while (siter < send)
            {
                if (diter >= dend)
                    return false;
                uchar32 c = _con.read(siter);
                write(_dst, diter, c);
            }
            _dst = select(_dst, dbegin, diter);
            return true;
        }

        bool concatenate(runes_t& str, const crunes_t& concat1, const crunes_t& concat2)
        {
            u32 const required = str.size() + concat1.size() + concat2.size();
            if (required > str.cap())
                return false;
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
            r.m_type  = str.get_type();
            r.m_ascii = str.m_ascii;
            r.m_str   = from;
            r.m_end   = to;
            r.m_eos   = str.m_eos;
            return r;
        }

        bool backwards(runes_t const& str, u32& cursor, s32 step)
        {
            ASSERT(step > 0);
            switch (str.get_type())
            {
                case ascii::TYPE: return utf::skip(str.m_ascii, cursor, str.m_str, str.m_end, -step);
                case utf8::TYPE: return utf::skip(str.m_utf8, cursor, str.m_str, str.m_end, -step);
                case utf16::TYPE: return utf::skip(str.m_utf16, cursor, str.m_str, str.m_end, -step);
                case utf32::TYPE: return utf::skip(str.m_utf32, cursor, str.m_str, str.m_end, -step);
            }
            return false;
        }

        uchar32 peek2(runes_t const& str, u32 cursor, u32& next)
        {
            next = cursor;
            switch (str.get_type())
            {
                case ascii::TYPE: return utf::read2(str.m_ascii, next);
                case utf8::TYPE: return utf::read2(str.m_utf8, next);
                case utf16::TYPE: return utf::read2(str.m_utf16, next);
                case utf32::TYPE: return utf::read2(str.m_utf32, next);
            }
            return cEOS;
        }

        uchar32 read2(runes_t const& str, u32& cursor)
        {
            switch (str.get_type())
            {
                case ascii::TYPE: return utf::read2(str.m_ascii, cursor);
                case utf8::TYPE: return utf::read2(str.m_utf8, cursor);
                case utf16::TYPE: return utf::read2(str.m_utf16, cursor);
                case utf32::TYPE: return utf::read2(str.m_utf32, cursor);
            }
            return cEOS;
        }

        bool write(runes_t const& str, u32& cursor, uchar32 c)
        {
            switch (str.get_type())
            {
                case ascii::TYPE: return utf::write(c, str.m_ascii, cursor, str.m_eos); break;
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
            switch (str.get_type())
            {
                case ascii::TYPE: n = utf::skip(str.m_ascii, cursor, str.m_str, str.m_end, step); break;
                case utf8::TYPE: n = utf::skip(str.m_utf8, cursor, str.m_str, str.m_end, step); break;
                case utf16::TYPE: n = utf::skip(str.m_utf16, cursor, str.m_str, str.m_end, step); break;
                case utf32::TYPE: n = utf::skip(str.m_utf32, cursor, str.m_str, str.m_end, step); break;
            }
            return n > 0;
        }
        bool backwards(crunes_t const& str, u32& cursor, s32 step)
        {
            ASSERT(step > 0);

            s32 n = 0;
            switch (str.get_type())
            {
                case ascii::TYPE: n = utf::skip(str.m_ascii, cursor, str.m_str, str.m_end, -step); break;
                case utf8::TYPE: n = utf::skip(str.m_utf8, cursor, str.m_str, str.m_end, -step); break;
                case utf16::TYPE: n = utf::skip(str.m_utf16, cursor, str.m_str, str.m_end, -step); break;
                case utf32::TYPE: n = utf::skip(str.m_utf32, cursor, str.m_str, str.m_end, -step); break;
            }
            return n < 0;
        }

        static inline bool is_valid(crunes_t const& str, u32 const& cursor) { return cursor >= str.m_str && cursor < str.m_end; }
    }  // namespace nrunes

    // ------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------
    runes_t::runes_t()
    {
        m_ascii = nullptr;
        m_str   = 0;
        m_end   = 0;
        m_eos   = 0;
    }

    runes_t::runes_t(runes_t const& other)
    {
        m_ascii = other.m_ascii;
        m_str   = other.m_str;
        m_end   = other.m_end;
        m_eos   = other.m_eos;
        m_type  = other.m_type;
    }

    runes_t::runes_t(ascii::prune _str, ascii::prune _end, u32 _type)
    {
        m_ascii = _str;
        m_str   = 0;
        m_end   = (u32)(_end - _str);
        m_eos   = (u32)(_end - _str);
        m_type  = _type;
    }
    runes_t::runes_t(ascii::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        m_ascii = _bos;
        m_str   = _str;
        m_end   = _end;
        m_eos   = _eos;
        m_type  = _type;
    }

    runes_t::runes_t(ucs2::prune _str, ucs2::prune _end, u32 _type)
    {
        m_ucs2 = _str;
        m_str  = 0;
        m_end  = (u32)(_end - _str);
        m_eos  = (u32)(_end - _str);
        m_type = _type;
    }
    runes_t::runes_t(ucs2::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        m_ucs2 = _bos;
        m_str  = _str;
        m_end  = _end;
        m_eos  = _eos;
        m_type = _type;
    }

    runes_t::runes_t(utf8::prune _str, utf8::prune _end, u32 _type)
    {
        m_utf8 = _str;
        m_str  = 0;
        m_eos  = (u32)(_end - _str);
        m_end  = (u32)(_end - _str);
        m_type = _type;
    }
    runes_t::runes_t(utf8::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        m_utf8 = _bos;
        m_str  = _str;
        m_end  = _end;
        m_eos  = _eos;
        m_type = _type;
    }

    runes_t::runes_t(utf16::prune _str, utf16::prune _end, u32 _type)
    {
        m_utf16 = _str;
        m_str   = 0;
        m_eos   = (u32)(_end - _str);
        m_end   = (u32)(_end - _str);
        m_type  = _type;
    }
    runes_t::runes_t(utf16::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        m_utf16 = _bos;
        m_str   = _str;
        m_end   = _end;
        m_eos   = _eos;
        m_type  = _type;
    }

    runes_t::runes_t(utf32::prune _str, utf32::prune _end, u32 _type)
    {
        m_utf32 = _str;
        m_str   = 0;
        m_eos   = (u32)(_end - _str);
        m_end   = (u32)(_end - _str);
        m_type  = _type;
    }
    runes_t::runes_t(utf32::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        m_utf32 = _bos;
        m_str   = _str;
        m_end   = _end;
        m_eos   = _eos;
        m_type  = _type;
    }

    u32 runes_t::size() const
    {
        u32 size = 0;
        switch (get_type())
        {
            case ascii::TYPE: size = (m_end - m_str); break;
            case utf32::TYPE: size = (m_end - m_str); break;
            case utf16::TYPE:
            {
                u32 str = m_str;
                while (m_utf16[str].value != 0 && str < m_end)
                {
                    size += 1;
                    utf::reading::read(m_utf16, str, m_end);
                }
                break;
            }
            case utf8::TYPE:
            {
                u32 str = m_str;
                while (m_utf8[str].value != 0 && str < m_end)
                {
                    size += 1;
                    utf::reading::read(m_utf8, str, m_end);
                }
                break;
            }
        }
        return size;
    }

    u32 runes_t::cap() const
    {
        u32 cap = 0;
        switch (get_type())
        {
            case ascii::TYPE: cap = (u32)((m_eos - m_str)); break;
            case utf8::TYPE: cap = (u32)((m_eos - m_str)); break;
            case utf16::TYPE: cap = (u32)((m_eos - m_str)); break;
            case utf32::TYPE: cap = (u32)((m_eos - m_str)); break;
        }
        return cap;
    }

    bool runes_t::is_empty() const { return size() == 0; }
    bool runes_t::is_valid() const { return m_end < m_eos; }
    bool runes_t::is_nil() const { return m_ascii == nullptr; }
    void runes_t::reset() { m_end = m_str; }
    void runes_t::clear()
    {
        m_ascii = 0;
        m_end   = 0;
    }
    void runes_t::term()
    {
        switch (get_type())
        {
            case ascii::TYPE: m_ascii[m_end] = ascii::TERMINATOR; break;
            case utf32::TYPE: m_utf32[m_end] = utf32::TERMINATOR; break;
            case utf16::TYPE: m_utf16[m_end] = utf16::TERMINATOR; break;
            case utf8::TYPE: m_utf8[m_end] = utf8::TERMINATOR; break;
        }
    }

    bool runes_t::at_end(u32 cursor) const { return cursor >= m_end; }
    bool runes_t::is_valid(u32 cursor) const { return (cursor >= m_str && cursor < m_end); }

    uchar32 runes_t::peek(u32 cursor, u32& next) const
    {
        next = cursor;
        switch (m_type)
        {
            case ascii::TYPE: return utf::reading::read(m_ascii, next, m_end);
            case ucs2::TYPE: return utf::reading::read(m_ucs2, next, m_end);
            case utf8::TYPE: return utf::reading::read(m_utf8, next, m_end);
            case utf16::TYPE: return utf::reading::read(m_utf16, next, m_end);
            case utf32::TYPE: return utf::reading::read(m_utf32, next, m_end);
        }
        return cEOS;
    }

    bool runes_t::write(uchar32 c)
    {
        switch (get_type())
        {
            case ascii::TYPE: return utf::write(c, m_ascii, m_end, m_eos);
            case ucs2::TYPE: return utf::write(c, m_ucs2, m_end, m_eos);
            case utf8::TYPE: return utf::write(c, m_utf8, m_end, m_eos);
            case utf16::TYPE: return utf::write(c, m_utf16, m_end, m_eos);
            case utf32::TYPE: return utf::write(c, m_utf32, m_end, m_eos);
        }
        return false;
    }

    bool runes_t::scan(u32& cursor, const crunes_t& until_chars, uchar32& encountered) const
    {
        while (cursor < m_end)
        {
            u32     next;
            uchar32 c = peek(cursor, next);
            if (nrunes::contains(until_chars, c))
            {
                encountered = c;
                return true;
            }
            cursor = next;
        }
        encountered = cEOS;
        return true;
    }

    void runes_t::skip(u32& cursor, const crunes_t& skip_chars) const
    {
        while (cursor < m_end)
        {
            u32     next;
            uchar32 c = peek(cursor, next);
            if (!nrunes::contains(skip_chars, c))
                break;
            cursor = next;
        }
    }

    runes_t& runes_t::operator+=(const crunes_t& str)
    {
        crunes_t cstr(str);
        nrunes::concatenate(*this, cstr);
        return *this;
    }

    runes_t& runes_t::operator+=(const runes_t& str)
    {
        nrunes::concatenate(*this, str);
        return *this;
    }

    runes_t& runes_t::operator+=(ascii::rune c)
    {
        ascii::rune str[2];
        str[0] = c;
        str[1] = ascii::TERMINATOR;
        crunes_t cstr(str, &str[1]);
        nrunes::concatenate(*this, cstr);
        return *this;
    }

    runes_t& runes_t::operator+=(utf32::rune c)
    {
        utf32::rune str[2];
        str[0] = c;
        str[1] = utf32::TERMINATOR;
        crunes_t cstr(str, &str[1]);
        nrunes::concatenate(*this, cstr);
        return *this;
    }

    runes_t& runes_t::operator=(crunes_t const& cstr)
    {
        nrunes::concatenate(*this, cstr);
        return *this;
    }

    runes_t& runes_t::operator=(runes_t const& other)
    {
        m_ascii = other.m_ascii;
        m_str   = other.m_str;
        m_end   = other.m_end;
        m_eos   = other.m_eos;
        m_type  = other.m_type;
        return *this;
    }

    // ------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------
    static const char* sNullAsciiString = "\0\0\0\0";

    crunes_t::crunes_t()
    {
        m_ascii = sNullAsciiString;
        m_str   = 0;
        m_end   = 0;
        m_eos   = 0;
        m_type  = ascii::TYPE;
    }

    crunes_t::crunes_t(runes_t const& _str)
    {
        m_ascii = _str.m_ascii;
        m_str   = _str.m_str;
        m_end   = _str.m_end;
        m_eos   = _str.m_eos;
        m_type  = _str.m_type;
    }

    crunes_t::crunes_t(crunes_t const& _str)
    {
        m_ascii = _str.m_ascii;
        m_str   = _str.m_str;
        m_end   = _str.m_end;
        m_eos   = _str.m_eos;
        m_type  = _str.m_type;
    }

    crunes_t::crunes_t(crunes_t const& _str, u32 from, u32 to)
    {
        ASSERT(from >= _str.m_str && from < _str.m_eos);
        ASSERT(to >= _str.m_str && to < _str.m_eos);
        ASSERT(from <= to);
        m_ascii = _str.m_ascii;
        m_str   = from;
        m_end   = to;
        m_eos   = _str.m_eos;
    }

    crunes_t::crunes_t(ascii::pcrune _bos, u32 _str, u32 _end, u32 _eos, u8 _type)
    {
        m_ascii = _bos;
        m_str   = _str;
        m_end   = _end;
        m_eos   = _eos;
        m_type  = _type;
    }

    crunes_t::crunes_t(ascii::pcrune _str)
    {
        m_ascii           = _str;
        ascii::pcrune end = utf::endof(_str, nullptr);
        m_str             = 0;
        m_end             = (u32)(end - _str);
        m_eos             = m_end;
        m_type            = ascii::TYPE;
    }

    crunes_t::crunes_t(ascii::pcrune _str, ascii::pcrune _end)
    {
        m_ascii = _str;
        m_str   = 0;
        m_end   = (u32)(_end - _str);
        m_eos   = (u32)(_end - _str);
        m_type  = ascii::TYPE;
    }

    crunes_t::crunes_t(ascii::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        m_ascii = _bos;
        m_str   = _str;
        m_end   = _end;
        m_eos   = _eos;
        m_type  = ascii::TYPE;
    }

    crunes_t::crunes_t(ucs2::pcrune _str)
    {
        m_ucs2           = _str;
        m_str            = 0;
        ucs2::pcrune end = utf::endof(_str, nullptr);
        m_end            = (u32)(end - _str);
        m_eos            = m_end;
        m_type           = ucs2::TYPE;
    }

    crunes_t::crunes_t(ucs2::pcrune _str, ucs2::pcrune _end)
    {
        m_ucs2 = _str;
        m_str  = 0;
        m_end  = (u32)(_end - _str);
        m_eos  = (u32)(_end - _str);
        m_type = ucs2::TYPE;
    }

    crunes_t::crunes_t(ucs2::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        m_ucs2 = _bos;
        m_str  = _str;
        m_end  = _end;
        m_eos  = _eos;
        m_type = ucs2::TYPE;
    }

    crunes_t::crunes_t(utf8::pcrune _str)
    {
        m_utf8           = _str;
        m_str            = 0;
        utf8::pcrune end = utf::endof(_str, nullptr);
        m_end            = (u32)(end - _str);
        m_eos            = m_end;
        m_type           = utf8::TYPE;
    }
    crunes_t::crunes_t(utf8::pcrune _str, utf8::pcrune _end)
    {
        m_utf8 = _str;
        m_str  = 0;
        m_end  = (u32)(_end - _str);
        m_eos  = (u32)(_end - _str);
        m_type = utf8::TYPE;
    }
    crunes_t::crunes_t(utf8::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        m_utf8 = _bos;
        m_str  = _str;
        m_end  = _end;
        m_eos  = _eos;
        m_type = utf8::TYPE;
    }

    crunes_t::crunes_t(utf16::pcrune _str)
    {
        m_utf16           = _str;
        m_str             = 0;
        utf16::pcrune end = utf::endof(_str, nullptr);
        m_end             = (u32)(end - _str);
        m_eos             = m_end;
        m_type            = utf16::TYPE;
    }
    crunes_t::crunes_t(utf16::pcrune _str, utf16::pcrune _end)
    {
        m_utf16 = _str;
        m_str   = 0;
        m_end   = (u32)(_end - _str);
        m_eos   = (u32)(_end - _str);
        m_type  = utf16::TYPE;
    }
    crunes_t::crunes_t(utf16::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        m_utf16 = _bos;
        m_str   = _str;
        m_end   = _end;
        m_eos   = _eos;
        m_type  = utf16::TYPE;
    }

    crunes_t::crunes_t(utf32::pcrune _str)
    {
        m_utf32           = _str;
        m_str             = 0;
        utf32::pcrune end = utf::endof(_str, nullptr);
        m_end             = (u32)(end - _str);
        m_eos             = m_end;
        m_type            = utf32::TYPE;
    }
    crunes_t::crunes_t(utf32::pcrune _str, utf32::pcrune _end)
    {
        m_utf32 = _str;
        m_str   = 0;
        m_eos   = (u32)(_end - _str);
        m_end   = (u32)(_end - _str);
        m_type  = utf32::TYPE;
    }
    crunes_t::crunes_t(utf32::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        m_utf32 = _bos;
        m_str   = _str;
        m_end   = _end;
        m_eos   = _eos;
        m_type  = utf32::TYPE;
    }

    crunes_t& crunes_t::operator=(crunes_t const& other)
    {
        m_ascii = other.m_ascii;
        m_str   = other.m_str;
        m_end   = other.m_end;
        m_eos   = other.m_eos;
        m_type  = other.m_type;
        return *this;
    }

    u32 crunes_t::size() const
    {
        u32 size = 0;
        switch (get_type())
        {
            case ascii::TYPE: size = (m_end - m_str); break;
            case utf32::TYPE: size = (m_end - m_str); break;
            case utf16::TYPE:
            {
                utf16::pcrune end = m_utf16 + m_end;
                utf16::pcrune eos = m_utf16 + m_eos;
                size              = (u32)utf16::strlen(m_utf16, end, eos);
            }
            break;
            case utf8::TYPE:
            {
                utf8::pcrune end = m_utf8 + m_end;
                utf8::pcrune eos = m_utf8 + m_eos;
                size             = (u32)utf8::strlen(m_utf8, end, eos);
            }
            break;
            default: ASSERT(false); break;
        }
        return size;
    }

    bool crunes_t::is_empty() const { return m_end == m_str; }
    bool crunes_t::is_valid() const { return m_end <= m_eos; }
    bool crunes_t::is_nil() const { return m_ascii == nullptr; }
    void crunes_t::reset() { m_end = m_str; }
    void crunes_t::clear()
    {
        m_ascii = 0;
        m_end   = 0;
    }

    bool crunes_t::at_end(u32 cursor) const { return cursor >= m_end; }
    bool crunes_t::is_valid(u32 cursor) const { return (cursor >= m_str && cursor < m_end); }

    uchar32 crunes_t::peek(u32 cursor, u32& next) const
    {
        next = cursor;
        switch (m_type)
        {
            case ascii::TYPE: return utf::reading::read(m_ascii, next, m_end);
            case ucs2::TYPE: return utf::reading::read(m_ucs2, next, m_end);
            case utf8::TYPE: return utf::reading::read(m_utf8, next, m_end);
            case utf16::TYPE: return utf::reading::read(m_utf16, next, m_end);
            case utf32::TYPE: return utf::reading::read(m_utf32, next, m_end);
        }
        return cEOS;
    }

    bool crunes_t::scan(u32& cursor, const crunes_t& until_chars, uchar32& encountered) const
    {
        uchar32 c = 0;
        switch (get_type())
        {
            case ascii::TYPE:
                if (cursor >= m_str)
                {
                    while (cursor < m_end)
                    {
                        c = (uchar32)m_ascii[cursor];
                        if (until_chars.contains(c))
                        {
                            encountered = c;
                            return true;
                        }
                        cursor++;
                    }
                }
                break;
            case utf8::TYPE:
                if (cursor >= m_str)
                {
                    while (cursor < m_end)
                    {
                        u32 next = cursor;
                        c        = utf::reading::read(m_utf8, next, m_end);
                        if (until_chars.contains(c))
                        {
                            encountered = c;
                            return true;
                        }
                        cursor = next;
                    }
                }
                break;
            case utf16::TYPE:
                if (cursor >= m_str)
                {
                    while (cursor < m_end)
                    {
                        u32 next = cursor;
                        c        = utf::reading::read(m_utf16, next, m_end);
                        if (until_chars.contains(c))
                        {
                            encountered = c;
                            return true;
                        }
                        cursor = next;
                    }
                }
                break;
            case utf32::TYPE:
                if (cursor >= m_str)
                {
                    while (cursor < m_end)
                    {
                        u32 next = cursor;
                        c        = utf::reading::read(m_utf32, next, m_end);
                        if (until_chars.contains(c))
                        {
                            encountered = c;
                            return true;
                        }
                        cursor = next;
                    }
                }
                break;
            default: ASSERT(false); break;
        }
        encountered = cEOS;
        return true;
    }

    void crunes_t::skip(u32& cursor, const crunes_t& skip_chars) const
    {
        switch (get_type())
        {
            case ascii::TYPE:
                while (cursor < m_end)
                {
                    uchar32 c = (uchar32)m_ascii[cursor];
                    if (!skip_chars.contains(c))
                        break;
                    cursor++;
                }
                break;
            case utf8::TYPE:
                while (cursor < m_end)
                {
                    u32     next = cursor;
                    uchar32 c    = utf::reading::read(m_utf8, next, m_end);
                    if (!skip_chars.contains(c))
                        break;
                    cursor = next;
                }
                break;
            case utf16::TYPE:
                while (cursor < m_end)
                {
                    u32     next = cursor;
                    uchar32 c    = utf::reading::read(m_utf16, next, m_end);
                    if (!skip_chars.contains(c))
                        break;
                    cursor = next;
                }
                break;
            case utf32::TYPE:
                while (cursor < m_end)
                {
                    u32     next = cursor;
                    uchar32 c    = utf::reading::read(m_utf32, next, m_end);
                    if (!skip_chars.contains(c))
                        break;
                    cursor = next;
                }
                break;
            default: ASSERT(false); break;
        }
    }

    bool crunes_t::contains(uchar32 contains) const
    {
        switch (get_type())
        {
            case ascii::TYPE:
            {
                u32 cursor = m_str;
                while (cursor < m_end)
                {
                    uchar32 c = (uchar32)m_ascii[cursor];
                    if (c == contains)
                        return true;
                    cursor++;
                }
            }
            break;
            case utf8::TYPE:
            {
                u32 cursor = m_str;
                while (cursor < m_end)
                {
                    u32     next = cursor;
                    uchar32 c    = utf::reading::read(m_utf8, next, m_end);
                    if (c == contains)
                        return true;
                    cursor = next;
                }
            }
            break;
            case utf16::TYPE:
            {
                u32 cursor = m_str;
                while (cursor < m_end)
                {
                    u32     next = cursor;
                    uchar32 c    = utf::reading::read(m_utf16, next, m_end);
                    if (c == contains)
                        return true;
                    cursor = next;
                }
            }
            break;
            case utf32::TYPE:
            {
                u32 cursor = m_str;
                while (cursor < m_end)
                {
                    u32     next = cursor;
                    uchar32 c    = utf::reading::read(m_utf32, next, m_end);
                    if (c == contains)
                        return true;
                    cursor = next;
                }
            }
            default:
                ASSERT(false);
                break;
                break;
        }
        return false;
    }

    crunes_t crunes_t::view(u32 from) const
    {
        if ((m_str + from) >= m_end)
            from = m_end - m_str;
        crunes_t r;
        r.m_type  = m_type;
        r.m_ascii = m_ascii;
        r.m_str   = m_str + from;
        r.m_end   = m_end;
        r.m_eos   = m_eos;
        return r;
    }

    crunes_t crunes_t::view(u32 from, u32 to) const
    {
        if ((m_str + from) >= m_end)
            from = m_end - m_str;
        if ((m_str + to) >= m_end)
            to = m_end - m_str;

        crunes_t r;
        r.m_type  = m_type;
        r.m_ascii = m_ascii;
        r.m_str   = m_str + from;
        r.m_end   = m_str + to;
        r.m_eos   = m_eos;
        return r;
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
            m_runes       = crunes_t(str, 0, len, len);
            m_cursor      = 0;
        }
        reader_t::reader_t(ascii::pcrune str, u32 len)
        {
            m_runes  = crunes_t(str, 0, len, len);
            m_cursor = 0;
        }
        reader_t::reader_t(ascii::pcrune str, ascii::pcrune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = crunes_t(str, 0, len, len);
            m_cursor      = 0;
        }
        reader_t::reader_t(utf8::pcrune str, utf8::pcrune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = crunes_t(str, 0, len, len);
            m_cursor      = 0;
        }
        reader_t::reader_t(utf16::pcrune str, utf16::pcrune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = crunes_t(str, 0, len, len);
            m_cursor      = 0;
        }
        reader_t::reader_t(utf32::pcrune str, utf32::pcrune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = crunes_t(str, 0, len, len);
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

        uchar32 reader_t::vpeek(s32 n) const
        {
            uchar32 c = '\0';
            if (!at_end() && n == 0)
            {
                u32 next = m_cursor;
                switch (m_runes.get_type())
                {
                    case ascii::TYPE: c = m_runes.m_ascii[m_cursor] & 0xff; break;
                    case utf8::TYPE: c = utf::reading::read(m_runes.m_utf8, next, m_runes.m_end); break;
                    case utf16::TYPE: c = utf::reading::read(m_runes.m_utf16, next, m_runes.m_end); break;
                    case utf32::TYPE: c = m_runes.m_utf32[m_cursor].value; break;
                    default: ASSERT(false); break;
                }
            }
            else
            {
                // n > 0
                // TODO IMPLEMENTATION
            }
            return c;
        }

        bool reader_t::vread(uchar32& c)
        {
            if (!at_end())
            {
                switch (m_runes.get_type())
                {
                    case ascii::TYPE: c = m_runes.m_ascii[m_cursor++] & 0xff; break;
                    case utf8::TYPE: c = utf::reading::read(m_runes.m_utf8, m_cursor, m_runes.m_end); break;
                    case utf16::TYPE: c = utf::reading::read(m_runes.m_utf16, m_cursor, m_runes.m_end); break;
                    case utf32::TYPE: c = m_runes.m_utf32[m_cursor++].value; break;
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
                case utf8::TYPE:
                    line.m_utf8 = m_runes.m_utf8;
                    line.m_str  = m_cursor;
                    line.m_eos  = m_runes.m_eos;
                    line.m_end  = m_runes.m_end;
                    while (!at_end() && utf::reading::read(m_runes.m_utf8, next, m_runes.m_end) != '\n')
                        m_cursor = next;
                    if (!at_end())
                        utf::reading::read(m_runes.m_utf8, m_cursor, m_runes.m_end);
                    line.m_end = m_cursor;
                    break;
                case utf16::TYPE:
                    line.m_utf16 = m_runes.m_utf16;
                    line.m_str   = m_cursor;
                    line.m_eos   = m_runes.m_eos;
                    line.m_end   = m_runes.m_end;
                    while (!at_end() && utf::reading::read(m_runes.m_utf16, next, m_runes.m_end) != '\n')
                        m_cursor = next;
                    if (!at_end())
                        utf::reading::read(m_runes.m_utf16, m_cursor, m_runes.m_end);
                    line.m_end = m_cursor;
                    break;
                case utf32::TYPE:
                    line.m_utf32 = m_runes.m_utf32;
                    line.m_str   = m_cursor;
                    line.m_eos   = m_runes.m_eos;
                    line.m_end   = m_runes.m_end;
                    while (!at_end() && line.m_utf32[m_cursor].value != '\n')
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
            switch (m_runes.get_type())
            {
                case ascii::TYPE:
                    while (c > 0 && !at_end())
                    {
                        m_cursor += 1;
                        c -= 1;
                    }
                    break;

                case utf8::TYPE: utf::skip(m_runes.m_utf8, m_cursor, m_runes.m_str, m_runes.m_end, c); break;
                case utf16::TYPE: utf::skip(m_runes.m_utf16, m_cursor, m_runes.m_str, m_runes.m_end, c); break;

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
            switch (m_runes.get_type())
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
                case utf8::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        u32 next = m_cursor;
                        skipped += 1;
                        c = utf::reading::read(m_runes.m_utf8, next, m_runes.m_end);
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
                        c = utf::reading::read(m_runes.m_utf16, next, m_runes.m_end);
                        if (!s_contains_char(chars, num_chars, c))
                            break;
                        m_cursor = next;
                    }
                    break;
                case utf32::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        skipped += 1;
                        c = m_runes.m_utf32[m_cursor].value;
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
            switch (m_runes.get_type())
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
                case utf8::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        u32 next = m_cursor;
                        skipped += 1;
                        c = utf::reading::read(m_runes.m_utf8, next, m_runes.m_end);
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
                        c = utf::reading::read(m_runes.m_utf16, next, m_runes.m_end);
                        if (s_contains_char(chars, num_chars, c))
                            return skipped;
                        m_cursor = next;
                    }
                    break;
                case utf32::TYPE:
                    while (m_cursor < m_runes.m_end)
                    {
                        skipped += 1;
                        c = m_runes.m_utf32[m_cursor].value;
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
            m_runes       = runes_t(str, 0, len, len, ascii::TYPE);
            m_cursor      = 0;
            m_count       = 0;
        }
        writer_t::writer_t(utf8::prune str, utf8::prune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = runes_t(str, 0, len, len, utf8::TYPE);
            m_cursor      = 0;
            m_count       = 0;
        }
        writer_t::writer_t(utf16::prune str, utf16::prune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = runes_t(str, 0, len, len, utf16::TYPE);
            m_cursor      = 0;
            m_count       = 0;
        }
        writer_t::writer_t(utf32::prune str, utf32::prune str_end)
        {
            u32 const len = (u32)(str_end - str);
            m_runes       = runes_t(str, 0, len, len, utf32::TYPE);
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
            crunes_t    cstr(str, 0, 1, 1);
            return vwrite(cstr);
        }

        s32 writer_t::vwrite(const char* str, const char* end)
        {
            crunes_t cstr(str, end);
            return vwrite(cstr);
        }

        s32 writer_t::vwrite(crunes_t const& str)
        {
            if (!at_end(m_cursor, m_runes))
            {
                uchar32  c;
                reader_t reader(str);
                s32      count = m_count;
                switch (m_runes.get_type())
                {
                    case ascii::TYPE:
                        while (reader.read(c) && !at_end(m_cursor, m_runes))
                        {
                            m_runes.m_ascii[m_cursor] = (ascii::rune)c;
                            m_cursor += 1;
                            m_count += 1;
                        }
                        return m_count - count;
                    case utf8::TYPE:
                        while (reader.read(c) && !at_end(m_cursor, m_runes))
                        {
                            utf::write(c, m_runes.m_utf8, m_cursor, m_runes.m_eos);
                            m_count += 1;
                        }
                        return m_count - count;
                    case utf16::TYPE:
                        while (reader.read(c) && !at_end(m_cursor, m_runes))
                        {
                            utf::write(c, m_runes.m_utf16, m_cursor, m_runes.m_eos);
                            m_count += 1;
                        }
                        return m_count - count;
                    case utf32::TYPE:
                        while (reader.read(c) && !at_end(m_cursor, m_runes))
                        {
                            m_runes.m_utf32[m_cursor].value = c;
                            m_cursor += 1;
                            m_count += 1;
                        }
                        return m_count - count;
                    default: ASSERT(false); break;
                }
            }
            return 0;
        }

        void writer_t::vflush() {}

    }  // namespace nrunes
}  // namespace ncore
