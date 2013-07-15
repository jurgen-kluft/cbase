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

// x_console.cpp - Core console
#include "xbase\x_console.h"

//==============================================================================
// xCore namespace
//==============================================================================
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
			::OutputDebugString(str);
			::fputs(str, stdout);
			return len;
		}
	};

//==============================================================================
// END xCore namespace
//==============================================================================
};


#endif
