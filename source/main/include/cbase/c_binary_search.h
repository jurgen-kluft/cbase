#ifndef __CCORE_BINARY_SEARCH_H__
#define __CCORE_BINARY_SEARCH_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
	typedef	s8		(*compare_predicate_fn)(const void* inItem, const void* inData, s64 inIndex);

	// Exact search, behaves like g_LowerBound by returning the index to the lowest equal data (lower bound) and not
	// just the first equal data that is encountered.
	extern s64		g_BinarySearch(const void* inItem, const void* inData, u64 inLength, compare_predicate_fn predicate);

	extern s64		g_LowerBound(const void* inItem, const void* inData, u64 inLength, compare_predicate_fn predicate);
	extern s64		g_UpperBound(const void* inItem, const void* inData, u64 inLength, compare_predicate_fn predicate);
};

#endif	///< __CCORE_BINARY_SEARCH_H__
