// x_integer.h - Basic bit/int trick operations
#ifndef __CBASE_INTEGER_UTILS_H__
#define __CBASE_INTEGER_UTILS_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "cbase/c_debug.h"

namespace ncore
{
#undef min
#undef max

#ifdef log2
#undef log2
#endif

#ifndef __GNU_C__
#define D_INTU_OPTIMIZED
#endif
    namespace math
    {
        template <class T> T min(T integerA, T integerB);             ///< Return the smallest value of 2
        template <class T> T min(T integerA, T integerB, T integerC); ///< Return the smallest value of 3
        template <class T> T max(T integerA, T integerB);             ///< Return the biggest value
        template <class T> T max(T integerA, T integerB, T integerC); ///< Return the biggest value of 3

        template <class T> void sort(T& integerA, T& integerB); ///< Return integerA = smallest value, integerB = biggest value
        template <class T> void sort(T* values, u32 count);     ///< Sort array of values

        template <class T> T sign(T integerA)
        {
            if (integerA > 0)
                return 1;
            else if (integerA < 0)
                return -1;
            else
                return 0;
        }
        template <class T> T clamp(T integerA, T low, T high); ///< Return the clamp value

        /// clamp sub-range inside of encompassing range
        template <class T> void clampRange(T& subfrom, T& subto, T enfrom, T ento)
        {
            sort(subfrom, subto);
            sort(enfrom, ento);
            clamp(subfrom, enfrom, ento);
            clamp(subto, enfrom, ento);
        }

        inline u32  align(u32 integer, u32 alignment);     ///< Return the aligned value of integer
        inline u32  alignDown(u32 integer, u32 alignment); ///< Return the aligned value of integer
        inline s32  alignUp(s32 integer, s32 alignment);   ///< Return the aligned value of integer
        inline u32  alignUp(u32 integer, u32 alignment);   ///< Return the aligned value of integer
        inline u64  alignUp(u64 integer, u64 alignment);   ///< Return the aligned value of integer
        inline bool isAligned(u32 integer, u32 alignment); ///< Return True if integer is aligned

        inline s32  abs(s32 integer);                     ///< Return the absolute value of integer
        inline s32  neg(s32 integer, bool inDoNotNegate); ///< Return the conditionally negated value of integer
        inline s32  sqr(s32 integer);                     ///< Return the square of integer
        inline u32  sqr(u32 integer);                     ///< Return the unsigned square of integer
        inline s32  mod(s32 integer, s32 inModuloValue);  ///< Return the modulo of integer using inModuloValue
        inline s32  gcd(s32 inIntegerA, s32 integerB);    ///< Greatest common denominator (biggest modulo value of both integers)
        inline s32  average(s32 integerA, s32 integerB);  ///< Calculate average of A & B without overflow or s64 use
        inline bool ispo2(s32 integer);                   ///< Check if integer is a power-of-two
        inline bool ispo2(u32 integer);                   ///< Check if integer is a power-of-two
        inline bool ispo2(u64 integer);                   ///< Check if integer is a power-of-two
        inline u32  ceilpo2(u32 integer);                 ///< Return the smallest power-of-two larger than integer
        inline u32  floorpo2(u32 integer);                ///< Return the biggest power-of-two smaller than integer
        inline u32  next_power_of_two(u32 val) { return ceilpo2(val); }
        inline u32  bitReverse(u32 integer); ///< Reverse bits in 32 bit word
        inline s8   countBits(u8 integer);   ///< count one bits in 8 bit word
        inline s8   countBits(u16 integer);  ///< count one bits in 16 bit word
        inline s8   countBits(u32 integer);  ///< count one bits in 32 bit word
        inline s8   countBits(u64 integer);  ///< count one bits in 32 bit word

        inline s8 ilog2(u32 integer); ///< Log2 of a 32-bit integer
        inline s8 ilog2(s32 integer) { return ilog2((u32)integer); }
        inline s8 ilog2(u64 integer); ///< Log2 of a 64-bit integer
        inline s8 ilog2(s64 integer) { return ilog2((u64)integer); }

        inline s8  countTrailingZeros(u8 integer);        ///< find the number of trailing zeros in 8-bit v
        inline s8  countLeadingZeros(u8 integer);         ///< find the number of trailing zeros in 8-bit v
        inline s8  countTrailingZeros(u16 integer);       ///< find the number of trailing zeros in 16-bit v
        inline s8  countLeadingZeros(u16 integer);        ///< find the number of leading zeros in 16-bit v
        inline s8  countTrailingZeros(u32 integer);       ///< find the number of trailing zeros in 32-bit v
        inline s8  countLeadingZeros(u32 integer);        ///< find the number of leading zeros in 32-bit v
        inline s8  countTrailingZeros(u64 integer);       ///< find the number of trailing zeros in 64-bit v
        inline s8  countLeadingZeros(u64 integer);        ///< find the number of leading zeros in 64-bit v
        inline u32 leastSignificantOneBit(u32 inInteger); ///< Return v but with only the Least Significant Bit "1"
        inline u32 mostSignificantOneBit(u32 integer);    ///< Return v but with only the Most Significant Bit "1"
        inline s8  leastSignificantBit(u32 integer);      ///< Return the bit index of the Least Significant Bit "1"
        inline s8  mostSignificantBit(u32 integer);       ///< Return the bit index of the Most Significant Bit "1"
        inline s8  findFirstBit(u8 integer);              ///< find the bit position/index of the first bit from low to high
        inline s8  findLastBit(u8 integer);               ///< find the bit position/index of the first bit from high to low
        inline s8  findFirstBit(u16 integer);             ///< find the bit position/index of the first bit from low to high
        inline s8  findLastBit(u16 integer);              ///< find the bit position/index of the first bit from high to low
        inline s8  findFirstBit(u32 integer);             ///< find the bit position/index of the first bit from low to high
        inline s8  findLastBit(u32 integer);              ///< find the bit position/index of the first bit from high to low
        inline s8  findFirstBit(u64 integer);             ///< find the bit position/index of the first bit from low to high
        inline s8  findLastBit(u64 integer);              ///< find the bit position/index of the first bit from high to low
        inline u32 rol32(u32 integer, u32 shift);         ///< Roll all the bits in integer to the left by shift number of bits
        inline u32 ror32(u32 integer, u32 shift);         ///< Roll all the bits in integer to the right by shift number of bits

        // template specialization declaration
        template <> s32 min(s32 integerA, s32 integerB);               ///< Return the smallest value
        template <> s32 min(s32 integerA, s32 integerB, s32 integerC); ///< Return the smallest value of 3
        template <> s32 max(s32 integerA, s32 integerB);               ///< Return the biggest value
        template <> s32 max(s32 integerA, s32 integerB, s32 integerC); ///< Return the biggest value of 3
    }                                                                  // namespace math
};                                                                     // namespace ncore

//==============================================================================
// INLINES
//==============================================================================
#include "cbase/private/c_integer_inline.h"

#if defined TARGET_PC && defined COMPILER_MSVC
#include "cbase/private/c_integer_inline_win32.h"
#elif defined TARGET_PC && defined COMPILER_CLANG
#include "cbase/private/c_integer_inline_generic.h"
#elif defined TARGET_MAC
#include "cbase/private/c_integer_inline_mac.h"
#else
#include "cbase/private/c_integer_inline_generic.h"
#endif

#endif ///< __CBASE_INTEGER_UTILS_H__
