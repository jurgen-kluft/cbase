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
                while (bounds[0] > 0 && (predicate(inItem, inData, bounds[0] - 1) == 0))
                    --bounds[0];
                return bounds[0];
            }
        }

        return -bounds[0];  // return the (negative) index where the value should be inserted
    }
    s64 g_LowerBound(const void* inItem, const void* inData, u64 inLength, compare_predicate_fn predicate)
    {
        // Do a binary search on the data to obtain the lowest index of a lesser-or-equal (LTE) item
        s8 r;
        r = predicate(inItem, inData, 0);
        if (r == -1)
            return 0;
        else if (r == 0)
            return 0;

        r = predicate(inItem, inData, (s64)inLength - 1);
        if (r == 1)
            return (s64)inLength;

        s64 bounds[2] = {0, (s64)inLength};
        s64 mid       = bounds[1] / 2;
        while (true)
        {
            r = predicate(inItem, inData, mid);
            if (r == 0)
            {
                if (mid > 0 && (predicate(inItem, inData, mid - 1) == 0))
                {
                    r = -1;
                }
                else
                {
                    break;
                }
            }

            bounds[(1 - r) >> 1] = mid;
            s64 const distance   = ((bounds[1] - bounds[0]) / 2);
            if (distance == 0)
            {
                mid -= (1 - r) >> 1;
                break;
            }
            mid = bounds[0] + distance;
        }

        return mid;
    }

    s64 g_UpperBound(const void* inItem, const void* inData, u64 inLength, compare_predicate_fn predicate)
    {
        // Do a binary search on the data to obtain the highest index of a greater-or-equal (GTE) item
        s8 r;
        r = predicate(inItem, inData, 0);
        if (r == -1)
            return 0;

        r = predicate(inItem, inData, (s64)inLength - 1);
        if (r == 1)
            return (s64)inLength - 1;
        else if (r == 0)
            return (s64)inLength - 1;

        s64 bounds[2] = {0, (s64)inLength};
        s64 mid       = bounds[1] / 2;
        while (true)
        {
            r = predicate(inItem, inData, mid);
            if (r == 0)
            {
                if ((u64)(mid + 1) < inLength && (predicate(inItem, inData, mid + 1) == 0))
                {
                    r = 1;
                }
                else
                {
                    break;
                }
            }

            bounds[(1 - r) >> 1] = mid;
            s64 const distance   = ((bounds[1] - bounds[0]) / 2);
            if (distance == 0)
            {
                mid -= (1 - r) >> 1;
                break;
            }
            mid = bounds[0] + distance;
        }

        return mid;
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
