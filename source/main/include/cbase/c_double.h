#ifndef __CBASE_F64_FUNCTIONS_H__
#define __CBASE_F64_FUNCTIONS_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_debug.h"

namespace ncore
{
    namespace xf64
    {
        inline f64 abs(f64 inNumber);
        inline f64 neg(f64 inNumber);

        // Snap a value
        inline f64 snapDown(f64 fValue, f64 fSnapValue);
        inline f64 snapUp(f64 fValue, f64 fSnapValue);

        //---------------------------------------------------------------------------------------------------------------------
        // Limits and const values
        //---------------------------------------------------------------------------------------------------------------------
        inline f64 positiveZero();
        inline f64 negativeZero();
#ifdef IEEE_FLOATS
        inline f64 nan();
        inline f64 positiveInfinity();
        inline f64 negativeInfinity();
#endif
        inline f64 positiveMaximum();
        inline f64 negativeMaximum();
        inline f64 positiveMinimum();
        inline f64 negativeMinimum();

        // De-normalized (DEN) positive and negative minimums
        inline f64 positiveMinimumDEN();
        inline f64 negativeMinimumDEN();

        //---------------------------------------------------------------------------------------------------------------------
        //    Rational Determination.
        //---------------------------------------------------------------------------------------------------------------------

        inline bool isInfinite(f64 inNumber);
        inline bool isNAN(f64 inNumber);
        inline bool isFinite(f64 inNumber);
        inline bool isRational(f64 inNumber);

        inline u64 bin(f64 inNumber);
        inline s64 sbin(f64 inNumber);

        inline f64 toF64(u64 inNumber);
        inline s64 toS64(f64 inNumber);
        inline u64 toU64(f64 inNumber);

        inline bool isEqual(f64 inNumber, f64 inG);
        inline bool isNotEqual(f64 inNumber, f64 inG);

        inline f64 binaryAnd(f64 inNumber, s32 inB);
        inline f64 binaryOr(f64 inNumber, s32 inB);

        inline s64 fraction(f64 inNumber);
        inline s32 exponentBinary(f64 inNumber);
        inline s32 exponent(f64 f);

        inline u64 signMask(f64 inNumber);
        inline s32 signBit(f64 inNumber);
        inline s32 signBitSigned(f64 inNumber);
        inline s32 sign(f64 inNumber);
        inline s32 sign(f64 inNumber, f64 inDelta);

        inline bool isNearZero(f64 inNumber, s32 inExp = -80);
        inline bool isNear(f64 inNumber, f64 inRef, s32 inExp = -80);

        inline s32 roundToInt(f64 inNumber);

        //---------------------------------------------------------------------------------------------------------------------
        //    Squared function.
        //---------------------------------------------------------------------------------------------------------------------

        inline f64 squared(f64 inNumber);
        inline f64 oneOver(f64 inNumber);

        //---------------------------------------------------------------------------------------------------------------------
        //    Compare routines. Like is equal etc.
        //---------------------------------------------------------------------------------------------------------------------

        inline bool isZero(f64 inNumber);
        inline bool isPositive(f64 inNumber);
        inline bool isNegative(f64 inNumber);
        inline bool isLessPositive(f64 inLHS, f64 inRHS);
        inline bool isGreater(f64 inLHS, f64 inRHS);
    }; // namespace xf64

//==============================================================================
// INLINES
//==============================================================================
#include "private/c_double_inline.h"

//==============================================================================
// INLINE PC
//==============================================================================
#if defined TARGET_PC
#    include "private/c_double_inline_win32.h"

//==============================================================================
// INLINE MAC
//==============================================================================
#elif defined(TARGET_MAC)
#    include "private/c_double_inline_mac.h"

//==============================================================================
// UNKNOWN PLATFORM
//==============================================================================
#else
#    error "Current platform is not supported!"

#endif

}; // namespace ncore

#endif ///< __CBASE_F64_FUNCTIONS_H__
