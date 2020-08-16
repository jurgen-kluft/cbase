#include "xbase/x_runes.h"
#include "xbase/x_debug.h"
#include "xbase/x_integer.h"
#include "xbase/x_console.h"
#include "xbase/x_va_list.h"
#include "xbase/x_runes.h"
#include "xbase/x_printf.h"

namespace xcore
{
    namespace utf
    {
        inline s32 sequence_sizeof_utf8(uchar8 c)
        {
            u8 lead = c;
            if (lead < 0x80)
                return 1;
            else if ((lead >> 5) == 0x6)
                return 2;
            else if ((lead >> 4) == 0xe)
                return 3;
            else if ((lead >> 3) == 0x1e)
                return 4;
            else
                return 0;
        }

        uchar32 peek(ascii::crunes const& str)
        {
            uchar32 c = utf32::TERMINATOR;
            if (can_read(str))
                c = str.m_str[0];
            return c;
        }

        uchar32 peek(utf8::crunes const& str)
        {
            utf8::pcrune src = str.m_str;
            return read(src, str.m_end);
        }

        uchar32 peek(utf16::crunes const& str)
        {
            utf16::pcrune src = str.m_str;
            return read(src, str.m_end);
        }

        uchar32 peek(utf32::crunes const& str)
        {
            uchar32 c = utf32::TERMINATOR;
            if (can_read(str))
                c = str.m_str[0];
            return c;
        }

        s32 compare(ascii::pcrune stra, ascii::pcrune strb)
        {
            uchar32 ca = *stra;
            uchar32 cb = *strb;
            while (ca != ascii::TERMINATOR && cb != ascii::TERMINATOR)
            {
                if (ca < cb)
                    return -1;
                else if (ca > cb)
                    return 1;
                ca = *(++stra);
                cb = *(++strb);
            }
            if (ca == cb)
                return 0;
            if (ca == ascii::TERMINATOR)
                return -1;
            return 1;
        }

        s32 compare(ascii::pcrune stra, utf32::pcrune strb)
        {
            uchar32 ca = *stra;
            uchar32 cb = *strb;
            while (ca != ascii::TERMINATOR && cb != utf32::TERMINATOR)
            {
                if (ca < cb)
                    return -1;
                else if (ca > cb)
                    return 1;
                ca = *(++stra);
                cb = *(++strb);
            }
            if (ca == cb)
                return 0;
            if (ca == ascii::TERMINATOR)
                return -1;
            return 1;
        }

        s32 compare(utf32::pcrune stra, utf32::pcrune strb)
        {
            uchar32 ca = *stra;
            uchar32 cb = *strb;
            while (ca != utf32::TERMINATOR && cb != utf32::TERMINATOR)
            {
                if (ca < cb)
                    return -1;
                else if (ca > cb)
                    return 1;
                ca = *(++stra);
                cb = *(++strb);
            }
            if (ca == cb)
                return 0;
            if (ca == utf32::TERMINATOR)
                return -1;
            return 1;
        }

        s32 compare(utf32::pcrune stra, ascii::pcrune strb)
        {
            uchar32 ca = *stra;
            uchar32 cb = *strb;
            while (ca != utf32::TERMINATOR && cb != ascii::TERMINATOR)
            {
                if (ca < cb)
                    return -1;
                else if (ca > cb)
                    return 1;
                ca = *(++stra);
                cb = *(++strb);
            }
            if (ca == cb)
                return 0;
            if (ca == utf32::TERMINATOR)
                return -1;
            return 1;
        }

        s32 compare(ascii::crunes const& cstra, utf32::pcrune strb)
        {
            ascii::pcrune stra = cstra.m_str;
            uchar32       ca   = *stra;
            uchar32       cb   = *strb;
            while (stra < cstra.m_end && cb != utf32::TERMINATOR)
            {
                if (ca < cb)
                    return -1;
                else if (ca > cb)
                    return 1;
                ca = *(++stra);
                cb = *(++strb);
            }
            if (stra == cstra.m_end)
                return 0;
            if (ascii::TERMINATOR < cb)
                return -1;
            return 1;
        }

        uchar32 read(ascii::rune const*& str, ascii::rune const* end)
        {
            uchar32 c = ((uchar32)*str) & 0xFF;
            if (c != utf32::TERMINATOR && (end == NULL || str < end))
            {
                str++;
            }
            return c;
        }

        uchar32 read(utf8::rune const*& str, utf8::rune const* end)
        {
            uchar32 c = *str;
            if (c != utf32::TERMINATOR)
            {
                s32 const l = sequence_sizeof_utf8((uchar8)c);
                if (end != NULL && (str + l) > end)
                    return c;

                switch (l)
                {
                    case 0:
                        c = '?'; // Illegal character in utf8, replace with '?'
                        break;
                    case 1: break;
                    case 2:
                        str++;
                        c = ((c << 6) & 0x7ff) + ((str[0]) & 0x3f);
                        break;
                    case 3:
                        str++;
                        c = ((c << 12) & 0xffff) + (((str[0]) << 6) & 0xfff);
                        ++str;
                        c += (str[0]) & 0x3f;
                        break;
                    case 4:
                        str++;
                        c = ((c << 18) & 0x1fffff) + (((str[0]) << 12) & 0x3ffff);
                        ++str;
                        c += ((str[0]) << 6) & 0xfff;
                        ++str;
                        c += (str[0]) & 0x3f;
                        break;
                }
                str++;
            }
            return c;
        }

        uchar32 read(utf16::rune const*& str, utf16::rune const* end)
        {
            uchar32 out_c;
            uchar16 c = *str;
            if (c == utf16::TERMINATOR || (end != NULL && str >= end))
            {
                out_c = utf32::TERMINATOR;
                return out_c;
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

            if (end == NULL || (str + l) <= end)
            {
                // @TODO: This needs to be fixed since the case where end==nullptr needs to be handled correctly
                out_c = 0;
                for (s32 i = 0; i < l; i++)
                {
                    c     = str[i];
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

        uchar32 read(utf32::rune const*& str, utf32::rune const* end)
        {
            uchar32 c = *str;
            if (c != utf32::TERMINATOR && (end == NULL || str < end))
            {
                str++;
            }
            return c;
        }

        uchar32 read(ascii::rune*& str, ascii::rune const* end)
        {
            uchar32 c = *str;
            if (c != utf32::TERMINATOR && (end == NULL || str < end))
            {
                str++;
            }
            return c;
        }

        uchar32 read(utf8::rune*& str, utf8::rune const* end)
        {
            uchar32 c = *str;
            if (c != utf32::TERMINATOR && (end == NULL || str < end))
            {
                const uchar8* cstr = str;
                c                  = read(cstr, end);
                str                = (uchar8*)cstr;
            }
            return c;
        }

        uchar32 read(utf32::rune*& str, utf32::rune const* end)
        {
            uchar32 c = *str;
            if (c != utf32::TERMINATOR && (end == NULL || str < end))
            {
                str++;
            }
            return c;
        }

        uchar32 read(ascii::crunes& str) { return read(str.m_str, str.m_end); }
        uchar32 read(utf8::crunes& str) { return read(str.m_str, str.m_end); }
        uchar32 read(utf16::crunes& str) { return read(str.m_str, str.m_end); }
        uchar32 read(utf32::crunes& str) { return read(str.m_str, str.m_end); }

        // ####### WRITE ########

        void write(uchar32 c, uchar*& dest, uchar const* end)
        {
            if (c > 0x7f)
            {
                c = '?';
            }
            if (dest < end)
            {
                dest[0] = (uchar)c;
                dest++;
            }
        }

        void write(uchar32 cp, uchar8*& dest, uchar8 const* end)
        {
            if (cp < 0x80)
            { // one octet
                if (dest < end)
                {
                    *(dest++) = static_cast<uchar8>(cp);
                }
            }
            else if (cp < 0x800)
            { // two octets
                if ((dest + 1) < end)
                {
                    *(dest++) = static_cast<uchar8>((cp >> 6) | 0xc0);
                    *(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
                }
            }
            else if (cp < 0x10000)
            { // three octets
                if ((dest + 2) < end)
                {
                    *(dest++) = static_cast<uchar8>((cp >> 12) | 0xe0);
                    *(dest++) = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
                    *(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
                }
            }
            else
            { // four octets
                if ((dest + 3) < end)
                {
                    *(dest++) = static_cast<uchar8>((cp >> 18) | 0xf0);
                    *(dest++) = static_cast<uchar8>(((cp >> 12) & 0x3f) | 0x80);
                    *(dest++) = static_cast<uchar8>(((cp >> 6) & 0x3f) | 0x80);
                    *(dest++) = static_cast<uchar8>((cp & 0x3f) | 0x80);
                }
            }
        }

        void write(uchar32 rune, uchar16*& dest, uchar16 const* end)
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

            if (dest != NULL && len > 0)
            {
                if (len == 1 && dest < end)
                {
                    *dest++ = (uchar16)rune;
                }
                else if ((dest + 1) < end)
                {
                    // 20-bit intermediate value
                    u32 const iv = rune - 0x10000;
                    *dest++      = static_cast<uchar16>((iv >> 10) + 0xd800);
                    *dest++      = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
                }
            }
        }

        bool write(uchar32 c, uchar16*& dst, uchar16* end)
        {
            if (!is_eos(dst) && dst < end)
            {
                write(c, dst, end);
                return true;
            }
            return false;
        }

        void write(uchar32 c, uchar32*& dst, uchar32 const* end)
        {
            if (dst < end)
            {
                dst[0] = c;
                dst++;
            }
        }

        bool write(uchar32 c, ascii::runes& dst)
        {
            if (can_write(dst))
            {
                write(c, dst.m_end, dst.m_eos);
                return true;
            }
            return false;
        }

        bool write(uchar32 c, utf8::runes& dst)
        {
            if (can_write(dst))
            {
                write(c, dst.m_end, dst.m_eos);
                return true;
            }
            return false;
        }

        bool write(uchar32 c, utf16::runes& dst)
        {
            if (can_write(dst))
            {
                write(c, dst.m_end, dst.m_eos);
                return true;
            }
            return false;
        }

        bool write(uchar32 c, utf32::runes& dst)
        {
            if (can_write(dst))
            {
                write(c, dst.m_end, dst.m_eos);
                return true;
            }
            return false;
        }

        static bool terminate_str(ascii::runes& dst)
        {
            if (can_write(dst))
            {
                uchar32      c   = ascii::TERMINATOR;
                ascii::prune end = dst.m_end;
                write(c, end, dst.m_eos);
                return true;
            }
            return false;
        }

        static bool terminate_str(utf8::runes& dst)
        {
            if (can_write(dst))
            {
                uchar32     c   = utf8::TERMINATOR;
                utf8::prune end = dst.m_end;
                write(c, end, dst.m_eos);
                return true;
            }
            return false;
        }

        static bool terminate_str(utf16::runes& dst)
        {
            if (can_write(dst))
            {
                uchar32      c   = utf16::TERMINATOR;
                utf16::prune end = dst.m_end;
                write(c, end, dst.m_eos);
                return true;
            }
            return false;
        }

        static bool terminate_str(utf32::runes& dst)
        {
            if (can_write(dst))
            {
                uchar32      c   = utf32::TERMINATOR;
                utf32::prune end = dst.m_end;
                write(c, end, dst.m_eos);
                return true;
            }
            return false;
        }

        // const char* -> ascii / utf16 / utf32
        void copy(char const* sstr, ascii::runes& dstr, bool write_terminator)
        {
            while (*sstr != '\0')
            {
                uchar32 c = *sstr++;
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(char const* sstr, utf16::runes& dstr, bool write_terminator)
        {
            while (*sstr != '\0')
            {
                uchar32 c = *sstr++;
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(char const* sstr, utf32::runes& dstr, bool write_terminator)
        {
            while (*sstr != '\0')
            {
                uchar32 c = *sstr++;
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        // -> ASCII

        void copy(ascii::crunes const& sstr, ascii::runes& dstr, bool write_terminator)
        {
            ascii::crunes src = sstr;
            while (can_read(src))
            {
                uchar32 c = read(src);
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf16::crunes const& sstr, ascii::runes& dstr, bool write_terminator)
        {
            utf16::crunes src = sstr;
            while (can_read(src))
            {
                uchar32 c = read(src);
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf32::crunes const& sstr, ascii::runes& dstr, bool write_terminator)
        {
            utf32::crunes src = sstr;
            while (can_read(src))
            {
                uchar32 c = read(src);
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        // -> UTF-16

        void copy(ascii::crunes const& sstr, utf16::runes& dstr, bool write_terminator)
        {
            ascii::crunes src = sstr;
            while (can_read(src))
            {
                uchar32 c = read(src);
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf16::crunes const& sstr, utf16::runes& dstr, bool write_terminator)
        {
            utf16::crunes src = sstr;
            while (can_read(src))
            {
                uchar32 c = read(src);
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf32::crunes const& sstr, utf16::runes& dstr, bool write_terminator)
        {
            utf32::crunes src = sstr;
            while (can_read(src))
            {
                uchar32 c = read(src);
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        // -> UTF-32

        void copy(ascii::crunes const& sstr, utf32::runes& dstr, bool write_terminator)
        {
            ascii::crunes src = sstr;
            while (can_read(src))
            {
                uchar32 c = read(src);
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf16::crunes const& sstr, utf32::runes& dstr, bool write_terminator)
        {
            utf16::crunes src = sstr;
            while (true)
            {
                uchar32 c = read(src);
                if (c == '\0')
                    break;
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        void copy(utf32::crunes const& sstr, utf32::runes& dstr, bool write_terminator)
        {
            utf32::crunes src = sstr;
            while (true)
            {
                uchar32 c = read(src);
                if (c == '\0')
                    break;
                if (!write(c, dstr))
                    break;
            }
            if (write_terminator)
                terminate_str(dstr);
        }

        s32 size(uchar32 c)
        {
            s32 len = 0;
            if (c <= 0x7f)
            {
                len = 1;
            }
            else if (c < 0x0800)
            {
                len = 2;
            }
            else if (c < 0xd800)
            {
                len = 3;
            }
            else if (c < 0xe000)
            {
            }
            else if (c < 0x010000)
            {
                len = 3;
            }
            else if (c < 0x110000)
            {
                len = 4;
            }

            return len;
        }

        bool is_eos(uchar const* str) { return str[0] == '\0'; }

        bool is_eos(uchar8 const* str) { return str[0] == '\0'; }

        bool is_eos(uchar16 const* str) { return str[0] == '\0'; }

        bool is_eos(uchar32 const* str) { return str[0] == '\0'; }

        bool can_read(ascii::runes const& str)
        {
            if (str.m_end == nullptr)
                return str.m_str[0] != '\0';
            return str.m_str < str.m_end && str.m_str[0] != '\0';
        }

        bool can_read(utf8::runes const& str)
        {
            if (str.m_end == nullptr)
                return str.m_str[0] != '\0';
            return str.m_str < str.m_end && str.m_str[0] != '\0';
        }

        bool can_read(utf16::runes const& str)
        {
            if (str.m_end == nullptr)
                return str.m_str[0] != '\0';
            return str.m_str < str.m_end && str.m_str[0] != '\0';
        }

        bool can_read(utf32::runes const& str)
        {
            if (str.m_end == nullptr)
                return str.m_str[0] != '\0';
            return str.m_str < str.m_end && str.m_str[0] != '\0';
        }

        bool can_read(ascii::crunes const& str)
        {
            if (str.m_end == nullptr)
                return str.m_str[0] != '\0';
            return str.m_str < str.m_end && str.m_str[0] != '\0';
        }

        bool can_read(utf8::crunes const& str)
        {
            if (str.m_end == nullptr)
                return str.m_str[0] != '\0';
            return str.m_str < str.m_end && str.m_str[0] != '\0';
        }

        bool can_read(utf16::crunes const& str)
        {
            if (str.m_end == nullptr)
                return str.m_str[0] != '\0';
            return str.m_str < str.m_end && str.m_str[0] != '\0';
        }

        bool can_read(utf32::crunes const& str)
        {
            if (str.m_end == nullptr)
                return str.m_str[0] != '\0';
            return str.m_str < str.m_end && str.m_str[0] != '\0';
        }

        bool can_write(ascii::runes const& str)
        {
            ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
            return str.m_end < str.m_eos;
        }

        bool can_write(utf8::runes const& str)
        {
            ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
            return str.m_end < str.m_eos;
        }

        bool can_write(utf16::runes const& str)
        {
            ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
            return str.m_end < str.m_eos;
        }

        bool can_write(utf32::runes const& str)
        {
            ASSERT(str.m_end != nullptr && str.m_eos != nullptr);
            return str.m_end < str.m_eos;
        }

        bool read_is_crln(ascii::runes const& str)
        {
            ASSERT(str.m_end != nullptr);
            return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
        }

        bool read_is_crln(utf8::runes const& str)
        {
            ASSERT(str.m_end != nullptr);
            return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
        }

        bool read_is_crln(utf32::runes const& str)
        {
            ASSERT(str.m_end != nullptr);
            return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
        }

        bool read_is_crln(ascii::crunes const& str)
        {
            ASSERT(str.m_end != nullptr);
            return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
        }

        bool read_is_crln(utf8::crunes const& str)
        {
            ASSERT(str.m_end != nullptr);
            return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
        }

        bool read_is_crln(utf32::crunes const& str)
        {
            ASSERT(str.m_end != nullptr);
            return (str.m_str + 1) < str.m_end && (str.m_str[0] == '\r' && str.m_str[1] == '\n');
        }

    } // namespace utf
};    // namespace xcore

#define XNAMESPACE_NAME ascii
#include "x_runes_impl.cpp"
#undef XNAMESPACE_NAME

#define XNAMESPACE_NAME utf32
#include "x_runes_impl.cpp"
#undef XNAMESPACE_NAME

namespace xcore
{
    namespace utf32
    {
        void copy(const ascii::crunes& src, runes& dst, alloc* allocator, s32 size_alignment)
        {
            s32 const required = dst.size() + src.size();
            if (required > dst.cap())
            {
                resize(dst, required, allocator, size_alignment);
            }
            utf::copy(src, dst);
        }
    } // namespace utf32
} // namespace xcore