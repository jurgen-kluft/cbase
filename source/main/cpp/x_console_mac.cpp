#include "xbase/x_target.h"
#ifdef TARGET_MAC

#include <stdio.h>
#include <iostream>

#include "xbase/x_console.h"
#include "xbase/x_runes.h"
#include "xbase/x_runes.h"

namespace xcore
{
    class xconsole_out_macos : public xconsole::xout
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

        virtual s32 color(xconsole::EColor color)
        {
            const char* fg = nullptr;
            const char* bg = nullptr;
            switch (color)
            {
                case xconsole::NORMAL: fg = "0"; break;
                case xconsole::BLACK: fg = "30"; break;
                case xconsole::RED: fg = "31"; break;
                case xconsole::GREEN: fg = "32"; break;
                case xconsole::YELLOW: fg = "33"; break;
                case xconsole::BLUE: fg = "34"; break;
                case xconsole::MAGENTA: fg = "35"; break;
                case xconsole::CYAN: fg = "36"; break;
                case xconsole::LTGREY: fg = "37"; break;
                case xconsole::BG_BLACK: bg = "40"; break;
                case xconsole::BG_RED: bg = "41"; break;
                case xconsole::BG_GREEN: bg = "42"; break;
                case xconsole::BG_YELLOW: bg = "43"; break;
                case xconsole::BG_BLUE: bg = "44"; break;
                case xconsole::BG_MAGENTA: bg = "45"; break;
                case xconsole::BG_CYAN: bg = "46"; break;
                case xconsole::BG_LTGREY: bg = "47"; break;
            }
            if (fg != nullptr && bg != nullptr)
            {
                printf("\x1b[%s;%sm", fg, bg);
            }
            if (fg != nullptr && bg == nullptr)
            {
                printf("\x1b[%sm", fg);
            }
            if (fg == nullptr && bg != nullptr)
            {
                printf("\x1b[%sm", bg);
            }

            return 0;
        }

        virtual void writeln()
        {
            ascii::rune eol[] = {'\n', 0, 0};
            write(ascii::crunes(eol, eol + 1));
        }

        virtual s32 write(const ascii::crunes& str)
        {
            const s32 maxlen = 256;
            uchar8    str8[maxlen + 4];

            s32           l   = 0;
            ascii::crunes src = str;
            while (utf::can_read(src))
            {
                uchar8* dst8     = (uchar8*)str8;
                uchar8* end8     = dst8 + maxlen;
                str8[maxlen + 0] = 0;
                str8[maxlen + 1] = 0;
                str8[maxlen + 2] = 0;
                str8[maxlen + 3] = 0;

                s32 ll = 0;
                while (utf::can_read(src) && dst8 < end8)
                {
                    uchar32 c = utf::read(src);
                    utf::write(c, dst8, end8);
                    ll += 1;
                }
                if (ll < maxlen)
                {
                    str8[ll]     = 0;
                    str8[ll + 1] = 0;
                }
                printf("%s", (const char*)str8);
                l += ll;
            }

            return l;
        }

        virtual s32 write(const utf32::crunes& str)
        {
            const s32 maxlen = 256;
            uchar8    str8[maxlen + 4];

            s32           l   = 0;
            utf32::crunes src = str;
            while (utf::can_read(src))
            {
                uchar8* dst8     = (uchar8*)str8;
                uchar8* end8     = dst8 + maxlen;
                str8[maxlen + 0] = 0;
                str8[maxlen + 1] = 0;
                str8[maxlen + 2] = 0;
                str8[maxlen + 3] = 0;

                s32 ll = 0;
                while (utf::can_read(src) && dst8 < end8)
                {
                    uchar32 c = utf::read(src);
                    utf::write(c, dst8, end8);
                    ll += 1;
                }
                if (ll < maxlen)
                {
                    str8[ll]     = 0;
                    str8[ll + 1] = 0;
                }
                printf("%s", (const char*)str8);
                l += ll;
            }

            return l;
        }
    };

    xconsole::xout* gGetDefaultConsoleOut()
    {
        static xconsole_out_macos sConsoleOut;
        return &sConsoleOut;
    }

}; // namespace xcore

#endif
