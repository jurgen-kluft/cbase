// x_integer.h - Basic bit/int trick operations
#ifndef __XBASE_INTEGER_UTILS_H__
#define __XBASE_INTEGER_UTILS_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/private/x_int64.h"
#include "xbase/private/x_uint64.h"

#include "xbase/private/x_int128.h"
#include "xbase/private/x_uint128.h"

#include "xbase/x_debug.h"

namespace xcore
{
    //------------------------------------------------------------------------------
    // Author:
    //     Jurgen Kluft
    // Description:
    //     Practical integer calculus to assist the developer with integer math.
    //
    //<P>  The most practical functions are:
    //     - abs
    //     - mod
    //     - power of 2
    //     - log2
    //
    //------------------------------------------------------------------------------
    #undef min
    #undef max

    #ifdef log2
        #undef log2
    #endif

    #ifndef __GNU_C__
        #define X_INTU_OPTIMIZED
    #endif

    template <class T> T    xmin(T integerA, T integerB);    ///< Return the smallest value
    template <class T> T    xmax(T integerA, T integerB);    ///< Return the biggest value
    template <class T> void xsort(T& integerA, T& integerB); ///< Return integerA = smallest value, integerB = biggest value
    template <class T> T    xsign(T integerA)
    {
        if (integerA > 0)
            return 1;
        else if (integerA < 0)
            return -1;
        else
            return 0;
    }
    template <class T> T xclamp(T integerA, T low, T high); ///< Return the clamp value

    inline s32   xalign(s32 integer, s32 alignment);     ///< Return the aligned value of integer
    inline s32   xalignDown(s32 integer, s32 alignment); ///< Return the aligned value of integer
    inline s32   xalignUp(s32 integer, s32 alignment);   ///< Return the aligned value of integer
    inline xbool xisAligned(s32 integer, s32 alignment); ///< Return xTRUE if integer is aligned
    inline s32   xabs(s32 integer);                      ///< Return the absolute value of integer
    inline s32   xneg(s32 integer, bool inDoNotNegate);  ///< Return the conditionally negated value of integer
    inline s32   xsqr(s32 integer);                      ///< Return the square of integer
    inline u32   xsqr(u32 integer);                      ///< Return the unsigned square of integer
    inline s32   xmod(s32 integer, s32 inModuloValue);   ///< Return the modulo of integer using inModuloValue
    inline s32   xgcd(s32 inIntegerA, s32 integerB);     ///< Greatest common denominator (biggest modulo value of both integers)
    inline s32   xaverage(s32 integerA, s32 integerB);   ///< Calculate average of A & B without overflow or s64 use
    inline xbool xispo2(s32 integer);                    ///< Check if integer is a power-of-two
    inline u32   xceilpo2(u32 integer);                  ///< Return the smallest power-of-two larger than integer
    inline u32   xfloorpo2(u32 integer);                 ///< Return the biggest power-of-two smaller than integer
    inline u32   xbitReverse(u32 integer);               ///< Reverse bits in 32 bit word
    inline s32   xcountBits(u32 integer);                ///< count one bits in 32 bit word
    inline s32   xcountBits(u64 integer);                ///< count one bits in 32 bit word
    inline s32   xilog2(s32 integer);                    ///< Log2 of an integer
    inline s32   xcountTrailingZeros(u32 integer);       ///< find the number of trailing zeros in 32-bit v
    inline s32   xcountLeadingZeros(u32 integer);        ///< find the number of leading zeros in 32-bit v
    inline s32   xcountTrailingZeros(u64 integer);       ///< find the number of trailing zeros in 64-bit v
    inline s32   xcountLeadingZeros(u64 integer);        ///< find the number of leading zeros in 64-bit v
    inline s32   xleastSignificantOneBit(u32 inInteger); ///< Return v but with only the Least Significant Bit "1"
    inline s32   xmostSignificantOneBit(u32 integer);    ///< Return v but with only the Most Significant Bit "1"
    inline s32   xleastSignificantBit(u32 integer);      ///< Return the bit index of the Least Significant Bit "1"
    inline s32   xmostSignificantBit(u32 integer);       ///< Return the bit index of the Most Significant Bit "1"
    inline s32   xfindFirstBit(u32 integer);             ///< find the bit position/index of the first bit from low to high
    inline s32   xfindLastBit(u32 integer);              ///< find the bit position/index of the first bit from high to low
    inline u32   xrol32(u32 integer, u32 shift);         ///< Roll all the bits in integer to the left by shift number of bits
    inline u32   xror32(u32 integer, u32 shift);         ///< Roll all the bits in integer to the right by shift number of bits

    // template specialization declaration
    template <> s32 xmin(s32 integerA, s32 integerB); ///< Return the smallest value
    template <> s32 xmax(s32 integerA, s32 integerB); ///< Return the biggest value

    //==============================================================================
    // INLINES
    //==============================================================================
    #include "xbase/private/x_integer_inline.h"

    #if defined TARGET_PC
        #include "xbase/private/x_integer_inline_win32.h"
    #elif defined TARGET_MAC
        #include "xbase/private/x_integer_inline_mac.h"
    #else
        #error "Current platform is not supported!"
    #endif

}; // namespace xcore

#endif ///< __XBASE_INTEGER_UTILS_H__
