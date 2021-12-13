#ifndef __XBASE_H__
#define __XBASE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xbase
{
	extern void init(xcore::s32 number_of_threads=1, xcore::s32 temporary_allocator_size=8192);
	extern void exit();
};

#endif // __XBASE_H__
