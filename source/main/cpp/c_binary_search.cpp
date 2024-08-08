#include "cbase/c_binary_search.h"

namespace ncore
{
    s64 g_BinarySearch(const void* inItem, const void* inData, s64 inLength, compare_predicate_fn predicate)
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

};  // namespace ncore
