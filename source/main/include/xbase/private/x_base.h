#ifndef __XBASE_H__
#define __XBASE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xcore
{
	class alloc_t;
	extern alloc_t*	x_GetXBaseAllocator();
};
#endif // __XBASE_H__
