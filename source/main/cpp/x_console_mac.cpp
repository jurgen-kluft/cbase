#include "xbase/x_target.h"
#ifdef TARGET_MAC

#include <stdio.h>
#include <iostream>

#include "xbase/x_console.h"
#include "xbase/x_runes.h"

namespace xcore
{
    class xconsole_out_macos : public console_t::out_t
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

		void writeutf8(uchar32 cp, uchar8*& dest, uchar8 const* end)
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

        virtual void writeln()
        {
            ascii::rune eol[] = {'\n', 0, 0};
            write(ascii::crunes_t(eol, eol + 1));
        }

		virtual s32 write(const crunes_t& str)
		{
			switch (str.m_type)
			{
			case ascii::TYPE: return write(str.m_runes.m_ascii);
			case utf32::TYPE: return write(str.m_runes.m_utf32);
			default: break;
				//@todo: UTF-8 and UTF-16
			}
			return 0;
		}

        virtual s32 write(const ascii::crunes_t& str)
        {
            const s32 maxlen = 256;
            uchar8    str8[maxlen + 4];

            s32           l   = 0;
            ascii::pcrune src = str.m_str;
            ascii::pcrune end = str.m_end;
            while (src < end)
            {
                uchar8* dst8     = (uchar8*)str8;
                uchar8* end8     = dst8 + maxlen;
                str8[maxlen + 0] = 0;
                str8[maxlen + 1] = 0;
                str8[maxlen + 2] = 0;
                str8[maxlen + 3] = 0;

                s32 ll = 0;
                while (src < end && dst8 < end8)
                {
                    uchar32 c = *src++;
                    writeutf8(c, dst8, end8);
                    ll += 1;
                }
                if (ll < maxlen)
                {
                    str8[ll]     = 0;
                    str8[ll + 1] = 0;
                }
				::printf("%s", (const char*)str8);
                l += ll;
            }

            return l;
        }

        virtual s32 write(const utf32::crunes_t& str)
        {
            const s32 maxlen = 256;
            uchar8    str8[maxlen + 4];

            s32           l   = 0;
			utf32::pcrune src = str.m_str;
			utf32::pcrune end = str.m_end;
			while (src < end)
			{
                uchar8* dst8     = (uchar8*)str8;
                uchar8* end8     = dst8 + maxlen;
                str8[maxlen + 0] = 0;
                str8[maxlen + 1] = 0;
                str8[maxlen + 2] = 0;
                str8[maxlen + 3] = 0;

                s32 ll = 0;
				while (src < end && dst8 < end8)
                {
                    uchar32 c = *src++;
                    writeutf8(c, dst8, end8);
                    ll += 1;
                }
                if (ll < maxlen)
                {
                    str8[ll]     = 0;
                    str8[ll + 1] = 0;
                }
				::printf("%s", (const char*)str8);
                l += ll;
            }

            return l;
        }
    };

    console_t::out_t* gGetDefaultConsoleOut()
    {
        static xconsole_out_macos sConsoleOut;
        return &sConsoleOut;
    }

}; // namespace xcore

#endif
