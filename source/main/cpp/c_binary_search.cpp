#include "cbase/c_binary_search.h"

namespace ncore
{
    s64 g_BinarySearch(const void* inItem, const void* inData, u64 inLength, compare_predicate_fn predicate)
    {
        s64 bounds[2] = {0, (s64)inLength - 1};
        while (bounds[0] <= bounds[1])
        {
            const s64 mid        = (bounds[0] + bounds[1]) >> 1;
            const s8  r          = predicate(inItem, inData, mid);
            bounds[(1 - r) >> 1] = mid + r;
            if (r == 0)
            {
                s64 i = mid;
                while (i > 0 && (predicate(inItem, inData, i - 1) == 0))
                    --i;
                return i;
            }
        }
        return -1;  // could not find item
    }
    s64 g_LowerBound(const void* inItem, const void* inData, u64 inLength, compare_predicate_fn predicate)
    {
        s64 bounds[2] = {0, (s64)inLength - 1};
        while (bounds[0] <= bounds[1])
        {
            const s64 mid        = (bounds[0] + bounds[1]) >> 1;
            const s8  r          = predicate(inItem, inData, mid);
            bounds[(1 - r) >> 1] = mid + r;
            if (r == 0)
            {
                s64 i = mid;
                while (i > 0 && (predicate(inItem, inData, i - 1) == 0))
                    --i;
                return i;
            }
        }
        return bounds[0];  // could not find item
    }

    s64 g_UpperBound(const void* inItem, const void* inData, u64 inLength, compare_predicate_fn predicate)
    {
        s64 bounds[2] = {0, (s64)inLength - 1};
        while (bounds[0] <= bounds[1])
        {
            const s64 mid        = (bounds[0] + bounds[1]) >> 1;
            const s8  r          = predicate(inItem, inData, mid);
            bounds[(1 - r) >> 1] = mid + r;
            if (r == 0)
            {
                s64 i = mid + 1;
                while (i < inLength && (predicate(inItem, inData, i) == 0))
                    ++i;
                return i - 1;
            }
        }
        return bounds[0];  // could not find item
    }

    // Mono Bound
    template <typename T>
    s32 g_BinarySearch_mb_T(T const* array, u32 array_size, T key)
    {
        if (array_size == 0)
            return -1;

        u32 bot = 0;
        u32 top = array_size;
        while (top > 1)
        {
            u32 const mid = top >> 1;
            if (key >= array[bot + mid])
            {
                bot += mid;
            }
            top -= mid;
        }

        if (key == array[bot])
        {
            return bot;
        }
        return -1;
    }

    s32 g_BinarySearch_mb(s16 const* array, u32 array_size, s16 key) { return g_BinarySearch_mb_T(array, array_size, key); }
    s32 g_BinarySearch_mb(s32 const* array, u32 array_size, s32 key) { return g_BinarySearch_mb_T(array, array_size, key); }
    s32 g_BinarySearch_mb(s64 const* array, u32 array_size, s64 key) { return g_BinarySearch_mb_T(array, array_size, key); }
    s32 g_BinarySearch_mb(u16 const* array, u32 array_size, u16 key) { return g_BinarySearch_mb_T(array, array_size, key); }
    s32 g_BinarySearch_mb(u32 const* array, u32 array_size, u32 key) { return g_BinarySearch_mb_T(array, array_size, key); }
    s32 g_BinarySearch_mb(u64 const* array, u32 array_size, u64 key) { return g_BinarySearch_mb_T(array, array_size, key); }

};  // namespace ncore
