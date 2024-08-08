#ifndef __CBASE_BINARY_SEARCH_H__
#define __CBASE_BINARY_SEARCH_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_binary_search.h"

namespace ncore
{
    // Exact search, behaves like g_LowerBound by returning the index to the lowest equal data (lower bound) and not
    // just the first equal data that is encountered.
    extern s64 g_BinarySearch(const void* inItem, const void* inData, s64 inLength, compare_predicate_fn predicate);

    extern s64 g_LowerBound(const void* inItem, const void* inData, s64 inLength, compare_predicate_fn predicate);
    extern s64 g_UpperBound(const void* inItem, const void* inData, s64 inLength, compare_predicate_fn predicate);

};  // namespace ncore

#endif  ///< __CCORE_BINARY_SEARCH_H__
