#ifndef __CBASE_H__
#define __CBASE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace cbase
{
	extern void init(ncore::s32 number_of_threads=1, ncore::s32 temporary_allocator_size=128*1024);
	extern void exit();
};

#endif // __CBASE_H__
