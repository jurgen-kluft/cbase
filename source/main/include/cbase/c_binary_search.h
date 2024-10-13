#ifndef __CBASE_BINARY_SEARCH_H__
#define __CBASE_BINARY_SEARCH_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_binary_search.h"

namespace ncore
{
    extern s32 g_LowerBoundOpaque(void const* key, void const* array, u32 array_size, const void* user_data, less_predicate_fn is_less, equal_predicate_fn is_equal);
    extern s32 g_UpperBoundOpaque(void const* key, void const* array, u32 array_size, const void* user_data, less_predicate_fn is_less, equal_predicate_fn is_equal);

    template <typename T>
    s32 g_LowerBound(T const* array, u32 array_size, T key)
    {
        if (array_size == 0)
            return -1;

        u32 bottom = 0;
        u32 range  = array_size;
        while (range > 1)
        {
            u32 const middle = range >> 1;
            // if (!is_less(key, array, bottom + middle, user_data))
            if (!(key < array[bottom + middle]))
            {
                bottom += middle;
            }
            range -= middle;
        }

        if (key == array[bottom])
        {
            while (bottom > 0 && key == array[bottom - 1])
                --bottom;
            return bottom;
        }
        // any items before bottom are less than key ?
        while (bottom > 0 && key < array[bottom - 1])
            --bottom;
        return bottom;
    }

    template <typename T>
    s32 g_UpperBound(T const* array, u32 array_size, T key)
    {
        if (array_size == 0)
            return -1;

        u32 bottom = 0;
        u32 range  = array_size;
        while (range > 1)
        {
            u32 const middle = range >> 1;
            // if (!is_less(key, array, bottom + middle, user_data))
            if (!(key < array[bottom + middle]))
            {
                bottom += middle;
            }
            range -= middle;
        }

        if (key == array[bottom])
        {
            while (bottom < array_size - 1 && key == array[bottom + 1])
                ++bottom;
            return bottom;
        }

        // any items after bottom are greater than key ?
        while (bottom < array_size - 1 && !(key < array[bottom + 1]))
            ++bottom;
        return bottom;
    }

};  // namespace ncore

#endif  ///< __CCORE_BINARY_SEARCH_H__
