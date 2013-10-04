/**
* @file x_console_x360.cpp
* Core console on x360
*/

#include "xbase\x_target.h"
#ifdef TARGET_360

// Windows includes first
#include <stdio.h>

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
            printf(str);
			return len;
        }
    };

};
/**
 *  END xCore namespace
 */

#endif
