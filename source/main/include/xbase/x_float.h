// x_float.h - 32 bit floating point functions
#ifndef __XBASE_F32_FUNCTIONS_H__
#define __XBASE_F32_FUNCTIONS_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_debug.h"

namespace xcore
{
	namespace x_f32u
	{
		inline f32      abs(f32 inValue);
		inline f32      neg(f32 inNumber);

		inline f32      fmin(f32 numberA, f32 numberB);
		inline f32      fmax(f32 numberA, f32 numberB);
		inline f32      clamp(f32 number, f32 low, f32 high);

		//---------------------------------------------------------------------------------------------------------------------
		// Limits and const values
		//---------------------------------------------------------------------------------------------------------------------
#ifdef IEEE_FLOATS
        inline f32      nan();
        inline f32      positiveInfinity();
        inline f32      negativeInfinity();
#endif
		inline f32      positiveZero();
		inline f32      negativeZero();
		inline f32      positiveMaximum();
		inline f32      negativeMaximum();

		// De-normalized (DEN) positive and negative minimums
		inline f32      positiveMinimum();
		inline f32      negativeMinimum();

		//---------------------------------------------------------------------------------------------------------------------
		//    Rational Determination.
		//---------------------------------------------------------------------------------------------------------------------
		inline xbool    isPositiveZero(f32 inNumber);
		inline xbool    isNegativeZero(f32 inNumber);
		inline xbool    isPositiveInfinite(f32 inNumber);
		inline xbool    isNegativeInfinite(f32 inNumber);

		inline xbool    isInfinite(f32 inNumber);
		inline xbool    isNAN(f32 inNumber);
		inline xbool    isFinite(f32 inNumber);
		inline xbool    isRational(f32 inNumber);

        inline u32      bin(f32 inNumber);
		inline s32      sbin(f32 inNumber);

        inline f32      toF32(u32 inNumber);
		inline s32      toS32(f32 inNumber);
		inline u32      toU32(f32 inNumber);

		inline xbool    isEqual(f32 inNumber, f32 inG);
		inline xbool    isNotEqual(f32 inNumber, f32 inG);

		inline f32      binaryAnd(f32 inNumber, u32 inB);
		inline f32      binaryOr(f32 inNumber, u32 inB);

		inline s32      fraction(f32 inNumber);
		inline s32      exponentBinary(f32 inNumber);
		inline s32      exponent(f32 f);

		inline u32      signMask(f32 inNumber);
		inline s32      signBit(f32 inNumber);
		inline s32      signBitSigned(f32 inNumber);
		inline s32      sign(f32 inNumber);
		inline s32      sign(f32 inNumber, f32 inDelta);

		inline xbool    isZero(f32 inNumber);
		inline xbool    isNearZero(f32 inNumber, s32 inExp = -80);
		inline xbool    isNear(f32 inNumber, f32 inRef, s32 inExp =- 80);

		inline s32      roundToInt(f32 inNumber);

		//---------------------------------------------------------------------------------------------------------------------
		//    Squared function. 
		//---------------------------------------------------------------------------------------------------------------------

		inline f32      squared(f32 inNumber);
		inline f32      oneOver(f32 inNumber);

		//---------------------------------------------------------------------------------------------------------------------
		//    Trick routines. Optimized for floats and sometimes for doubles
		//---------------------------------------------------------------------------------------------------------------------

		inline s32      scaledFloatToInt(f32 inNumber, s32 inLog);
		inline f32      minimumIncrement(f32 inNumber);

		//---------------------------------------------------------------------------------------------------------------------
		//    Compare routines
		//---------------------------------------------------------------------------------------------------------------------

		inline xbool    isPositive(f32 inNumber);
		inline xbool    isNegative(f32 inNumber);
		inline xbool    isLessPositive(f32 inLHS, f32 inRHS);
		inline xbool    isGreater(f32 inLHS, f32 inRHS);
	};

	//==============================================================================
	// INLINES
	//==============================================================================
	#include "private/x_float_inline.h"

	//==============================================================================
	// INLINE PC
	//==============================================================================
	#if defined TARGET_PC
		#include "private/x_float_inline_win32.h"

    //==============================================================================
    // INLINE OSX
    //==============================================================================
    #elif defined TARGET_MAC
        #include "private/x_float_inline_mac.h"

	#endif

};

#endif    ///< __XBASE_F32_FUNCTIONS_H__
