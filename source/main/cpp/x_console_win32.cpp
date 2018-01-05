/**
* @file x_console_win32.cpp
* Core console on win32
*/

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
#include "xbase/x_string_utf.h"

/**
 * xCore namespace
 */
namespace xcore
{
	namespace xconsole_out
	{
		s32 color(xconsole::EColor color)
		{
			return 0;
		}

		s32 write_uchar(const uchar* str, const uchar* str_end)
		{
			const s32 maxlen = 1020;
			uchar16 str16[maxlen + 4];
			uchar16* dst16 = (uchar16*)str16;
			uchar16* end16 = dst16 + maxlen;

			s32 l = 0;
			const uchar* src = str;
			while (!utf::is_eos(src) && src < (const uchar*)str_end && dst16 < end16)
			{
				uchar32 c;
				utf::read(src, c);
				uchar16* next = utf::write(c, dst16);
				dst16 = next;
				l += 1;
			}
			str16[maxlen - 1] = 0;

			::OutputDebugStringW((LPCWSTR)str16);
			::fputws((const wchar_t*)str16, stdout);
			return l;
		}

		s32 write_uchar8(xcuchar8s const& str)
		{
			const s32 maxlen = 1020;
			uchar16 str16[maxlen + 4];
			uchar16* dst16 = (uchar16*)str16;
			uchar16* end16 = dst16 + maxlen;

			s32 l = 0;
			const uchar8* src = (const uchar8*)str;
			while (!utf::is_eos(src) && src < (const uchar8*)str_end && dst16 < end16)
			{
				uchar32 c;
				utf::read(src, c);
				uchar16* next = utf::write(c, dst16);
				dst16 = next;
				l += 1;
			}
			str16[maxlen - 1] = 0;

			::OutputDebugStringW((LPCWSTR)str16);
			::fputws((const wchar_t*)str16, stdout);
			return l;
		}

		s32 write_uchar32(const uchar32* str, const uchar32* str_end)
		{
			const s32 maxlen = 1020;
			uchar16 str16[maxlen +4];
			uchar16* dst16 = (uchar16*)str16;
			uchar16* end16 = dst16 + maxlen;

			s32 l = 0;
			const uchar32* src = (const uchar32*)str;
			while (!utf::is_eos(src) && src < (const uchar32*)str_end && dst16 < end16)
			{
				uchar32 c;
				utf::read(src, c);
				uchar16* next = utf::write(c, dst16);
				dst16 = next;
				l += 1;
			}
			str16[maxlen - 1] = 0;

			::OutputDebugStringW((LPCWSTR)str16);
			::fputws((const wchar_t*)str16, stdout);
			return l;
		}

	};

};
/**
 *  END xCore namespace
 */

#endif
