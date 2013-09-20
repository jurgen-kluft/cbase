/**
* @file x_console_3ds.cpp
* Core console on 3ds
*/

#include "xbase\x_target.h"
#ifdef TARGET_3DS

#include <nn/os.h>
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
			NN_LOG(str);
			return len;
		}
	};
};
/**
 *  END xCore namespace
 */

#endif
