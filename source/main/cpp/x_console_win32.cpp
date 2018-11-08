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
#include "xbase/x_runes.h"
#include "xbase/x_runes.h"

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

		s32 write(const ascii::crunes& str)
		{
			const s32 maxlen = 1020;
			uchar16 str16[maxlen + 4];
			uchar16* dst16 = (uchar16*)str16;
			uchar16* end16 = dst16 + maxlen;

			s32 l = 0;
			ascii::crunes src = str;
			while (!utf::can_read(src) && dst16 < end16)
			{
				uchar32 c = utf::read(src);
				utf::write(c, dst16, end16);
				l += 1;
			}
			str16[maxlen - 1] = 0;

			::OutputDebugStringW((LPCWSTR)str16);
			::fputws((const wchar_t*)str16, stdout);
			return l;
		}

		s32 write(utf8::crunes const& str)
		{
			const s32 maxlen = 1020;
			uchar16 str16[maxlen + 4];
			uchar16* dst16 = (uchar16*)str16;
			uchar16* end16 = dst16 + maxlen;

			s32 l = 0;
			utf8::crunes src = str;
			while (!utf::can_read(src) && dst16 < end16)
			{
				uchar32 c = utf::read(src);
				utf::write(c, dst16, end16);
				l += 1;
			}
			str16[maxlen - 1] = 0;

			::OutputDebugStringW((LPCWSTR)str16);
			::fputws((const wchar_t*)str16, stdout);
			return l;
		}

		s32 write(const utf32::crunes& str)
		{
			const s32 maxlen = 1020;
			uchar16 str16[maxlen +4];
			uchar16* dst16 = (uchar16*)str16;
			uchar16* end16 = dst16 + maxlen;

			s32 l = 0;
			utf32::crunes src = str;
			while (!utf::can_read(src) && dst16 < end16)
			{
				uchar32 c = utf::read(src);
				utf::write(c, dst16, end16);
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
