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
#include "xbase/x_chars.h"

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

		s32 write(const xcuchars& str)
		{
			const s32 maxlen = 1020;
			uchar16 str16[maxlen + 4];
			uchar16* dst16 = (uchar16*)str16;
			uchar16* end16 = dst16 + maxlen;

			s32 l = 0;
			xcuchars src = str;
			while (!utf::is_eos(src) && dst16 < end16)
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

		s32 write(xcuchar8s const& str)
		{
			const s32 maxlen = 1020;
			uchar16 str16[maxlen + 4];
			uchar16* dst16 = (uchar16*)str16;
			uchar16* end16 = dst16 + maxlen;

			s32 l = 0;
			xcuchar8s src = str;
			while (!utf::is_eos(src) && dst16 < end16)
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

		s32 write(const xcuchar32s& str)
		{
			const s32 maxlen = 1020;
			uchar16 str16[maxlen +4];
			uchar16* dst16 = (uchar16*)str16;
			uchar16* end16 = dst16 + maxlen;

			s32 l = 0;
			xcuchar32s src = str;
			while (!utf::is_eos(src) && dst16 < end16)
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
