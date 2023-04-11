#include "cbase/c_debug.h"
#include "cbase/c_binary_search.h"

namespace ncore
{
    s64 g_BinarySearch(const void* inItem, const void* inData, u64 inLength, compare_predicate_fn predicate)
    {
        s64 bounds[2] = {0, (s64)inLength - 1};
        while (bounds[0] <= bounds[1])
        {
            const s32 mid = (bounds[0] + bounds[1]) >> 1;
            const s8  r   = predicate(inItem, inData, mid);
            bounds[(1 - r) >> 1] = mid + r;
            if (r == 0)
            {
                while (bounds[0] > 0 && (predicate(inItem, inData, bounds[0] - 1) == 0))
                    --bounds[0];
                return bounds[0];
            }
        }

        return -bounds[0]; // return the (negative) index where the value should be inserted
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

        r = predicate(inItem, inData, inLength - 1);
        if (r == 1)
            return inLength;

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
            u64 const distance   = ((bounds[1] - bounds[0]) / 2);
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

        r = predicate(inItem, inData, inLength - 1);
        if (r == 1)
            return inLength - 1;
        else if (r == 0)
            return inLength - 1;

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

}; // namespace ncore
