/**
* @file x_console_psp.cpp
* Core console on psp
*/

#include "xbase\x_target.h"
#ifdef TARGET_PSP

#include <stdio.h>

#include "xbase\x_console.h"
#include "xbase\x_string_std.h"

/**
 * xCore namespace
 */
namespace xcore
{
	namespace xconsole_out
	{
		#define ESCCHAR     "\x1b"

		const char* sTTYColors[] = 
		{
			ESCCHAR"[0m",
			ESCCHAR"[30m",
			ESCCHAR"[31m",
			ESCCHAR"[32m",
			ESCCHAR"[33m",
			ESCCHAR"[34m",
			ESCCHAR"[35m",
			ESCCHAR"[36m",
			ESCCHAR"[37m",

			ESCCHAR"[40m",
			ESCCHAR"[41m",
			ESCCHAR"[42m",
			ESCCHAR"[43m",
			ESCCHAR"[44m",
			ESCCHAR"[45m",
			ESCCHAR"[46m",
			ESCCHAR"[47m",
		};

		s32 color(xconsole::EColor color)
		{
			printf(sTTYColors[color]);
			return x_strlen(sTTYColors[color]);
		}

		s32 write(const char* str, s32 len)
		{
			printf(str);
			return len;
		}
	};
};
/**
 *  END xCore namespace
 */

#endif
