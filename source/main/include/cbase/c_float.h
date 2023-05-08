#ifndef __CBASE_F32_FUNCTIONS_H__
#define __CBASE_F32_FUNCTIONS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_debug.h"

namespace ncore
{
    namespace math
    {
        inline f32 abs(f32 inValue);
        inline f32 neg(f32 inNumber);

        inline f32 fmin(f32 numberA, f32 numberB);
        inline f32 fmax(f32 numberA, f32 numberB);
        inline f32 clamp(f32 number, f32 low, f32 high);

        //---------------------------------------------------------------------------------------------------------------------
        // Limits and const values
        //---------------------------------------------------------------------------------------------------------------------
#ifdef D_IEEE_FLOATS
        inline f32 nan();
        inline f32 positiveInfinity();
        inline f32 negativeInfinity();
#endif
        inline f32 positiveZero();
        inline f32 negativeZero();
        inline f32 positiveMaximum();
        inline f32 negativeMaximum();

        // De-normalized (DEN) positive and negative minimums
        inline f32 positiveMinimum();
        inline f32 negativeMinimum();

        //---------------------------------------------------------------------------------------------------------------------
        //    Rational Determination.
        //---------------------------------------------------------------------------------------------------------------------
        inline bool isPositiveZero(f32 inNumber);
        inline bool isNegativeZero(f32 inNumber);
        inline bool isPositiveInfinite(f32 inNumber);
        inline bool isNegativeInfinite(f32 inNumber);

        inline bool isInfinite(f32 inNumber);
        inline bool isNAN(f32 inNumber);
        inline bool isFinite(f32 inNumber);
        inline bool isRational(f32 inNumber);

        inline u32 bin(f32 inNumber);
        inline s32 sbin(f32 inNumber);

        inline f32 toF32(u32 inNumber);
        inline s32 toS32(f32 inNumber);
        inline u32 toU32(f32 inNumber);

        inline bool isEqual(f32 inNumber, f32 inG);
        inline bool isNotEqual(f32 inNumber, f32 inG);

        inline f32 binaryAnd(f32 inNumber, u32 inB);
        inline f32 binaryOr(f32 inNumber, u32 inB);

        inline s32 fraction(f32 inNumber);
        inline s32 exponentBinary(f32 inNumber);
        inline s32 exponent(f32 f);

        inline u32 signMask(f32 inNumber);
        inline s32 signBit(f32 inNumber);
        inline s32 signBitSigned(f32 inNumber);
        inline s32 sign(f32 inNumber);
        inline s32 sign(f32 inNumber, f32 inDelta);

        inline bool isZero(f32 inNumber);
        inline bool isNearZero(f32 inNumber, s32 inExp = -80);
        inline bool isNear(f32 inNumber, f32 inRef, s32 inExp = -80);

        inline s32 roundToInt(f32 inNumber);

        //---------------------------------------------------------------------------------------------------------------------
        //    Squared function.
        //---------------------------------------------------------------------------------------------------------------------

        inline f32 squared(f32 inNumber);
        inline f32 oneOver(f32 inNumber);

        //---------------------------------------------------------------------------------------------------------------------
        //    Trick routines. Optimized for floats and sometimes for doubles
        //---------------------------------------------------------------------------------------------------------------------

        inline s32 scaledFloatToInt(f32 inNumber, s32 inLog);
        inline f32 minimumIncrement(f32 inNumber);

        //---------------------------------------------------------------------------------------------------------------------
        //    Compare routines
        //---------------------------------------------------------------------------------------------------------------------

        inline bool isPositive(f32 inNumber);
        inline bool isNegative(f32 inNumber);
        inline bool isLessPositive(f32 inLHS, f32 inRHS);
        inline bool isGreater(f32 inLHS, f32 inRHS);
    }; 

#include "private/c_float_inline.h"

#if defined TARGET_PC
#    include "private/c_float_inline_win32.h"
#elif defined TARGET_MAC
#    include "private/c_float_inline_mac.h"
#endif

}; // namespace ncore

#endif ///< __CBASE_F32_FUNCTIONS_H__
