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
    class xconsole_out_win32 : public xconsole::xout
    {
    public:
        virtual s32 color(xconsole::EColor color) { return 0; }

        virtual void writeln()
        {
            utf32::rune line32[] = {'\r', 0};
            write(utf32::crunes(line32, line32 + 1));
        }

        virtual s32 write(const ascii::crunes& str)
        {
            const s32 maxlen = 252;
            uchar16   str16[maxlen + 4];

            s32           l   = 0;
            ascii::crunes src = str;
            while (utf::can_read(src))
            {
                uchar16* dst16 = (uchar16*)str16;
                uchar16* end16 = dst16 + maxlen;
                s32      ll    = 0;
                while (utf::can_read(src) && dst16 < end16)
                {
                    uchar32 c = utf::read(src);
                    utf::write(c, dst16, end16);
                    ll += 1;
                }
                str16[ll] = 0;
                ::OutputDebugStringW((LPCWSTR)str16);
                ::fputws((const wchar_t*)str16, stdout);
                l += ll;
            }

            return l;
        }

        virtual s32 write(const utf32::crunes& str)
        {
            const s32 maxlen = 252;
            uchar16   str16[maxlen + 4];

            s32           l   = 0;
            utf32::crunes src = str;
            while (utf::can_read(src))
            {
                uchar16* dst16 = (uchar16*)str16;
                uchar16* end16 = dst16 + maxlen;
                s32      ll    = 0;
                while (utf::can_read(src) && dst16 < end16)
                {
                    uchar32 c = utf::read(src);
                    utf::write(c, dst16, end16);
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

    xconsole::xout* gGetDefaultConsoleOut()
    {
        static xconsole_out_win32 sConsoleOut;
        return &sConsoleOut;
    }

}; // namespace xcore
    /**
     *  END xCore namespace
     */

#endif
