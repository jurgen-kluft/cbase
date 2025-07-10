#include "ccore/c_target.h"
#ifdef TARGET_MAC

#    include <stdio.h>
#    include <iostream>

#    include "cbase/c_console.h"
#    include "cbase/c_printf.h"
#    include "cbase/c_runes.h"

namespace ncore
{
    static s32 write_ascii(ascii::pcrune _str, u32 _begin, u32 _end);
    static s32 write_utf16(utf16::pcrune _str, u32 _begin, u32 _end);
    static s32 write_utf32(utf32::pcrune _str, u32 _begin, u32 _end);

    class out_writer_t : public nrunes::iwriter_t
    {
    public:
        inline out_writer_t() {}

        virtual s32 vwrite(uchar32 c)
        {
            utf32::rune src[2] = {c, 0};
            return write_utf32(src, 0, 1);
        }

        virtual s32 vwrite(const char* str)
        {
            const s32 maxlen = 252;
            char      str8[maxlen + 4];

            s32           l   = 0;
            ascii::pcrune src = str;
            while (*src != 0)
            {
                char* dst8 = (char*)str8;
                char* end8 = dst8 + maxlen;
                while (*src != 0 && dst8 < end8)
                {
                    *dst8++ = (char)(*src++);
                }
                *dst8 = 0;
                ::printf("%s", (const char*)str8);
                l += dst8 - str8;
            }
            return l;
        }

        virtual s32 vwrite(const char* str, const char* end)
        {
            u32 len = end - str;
            return write_ascii(str, 0, len);
        }

        virtual s32 vwrite(crunes_t const& str)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return write_ascii(str.m_ascii, str.m_str, str.m_end);
                case utf16::TYPE: return write_utf16(str.m_utf16, str.m_str, str.m_end);
                case utf32::TYPE: return write_utf32(str.m_utf32, str.m_str, str.m_end);
                default:  //@todo: UTF-8
                    break;
            }
            return 0;
        }

        virtual void vflush() {}
    };

    static s32 write_utf16(uchar32 rune, uchar16*& dest, uchar16 const* end)
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

        if (dest != nullptr && len > 0)
        {
            if (len == 1 && dest < end)
            {
                *dest++ = (uchar16)rune;
                return 1;
            }
            else if ((dest + 1) < end)
            {
                // 20-bit intermediate value
                u32 const iv = rune - 0x10000;
                *dest++      = static_cast<uchar16>((iv >> 10) + 0xd800);
                *dest++      = static_cast<uchar16>((iv & 0x03ff) + 0xdc00);
                return 1;
            }
        }
        return 0;
    }

    static s32 write_ascii(ascii::pcrune _str, u32 _begin, u32 _end)
    {
        const s32 maxlen = 252;
        char      str8[maxlen + 4];

        s32           l   = 0;
        ascii::pcrune src = _str + _begin;
        ascii::pcrune end = _str + _end;
        while (src < end)
        {
            char* dst8 = (char*)str8;
            char* end8 = dst8 + maxlen;
            while (src < end && dst8 < end8)
            {
                *dst8++ = (char)(*src++);
            }
            *dst8 = 0;
            ::printf("%s", (const char*)str8);
            l += dst8 - str8;
        }
        return l;
    }

    static s32 write_utf16(utf16::pcrune _str, u32 _begin, u32 _end)
    {
        const s32 maxlen = 252;
        uchar16   str16[maxlen + 4];

        s32           l   = 0;
        utf16::pcrune src = _str + _begin;
        utf16::pcrune end = _str + _end;
        while (src < end)
        {
            uchar16* dst16 = (uchar16*)str16;
            uchar16* end16 = dst16 + maxlen;
            while (src < end && dst16 < end16)
            {
                *dst16++ = (*src++).r;
            }
            *dst16 = 0;
            ::wprintf(L"%s", (const wchar_t*)str16);
            l += dst16 - str16;
        }
        return l;
    }

    static s32 write_utf32(utf32::pcrune _str, u32 _begin, u32 _end)
    {
        const s32 maxlen = 255;
        uchar32   str32[maxlen + 1];

        s32           l   = 0;
        utf32::pcrune src = _str + _begin;
        utf32::pcrune end = _str + _end;
        while (src < end)
        {
            uchar32* dst32 = (uchar32*)str32;
            uchar32* end32 = dst32 + maxlen;
            while (src < end && dst32 < end32)
            {
                *dst32++ = (*src++).r;
            }
            *dst32 = 0;
            ::wprintf(L"%s", (const wchar_t*)str32);
            l += dst32 - str32;
        }
        return l;
    }

    class console_out_macos : public console_t::out_t
    {
    public:
        // +---------+------------+------------+
        // |  color  | foreground | background |
        // |         |    code    |    code    |
        // +---------+------------+------------+
        // | black   |     30     |     40     |
        // | red     |     31     |     41     |
        // | green   |     32     |     42     |
        // | yellow  |     33     |     43     |
        // | blue    |     34     |     44     |
        // | magenta |     35     |     45     |
        // | cyan    |     36     |     46     |
        // | white   |     37     |     47     |
        // +---------+------------+------------+

        // To set only the foreground color:                 "\x1b[30m"
        // To set only the background color:                 "\x1b[40m"
        // To set foreground and background color:           "\x1b[30;40m"
        // To reset all color attributes back to normal:     "\x1b[0m"

        virtual s32 color(console_t::EColor color)
        {
            const char* fg = nullptr;
            const char* bg = nullptr;
            switch (color)
            {
                case console_t::NORMAL: fg = "0"; break;
                case console_t::BLACK: fg = "30"; break;
                case console_t::RED: fg = "31"; break;
                case console_t::GREEN: fg = "32"; break;
                case console_t::YELLOW: fg = "33"; break;
                case console_t::BLUE: fg = "34"; break;
                case console_t::MAGENTA: fg = "35"; break;
                case console_t::CYAN: fg = "36"; break;
                case console_t::LTGREY: fg = "37"; break;
                case console_t::BG_BLACK: bg = "40"; break;
                case console_t::BG_RED: bg = "41"; break;
                case console_t::BG_GREEN: bg = "42"; break;
                case console_t::BG_YELLOW: bg = "43"; break;
                case console_t::BG_BLUE: bg = "44"; break;
                case console_t::BG_MAGENTA: bg = "45"; break;
                case console_t::BG_CYAN: bg = "46"; break;
                case console_t::BG_LTGREY: bg = "47"; break;
            }
            if (fg != nullptr && bg != nullptr)
            {
                ::printf("\x1b[%s;%sm", fg, bg);
            }
            if (fg != nullptr && bg == nullptr)
            {
                ::printf("\x1b[%sm", fg);
            }
            if (fg == nullptr && bg != nullptr)
            {
                ::printf("\x1b[%sm", bg);
            }

            return 0;
        }

        virtual void writeln()
        {
            ascii::rune line32[] = {'\n', 0};
            write_ascii(line32, 0, 1);
        }

        virtual s32 write(const crunes_t& str)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return write_ascii(str.m_ascii, str.m_str, str.m_end);
                case utf16::TYPE: return write_utf16(str.m_utf16, str.m_str, str.m_end);
                case utf32::TYPE: return write_utf32(str.m_utf32, str.m_str, str.m_end);
                default:  //@todo: UTF-8
                    break;
            }
            return 0;
        }

        virtual s32 write(const crunes_t& str, const va_t* argv, s32 argc)
        {
            out_writer_t dstwriter;
            vzprintf(&dstwriter, str, argv, argc);
            dstwriter.flush();
            return 0;
        }
    };

    console_t::out_t* gGetDefaultConsoleOut()
    {
        static console_out_macos sConsoleOut;
        return &sConsoleOut;
    }

};  // namespace ncore

#endif
