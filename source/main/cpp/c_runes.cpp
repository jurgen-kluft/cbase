#include "cbase/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_debug.h"
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

        // UTF sequence sizes
        static inline s32 sequence_sizeof_utf8(uchar32 c);
        static inline s32 sequence_sizeof_utf16(uchar32 c);

        static inline s32 sequence_sizeof_utf8(uchar32 c)
        {
            if (c < 0x80)
                return 1;
            else if ((c >> 5) == 0x6)
                return 2;
            else if ((c >> 4) == 0xe)
                return 3;
            else if ((c >> 3) == 0x1e)
                return 4;
            return 0;
        }

        static inline s32 sequence_sizeof_utf16(uchar32 c)
        {
            if (c < 0xd800)
                return 1;
            else if (c < 0xdc00)
                return 2;
            return 1;
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

        static utf8::pcrune endof(utf8::pcrune str, u32 num_runes)
        {
            if (str == nullptr)
                return nullptr;

            utf8::pcrune end = str;
            while (*end != utf8::TERMINATOR && num_runes > 0)
            {
                utf8::rune c = *end;
                s32 const  l = sequence_sizeof_utf8(c);
                end += l;
                num_runes--;
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
                while (*end != utf8::TERMINATOR)
                {
                    utf8::rune c = *end;
                    s32 const  l = sequence_sizeof_utf8(c);
                    end += l;
                }
            }
            else
            {
                while (*end != utf8::TERMINATOR && end < eos)
                {
                    utf8::rune c = *end;
                    s32 const  l = sequence_sizeof_utf8(c);
                    end += l;
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
                    s32 const   l = sequence_sizeof_utf16(c);
                    end += l;
                }
            }
            else
            {
                while (*end != utf16::TERMINATOR && end < eos)
                {
                    utf16::rune c = *end;
                    s32 const   l = sequence_sizeof_utf16(c);
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
                while (*end != utf32::TERMINATOR)
                    end++;
            }
            else
            {
                while (*end != utf32::TERMINATOR && end < eos)
                    end++;
            }
            return end;
        }

        // read

        static inline uchar32 read(ascii::pcrune bos, u32& str, u32 const end)
        {
            if ((str < end) && bos[str] != ascii::TERMINATOR)
            {
                return bos[str++];
            }
            return ascii::TERMINATOR;
        }

        static uchar32 read(utf8::pcrune bos, u32& str, u32 const end)
        {
            uchar32 c = bos[str];
            if (c != utf8::TERMINATOR)
            {
                s32 const l = sequence_sizeof_utf8((uchar8)c);
                if ((str + l) > end)
                    return c;

                switch (l)
                {
                    case 0:
                        c = '?'; // Illegal character in utf8, replace with '?'
                        break;
                    case 1: break;
                    case 2:
                        str++;
                        c = ((c << 6) & 0x7ff) + ((bos[str]) & 0x3f);
                        break;
                    case 3:
                        str++;
                        c = ((c << 12) & 0xffff) + (((bos[str]) << 6) & 0xfff);
                        ++str;
                        c += (bos[str]) & 0x3f;
                        break;
                    case 4:
                        str++;
                        c = ((c << 18) & 0x1fffff) + (((bos[str]) << 12) & 0x3ffff);
                        ++str;
                        c += ((bos[str]) << 6) & 0xfff;
                        ++str;
                        c += (bos[str]) & 0x3f;
                        break;
                }
                str++;
            }
            return c;
        }

        static uchar32 read(utf16::pcrune bos, u32& str, u32 const end)
        {
            uchar16 c = bos[str];
            if (c == utf16::TERMINATOR || (str >= end))
            {
                return utf16::TERMINATOR;
            }

            s32 l = 0;
            if (c < 0xd800)
            {
                l = 1;
            }
            else if (c < 0xdc00)
            {
                l = 2;
            }

            uchar32 out_c;
            if ((str + l) <= end)
            {
                // @TODO: This needs to be fixed since the case where end==nullptr needs to be handled correctly
                out_c = 0;
                for (s32 i = 0; i < l; i++)
                {
                    c     = bos[str + i];
                    out_c = out_c << 16;
                    out_c = out_c | c;
                }
                str += l;
            }
            else
            {
                out_c = '\0';
            }
            return out_c;
        }

        static uchar32 read(utf32::pcrune bos, u32& str, u32 const end)
        {
            uchar32 c = bos[str];
            if (c != utf32::TERMINATOR && (str < end))
            {
                str++;
            }
            return c;
        }

        static inline uchar32 read(ascii::crunes_t const& str, u32& cursor) { return read(str.m_bos, cursor, str.m_end); }
        static inline uchar32 read(utf8::crunes_t const& str, u32& cursor) { return read(str.m_bos, cursor, str.m_end); }
        static inline uchar32 read(utf16::crunes_t const& str, u32& cursor) { return read(str.m_bos, cursor, str.m_end); }
        static inline uchar32 read(utf32::crunes_t const& str, u32& cursor) { return read(str.m_bos, cursor, str.m_end); }

        static inline uchar32 read(ascii::runes_t const& str, u32& cursor) { return read(str.m_bos, cursor, str.m_end); }
        static inline uchar32 read(utf8::runes_t const& str, u32& cursor) { return read(str.m_bos, cursor, str.m_end); }
        static inline uchar32 read(utf16::runes_t const& str, u32& cursor) { return read(str.m_bos, cursor, str.m_end); }
        static inline uchar32 read(utf32::runes_t const& str, u32& cursor) { return read(str.m_bos, cursor, str.m_end); }

        static uchar32 read(runes_t const& str, u32& cursor)
        {
            switch (str.get_type())
            {
                case ascii::TYPE: return read(str.m_ascii.m_bos, cursor, str.m_ascii.m_end);
                case utf8::TYPE: return read(str.m_utf8.m_bos, cursor, str.m_utf8.m_end);
                case utf16::TYPE: return read(str.m_utf16.m_bos, cursor, str.m_utf16.m_end);
                case utf32::TYPE: return read(str.m_utf32.m_bos, cursor, str.m_utf32.m_end);
            }
            return '\0';
        }
        static uchar32 read(crunes_t const& str, u32& cursor)
        {
            switch (str.get_type())
            {
                case ascii::TYPE: return read(str.m_ascii.m_bos, cursor, str.m_ascii.m_end);
                case utf8::TYPE: return read(str.m_utf8.m_bos, cursor, str.m_utf8.m_end);
                case utf16::TYPE: return read(str.m_utf16.m_bos, cursor, str.m_utf16.m_end);
                case utf32::TYPE: return read(str.m_utf32.m_bos, cursor, str.m_utf32.m_end);
            }
            return '\0';
        }

        static uchar32 peek(ascii::runes_t const& str, u32 ptr) { return read(str.m_bos, ptr, str.m_end); }
        static uchar32 peek(utf8::runes_t const& str, u32 ptr) { return read(str.m_bos, ptr, str.m_end); }
        static uchar32 peek(utf16::runes_t const& str, u32 ptr) { return read(str.m_bos, ptr, str.m_end); }
        static uchar32 peek(utf32::runes_t const& str, u32 ptr) { return read(str.m_bos, ptr, str.m_end); }

        static uchar32 peek(runes_t const& str, u32 ptr)
        {
            switch (str.get_type())
            {
                case ascii::TYPE: return peek(str.m_ascii, ptr);
                case utf8::TYPE: return peek(str.m_utf8, ptr);
                case utf16::TYPE: return peek(str.m_ascii, ptr);
                case utf32::TYPE: return peek(str.m_ascii, ptr);
                default: ASSERT(false); break;
            }
            return '\0';
        }

        static uchar32 peek(crunes_t const& str, u32 ptr)
        {
            switch (str.get_type())
            {
                case ascii::TYPE: return peek(str.m_ascii, ptr);
                case utf8::TYPE: return peek(str.m_utf8, ptr);
                case utf16::TYPE: return peek(str.m_ascii, ptr);
                case utf32::TYPE: return peek(str.m_ascii, ptr);
                default: ASSERT(false); break;
            }
            return '\0';
        }

        static uchar32 peek(ascii::crunes_t const& str, u32 ptr) { return read(str.m_bos, ptr, str.m_end); }
        static uchar32 peek(utf8::crunes_t const& str, u32 ptr) { return read(str.m_bos, ptr, str.m_end); }
        static uchar32 peek(utf16::crunes_t const& str, u32 ptr) { return read(str.m_bos, ptr, str.m_end); }
        static uchar32 peek(utf32::crunes_t const& str, u32 ptr) { return read(str.m_bos, ptr, str.m_end); }

        static s32 skip(ascii::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            if (count < 0)
            {
                cursor += count;
                if (cursor < begin)
                {
                    count  = (cursor - count) - begin;
                    cursor = begin;
                }
            }
            else
            {
                cursor += count;
                if (cursor >= end)
                {
                    count  = end - (cursor - count);
                    cursor = end;
                }
            }
            return count;
        }

        static s32 skip(utf8::pcrune const str, u32& cursor, u32 const begin, u32 const end, s32 count)
        {
            ASSERT(cursor >= begin);
            ASSERT(cursor <= end);

            if (count < 0)
            {
                u32       forward_cursor = begin;
                const s32 len            = skip(str, forward_cursor, begin, cursor, /* good enough estimate */ cursor - begin);
                if (len == 0)
                    return 0;

                // Do it again but now stop at (len + count) (count is negative)
                cursor = begin;
                return skip(str, cursor, begin, cursor, len + count);
            }
            else
            {
                s32     skipcount = 0;
                uchar32 c         = str[cursor];
                while (c != utf8::TERMINATOR && count > 0)
                {
                    s32 const l = sequence_sizeof_utf8(c);
                    if ((cursor + l) > end)
                        return false;

                    switch (l)
                    {
                        case 0:
                            c = '?'; // Illegal character in utf8, replace with '?'
                            cursor++;
                            break;
                        case 1: c = str[cursor++]; break;
                        case 2:
                            cursor++;
                            c = ((c << 6) & 0x7ff) + ((str[cursor]) & 0x3f);
                            cursor++;
                            break;
                        case 3:
                            cursor++;
                            c = ((c << 12) & 0xffff) + (((str[cursor]) << 6) & 0xfff);
                            cursor++;
                            c += (str[cursor]) & 0x3f;
                            cursor++;
                            break;
                        case 4:
                            cursor++;
                            c = ((c << 18) & 0x1fffff) + (((str[cursor]) << 12) & 0x3ffff);
                            cursor++;
                            c += ((str[cursor]) << 6) & 0xfff;
                            cursor++;
                            c += (str[cursor]) & 0x3f;
                            cursor++;
                            break;
                    }
                    skipcount++;
                    count -= 1;
                }
                return skipcount;
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
                const s32 len            = skip(str, forward_cursor, begin, cursor, /* good enough estimate */ cursor - begin);
                if (len == 0)
                    return 0;

                // Do it again but now stop at (len + count) (count is negative)
                cursor = begin;
                return skip(str, cursor, begin, cursor, len + count);
            }
            else
            {
                s32     skipcount = 0;
                uchar32 c         = str[cursor];
                while (c != utf16::TERMINATOR && (cursor < end) && count > 0)
                {
                    s32 const l = sequence_sizeof_utf16(c);
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
                cursor += count;
                if (cursor < begin)
                {
                    count  = (cursor - count) - begin;
                    cursor = begin;
                }
            }
            else
            {
                cursor += count;
                if (cursor >= end)
                {
                    count  = end - (cursor - count);
                    cursor = end;
                }
            }
            return count;
        }

        // ####### WRITE ########

        static bool write(uchar32 c, ascii::prune bos, u32& cursor, u32 end)
        {
            if (c > 0x7f)
            {
                c = '?';
            }
            if (cursor < end)
            {
                bos[cursor++] = (uchar)c;
                return true;
            }
            return false;
        }

        static bool write(uchar32 cp, utf8::prune bos, u32& cursor, u32 end)
        {
            if (cp < 0x80)
            { // one octet
                if (cursor < end)
                {
                    bos[cursor++] = static_cast<uchar8>(cp);
                    return true;
                }
            }
            else if (cp < 0x800)
            { // two octets
                if ((cursor + 1) < end)
                {
                    bos[cursor++] = static_cast<uchar8>((cp >> 6) | 0xc0);
                    bos[cursor++] = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            else if (cp < 0x10000)
            { // three octets
                if ((cursor + 2) < end)
                {
                    bos[cursor++] = static_cast<uchar8>((cp >> 12) | 0xe0);
                    bos[cursor++] = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
                    bos[cursor++] = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            else
            { // four octets
                if ((cursor + 3) < end)
                {
                    bos[cursor++] = static_cast<uchar8>((cp >> 18) | 0xf0);
                    bos[cursor++] = static_cast<uchar8>(((cp >> 12) & 0x3f) | 0x80);
                    bos[cursor++] = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
                    bos[cursor++] = static_cast<uchar8>((cp & 0x3f) | 0x80);
                    return true;
                }
            }
            return false;
        }

        static bool write(uchar32 rune, utf16::prune bos, u32& cursor, u32 end)
        {
            s32 len = 0;
            if (rune < 0xd800)
            {
                len = 1;
            }
            else if (rune < 0xe000)
            {
                len = 0;
            }
            else if (rune < 0x010000)
            {
                len = 1;
            }
            else if (rune < 0x110000)
            {
                len = 2;
            }

            if (len > 0)
            {
                if (len == 1 && cursor < end)
                {
                    bos[cursor++] = (uchar16)rune;
                    return true;
                }
                else if ((cursor + 1) < end)
                {
                    // 20-bit intermediate value
                    u32 const iv  = rune - 0x10000;
                    bos[cursor++] = static_cast<uchar16>((iv >> 10) + 0xd800);
                    bos[cursor++] = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
                    return true;
                }
            }
            return false;
        }

        static bool write(uchar32 c, utf32::prune bos, u32& cursor, u32 end)
        {
            if (cursor < end)
            {
                bos[cursor++] = c;
                return true;
            }
            return false;
        }

        static inline bool write(uchar32 c, ascii::runes_t const& dst, u32& cursor) { return write(c, dst.m_bos, cursor, dst.m_eos); }
        static inline bool write(uchar32 c, utf8::runes_t const& dst, u32& cursor) { return write(c, dst.m_bos, cursor, dst.m_eos); }
        static inline bool write(uchar32 c, utf16::runes_t const& dst, u32& cursor) { return write(c, dst.m_bos, cursor, dst.m_eos); }
        static inline bool write(uchar32 c, utf32::runes_t const& dst, u32& cursor) { return write(c, dst.m_bos, cursor, dst.m_eos); }

        static inline bool terminate_str(ascii::runes_t& dst) { return write(ascii::TERMINATOR, dst.m_bos, dst.m_end, dst.m_eos); }
        static inline bool terminate_str(utf8::runes_t& dst) { return write(utf8::TERMINATOR, dst.m_bos, dst.m_end, dst.m_eos); }
        static inline bool terminate_str(utf16::runes_t& dst) { return write(utf16::TERMINATOR, dst.m_bos, dst.m_end, dst.m_eos); }
        static inline bool terminate_str(utf32::runes_t& dst) { return write(utf32::TERMINATOR, dst.m_bos, dst.m_end, dst.m_eos); }

    } // namespace utf

    namespace ascii
    {
        s32 strlen(pcrune str, pcrune* end, pcrune eos)
        {
            pcrune iter = str;
            if (eos == nullptr)
            {
                while (*iter != TERMINATOR)
                    iter++;
            }
            else
            {
                while (*iter != TERMINATOR && iter < eos)
                    iter++;
            }
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

        static s32 divmod10(u32 value, s8& remainder)
        {
            const u32 q = (value / 10);
            remainder   = (s8)(value - (q * 10));
            return (s32)(q);
        }

        static s32 divmod10(s32 value, s8& remainder)
        {
            const s32 q = (value / 10);
            remainder   = (s8)(value - (q * 10));
            return q;
        }

        bool asStr(u32 val, char const* str, char*& cursor, char const* end, s32 base, bool octzero, bool lowercase)
        {
            ASSERT(str != nullptr && cursor != nullptr && end != nullptr);

            if ((end - cursor) < 10)
                return false;

            uchar32 c;
            s32     sval;
            s8      mod;

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
                        c    = to_dec_char(mod);
                        w -= 1;
                        *w = c;
                    }
                    else
                    {
                        sval = (s32)val;
                    }

                    do
                    {
                        sval = divmod10(sval, mod);
                        c    = to_dec_char(mod);
                        w -= 1;
                        *w = c;
                    } while (sval != 0);

                    break;

                case 8:
                    do
                    {
                        c = to_dec_char(val & 7);
                        w -= 1;
                        *w = c;
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
                        *w = to_hex_char(val & 15, lowercase);
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
            return true;
        }

        bool asStr(s32 val, char const* str, char*& cursor, char const* end, s32 base, bool octzero, bool lowercase)
        {
            ASSERT(str != nullptr && cursor != nullptr && end != nullptr);

            if ((end - cursor) < 11)
                return false;

            if (val < 0)
            {
                *cursor++ = '-';
                val       = -val;
            }
            return asStr((u32)val, str, cursor, end, base, octzero, lowercase);
        }

        static s64 divmod10(u64 value, s8& remainder)
        {
            const u64 q = (value / 10);
            remainder   = (s8)(value - (q * 10));
            return (s64)(q);
        }

        static s64 divmod10(s64 value, s8& remainder)
        {
            const s64 q = (value / 10);
            remainder   = (s8)(value - (q * 10));
            return q;
        }

        bool asStr(u64 val, char const* str, char*& cursor, char const* end, s32 base, bool octzero, bool lowercase)
        {
            uchar32 c;
            s64     sval;
            s8      mod;

            if ((end - cursor) < 20)
                return false;

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
                        c    = to_dec_char(mod);
                        *--w = c;
                    }
                    else
                    {
                        sval = (s64)val;
                    }

                    do
                    {
                        sval = divmod10(sval, mod);
                        c    = to_dec_char(mod);
                        *--w = c;
                    } while (sval != 0);

                    break;

                case 8:
                    do
                    {
                        c    = to_dec_char(val & 7);
                        *--w = c;
                        val >>= 3;
                    } while (val);

                    if (octzero && c != '0')
                    {
                        c    = '0';
                        *--w = c;
                    }

                    break;

                case 16:
                    do
                    {
                        c    = to_hex_char((u8)val, lowercase);
                        *--w = c;
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

            return true;
        }

        bool asStr(s64 val, char const* str, char*& cursor, char const* end, s32 base, bool octzero, bool lowercase)
        {
            ASSERT(str != nullptr && cursor != nullptr && end != nullptr);

            if ((end - cursor) < 11)
                return false;

            if (val < 0)
            {
                *cursor++ = '-';
                val       = -val;
            }
            return asStr((u64)val, str, cursor, end, base, octzero, lowercase);
        }

        // enum EBoolTypes
        // {
        //     TrueFalse = 0,
        //     YesNo = 1,
        //     LowerCase = 0,
        //     UpperCase = 2,
        //     CamelCase = 4,
        // };

        bool asStr(bool val, char const* str, char*& cursor, char const* end, s8 caseType)
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
                default: ASSERT(false); return false;
            }
            while (*boolstr != '\0' && cursor < end)
                *cursor++ = *boolstr++;
            return *boolstr == '\0';
        }

    } // namespace ascii

    namespace utf8
    {
        s32 strlen(pcrune str, pcrune& end, pcrune eos)
        {
            s32 chars = 0;
            end       = str;
            if (eos == nullptr)
            {
                while (*end != TERMINATOR)
                    end++, chars++;
            }
            else
            {
                while (*end != TERMINATOR && end < eos)
                    end++, chars++;
            }
            return chars;
        }
    } // namespace utf8

    namespace utf16
    {
        s32 strlen(pcrune str, pcrune& end, pcrune eos)
        {
            s32 chars = 0;
            end       = str;
            if (eos == nullptr)
            {
                while (*end != TERMINATOR)
                    end++, chars++;
            }
            else
            {
                while (*end != TERMINATOR && end < eos)
                    end++, chars++;
            }
            return chars;
        }
    } // namespace utf16

    namespace utf32
    {
        s32 strlen(pcrune str, pcrune& end, pcrune eos)
        {
            s32 chars = 0;
            end       = str;
            if (eos == nullptr)
            {
                while (*end != TERMINATOR)
                    end++, chars++;
            }
            else
            {
                while (*end != TERMINATOR && end < eos)
                    end++, chars++;
            }
            return chars;
        }
    } // namespace utf32

    // u32 functions
    static u32     get_begin(runes_t const& str);
    static u32     get_end(runes_t const& str);
    static u32     get_eos(runes_t const& str);
    static runes_t select(runes_t const& str, u32 from, u32 to);
    static bool    forwards(runes_t const& str, u32& cursor, s32 step = 1);
    static bool    backwards(runes_t const& str, u32& cursor, s32 step = 1);
    static uchar32 peek(runes_t const& str, u32 cursor);
    static uchar32 read(runes_t const& str, u32& cursor);
    static bool    write(runes_t const& str, u32& cursor, uchar32 c);
    static bool    is_valid(runes_t const& str, u32 cursor);

    // u32 functions
    static u32      get_begin(crunes_t const& str);
    static u32      get_end(crunes_t const& str);
    static u32      get_eos(crunes_t const& str);
    static crunes_t select(crunes_t const& str, u32 from, u32 to);
    static bool     forwards(crunes_t const& str, u32& cursor, s32 step = 1);
    static bool     backwards(crunes_t const& str, u32& cursor, s32 step = 1);
    static uchar32  peek(crunes_t const& str, u32 const& cursor);
    static uchar32  read(crunes_t const& str, u32& cursor);
    static bool     write(crunes_t const& str, u32& cursor, uchar32 c);
    static bool     is_valid(crunes_t const& str, u32 const& cursor);

    static inline runes_t crunes_to_runes(runes_t const& str, crunes_t const& sel)
    {
        runes_t r(str);
        r.m_ascii.m_str += (sel.m_ascii.m_str - str.m_ascii.m_str);
        r.m_ascii.m_end += (sel.m_ascii.m_end - str.m_ascii.m_end);
        return r;
    }

    static inline runes_t  nothing_found(runes_t const& str) { return runes_t(str.m_ascii.m_bos, str.m_ascii.m_str, str.m_ascii.m_str, str.m_ascii.m_str, str.get_type()); }
    static inline crunes_t nothing_found(crunes_t const& str) { return crunes_t(str.m_ascii.m_bos, str.m_ascii.m_str, str.m_ascii.m_str, str.m_ascii.m_str, str.get_type()); }

    crunes_t find(crunes_t const& _str, uchar32 _c, bool _casesensitive)
    {
        u32 iter = get_begin(_str);
        u32 end  = get_end(_str);
        while (iter < end)
        {
            uchar32    c     = peek(_str, iter);
            bool const equal = _casesensitive ? is_equal(c, _c) : is_equalfold(c, _c);
            if (equal)
            {
                u32 begin = iter;
                forwards(_str, iter);
                return select(_str, begin, iter);
            }
            forwards(_str, iter);
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

            uchar32    c     = peek(_str, iter);
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

    bool contains(runes_t const& _str, uchar32 _c, bool _casesensitive = true)
    {
        runes_t pos = find(_str, _c, _casesensitive);
        return !pos.is_empty();
    }

    bool contains(crunes_t const& _str, uchar32 _c, bool _casesensitive = true)
    {
        crunes_t pos = find(_str, _c, _casesensitive);
        return !pos.is_empty();
    }

    crunes_t find(crunes_t const& _str, crunes_t const& _find, bool _casesensitive)
    {
        if (_str.is_empty())
            return nothing_found(_str);
        if (_find.is_empty())
            return nothing_found(_str);
        if (_find.size() > _str.size())
            return nothing_found(_str);

        u32 iterb = get_begin(_str);
        u32 itere = iterb;
        forwards(_str, itere, _find.size());
        u32 end = get_end(_str);
        while (itere <= end)
        {
            crunes_t  sel = select(_str, iterb, itere);
            s32 const cmp = compare(sel, _find, _casesensitive);
            if (cmp == 0)
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
        if (_str.is_empty())
            return nothing_found(_str);
        if (_find.is_empty())
            return nothing_found(_str);
        if (_find.size() > _str.size())
            return nothing_found(_str);

        u32 begin = get_begin(_str);
        u32 itere = get_end(_str);
        u32 iterb = itere;
        backwards(_str, iterb, _find.size());
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
            u32     pos = iter;
            uchar32 c   = read(_str, iter);

            // See if this char can be found in the 'set'
            crunes_t found = find(_any, c, _casesensitive);
            if (!found.is_empty())
                return select(_str, pos, iter);
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
    crunes_t findSelectUntil(const crunes_t& _str, uchar32 _c, bool _casesensitive)
    {
        crunez_t<utf32::rune, 2> _find(_c);
        return findSelectUntil(_str, _find, _casesensitive);
    }

    crunes_t findSelectUntil(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t sel = find(_str, _find, _casesensitive);
        if (sel.is_empty())
            return nothing_found(_str);
        return crunes_t(_str.m_ascii.m_bos, _str.m_ascii.m_str, sel.m_ascii.m_str, _str.m_ascii.m_eos, _str.get_type());
    }

    crunes_t findLastSelectUntil(const crunes_t& _str, uchar32 _c, bool _casesensitive)
    {
        crunez_t<utf32::rune, 2> _find(_c);
        return findLastSelectUntil(_str, _find, _casesensitive);
    }

    crunes_t findLastSelectUntil(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t sel = findLast(_str, _find, _casesensitive);
        if (sel.is_empty())
            return nothing_found(_str);
        return crunes_t(_str.m_ascii.m_bos, _str.m_ascii.m_str, sel.m_ascii.m_str, _str.m_ascii.m_eos, _str.get_type());
    }

    crunes_t findSelectUntilIncluded(const crunes_t& _str, uchar32 _c, bool _casesensitive)
    {
        crunez_t<utf32::rune, 2> _find(_c);
        return findSelectUntilIncluded(_str, _find, _casesensitive);
    }

    crunes_t findSelectUntilIncluded(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t sel = find(_str, _find, _casesensitive);
        if (sel.is_empty())
            return nothing_found(_str);
        return crunes_t(_str.m_ascii.m_bos, _str.m_ascii.m_str, sel.m_ascii.m_end, _str.m_ascii.m_eos, _str.get_type());
    }

    crunes_t findLastSelectUntilIncluded(const crunes_t& _str, uchar32 _c, bool _casesensitive)
    {
        crunez_t<utf32::rune, 2> _find(_c);
        return findLastSelectUntilIncluded(_str, _find, _casesensitive);
    }

    crunes_t findLastSelectUntilIncluded(const crunes_t& _str, const crunes_t& _find, bool _casesensitive)
    {
        crunes_t sel = findLast(_str, _find, _casesensitive);
        if (sel.is_empty())
            return nothing_found(_str);
        return crunes_t(_str.m_ascii.m_bos, _str.m_ascii.m_str, sel.m_ascii.m_end, _str.m_ascii.m_eos, _str.get_type());
    }

    crunes_t selectAfterSelection(const crunes_t& _str, const crunes_t& _sel)
    {
        if (!_sel.is_empty())
        {
            crunes_t sel;
            sel.m_ascii.m_bos = _str.m_ascii.m_bos;
            sel.m_ascii.m_eos = _str.m_ascii.m_eos;
            sel.m_ascii.m_str = _sel.m_ascii.m_end;
            sel.m_ascii.m_end = _str.m_ascii.m_end;
            return sel;
        }
        return nothing_found(_str);
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

    crunes_t selectBetween(const crunes_t& _str, uchar32 _left, uchar32 _right)
    {
        crunes_t l = find(_str, _right);
        if (!l.is_empty())
        {
            crunes_t after = selectAfterExclude(_str, l);
            crunes_t r     = findLast(after, _left);
            if (!l.is_empty())
            {
                return crunes_t(_str.m_ascii.m_bos, l.m_ascii.m_end, r.m_ascii.m_str, _str.m_ascii.m_eos, _str.get_type());
            }
        }
        return nothing_found(_str);
    }

    crunes_t selectNextBetween(const crunes_t& _str, const crunes_t& _selection, uchar32 _left, uchar32 _right)
    {
        crunes_t str;
        str.m_ascii.m_bos = _str.m_ascii.m_bos;
        str.m_ascii.m_eos = _str.m_ascii.m_eos;
        str.m_ascii.m_end = _str.m_ascii.m_end;
        str.m_ascii.m_str = _selection.m_ascii.m_end;
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
                return crunes_t(_str.m_ascii.m_bos, l.m_ascii.m_end, r.m_ascii.m_str, _str.m_ascii.m_eos, _str.get_type());
            }
        }
        return nothing_found(_str);
    }

    crunes_t selectPreviousBetween(const crunes_t& _str, const crunes_t& _selection, uchar32 _left, uchar32 _right)
    {
        crunes_t str;
        str.m_ascii.m_bos   = _str.m_ascii.m_bos;
        str.m_ascii.m_eos   = _str.m_ascii.m_eos;
        str.m_ascii.m_str   = _str.m_ascii.m_str;
        str.m_ascii.m_end   = _selection.m_ascii.m_str;
        str.m_ascii.m_flags = _str.m_ascii.m_flags;
        return selectBetweenLast(str, _left, _right);
    }

    crunes_t selectBeforeExclude(const crunes_t& _str, const crunes_t& _selection)
    {
        crunes_t str;
        str.m_ascii.m_bos   = _str.m_ascii.m_bos;
        str.m_ascii.m_eos   = _str.m_ascii.m_eos;
        str.m_ascii.m_str   = _str.m_ascii.m_str;
        str.m_ascii.m_end   = _selection.m_ascii.m_str;
        str.m_ascii.m_flags = _str.m_ascii.m_flags;
        return str;
    }

    crunes_t selectBeforeInclude(const crunes_t& _str, const crunes_t& _selection)
    {
        crunes_t str;
        str.m_ascii.m_bos   = _str.m_ascii.m_bos;
        str.m_ascii.m_eos   = _str.m_ascii.m_eos;
        str.m_ascii.m_str   = _str.m_ascii.m_str;
        str.m_ascii.m_end   = _selection.m_ascii.m_end;
        str.m_ascii.m_flags = _str.m_ascii.m_flags;
        return str;
    }

    crunes_t selectAfterExclude(const crunes_t& _str, const crunes_t& _selection)
    {
        crunes_t sel;
        sel.m_ascii.m_bos   = _str.m_ascii.m_bos;
        sel.m_ascii.m_eos   = _str.m_ascii.m_eos;
        sel.m_ascii.m_str   = _selection.m_ascii.m_end;
        sel.m_ascii.m_end   = _str.m_ascii.m_end;
        sel.m_ascii.m_flags = _str.m_ascii.m_flags;
        return sel;
    }

    crunes_t selectAfterInclude(const crunes_t& _str, const crunes_t& _selection)
    {
        crunes_t sel;
        sel.m_ascii.m_bos   = _str.m_ascii.m_bos;
        sel.m_ascii.m_eos   = _str.m_ascii.m_eos;
        sel.m_ascii.m_str   = _selection.m_ascii.m_str;
        sel.m_ascii.m_end   = _str.m_ascii.m_end;
        sel.m_ascii.m_flags = _str.m_ascii.m_flags;
        return sel;
    }

    crunes_t selectOverlap(const crunes_t& _lstr, const crunes_t& _rstr)
    {
        u32 lend = get_end(_lstr);
        u32 rend = get_end(_rstr);

        crunes_t lstr(_lstr);
        while (!lstr.is_empty())
        {
            u32 lcursor = get_begin(lstr);
            u32 rcursor = get_begin(_rstr);

            uchar32 lc, rc;
            do
            {
                lc = read(lstr, lcursor);
                rc = read(_rstr, rcursor);
            } while (lc == rc && lcursor < lend && rcursor < rend);

            if (lc == rc && lcursor == lend && rcursor <= rend)
            {
                return crunes_t(lstr.m_ascii.m_bos, lstr.m_ascii.m_str, lstr.m_ascii.m_end, lstr.m_ascii.m_eos, lstr.get_type());
            }
            lstr.read();
        }

        return nothing_found(_lstr);
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
                lc = read(_lstr, lcursor);
                rc = read(_rstr, rcursor);
            } while (lc == rc && lcursor < lend && rcursor < rend);
        }
        else
        {
            do
            {
                lc = read(_lstr, lcursor);
                rc = read(_rstr, rcursor);
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
    bool is_decimal(crunes_t const& _str)
    {
        u32 iter = get_begin(_str);
        u32 end  = get_end(_str);
        while (iter < end)
        {
            uchar32 c = peek(_str, iter);
            if (c != '-')
                break;
            read(_str, iter);
        }
        while (iter < end)
        {
            uchar32 c = read(_str, iter);
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
            uchar32 c = read(_str, iter);
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
            uchar32 c = read(_str, iter);
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
            uchar32 c = read(_str, iter);
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
            format_str[2] = '0' + numFractionalDigits / 10;
            format_str[3] = '0' + numFractionalDigits % 10;
        }
        crunes_t format(format_str, 0, 5, 5);
        sprintf(str, format, va_t(val));
    }

    void to_string(runes_t& str, f64 val, s32 numFractionalDigits)
    {
        ascii::rune format_str[] = {'%', '.', '0', '2', 'f', ascii::TERMINATOR};
        if (numFractionalDigits != 2 && numFractionalDigits > 0 && numFractionalDigits < 100)
        {
            format_str[2] = '0' + numFractionalDigits / 10;
            format_str[3] = '0' + numFractionalDigits % 10;
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
            uchar32 c = read(_str, iter);
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
            uchar32 c = read(_str, iter);
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
            uchar32 c = read(_str, iter);
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
            uchar32 c = peek(_str, iter);
            if ((c >= 'a') && (c <= 'z'))
            {
                c += ('A' - 'a');
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
            uchar32 c = peek(_str, iter);
            if ((c >= 'A') && (c <= 'Z'))
            {
                c += ('a' - 'A');
            }
            write(_str, iter, c);
        }
    }

    // ------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------

    bool starts_with(crunes_t const& _str, uchar32 start_char)
    {
        u32     iter = get_begin(_str);
        uchar32 c    = peek(_str, iter);
        return start_char == c;
    }

    bool starts_with(crunes_t const& _str, crunes_t const& _start)
    {
        if (_start.size() > _str.size())
            return false;

        u32 liter = get_begin(_str);
        u32 lend  = get_end(_str);
        u32 riter = get_begin(_start);
        u32 rend  = get_end(_start);
        while (liter < lend && riter < rend)
        {
            uchar32 lc = read(_str, liter);
            uchar32 rc = read(_start, riter);
            if (lc != rc)
                return false;
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
            uchar32 c = read(_str, last);
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
        backwards(_str, liter, _end.size());
        u32 rend  = get_end(_end);
        u32 riter = get_begin(_end);

        while (liter < lend && riter < rend)
        {
            uchar32 lc = read(_str, liter);
            uchar32 rc = read(_end, riter);
            if (lc != rc)
                return false;
        }
        return liter == lend && riter == rend;
    }

    uchar32 first_char(crunes_t const& _str)
    {
        u32     iter = get_begin(_str);
        uchar32 c    = peek(_str, iter);
        return c;
    }

    uchar32 last_char(crunes_t const& _str)
    {
        u32 iter = get_end(_str);
        u32 last = iter;
        backwards(_str, last, 1);
        uchar32 c = peek(_str, last);
        return c;
    }

    // ------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------

    void removeSelection(runes_t& str, crunes_t const& selection)
    {
        //@note: treated as a mem move
        u32       dst = selection.m_ascii.m_str;
        u32       src = selection.m_ascii.m_end;
        const u32 end = str.m_ascii.m_end;
        while (src < end)
        {
            str.m_ascii.m_bos[dst++] = str.m_ascii.m_bos[src++];
        }
        str.m_ascii.m_end = dst;
    }

    void keepOnlySelection(runes_t& str, crunes_t const& keep)
    {
        //@note: treated as a mem move
        u32       dst = str.m_ascii.m_str;
        u32       src = keep.m_ascii.m_str;
        u32 const end = keep.m_ascii.m_end;
        while (src < end)
        {
            // *dst++ = *src++;
            str.m_ascii.m_bos[dst++] = str.m_ascii.m_bos[src++];
        }
        str.m_ascii.m_end = dst;
    }

    void replaceSelection(runes_t& _str, crunes_t const& _selection, crunes_t const& _replace)
    {
        ASSERT(_str.get_type() == _selection.get_type());
        ASSERT(_str.get_type() == _replace.get_type());

        runes_t str = crunes_to_runes(_str, _selection);

        // NOTE!! : The logic here is based on memory copy and size in bytes, we do not consider number of characters

        s32 const selected_len = (s32)(str.m_ascii.m_end - str.m_ascii.m_str);
        s32 const replace_len  = (s32)(_replace.m_ascii.m_end - _replace.m_ascii.m_str);

        u32 end;
        if (selected_len < replace_len)
        {
            // Move, expanding
            u32 move_len = replace_len - selected_len;
            if (move_len > (str.m_ascii.m_eos - str.m_ascii.m_end))
                move_len = (s32)(str.m_ascii.m_eos - str.m_ascii.m_end);

            // No matter what, push out anything at the end!
            u32 dst = (str.m_ascii.m_end + (str.m_ascii.m_eos - str.m_ascii.m_end) - 1);
            u32 src = (dst - move_len);
            while (dst > str.m_ascii.m_end)
            {
                //*dst-- = *src--;
                str.m_ascii.m_bos[dst--] = str.m_ascii.m_bos[src--];
            }

            end = _str.m_ascii.m_end + move_len; // Update str_end
        }
        else if (selected_len > replace_len)
        {
            // Move, shrinking
            s32 move_len = selected_len - replace_len;
            u32 dst      = str.m_ascii.m_end - move_len;
            u32 src      = str.m_ascii.m_end;
            while (src < str.m_ascii.m_eos)
            {
                //*dst++ = *src++;
                str.m_ascii.m_bos[dst++] = str.m_ascii.m_bos[src++];
            }

            end = _str.m_ascii.m_end - move_len; // Update str_end
        }
        else
        {
            end = _str.m_ascii.m_end;
        }

        // Replace
        u32 src     = _replace.m_ascii.m_str;
        u32 src_end = _replace.m_ascii.m_str + replace_len;
        u32 dst     = str.m_ascii.m_str;
        while (src < src_end)
        {
            //*dst++ = *src++;
            str.m_ascii.m_bos[dst++] = _replace.m_ascii.m_bos[src++];
        }
        _str.m_ascii.m_end = end;
    }

    static inline bool isIn(runes_t const& str, crunes_t const& selection) { return (selection.m_ascii.m_str >= str.m_ascii.m_str && selection.m_ascii.m_str < str.m_ascii.m_end && selection.m_ascii.m_end <= str.m_ascii.m_end); }

    static crunes_t relocate(runes_t const& from, runes_t const& to, crunes_t const& selection)
    {
        if (isIn(from, selection))
        {
            crunes_t nselection(to);
            s32      stroffset       = (s32)(selection.m_ascii.m_str - from.m_ascii.m_str);
            s32      endoffset       = (s32)(selection.m_ascii.m_end - from.m_ascii.m_str);
            nselection.m_ascii.m_end = nselection.m_ascii.m_str + endoffset;
            nselection.m_ascii.m_str = nselection.m_ascii.m_str + stroffset;
            return nselection;
        }
        return crunes_t();
    }

    void replaceSelection(runes_t& str, crunes_t const& selection, crunes_t const& replace, runes_alloc_t* allocator, s32 size_alignment)
    {
        s64 const diff = replace.size() - selection.size();
        if (diff > 0)
        {
            // Do we need to allocate additional space for @str ?
            // This will invalidate @selection
            if ((str.size() + diff) > str.cap())
            {
                runes_t nstr = allocator->allocate(0, (str.size() + diff + size_alignment) & ~(size_alignment - 1), str.get_type());
                copy(str, nstr);

                // Relocate selection
                crunes_t nselection = relocate(str, nstr, selection);

                // Deallocate original str
                allocator->deallocate(str);
                str = nstr;
            }
        }
        replaceSelection(str, selection, replace);
    }

    void findReplace(runes_t& _str, uchar32 _find, uchar32 _replace, bool _casesensitive)
    {
        runez_t<utf32::rune, 3> find(_find);
        runez_t<utf32::rune, 3> replace(_replace);
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
    void insert(runes_t& str, crunes_t const& insert, runes_alloc_t* allocator, s32 size_alignment) {}
    void insert(runes_t& str, crunes_t const& selection, crunes_t const& insert) {}
    void insert(runes_t& str, crunes_t const& selection, crunes_t const& insert, runes_alloc_t* allocator, s32 size_alignment) {}

    void trim(runes_t& str)
    {
        utf32::rune charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t    charset(charseta, &charseta[2]);
        trimLeft(str, charseta);
        trimRight(str, charseta);
    }

    void trimLeft(runes_t& str)
    {
        utf32::rune charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t    charset(charseta, &charseta[2]);
        trimLeft(str, charseta);
    }

    void trimRight(runes_t& str)
    {
        utf32::rune charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t    charset(charseta, &charseta[2]);
        trimRight(str, charseta);
    }

    void trim(runes_t& str, uchar32 _c)
    {
        utf32::rune charseta[] = {_c, utf32::TERMINATOR};
        crunes_t    charset(charseta, &charseta[1]);
        trimLeft(str, charseta);
        trimRight(str, charseta);
    }

    void trimLeft(runes_t& str, uchar32 _c)
    {
        utf32::rune charset[] = {_c, utf32::TERMINATOR};
        trimLeft(str, charset);
    }

    void trimRight(runes_t& str, uchar32 _c)
    {
        utf32::rune charset[2] = {_c, utf32::TERMINATOR};
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
            uchar32 c1    = read(_str, iter);
            u32     citer = get_begin(_charset);
            trim          = false;
            while (citer < cend)
            {
                uchar32 c2 = read(_charset, citer);
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
            backwards(_str, iter, 1);
            uchar32 c1    = peek(_str, iter);
            u32     citer = get_begin(_charset);
            trim          = false;
            while (citer < cend)
            {
                uchar32 c2 = peek(_charset, citer);
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
        uchar32  charseta[] = {'\'', '"', utf32::TERMINATOR};
        crunes_t charset(charseta, 0, 2, 2);
        trim(str, charseta);
    }

    void trimQuotes(runes_t& str, uchar32 quote)
    {
        uchar32  charseta[] = {quote, utf32::TERMINATOR};
        crunes_t charset(charseta, 0, 1, 1);
        trim(str, charseta);
    }

    void trimDelimiters(runes_t& str, uchar32 _left, uchar32 _right)
    {
        trimLeft(str, _left);
        trimRight(str, _right);
    }

    void trim(crunes_t& str)
    {
        uchar32  charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t charset(charseta, 0, 2, 2);
        trimLeft(str, charseta);
        trimRight(str, charseta);
    }

    void trimLeft(crunes_t& str)
    {
        uchar32  charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t charset(charseta, 0, 2, 2);
        trimLeft(str, charseta);
    }

    void trimRight(crunes_t& str)
    {
        uchar32  charseta[] = {' ', '\t', utf32::TERMINATOR};
        crunes_t charset(charseta, 0, 2, 2);
        trimRight(str, charseta);
    }

    void trim(crunes_t& str, uchar32 _c)
    {
        uchar32  charseta[] = {_c, utf32::TERMINATOR};
        crunes_t charset(charseta, 0, 1, 1);
        trimLeft(str, charseta);
        trimRight(str, charseta);
    }

    void trimLeft(crunes_t& str, uchar32 _c)
    {
        uchar32  charseta[] = {_c, utf32::TERMINATOR};
        crunes_t charset(charseta, 0, 1, 1);
        trimLeft(str, charseta);
    }

    void trimRight(crunes_t& str, uchar32 _c)
    {
        uchar32  charseta[2] = {_c, utf32::TERMINATOR};
        crunes_t charset(charseta, 0, 1, 1);
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
            uchar32 c1    = read(_str, iter);
            u32     citer = get_begin(_charset);
            trim          = false;
            while (citer < cend)
            {
                uchar32 c2 = read(_charset, citer);
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
            uchar32 c1    = peek(_str, iter);
            u32     citer = get_begin(_charset);
            trim          = false;
            while (citer < cend)
            {
                uchar32 c2 = read(_charset, citer);
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
        uchar32  charseta[] = {'\'', '"', utf32::TERMINATOR};
        crunes_t charset(charseta, 0, 2, 2);
        trim(str, charseta);
    }

    void trimQuotes(crunes_t& str, uchar32 quote)
    {
        uchar32  charseta[] = {quote, utf32::TERMINATOR};
        crunes_t charset(charseta, 0, 1, 1);
        trim(str, charseta);
    }

    void trimDelimiters(crunes_t& str, uchar32 _left, uchar32 _right)
    {
        trimLeft(str, _left);
        trimRight(str, _right);
    }

    void resize(runes_t& str, s32 cap, runes_alloc_t* allocator, s32 size_alignment)
    {
        runes_t nstr = allocator->allocate(0, cap, str.get_type());
        if (str.is_valid())
        {
            copy(str, nstr);
            allocator->deallocate(str);
        }
        str = nstr;
    }

    void copy(crunes_t const& _src, runes_t& _dst)
    {
        u32 siter  = get_begin(_src);
        u32 send   = get_end(_src);
        u32 dbegin = get_begin(_dst);
        u32 diter  = get_begin(_dst);
        u32 dend   = get_eos(_dst);
        while (diter < dend && siter < send)
        {
            uchar32 c = read(_src, siter);
            if (c == 0)
                break;
            write(_dst, diter, c);
        }
        _dst = select(_dst, dbegin, diter);
    }

    void concatenate(runes_t& _dst, crunes_t const& _con)
    {
        u32 siter  = get_begin(_con);
        u32 send   = get_end(_con);
        u32 dbegin = get_begin(_dst);
        u32 diter  = get_end(_dst);
        u32 dend   = get_eos(_dst);
        while (diter < dend && siter < send)
        {
            uchar32 c = read(_con, siter);
            write(_dst, diter, c);
        }
        _dst = select(_dst, dbegin, diter);
    }

    void copy(const crunes_t& src, runes_t& dst, runes_alloc_t* allocator, s32 size_alignment)
    {
        s32 const required = src.size();
        if (required > dst.cap())
        {
            allocator->deallocate(dst);
            resize(dst, required, allocator, size_alignment);
        }
        copy(src, dst);
    }

    void concatenate(runes_t& str, const crunes_t& concat, runes_alloc_t* allocator, s32 size_alignment)
    {
        s32 const required = str.size() + concat.size();
        if (required > str.cap())
        {
            resize(str, required, allocator, size_alignment);
        }
        concatenate(str, concat);
    }

    void concatenate(runes_t& str, const crunes_t& concat1, const crunes_t& concat2, runes_alloc_t* allocator, s32 size_alignment)
    {
        s32 const required = str.size() + concat1.size() + concat2.size();
        if (required > str.cap())
        {
            resize(str, required, allocator, size_alignment);
        }
        concatenate(str, concat1);
        concatenate(str, concat2);
    }

    // ---------------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------------
    // u32 functions
    inline u32 get_begin(runes_t const& str) { return str.m_ascii.m_str; }
    inline u32 get_end(runes_t const& str) { return str.m_ascii.m_end; }
    inline u32 get_eos(runes_t const& str) { return str.m_ascii.m_eos; }

    static runes_t select(runes_t const& str, u32 from, u32 to)
    {
        runes_t r;
        r.m_ascii.m_flags = str.get_type();
        r.m_ascii.m_bos   = str.m_ascii.m_bos;
        r.m_ascii.m_str   = from;
        r.m_ascii.m_end   = to;
        r.m_ascii.m_eos   = str.m_ascii.m_eos;
        return r;
    }
    bool forwards(runes_t const& str, u32& cursor, s32 step)
    {
        switch (str.get_type())
        {
            case ascii::TYPE: return utf::skip(str.m_ascii.m_bos, cursor, str.m_ascii.m_str, str.m_ascii.m_end, step);
            case utf8::TYPE: return utf::skip(str.m_utf8.m_bos, cursor, str.m_utf8.m_str, str.m_utf8.m_end, step);
            case utf16::TYPE: return utf::skip(str.m_utf16.m_bos, cursor, str.m_utf16.m_str, str.m_utf16.m_end, step);
            case utf32::TYPE: return utf::skip(str.m_utf32.m_bos, cursor, str.m_utf32.m_str, str.m_utf32.m_end, step);
        }
        return false;
    }
    bool backwards(runes_t const& str, u32& cursor, s32 step)
    {
        switch (str.get_type())
        {
            case ascii::TYPE: return utf::skip(str.m_ascii.m_bos, cursor, str.m_ascii.m_str, str.m_ascii.m_end, -step);
            case utf8::TYPE: return utf::skip(str.m_utf8.m_bos, cursor, str.m_utf8.m_str, str.m_utf8.m_end, -step);
            case utf16::TYPE: return utf::skip(str.m_utf16.m_bos, cursor, str.m_utf16.m_str, str.m_utf16.m_end, -step);
            case utf32::TYPE: return utf::skip(str.m_utf32.m_bos, cursor, str.m_utf32.m_str, str.m_utf32.m_end, -step);
        }
        return false;
    }
    uchar32 peek(runes_t const& str, u32 cursor)
    {
        switch (str.get_type())
        {
            case ascii::TYPE: return utf::peek(str.m_ascii, cursor);
            case utf8::TYPE: return utf::peek(str.m_utf8, cursor);
            case utf16::TYPE: return utf::peek(str.m_utf16, cursor);
            case utf32::TYPE: return utf::peek(str.m_utf32, cursor);
        }
        return '\0';
    }
    uchar32 read(runes_t const& str, u32& cursor)
    {
        switch (str.get_type())
        {
            case ascii::TYPE: return utf::read(str.m_ascii.m_bos, cursor, str.m_ascii.m_end);
            case utf8::TYPE: return utf::read(str.m_utf8.m_bos, cursor, str.m_utf8.m_end);
            case utf16::TYPE: return utf::read(str.m_utf16.m_bos, cursor, str.m_utf16.m_end);
            case utf32::TYPE: return utf::read(str.m_utf32.m_bos, cursor, str.m_utf32.m_end);
        }
        return '\0';
    }
    bool write(runes_t const& str, u32& cursor, uchar32 c)
    {
        switch (str.get_type())
        {
            case ascii::TYPE: return utf::write(c, str.m_ascii.m_bos, cursor, str.m_ascii.m_eos); break;
            case utf16::TYPE: return utf::write(c, str.m_utf16.m_bos, cursor, str.m_utf16.m_eos); break;
            case utf32::TYPE: return utf::write(c, str.m_utf32.m_bos, cursor, str.m_utf32.m_eos); break;
            default: return false;
        }
        return true;
    }

    static inline bool is_valid(runes_t const& str, u32 cursor) { return cursor >= str.m_ascii.m_str && cursor < str.m_ascii.m_end; }

    // ---------------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------------
    // u32 functions
    static inline u32 get_begin(crunes_t const& str) { return u32(str.m_ascii.m_str); }
    static inline u32 get_end(crunes_t const& str) { return u32(str.m_ascii.m_end); }
    static inline u32 get_eos(crunes_t const& str) { return u32(str.m_ascii.m_eos); }

    static crunes_t select(crunes_t const& str, u32 from, u32 to)
    {
        crunes_t r;
        r.m_ascii.m_flags = str.m_ascii.m_flags;
        r.m_ascii.m_bos   = str.m_ascii.m_bos;
        r.m_ascii.m_eos   = str.m_ascii.m_eos;
        r.m_ascii.m_str   = from;
        r.m_ascii.m_end   = to;
        return r;
    }

    static bool forwards(crunes_t const& str, u32& cursor, s32 step)
    {
        switch (str.get_type())
        {
            case ascii::TYPE: return utf::skip(str.m_ascii.m_bos, cursor, str.m_ascii.m_str, str.m_ascii.m_end, step);
            case utf8::TYPE: return utf::skip(str.m_utf8.m_bos, cursor, str.m_utf8.m_str, str.m_utf8.m_end, step);
            case utf16::TYPE: return utf::skip(str.m_utf16.m_bos, cursor, str.m_utf16.m_str, str.m_utf16.m_end, step);
            case utf32::TYPE: return utf::skip(str.m_utf32.m_bos, cursor, str.m_utf32.m_str, str.m_utf32.m_end, step);
        }
        return false;
    }
    bool backwards(crunes_t const& str, u32& cursor, s32 step)
    {
        switch (str.get_type())
        {
            case ascii::TYPE: return utf::skip(str.m_ascii.m_bos, cursor, str.m_ascii.m_str, str.m_ascii.m_end, -step);
            case utf8::TYPE: return utf::skip(str.m_utf8.m_bos, cursor, str.m_utf8.m_str, str.m_utf8.m_end, -step);
            case utf16::TYPE: return utf::skip(str.m_utf16.m_bos, cursor, str.m_utf16.m_str, str.m_utf16.m_end, -step);
            case utf32::TYPE: return utf::skip(str.m_utf32.m_bos, cursor, str.m_utf32.m_str, str.m_utf32.m_end, -step);
        }
        return false;
    }

    uchar32 peek(crunes_t const& str, u32 const& cursor)
    {
        uchar32 c = '\0';
        switch (str.get_type())
        {
            case ascii::TYPE: c = utf::peek(str.m_ascii, cursor); break;
            case utf8::TYPE: c = utf::peek(str.m_utf8, cursor); break;
            case utf16::TYPE: c = utf::peek(str.m_utf16, cursor); break;
            case utf32::TYPE: c = utf::peek(str.m_utf32, cursor); break;
        }

        return c;
    }
    uchar32 read(crunes_t const& str, u32& cursor)
    {
        uchar32 c = '\0';
        switch (str.get_type())
        {
            case ascii::TYPE: c = utf::read(str.m_ascii.m_bos, cursor, str.m_ascii.m_end); break;
            case utf8::TYPE: c = utf::read(str.m_utf8.m_bos, cursor, str.m_utf8.m_end); break;
            case utf16::TYPE: c = utf::read(str.m_utf16.m_bos, cursor, str.m_utf16.m_end); break;
            case utf32::TYPE: c = utf::read(str.m_utf32.m_bos, cursor, str.m_utf32.m_end); break;
        }

        return c;
    }

    static inline bool is_valid(crunes_t const& str, u32 const& cursor) { return cursor >= str.m_ascii.m_str && cursor < str.m_ascii.m_end; }

    // ------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------

    runes_t::runes_t()
    {
        m_ascii.m_bos = nullptr;
        m_ascii.m_str = 0;
        m_ascii.m_end = 0;
        m_ascii.m_eos = 0;
    }

    runes_t::runes_t(runes_t const& other)
    {
        m_ascii.m_bos   = other.m_ascii.m_bos;
        m_ascii.m_str   = other.m_ascii.m_str;
        m_ascii.m_end   = other.m_ascii.m_end;
        m_ascii.m_eos   = other.m_ascii.m_eos;
        m_ascii.m_flags = other.m_ascii.m_flags;
    }

    runes_t::runes_t(ascii::prune _str, ascii::prune _end, u32 _type)
    {
        m_ascii.m_bos   = _str;
        m_ascii.m_str   = 0;
        m_ascii.m_end   = (u32)(_end - _str);
        m_ascii.m_eos   = (u32)(_end - _str);
        m_ascii.m_flags = _type;
    }
    runes_t::runes_t(ascii::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        m_ascii.m_bos   = _bos;
        m_ascii.m_str   = _str;
        m_ascii.m_end   = _end;
        m_ascii.m_eos   = _eos;
        m_ascii.m_flags = _type;
    }

    runes_t::runes_t(utf8::prune _str, utf8::prune _end, u32 _type)
    {
        m_utf8.m_bos    = _str;
        m_utf8.m_str    = 0;
        m_utf8.m_eos    = (u32)(_end - _str);
        m_utf8.m_end    = (u32)(_end - _str);
        m_ascii.m_flags = _type;
    }
    runes_t::runes_t(utf8::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        m_utf8.m_bos    = _bos;
        m_utf8.m_str    = _str;
        m_utf8.m_end    = _end;
        m_utf8.m_eos    = _eos;
        m_ascii.m_flags = _type;
    }

    runes_t::runes_t(utf16::prune _str, utf16::prune _end, u32 _type)
    {
        m_utf16.m_bos   = _str;
        m_utf16.m_str   = 0;
        m_utf16.m_eos   = (u32)(_end - _str);
        m_utf16.m_end   = (u32)(_end - _str);
        m_ascii.m_flags = _type;
    }
    runes_t::runes_t(utf16::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        m_utf16.m_bos   = _bos;
        m_utf16.m_str   = _str;
        m_utf16.m_end   = _end;
        m_utf16.m_eos   = _eos;
        m_ascii.m_flags = _type;
    }

    runes_t::runes_t(utf32::prune _str, utf32::prune _end, u32 _type)
    {
        m_utf32.m_bos   = _str;
        m_utf32.m_str   = 0;
        m_utf32.m_eos   = (u32)(_end - _str);
        m_utf32.m_end   = (u32)(_end - _str);
        m_ascii.m_flags = _type;
    }
    runes_t::runes_t(utf32::prune _bos, u32 _str, u32 _end, u32 _eos, u32 _type)
    {
        m_utf32.m_bos   = _bos;
        m_utf32.m_str   = _str;
        m_utf32.m_end   = _end;
        m_utf32.m_eos   = _eos;
        m_ascii.m_flags = _type;
    }

    s32 runes_t::size() const
    {
        s32 size = 0;
        switch (get_type())
        {
            case ascii::TYPE: size = (s32)((m_ascii.m_end - m_ascii.m_str)); break;
            case utf32::TYPE: size = (s32)((m_utf32.m_end - m_utf32.m_str)); break;
            case utf16::TYPE:
            {
                u32 str = m_utf16.m_str;
                while (m_utf16.m_bos[str] != 0)
                {
                    size += 1;
                    utf::read(m_utf16, str);
                }
                break;
            }
            case utf8::TYPE:
            {
                u32 str = m_utf8.m_str;
                while (m_utf8.m_bos[str] != 0)
                {
                    size += 1;
                    utf::read(m_utf8, str);
                }
                break;
            }
        }
        return size;
    }
    s32 runes_t::cap() const
    {
        s32 cap = 0;
        switch (get_type())
        {
            case ascii::TYPE: cap = (s32)((m_ascii.m_eos - m_ascii.m_str)); break;
            case utf16::TYPE: cap = (s32)((m_utf16.m_eos - m_utf16.m_str)); break;
            case utf32::TYPE: cap = (s32)((m_utf32.m_eos - m_utf32.m_str)); break;
        }
        return cap;
    }

    bool runes_t::is_empty() const { return size() == 0; }
    bool runes_t::is_valid() const { return m_ascii.m_end < m_ascii.m_eos; }
    bool runes_t::is_nil() const { return m_ascii.m_bos == nullptr; }
    void runes_t::reset() { m_ascii.m_end = m_ascii.m_str; }
    void runes_t::clear()
    {
        m_ascii.m_str = 0;
        m_ascii.m_end = 0;
    }
    void runes_t::term()
    {
        switch (get_type())
        {
            case ascii::TYPE: m_ascii.m_bos[m_ascii.m_end++] = ascii::TERMINATOR; break;
            case utf32::TYPE: m_utf32.m_bos[m_utf32.m_end++] = utf32::TERMINATOR; break;
            case utf16::TYPE: m_utf16.m_bos[m_utf16.m_end++] = utf16::TERMINATOR; break;
            case utf8::TYPE: m_utf8.m_bos[m_utf8.m_end++] = utf8::TERMINATOR; break;
        }
    }

    uchar32 runes_t::peek() const
    {
        if (m_ascii.m_str < m_ascii.m_end)
        {
            uchar32 c;
            switch (get_type())
            {
                case ascii::TYPE: c = m_ascii.m_bos[m_ascii.m_str]; break;
                case utf8::TYPE: c = utf::peek(m_utf8, m_utf8.m_str); break;
                case utf16::TYPE: c = utf::peek(m_utf16, m_utf16.m_str); break;
                case utf32::TYPE: c = m_utf32.m_bos[m_utf32.m_str]; break;
                default: c = '\0'; break;
            }
            return c;
        }
        return '\0';
    }

    uchar32 runes_t::read()
    {
        if (m_ascii.m_str < m_ascii.m_end)
        {
            uchar32 c;
            switch (get_type())
            {
                case ascii::TYPE: c = m_ascii.m_bos[m_ascii.m_str++]; break;
                case utf8::TYPE: c = utf::read(m_utf8, m_utf8.m_end); break;
                case utf16::TYPE: c = utf::read(m_utf16, m_utf16.m_end); break;
                case utf32::TYPE: c = m_utf32.m_bos[m_utf32.m_str++]; break;
                default: c = '\0'; break;
            }
            return c;
        }
        return '\0';
    }

    bool runes_t::write(uchar32 c)
    {
        if (m_ascii.m_end < m_ascii.m_eos)
        {
            switch (get_type())
            {
                case ascii::TYPE: m_ascii.m_bos[m_ascii.m_end++] = c; return true;
                case utf8::TYPE: utf::write(c, m_utf8.m_bos, m_utf8.m_end, m_utf8.m_eos); return true;
                case utf16::TYPE: utf::write(c, m_utf16.m_bos, m_utf16.m_end, m_utf16.m_eos); return true;
                case utf32::TYPE: m_utf32.m_bos[m_utf32.m_end++] = c; return true;
                default: c = '\0'; break;
            }
        }
        return false;
    }

    bool runes_t::scan(u32& cursor, const crunes_t& until_chars, uchar32& encountered) const
    {
        uchar32 c;
        switch (get_type())
        {
            case ascii::TYPE:
                while (cursor < m_ascii.m_end)
                {
                    c = m_ascii.m_bos[cursor];
                    if (until_chars.contains(c))
                    {
                        encountered = c;
                        return true;
                    }
                    cursor++;
                }
                break;
            case utf8::TYPE:
                while (cursor < m_utf8.m_end)
                {
                    c = utf::peek(m_utf8, cursor);
                    if (until_chars.contains(c))
                    {
                        encountered = c;
                        return true;
                    }
                    utf::skip(m_utf8.m_bos, cursor, m_utf8.m_str, m_utf8.m_end, 1);
                }
                break;
            case utf16::TYPE:
                while (cursor < m_utf16.m_end)
                {
                    c = utf::peek(m_utf16, cursor);
                    if (until_chars.contains(c))
                    {
                        encountered = c;
                        return true;
                    }
                    utf::skip(m_utf8.m_bos, cursor, m_utf16.m_str, m_utf16.m_end, 1);
                }
                break;
            case utf32::TYPE:
                while (cursor < m_utf32.m_end)
                {
                    c = utf::peek(m_utf32, cursor);
                    if (until_chars.contains(c))
                    {
                        encountered = c;
                        return true;
                    }
                    utf::skip(m_utf8.m_bos, cursor, m_utf32.m_str, m_utf32.m_end, 1);
                }
                break;
        }
        encountered = cEOS;
        return true;
    }

    void runes_t::skip(u32& cursor, const crunes_t& skip_chars) const
    {
        switch (get_type())
        {
            case ascii::TYPE:
            {
                while (cursor < m_ascii.m_end)
                {
                    uchar32 c = m_ascii.m_bos[cursor];
                    if (!skip_chars.contains(c))
                        break;
                    cursor++;
                }
            }
            break;
            case utf8::TYPE:
            {
                while (cursor < m_utf8.m_end)
                {
                    uchar32 c = utf::peek(m_utf8, cursor);
                    if (!skip_chars.contains(c))
                        break;
                    utf::skip(m_utf8.m_bos, cursor, m_utf8.m_str, m_utf8.m_end, 1);
                }
            }
            break;
            case utf16::TYPE:
            {
                while (cursor < m_utf16.m_end)
                {
                    uchar32 c = utf::peek(m_utf16, cursor);
                    if (!skip_chars.contains(c))
                        break;
                    utf::skip(m_utf16.m_bos, cursor, m_utf16.m_str, m_utf16.m_end, 1);
                }
            }
            break;
            case utf32::TYPE:
            {
                while (cursor < m_utf32.m_end)
                {
                    uchar32 c = utf::peek(m_utf32, cursor);
                    if (!skip_chars.contains(c))
                        break;
                    utf::skip(m_utf32.m_bos, cursor, m_utf32.m_str, m_utf32.m_end, 1);
                }
            }
            break;
        }
    }

    runes_t& runes_t::operator+=(const ascii::crunes_t& str)
    {
        crunes_t cstr(str);
        concatenate(*this, cstr);
        return *this;
    }
    runes_t& runes_t::operator+=(const utf32::crunes_t& str)
    {
        crunes_t cstr(str);
        concatenate(*this, cstr);
        return *this;
    }
    runes_t& runes_t::operator+=(ascii::rune c)
    {
        ascii::rune str[2];
        str[0] = c;
        str[1] = ascii::TERMINATOR;
        crunes_t cstr(str, &str[1]);
        concatenate(*this, cstr);
        return *this;
    }
    runes_t& runes_t::operator+=(utf32::rune c)
    {
        utf32::rune str[2];
        str[0] = c;
        str[1] = utf32::TERMINATOR;
        crunes_t cstr(str, &str[1]);
        concatenate(*this, cstr);
        return *this;
    }
    runes_t& runes_t::operator=(runes_t const& other)
    {
        m_ascii = other.m_ascii;
        return *this;
    }

    // ------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------
    static const char* sNullAsciiString = "\0\0\0\0";

    crunes_t::crunes_t()
    {
        m_ascii.m_bos   = sNullAsciiString;
        m_ascii.m_str   = 0;
        m_ascii.m_end   = 0;
        m_ascii.m_eos   = 0;
        m_ascii.m_flags = ascii::TYPE;
    }
    crunes_t::crunes_t(runes_t const& _str) { m_ascii = _str.m_ascii; }
    crunes_t::crunes_t(crunes_t const& _str) { m_ascii = _str.m_ascii; }
    crunes_t::crunes_t(crunes_t const& _str, u32 from, u32 to)
    {
        ASSERT(from >= _str.m_ascii.m_str && from < _str.m_ascii.m_eos);
        ASSERT(to >= _str.m_ascii.m_str && to < _str.m_ascii.m_eos);
        ASSERT(from <= to);
        m_ascii.m_bos = _str.m_ascii.m_bos;
        m_ascii.m_str = from;
        m_ascii.m_end = to;
        m_ascii.m_eos = _str.m_ascii.m_eos;
    }
    crunes_t::crunes_t(ascii::crunes_t const& _str) { m_ascii = _str; }
    crunes_t::crunes_t(ascii::pcrune _str)
    {
        m_ascii.m_bos     = _str;
        m_ascii.m_str     = 0;
        ascii::pcrune end = utf::endof(_str, nullptr);
        m_ascii.m_end     = (u32)(end - _str);
        m_ascii.m_eos     = m_ascii.m_end;
        m_ascii.m_flags   = ascii::TYPE;
    }

    crunes_t::crunes_t(ascii::pcrune _str, ascii::pcrune _end)
    {
        m_ascii.m_bos   = _str;
        m_ascii.m_str   = 0;
        m_ascii.m_end   = (u32)(_end - _str);
        m_ascii.m_eos   = (u32)(_end - _str);
        m_ascii.m_flags = ascii::TYPE;
    }
    crunes_t::crunes_t(ascii::pcrune _bos, u32 _str, u32 _end, u32 _eos, u32 _flags)
    {
        m_ascii.m_bos   = _bos;
        m_ascii.m_str   = _str;
        m_ascii.m_end   = _end;
        m_ascii.m_eos   = _eos;
        m_ascii.m_flags = _flags;
    }

    crunes_t::crunes_t(utf8::crunes_t const& _str) { m_utf8 = _str; }
    crunes_t::crunes_t(utf8::pcrune _str)
    {
        m_utf8.m_bos     = _str;
        m_utf8.m_str     = 0;
        utf8::pcrune end = utf::endof(_str, nullptr);
        m_utf8.m_end     = (u32)(end - _str);
        m_utf8.m_eos     = m_utf8.m_end;
        m_utf8.m_flags   = utf8::TYPE;
    }
    crunes_t::crunes_t(utf8::pcrune _str, utf8::pcrune _end)
    {
        m_utf8.m_bos   = _str;
        m_utf8.m_str   = 0;
        m_utf8.m_end   = (u32)(_end - _str);
        m_utf8.m_eos   = (u32)(_end - _str);
        m_utf8.m_flags = utf8::TYPE;
    }
    crunes_t::crunes_t(utf8::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        m_utf8.m_bos   = _bos;
        m_utf8.m_str   = _str;
        m_utf8.m_end   = _end;
        m_utf8.m_eos   = _eos;
        m_utf8.m_flags = utf8::TYPE;
    }

    crunes_t::crunes_t(utf16::crunes_t const& _str) { m_utf16 = _str; }
    crunes_t::crunes_t(utf16::pcrune _str)
    {
        m_utf16.m_bos     = _str;
        m_utf16.m_str     = 0;
        utf16::pcrune end = utf::endof(_str, nullptr);
        m_utf16.m_end     = (u32)(end - _str);
        m_utf16.m_eos     = m_utf16.m_end;
        m_utf16.m_flags   = utf16::TYPE;
    }
    crunes_t::crunes_t(utf16::pcrune _str, utf16::pcrune _end)
    {
        m_utf16.m_bos   = _str;
        m_utf16.m_str   = 0;
        m_utf16.m_end   = (u32)(_end - _str);
        m_utf16.m_eos   = (u32)(_end - _str);
        m_utf16.m_flags = utf16::TYPE;
    }
    crunes_t::crunes_t(utf16::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        m_utf16.m_bos   = _bos;
        m_utf16.m_str   = _str;
        m_utf16.m_end   = _end;
        m_utf16.m_eos   = _eos;
        m_utf16.m_flags = utf16::TYPE;
    }

    crunes_t::crunes_t(utf32::crunes_t const& _str) { m_utf32 = _str; }
    crunes_t::crunes_t(utf32::pcrune _str)
    {
        m_utf32.m_bos     = _str;
        m_utf32.m_str     = 0;
        utf32::pcrune end = utf::endof(_str, nullptr);
        m_utf32.m_end     = (u32)(end - _str);
        m_utf32.m_eos     = m_utf32.m_end;
        m_utf32.m_flags   = utf32::TYPE;
    }
    crunes_t::crunes_t(utf32::pcrune _str, utf32::pcrune _end)
    {
        m_utf32.m_bos   = _str;
        m_utf32.m_str   = 0;
        m_utf32.m_eos   = (u32)(_end - _str);
        m_utf32.m_end   = (u32)(_end - _str);
        m_utf32.m_flags = utf32::TYPE;
    }
    crunes_t::crunes_t(utf32::pcrune _bos, u32 _str, u32 _end, u32 _eos)
    {
        m_utf32.m_bos   = _bos;
        m_utf32.m_str   = _str;
        m_utf32.m_end   = _end;
        m_utf32.m_eos   = _eos;
        m_utf32.m_flags = utf32::TYPE;
    }

    crunes_t& crunes_t::operator=(crunes_t const& other)
    {
        m_ascii = other.m_ascii;
        return *this;
    }

    s32 crunes_t::size() const
    {
        s32 size = 0;
        switch (get_type())
        {
            case ascii::TYPE: size = (s32)((m_ascii.m_end - m_ascii.m_str)); break;
            case utf32::TYPE: size = (s32)((m_utf32.m_end - m_utf32.m_str)); break;
            case utf16::TYPE:
            {
                utf16::pcrune end = m_utf16.m_bos + m_utf16.m_end;
                utf16::pcrune eos = m_utf16.m_bos + m_utf16.m_eos;
                size              = utf16::strlen(m_utf16.m_bos, end, eos);
            }
            break;
            case utf8::TYPE:
            {
                utf8::pcrune end = m_utf8.m_bos + m_utf8.m_end;
                utf8::pcrune eos = m_utf8.m_bos + m_utf8.m_eos;
                size             = utf8::strlen(m_utf8.m_bos, end, eos);
            }
            break;
            default: ASSERT(false); break;
        }
        return size;
    }
    bool crunes_t::is_empty() const { return m_ascii.m_end == m_ascii.m_str; }
    bool crunes_t::is_valid() const { return m_ascii.m_end <= m_ascii.m_eos; }
    bool crunes_t::is_nil() const { return m_ascii.m_bos == nullptr; }
    void crunes_t::reset() { m_ascii.m_end = m_ascii.m_str; }
    void crunes_t::clear()
    {
        m_ascii.m_str = 0;
        m_ascii.m_end = 0;
    }

    uchar32 crunes_t::peek() const
    {
        if (m_ascii.m_str < m_ascii.m_end)
        {
            uchar32 c;
            switch (get_type())
            {
                case ascii::TYPE: c = m_ascii.m_bos[m_ascii.m_str]; break;
                case utf8::TYPE: c = utf::peek(m_utf8, m_utf8.m_str); break;
                case utf16::TYPE: c = utf::peek(m_utf16, m_utf16.m_str); break;
                case utf32::TYPE: c = m_utf32.m_bos[m_utf32.m_str]; break;
                default: ASSERT(false); break;
            }
            return c;
        }
        return '\0';
    }

    uchar32 crunes_t::read()
    {
        if (m_ascii.m_str < m_ascii.m_end)
        {
            uchar32 c;
            switch (get_type())
            {
                case ascii::TYPE: c = m_ascii.m_bos[m_ascii.m_str++]; break;
                case utf8::TYPE: c = utf::read(m_utf8, m_utf8.m_str); break;
                case utf16::TYPE: c = utf::read(m_utf16, m_utf16.m_str); break;
                case utf32::TYPE: c = m_utf32.m_bos[m_utf32.m_str++]; break;
                default: ASSERT(false); break;
            }
            return c;
        }
        return '\0';
    }

    bool crunes_t::scan(u32& cursor, const crunes_t& until_chars, uchar32& encountered) const
    {
        uchar32 c;
        switch (get_type())
        {
            case ascii::TYPE:
                if (cursor >= m_ascii.m_str)
                {
                    while (cursor < m_ascii.m_end)
                    {
                        c = m_ascii.m_bos[cursor];
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
                if (cursor >= m_utf8.m_str)
                {
                    while (cursor < m_utf8.m_end)
                    {
                        c = utf::peek(m_utf8, cursor);
                        if (until_chars.contains(c))
                        {
                            encountered = c;
                            return true;
                        }
                        utf::skip(m_utf8.m_bos, cursor, m_utf8.m_str, m_utf8.m_end, 1);
                    }
                }
                break;
            case utf16::TYPE:
                if (cursor >= m_utf16.m_str)
                {
                    while (cursor < m_utf16.m_end)
                    {
                        c = utf::peek(m_utf16, cursor);
                        if (until_chars.contains(c))
                        {
                            encountered = c;
                            return true;
                        }
                        utf::skip(m_utf16.m_bos, cursor, m_utf16.m_str, m_utf16.m_end, 1);
                    }
                }
                break;
            case utf32::TYPE:
                if (cursor >= m_utf32.m_str)
                {
                    while (cursor < m_utf32.m_end)
                    {
                        c = utf::peek(m_utf32, cursor);
                        if (until_chars.contains(c))
                        {
                            encountered = c;
                            return true;
                        }
                        utf::skip(m_utf32.m_bos, cursor, m_utf32.m_str, m_utf32.m_end, 1);
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
                while (cursor < m_ascii.m_end)
                {
                    uchar32 c = m_ascii.m_bos[cursor];
                    if (!skip_chars.contains(c))
                        break;
                    cursor++;
                }
                break;
            case utf8::TYPE:
                while (cursor < m_utf8.m_end)
                {
                    uchar32 c = utf::peek(m_utf8, cursor);
                    if (!skip_chars.contains(c))
                        break;
                    utf::skip(m_utf8.m_bos, cursor, m_utf8.m_str, m_utf8.m_end, 1);
                }
                break;
            case utf16::TYPE:
                while (cursor < m_utf16.m_end)
                {
                    uchar32 c = utf::peek(m_utf16, cursor);
                    if (!skip_chars.contains(c))
                        break;
                    utf::skip(m_utf16.m_bos, cursor, m_utf16.m_str, m_utf16.m_end, 1);
                }
                break;
            case utf32::TYPE:
                while (cursor < m_utf32.m_end)
                {
                    uchar32 c = utf::peek(m_utf32, cursor);
                    if (!skip_chars.contains(c))
                        break;
                    utf::skip(m_utf32.m_bos, cursor, m_utf32.m_str, m_utf32.m_end, 1);
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
                u32 cursor = m_ascii.m_str;
                while (cursor < m_ascii.m_end)
                {
                    uchar32 c = m_ascii.m_bos[cursor];
                    if (c == contains)
                        return true;
                    cursor++;
                }
            }
            break;
            case utf8::TYPE:
            {
                u32 cursor = m_utf8.m_str;
                while (cursor < m_utf8.m_end)
                {
                    uchar32 c = utf::peek(m_utf8, cursor);
                    if (c == contains)
                        return true;
                    utf::skip(m_utf8.m_bos, cursor, m_utf8.m_str, m_utf8.m_end, 1);
                }
            }
            break;
            case utf16::TYPE:
            {
                u32 cursor = m_utf16.m_str;
                while (cursor < m_utf16.m_end)
                {
                    uchar32 c = utf::peek(m_utf16, cursor);
                    if (c == contains)
                        return true;
                    utf::skip(m_utf16.m_bos, cursor, m_utf16.m_str, m_utf16.m_end, 1);
                }
            }
            break;
            case utf32::TYPE:
            {
                u32 cursor = m_utf32.m_str;
                while (cursor < m_utf32.m_end)
                {
                    uchar32 c = utf::peek(m_utf32, cursor);
                    if (c == contains)
                        return true;
                    utf::skip(m_utf32.m_bos, cursor, m_utf32.m_str, m_utf32.m_end, 1);
                }
            }
            default:
                ASSERT(false);
                break;
                break;
        }
        return false;
    }

    // ------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------
    runes_reader_t::runes_reader_t() {}

    runes_reader_t::runes_reader_t(ascii::pcrune str)
    {
        ascii::pcrune str_end = str;
        while (*str_end != '\0')
            str_end++;
        u32 const len = (u32)(str_end - str);
        m_runes       = crunes_t(str, 0, len, len);
        m_cursor      = 0;
    }
    runes_reader_t::runes_reader_t(ascii::pcrune str, u32 len)
    {
        m_runes  = crunes_t(str, 0, len, len);
        m_cursor = 0;
    }
    runes_reader_t::runes_reader_t(ascii::pcrune str, ascii::pcrune str_end)
    {
        u32 const len = (u32)(str_end - str);
        m_runes       = crunes_t(str, 0, len, len);
        m_cursor      = 0;
    }
    runes_reader_t::runes_reader_t(utf8::pcrune str, utf8::pcrune str_end)
    {
        u32 const len = (u32)(str_end - str);
        m_runes       = crunes_t(str, 0, len, len);
        m_cursor      = 0;
    }
    runes_reader_t::runes_reader_t(utf16::pcrune str, utf16::pcrune str_end)
    {
        u32 const len = (u32)(str_end - str);
        m_runes       = crunes_t(str, 0, len, len);
        m_cursor      = 0;
    }
    runes_reader_t::runes_reader_t(utf32::pcrune str, utf32::pcrune str_end)
    {
        u32 const len = (u32)(str_end - str);
        m_runes       = crunes_t(str, 0, len, len);
        m_cursor      = 0;
    }
    runes_reader_t::runes_reader_t(crunes_t const& runes)
        : m_runes(runes)
    {
        m_cursor = m_runes.m_ascii.m_str;
    }

    crunes_t runes_reader_t::get_source() const
    {
        crunes_t str(m_runes);
        return str;
    }

    crunes_t runes_reader_t::get_current() const
    {
        crunes_t str(m_runes);
        str.m_ascii.m_str = m_cursor;
        return str;
    }

    bool runes_reader_t::at_end() const { return m_cursor >= m_runes.m_ascii.m_end; }
    bool runes_reader_t::vvalid() const { return m_cursor >= m_runes.m_ascii.m_str && m_cursor < m_runes.m_ascii.m_end; }

    void runes_reader_t::vreset() { m_cursor = m_runes.m_ascii.m_str; }

    uchar32 runes_reader_t::vpeek(s32 n) const
    {
        uchar32 c = '\0';
        if (!at_end() && n == 0)
        {
            switch (m_runes.get_type())
            {
                case ascii::TYPE: c = m_runes.m_ascii.m_bos[m_cursor] & 0xff; break;
                case utf8::TYPE: c = utf::peek(m_runes.m_utf8, m_cursor); break;
                case utf16::TYPE: c = utf::peek(m_runes.m_utf16, m_cursor); break;
                case utf32::TYPE: c = m_runes.m_utf32.m_bos[m_cursor]; break;
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

    bool runes_reader_t::vread(uchar32& c)
    {
        if (!at_end())
        {
            switch (m_runes.get_type())
            {
                case ascii::TYPE: c = m_runes.m_ascii.m_bos[m_cursor++] & 0xff; break;
                case utf8::TYPE: c = utf::read(m_runes.m_utf8, m_cursor); break;
                case utf16::TYPE: c = utf::read(m_runes.m_utf16, m_cursor); break;
                case utf32::TYPE: c = m_runes.m_utf32.m_bos[m_cursor++]; break;
                default: ASSERT(false); break;
            }
            return true;
        }
        c = '\0';
        return false;
    }

    bool runes_reader_t::vread_line(runes_t& line)
    {
        // TODO IMPLEMENTATION
        return false;
    }

    bool runes_reader_t::vview_line(crunes_t& line)
    {
        if (at_end())
            return false;

        line.m_ascii.m_flags = m_runes.m_ascii.m_flags;
        switch (m_runes.m_ascii.m_flags)
        {
            case ascii::TYPE:
                line.m_ascii.m_bos = m_runes.m_ascii.m_bos;
                line.m_ascii.m_str = m_cursor;
                line.m_ascii.m_eos = m_runes.m_ascii.m_eos;
                line.m_ascii.m_end = m_runes.m_ascii.m_end;
                while (!at_end() && line.m_ascii.m_bos[m_cursor] != '\n')
                    m_cursor++;
                if (!at_end())
                    m_cursor++;
                line.m_ascii.m_end = m_cursor;
                break;
            case utf8::TYPE:
                line.m_utf8.m_bos = m_runes.m_utf8.m_bos;
                line.m_utf8.m_str = m_cursor;
                line.m_utf8.m_eos = m_runes.m_utf8.m_eos;
                line.m_utf8.m_end = m_runes.m_utf8.m_end;
                while (!at_end() && utf::peek(m_runes.m_utf8, m_cursor) != '\n')
                    utf::read(m_runes.m_utf8, m_cursor);
                if (!at_end())
                    utf::read(m_runes.m_utf8, m_cursor);
                line.m_utf8.m_end = m_cursor;
                break;
            case utf16::TYPE:
                line.m_utf16.m_bos = m_runes.m_utf16.m_bos;
                line.m_utf16.m_str = m_cursor;
                line.m_utf16.m_eos = m_runes.m_utf16.m_eos;
                line.m_utf16.m_end = m_runes.m_utf16.m_end;
                while (!at_end() && utf::peek(m_runes.m_utf16, m_cursor) != '\n')
                    utf::read(m_runes.m_utf16, m_cursor);
                if (!at_end())
                    utf::read(m_runes.m_utf16, m_cursor);
                line.m_utf16.m_end = m_cursor;
                break;
            case utf32::TYPE:
                line.m_utf32.m_bos = m_runes.m_utf32.m_bos;
                line.m_utf32.m_str = m_cursor;
                line.m_utf32.m_eos = m_runes.m_utf32.m_eos;
                line.m_utf32.m_end = m_runes.m_utf32.m_end;
                while (!at_end() && line.m_utf32.m_bos[m_cursor] != '\n')
                    m_cursor++;
                if (!at_end())
                    m_cursor++;
                line.m_utf32.m_end = m_cursor;
                break;
            default: ASSERT(false); break;
        }

        return true;
    }

    void runes_reader_t::vskip(s32 c)
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

            case utf8::TYPE: utf::skip(m_runes.m_utf8.m_bos, m_cursor, m_runes.m_utf8.m_str, m_runes.m_utf8.m_end, c); break;
            case utf16::TYPE: utf::skip(m_runes.m_utf16.m_bos, m_cursor, m_runes.m_utf16.m_str, m_runes.m_utf16.m_end, c); break;

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

    runes_reader_t runes_reader_t::select(u32 const& from, u32 to) const
    {
        runes_reader_t reader;
        if (from >= reader.m_runes.m_ascii.m_str && to <= reader.m_runes.m_ascii.m_eos)
        {
            reader.m_runes.m_ascii.m_bos   = m_runes.m_ascii.m_bos;
            reader.m_runes.m_ascii.m_eos   = m_runes.m_ascii.m_eos;
            reader.m_runes.m_ascii.m_str   = from;
            reader.m_runes.m_ascii.m_end   = to;
            reader.m_runes.m_ascii.m_flags = m_runes.m_ascii.m_flags;
            reader.m_cursor                = from;
        }
        return reader;
    }

    runes_writer_t::runes_writer_t(ascii::prune str, ascii::prune str_end)
    {
        u32 const len = (u32)(str_end - str);
        m_runes       = runes_t(str, 0, len, len, ascii::TYPE);
        m_cursor      = 0;
        m_count       = 0;
    }
    runes_writer_t::runes_writer_t(utf8::prune str, utf8::prune str_end)
    {
        u32 const len = (u32)(str_end - str);
        m_runes       = runes_t(str, 0, len, len, ascii::TYPE);
        m_cursor      = 0;
        m_count       = 0;
    }
    runes_writer_t::runes_writer_t(utf16::prune str, utf16::prune str_end)
    {
        u32 const len = (u32)(str_end - str);
        m_runes       = runes_t(str, 0, len, len, ascii::TYPE);
        m_cursor      = 0;
        m_count       = 0;
    }
    runes_writer_t::runes_writer_t(utf32::prune str, utf32::prune str_end)
    {
        u32 const len = (u32)(str_end - str);
        m_runes       = runes_t(str, 0, len, len, ascii::TYPE);
        m_cursor      = 0;
        m_count       = 0;
    }
    runes_writer_t::runes_writer_t(runes_t const& runes)
        : m_runes(runes)
    {
        m_cursor = m_runes.m_ascii.m_str;
        m_count  = 0;
    }

    runes_t runes_writer_t::get_destination() const
    {
        runes_t str(m_runes);
        return str;
    }

    runes_t runes_writer_t::get_current() const
    {
        runes_t str(m_runes);
        str.m_ascii.m_end = m_cursor;
        return str;
    }

    void runes_writer_t::reset()
    {
        m_cursor = m_runes.m_ascii.m_str;
        m_count  = 0;
    }

    s32 runes_writer_t::count() const { return m_count; }

    static bool at_end(u32 p, runes_t const& str) { return p >= str.m_ascii.m_eos; }

    bool runes_writer_t::vwrite(uchar32 c)
    {
        if (!at_end(m_cursor, m_runes))
        {
            switch (m_runes.get_type())
            {
                case ascii::TYPE:
                    m_runes.m_ascii.m_bos[m_cursor] = c;
                    m_cursor += 1;
                    m_count += 1;
                    return true;
                case utf8::TYPE:
                    utf::write(c, m_runes.m_utf8, m_cursor);
                    m_count += 1;
                    return true;
                case utf16::TYPE:
                    utf::write(c, m_runes.m_utf16, m_cursor);
                    m_count += 1;
                    return true;
                case utf32::TYPE:
                    m_runes.m_utf32.m_bos[m_cursor] = c;
                    m_cursor += 1;
                    m_count += 1;
                    return true;
                default: ASSERT(false); break;
            }
        }
        return false;
    }

    bool runes_writer_t::vwrite(const char* str, const char* end)
    {
        if (!at_end(m_cursor, m_runes))
        {
            uchar32 c;
            switch (m_runes.get_type())
            {
                case ascii::TYPE:
                    while (str < end && !at_end(m_cursor, m_runes))
                    {
                        m_runes.m_ascii.m_bos[m_cursor] = *str++;
                        m_cursor += 1;
                        m_count += 1;
                    }
                    return true;
                case utf8::TYPE:
                    while (str < end && !at_end(m_cursor, m_runes))
                    {
                        c = *str++;
                        utf::write(c, m_runes.m_utf8, m_cursor);
                        m_count += 1;
                    }
                    return true;
                case utf16::TYPE:
                    while (str < end && !at_end(m_cursor, m_runes))
                    {
                        c = *str++;
                        utf::write(c, m_runes.m_utf16, m_cursor);
                        m_count += 1;
                    }
                    return true;
                case utf32::TYPE:
                    while (str < end && !at_end(m_cursor, m_runes))
                    {
                        m_runes.m_utf32.m_bos[m_cursor] = *str++;
                        m_cursor += 1;
                        m_count += 1;
                    }
                    return true;
                default: ASSERT(false); break;
            }
        }
        return false;
    }

    bool runes_writer_t::vwrite(crunes_t const& str)
    {
        if (!at_end(m_cursor, m_runes))
        {
            uchar32        c;
            runes_reader_t reader(str);
            switch (m_runes.get_type())
            {
                case ascii::TYPE:
                    while (reader.read(c) && !at_end(m_cursor, m_runes))
                    {
                        m_runes.m_ascii.m_bos[m_cursor] = (ascii::rune)c;
                        m_cursor += 1;
                        m_count += 1;
                    }
                    return true;
                case utf8::TYPE:
                    while (reader.read(c) && !at_end(m_cursor, m_runes))
                    {
                        utf::write(c, m_runes.m_utf8, m_cursor);
                        m_count += 1;
                    }
                    return true;
                case utf16::TYPE:
                    while (reader.read(c) && !at_end(m_cursor, m_runes))
                    {
                        utf::write(c, m_runes.m_utf16, m_cursor);
                        m_count += 1;
                    }
                    return true;
                case utf32::TYPE:
                    while (reader.read(c) && !at_end(m_cursor, m_runes))
                    {
                        m_runes.m_utf32.m_bos[m_cursor] = c;
                        m_cursor += 1;
                        m_count += 1;
                    }
                    return true;
                default: ASSERT(false); break;
            }
        }
        return false;
    }

    void runes_writer_t::vflush() {}

} // namespace ncore
