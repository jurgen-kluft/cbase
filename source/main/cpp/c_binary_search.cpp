#include "cbase/c_binary_search.h"

namespace ncore
{
#if 0
    s64 g_LowerBound(const void* inItem, const void* inData, s64 inLength, compare_predicate_fn predicate)
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

    s64 g_UpperBound(const void* inItem, const void* inData, s64 inLength, compare_predicate_fn predicate)
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
#endif

    s32 g_LowerBoundOpaque(void const* key, void const* array, u32 array_size, const void* user_data, less_predicate_fn is_less, equal_predicate_fn is_equal)
    {
        if (array_size == 0)
            return -1;

        u32 bottom = 0;
        u32 range  = array_size;
        while (range > 1)
        {
            u32 const middle = range >> 1;
            if (!is_less(key, array, bottom + middle, user_data))
            {
                bottom += middle;
            }
            range -= middle;
        }

        if (is_equal(key, array, bottom, user_data))
        {
            while (bottom > 0 && is_equal(key, array, bottom - 1, user_data))
                --bottom;
            return bottom;
        }

        // any items before bottom are less than key ?
        while (bottom > 0 && is_less(key, array, bottom - 1, user_data))
            --bottom;

        return bottom;
    }

    s32 g_UpperBoundOpaque(void const* key, void const* array, u32 array_size, const void* user_data, less_predicate_fn is_less, equal_predicate_fn is_equal)
    {
        if (array_size == 0)
            return -1;

        u32 bottom = 0;
        u32 range  = array_size;
        while (range > 1)
        {
            u32 const middle = range >> 1;
            if (!is_less(key, array, bottom + middle, user_data))
            {
                bottom += middle;
            }
            range -= middle;
        }

        if (is_equal(key, array, bottom, user_data))
        {
            while (bottom < array_size - 1 && is_equal(key, array, bottom + 1, user_data))
                ++bottom;
            return bottom;
        }

        // any items after bottom are greater than key ?
        while (bottom < array_size - 1 && !is_less(key, array, bottom + 1, user_data))
            ++bottom;

        return bottom;
    }

};  // namespace ncore
