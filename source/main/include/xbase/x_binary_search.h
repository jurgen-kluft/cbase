#ifndef __XCORE_BINARY_SEARCH_H__
#define __XCORE_BINARY_SEARCH_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

//==============================================================================
// xcore namespace
//==============================================================================
namespace xcore
{
	typedef	s32		(*compare_predicate)(const void* inItem, const void* inData, s32 inIndex);
	extern s32		x_BinarySearch(const void* inItem, const void* inData, s32 inLength, compare_predicate predicate);

	//==============================================================================
	// END xcore namespace
	//==============================================================================
};

#endif	/// __XCORE_BINARY_SEARCH_H__
