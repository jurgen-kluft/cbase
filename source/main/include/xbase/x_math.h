#ifndef __XBASE_MATH_H__
#define __XBASE_MATH_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_debug.h"

namespace xcore
{
#undef min
#undef max

#ifdef log2
#undef log2
#endif

    namespace math
    {
        template <class T> T minimum(T valA, T valB)
        {
            if (valA < valB)
                return valA;
            return valB;
        }
        template <class T> T maximum(T valA, T valB)
        {
            if (valA < valB)
                return valB;
            return valA;
        }

        template <class T> T sign(T valA)
        {
            if (valA > 0)
                return 1;
            else if (valA < 0)
                return -1;
            else
                return 0;
        }

        template <class T> T clamp(T valA, T low, T high)
        {
            if (valA < low)
                return low;
            else if (valA > high)
                return high;
            return valA;
        }

        template <class T> inline bool is_power_of_2(T val) { return (val != 0) && (((val) & (val - 1)) == 0); }

        template <class T> inline T power_of_2_ceiling(T val)
        {
            val--;
            switch (sizeof(T))
            {
                case 8: val |= val >> 32;
                case 4: val |= val >> 16;
                case 2: val |= val >> 8;
                case 1: val |= val >> 4;
            }
            val |= val >> 2;
            val |= val >> 1;
            return val + 1;
        }

    } // namespace math
} // namespace xcore

#endif ///< __XBASE_MATH_H__
