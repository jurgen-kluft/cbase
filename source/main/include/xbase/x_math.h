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

        static inline s8 log2(u64 value)
        {
            static constexpr s8 table[64] =
            {
                63,  0, 58,  1, 59, 47, 53,  2,
                60, 39, 48, 27, 54, 33, 42,  3,
                61, 51, 37, 40, 49, 18, 28, 20,
                55, 30, 34, 11, 43, 14, 22,  4,
                62, 57, 46, 52, 38, 26, 32, 41,
                50, 36, 17, 19, 29, 10, 13, 21,
                56, 45, 25, 31, 35, 16,  9, 12,
                44, 24, 15,  8, 23,  7,  6,  5
            };
            value |= value >> 1;
            value |= value >> 2;
            value |= value >> 4;
            value |= value >> 8;
            value |= value >> 16;
            value |= value >> 32;
            return table[((value - (value >> 1)) * 0x07EDD5E59A4E28C2) >> 58];
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
        template <> inline u32 power_of_2_ceiling(u32 val)
        {
            val--;
             val |= val >> 16;
             val |= val >> 8;
             val |= val >> 4;
            val |= val >> 2;
            val |= val >> 1;
            return val + 1;
        }
        template <> inline u16 power_of_2_ceiling(u16 val)
        {
            val--;
            val |= val >> 8;
            val |= val >> 4;
            val |= val >> 2;
            val |= val >> 1;
            return val + 1;
        }
        template <class T> inline T next_power_of_two(T val) { return power_of_2_ceiling(val); }

    } // namespace math
} // namespace xcore

#endif ///< __XBASE_MATH_H__
