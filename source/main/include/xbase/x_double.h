// x_double.h - 64 bit floating point functions
#ifndef __XBASE_F64_FUNCTIONS_H__
#define __XBASE_F64_FUNCTIONS_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{

	namespace x_f64u
	{
		inline f64			abs(f64 inNumber);
		inline f64			neg(f64 inNumber);

		// Snap a value
		inline f64			snapDown (f64 fValue, f64 fSnapValue);
		inline f64			snapUp (f64 fValue, f64 fSnapValue);

		//---------------------------------------------------------------------------------------------------------------------
		// Limits and const values
		//---------------------------------------------------------------------------------------------------------------------
		inline f64			positiveZero();
		inline f64			negativeZero();
#ifdef IEEE_FLOATS			
		inline f64			nan();
		inline f64			positiveInfinity();
		inline f64			negativeInfinity();
#endif
		inline f64			positiveMaximum();
		inline f64			negativeMaximum();
		inline f64			positiveMinimum();
		inline f64			negativeMinimum();

		// De-normalized (DEN) positive and negative minimums
		inline f64			positiveMinimumDEN();
		inline f64			negativeMinimumDEN();

		//---------------------------------------------------------------------------------------------------------------------
		//    Rational Determination. 
		//---------------------------------------------------------------------------------------------------------------------

        inline xbool		isInfinite(f64 inNumber);
		inline xbool		isNAN(f64 inNumber);
		inline xbool		isFinite(f64 inNumber);
		inline xbool		isRational(f64 inNumber);

        inline u64			bin(f64 inNumber);
		inline s64			sbin(f64 inNumber);

		inline f64			toF64(u64 inNumber);
		inline s64			toS64(f64 inNumber);
		inline u64			toU64(f64 inNumber);

		inline xbool		isEqual(f64 inNumber, f64 inG);
		inline xbool		isNotEqual(f64 inNumber, f64 inG);

		inline f64			binaryAnd(f64 inNumber, s32 inB);
		inline f64			binaryOr(f64 inNumber, s32 inB);

		inline s64			fraction(f64 inNumber);
		inline s32			exponentBinary(f64 inNumber);
		inline s32			exponent(f64 f);

		inline u64			signMask(f64 inNumber);
		inline s32			signBit(f64 inNumber);
		inline s32			signBitSigned(f64 inNumber);
		inline s32			sign(f64 inNumber);
		inline s32			sign(f64 inNumber, f64 inDelta);

		inline xbool		isNearZero(f64 inNumber, s32 inExp = -80);
		inline xbool		isNear(f64 inNumber, f64 inRef, s32 inExp=-80);

		inline s32			roundToInt(f64 inNumber);

		//---------------------------------------------------------------------------------------------------------------------
		//    Squared function. 
		//---------------------------------------------------------------------------------------------------------------------

		inline f64			squared(f64 inNumber);
		inline f64			oneOver(f64 inNumber);

		//---------------------------------------------------------------------------------------------------------------------
		//    Compare routines. Like is equal etc.  
		//---------------------------------------------------------------------------------------------------------------------

		inline xbool		isZero(f64 inNumber);
		inline xbool		isPositive(f64 inNumber);
		inline xbool		isNegative(f64 inNumber);
		inline xbool		isLessPositive(f64 inLHS, f64 inRHS);
		inline xbool		isGreater(f64 inLHS, f64 inRHS);
	};

	//==============================================================================
	// INLINES
	//==============================================================================
	#include "private\x_double_inline.h"

	//==============================================================================
	// INLINE PC
	//==============================================================================
	#if defined TARGET_PC
		#include "private\x_double_inline_win32.h"

	//==============================================================================
	// INLINE PSP
	//==============================================================================
	#elif defined TARGET_PSP
		#include "private\x_double_inline_psp.h"   

	//==============================================================================
	// INLINE X360
	//==============================================================================
	#elif defined TARGET_360
		#include "private\x_double_inline_x360.h"  

	//==============================================================================
	// INLINE WII
	//==============================================================================
	#elif defined TARGET_WII
		#include "private\x_double_inline_wii.h"

    //==============================================================================
    // INLINE PS3
    //==============================================================================
    #elif defined(TARGET_PS3)
        #include "private/x_double_inline_ps3.h"

	//==============================================================================
	// INLINE 3DS
	//==============================================================================
	#elif defined(TARGET_3DS)
		#include "private/x_double_inline_3ds.h"
	#else
		#error "Current platform is not supported!"
	#endif

//==============================================================================
// END xCore namespace
//==============================================================================
};

#endif    ///< __XBASE_F64_FUNCTIONS_H__
