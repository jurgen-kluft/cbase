#ifndef __XCORE_BINARY_SEARCH_H__
#define __XCORE_BINARY_SEARCH_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
	typedef	s32		(*compare_predicate)(const void* inItem, const void* inData, s32 inIndex);

	// Exact search, behaves like x_LowerBound by returning the index to the lowest equal data (lower bound) and not
	// just the first equal data that is encountered.
	extern u32		x_BinarySearch(const void* inItem, const void* inData, u32 inLength, compare_predicate predicate);

	extern u32		x_LowerBound(const void* inItem, const void* inData, u32 inLength, compare_predicate predicate);
	extern u32		x_UpperBound(const void* inItem, const void* inData, u32 inLength, compare_predicate predicate);
};

#endif	///< __XCORE_BINARY_SEARCH_H__
