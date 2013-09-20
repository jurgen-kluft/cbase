/**
* @file x_console_wii.cpp
* Core console on wii
*/

#include "xbase\x_target.h"
#ifdef TARGET_WII

#include <revolution/os.h>

#include "xbase\x_console.h"

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
			OSReport(str);
			return len;
		}
	};
};
/**
 *  END xCore namespace
 */

#endif
