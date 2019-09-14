#ifndef __XBASE_QUICK_SORT_H__
#define __XBASE_QUICK_SORT_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_debug.h"

namespace xcore
{
	//----------------------------------------------------------------------------------------------------------------
	// Custom QuickSort

	extern void xqsort(void *a,	// element_array
		s32 n,					// element_count
		s32 es,					// element_size
		s32 (*cmp)(const void* const le, const void* const re, void* data),
		void* data = 0);

};

#endif // __XBASE_QUICK_SORT_H__
