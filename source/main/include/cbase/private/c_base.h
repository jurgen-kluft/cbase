#ifndef __CBASE_H__
#define __CBASE_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace ncore
{
	class alloc_t;
	extern alloc_t*	x_GetXBaseAllocator();
};
#endif // __CBASE_H__
