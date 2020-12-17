#include "xbase/x_target.h"
#ifdef TARGET_PC

// Windows includes first
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMB
#define NOKANJI
#include <windows.h>
#include <time.h>
#include <stdio.h>

#include "xbase/x_console.h"
#include "xbase/x_runes.h"
#include "xbase/x_runes.h"

namespace xcore
{
    class xconsole_out_win32 : public console_t::out_t
    {
    public:
        virtual s32 color(console_t::EColor color) { return 0; }

        void write_utf16(uchar32 rune, uchar16*& dest, uchar16 const* end)
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
        virtual void writeln()
        {
            utf32::rune line32[] = {'\r', 0};
            crunes_t    line(line32, 1);
            write(line);
        }

        virtual s32 write(const crunes_t& str)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return write(str.m_runes.m_ascii);
                case utf32::TYPE: return write(str.m_runes.m_utf32);
                default: //@todo: UTF-8 and UTF-16
					break;
            }
            return 0;
        }

        virtual s32 write(const ascii::crunes_t& str)
        {
            const s32 maxlen = 252;
            uchar16   str16[maxlen + 4];

            s32           l   = 0;
            ascii::pcrune src = str.m_str;
            ascii::pcrune end = str.m_end;
            while (src < end)
            {
                uchar16* dst16 = (uchar16*)str16;
                uchar16* end16 = dst16 + maxlen;
                s32      ll    = 0;
                while (src < end && dst16 < end16)
                {
                    uchar32 c = *src++;
                    write_utf16(c, dst16, end16);
                    ll += 1;
                }
                str16[ll] = 0;
                ::OutputDebugStringW((LPCWSTR)str16);
                ::fputws((const wchar_t*)str16, stdout);
                l += ll;
            }
            return l;
        }

        virtual s32 write(const utf32::crunes_t& str)
        {
            const s32 maxlen = 252;
            uchar16   str16[maxlen + 4];

            s32           l   = 0;
            utf32::pcrune src = str.m_str;
            utf32::pcrune end = str.m_end;
            while (src < end)
            {
                uchar16* dst16 = (uchar16*)str16;
                uchar16* end16 = dst16 + maxlen;
                s32      ll    = 0;
                while (src < end && dst16 < end16)
                {
                    uchar32 c = *src++;
                    write_utf16(c, dst16, end16);
                    ll += 1;
                }
                str16[ll] = 0;
                ::OutputDebugStringW((LPCWSTR)str16);
                ::fputws((const wchar_t*)str16, stdout);
                l += ll;
            }
            return l;
        }
    };

    console_t::out_t* gGetDefaultConsoleOut()
    {
        static xconsole_out_win32 sConsoleOut;
        return &sConsoleOut;
    }

}; // namespace xcore
    
#endif
