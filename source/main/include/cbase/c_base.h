#ifndef __CBASE_H__
#define __CBASE_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xbase
{
	extern void init(ncore::s32 number_of_threads=1, ncore::s32 temporary_allocator_size=8192);
	extern void exit();
};

#endif // __CBASE_H__
