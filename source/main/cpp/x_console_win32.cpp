/**
* @file x_console_win32.cpp
* Core console on win32
*/

#include "xbase\x_target.h"
#ifdef TARGET_PC

// Windows includes first
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMB
#define NOKANJI
#include <windows.h>
#include <time.h>
#include <stdio.h>

#include "xbase\x_console.h"
#include "xbase\x_string_utf.h"

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

		s32 write(const char* str, s32 len)
		{
			::OutputDebugStringA(str);
			::fputs(str, stdout);
			return len;
		}

		s32 write8(const ustr8* str, s32 len)
		{
			ustr16 str16[1024];
			ustr16* dst16 = (ustr16*)str16;
			ustr16* end16 = dst16 + sizeof(str16);
			while (!UTF::iseos(str) && dst16 < end16) {
				uchar32 c;
				UTF::readu8(str, c);
				s32 l = UTF::uchar32to16(c, dst16);
				dst16 += l;
			}
			str16[sizeof(str16) - 1] = 0;

			::OutputDebugStringW((LPCWSTR)str16);
			::fputws((const wchar_t*)str, stdout);
			return len;
		}
	};

};
/**
 *  END xCore namespace
 */

#endif
