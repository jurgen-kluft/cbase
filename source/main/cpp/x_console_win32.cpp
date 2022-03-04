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
#include "xbase/x_printf.h"
#include "xbase/x_runes.h"
#include "xbase/x_va_list.h"

namespace xcore
{
    class out_writer_t : public irunes_writer_t
    {
        runez_t<utf32::rune, 64> m_write_to_console_cache;

    public:
        inline out_writer_t() {}

        virtual bool        vwrite(uchar32 c)
        {
            if (m_write_to_console_cache.size() >= m_write_to_console_cache.cap())
                vflush();
            m_write_to_console_cache += c;
            return true;
        }

		virtual bool        vwrite(const char* str, const char* end)
		{
			vflush();

			ascii::crunes_t src(str, end);
			return write_ascii(src)!=0;
		}

        virtual bool        vwrite(crunes_t const& str)
        {
            vflush();
            
            switch (str.m_type)
            {
                case ascii::TYPE: return out_writer_t::write_ascii(str.m_ascii)!=0;
                case utf16::TYPE: return out_writer_t::write_utf16(str.m_utf16)!=0;
                case utf32::TYPE: return out_writer_t::write_utf32(str.m_utf32)!=0;
                default: //@todo: UTF-8
                    break;
            }
            return true;
        }

        virtual void vflush()
        {
            if (m_write_to_console_cache.size() > 0)
            {
                crunes_t cachestr = m_write_to_console_cache;
                write_utf32(cachestr.m_utf32);
                m_write_to_console_cache.reset();
            }
        }

        static void write_utf16(uchar32 rune, uchar16*& dest, uchar16 const* end)
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

        static s32 write_ascii(const ascii::crunes_t& str)
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

        static s32 write_utf16(const utf16::crunes_t& str)
        {
            const s32 maxlen = 252;
            uchar16   str16[maxlen + 4];

            s32           l   = 0;
            utf16::pcrune src = str.m_str;
            utf16::pcrune end = str.m_end;
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

        static s32 write_utf32(const utf32::crunes_t& str)
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

    class xconsole_out_win32 : public console_t::out_t
    {
    public:
        virtual s32 color(console_t::EColor color) { return 0; }

        virtual void writeln()
        {
            ascii::rune line32[] = {'\r', 0};
            ascii::crunes_t line(line32, 1);
            out_writer_t::write_ascii(line);
        }

        virtual s32 write(const crunes_t& str)
        {
            switch (str.m_type)
            {
                case ascii::TYPE: return out_writer_t::write_ascii(str.m_ascii);
                case utf16::TYPE: return out_writer_t::write_utf16(str.m_utf16);
                case utf32::TYPE: return out_writer_t::write_utf32(str.m_utf32);
                default: //@todo: UTF-8
					break;
            }
            return 0;
        }

        virtual s32 write(const crunes_t& str, const va_list_t& args)
        {
            out_writer_t dstwriter;
            vzprintf(dstwriter, str, args);
            return 0;
        }
    };

    console_t::out_t* gGetDefaultConsoleOut()
    {
        static xconsole_out_win32 sConsoleOut;
        return &sConsoleOut;
    }

}; // namespace xcore
    
#endif
