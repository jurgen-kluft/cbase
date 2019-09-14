#ifndef __XBASE_H__
#define __XBASE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xcore
{
	class xalloc;
	extern xalloc*	x_GetXBaseAllocator();
};
#endif // __XBASE_H__
