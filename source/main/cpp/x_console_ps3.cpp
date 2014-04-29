/**
* @file x_console_ps3.cpp
* Core console on ps3
*/

#ifndef SPU

#include "xbase\x_target.h"
#if defined(TARGET_PS3)

#ifndef SPU
#include <stdio.h>
#else
#include <spu_printf.h>
#endif

#include "xbase\x_console.h"
#include "xbase\x_string_ascii.h"

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
#ifndef SPU
			printf(sTTYColors[color]);
#else
			spu_printf(sTTYColors[color]);
#endif // SPU
			return x_strlen(sTTYColors[color]);
		}

		s32 write(const char* str, s32 len)
		{
#ifndef SPU
			printf(str);
#else
			spu_printf(str);
#endif
			return len;
		}
	};

};
/**
 *  END xCore namespace
 */

#endif

#endif // ifndef SPU
