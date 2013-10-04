// x_integer.h - Basic bit/int trick operations
#ifndef __XBASE_INTEGER_UTILS_H__
#define __XBASE_INTEGER_UTILS_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\private\x_int64.h"
#include "xbase\private\x_uint64.h"

#include "xbase\private\x_int128.h"
#include "xbase\private\x_uint128.h"

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"

//==============================================================================
// xCore namespace
//==============================================================================
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

	namespace x_intu
	{
		#ifndef __GNU_C__ 
			#define X_INTU_OPTIMIZED
		#endif

		template< class T > T	min(T integerA, T integerB);			///< Return the smallest value
		template< class T > T	max(T integerA, T integerB);			///< Return the biggest value
		template< class T > T	sign(T integerA)		               { if (integerA > 0) return 1; else if (integerA < 0) return -1; else return 0; }
		template< class T > T	clamp(T integerA, T low, T high);		///< Return the clamp value

		inline s32			align(s32 integer, s32 alignment);			///< Return the aligned value of integer
		inline s32			alignDown(s32 integer, s32 alignment);		///< Return the aligned value of integer
		inline s32			alignUp(s32 integer, s32 alignment);		///< Return the aligned value of integer
		inline xbool		isAligned(s32 integer, s32 alignment);		///< Return xTRUE if integer is aligned
		inline s32			abs(s32 integer);							///< Return the absolute value of integer
		inline s32			neg(s32 integer, bool inDoNotNegate);		///< Return the conditionally negated value of integer
		inline s32			sqr(s32 integer);							///< Return the square of integer
		inline u32			sqr(u32 integer);							///< Return the unsigned square of integer
		inline s32			mod(s32 integer, s32 inModuloValue);		///< Return the modulo of integer using inModuloValue
		inline s32			gcd(s32 inIntegerA, s32 integerB);			///< Greatest common denominator (biggest modulo value of both integers)
		inline s32			average(s32 integerA, s32 integerB);		///< Calculate average of A & B without overflow or s64 use
		inline xbool		isPowerOf2(s32 integer);					///< Check if integer is a power-of-two
		inline u32			ceilPower2(u32 integer);					///< Return the smallest power-of-two larger than integer
		inline u32			floorPower2(u32 integer);					///< Return the biggest power-of-two smaller than integer
		inline u32			bitReverse(u32 integer);					///< Reverse bits in 32 bit word
		inline s32			countBits(u32 integer);						///< count one bits in 32 bit word
		inline s32			ilog2(s32 integer);							///< Log2 of an integer
		inline s32			countTrailingZeros(s32 integer);			///< find the number of trailing zeros in 32-bit v 
		inline s32			countLeadingZeros(s32 integer);				///< find the number of leading zeros in 32-bit v 
		inline s32			leastSignificantOneBit(u32 inInteger);		///< Return v but with only the Least Significant Bit "1"
		inline s32			mostSignificantOneBit(u32 integer);			///< Return v but with only the Most Significant Bit "1"
		inline s32			leastSignificantBit(u32 integer);			///< Return the bit index of the Least Significant Bit "1"
		inline s32			mostSignificantBit(u32 integer);			///< Return the bit index of the Most Significant Bit "1"
		inline s32			findFirstBit(u32 integer);					///< find the bit position/index of the first bit from low to high
		inline s32			findLastBit(u32 integer);					///< find the bit position/index of the first bit from high to low
		inline u32			rol32(u32 integer, u32 shift);				///< Roll all the bits in integer to the left by shift number of bits
		inline u32			ror32(u32 integer, u32 shift);				///< Roll all the bits in integer to the right by shift number of bits

		// template specialization declaration
		template<> s32	min(s32 integerA, s32 integerB);				///< Return the smallest value
		template<> s32	max(s32 integerA, s32 integerB);				///< Return the biggest value
	};

	//==============================================================================
	// INLINES
	//==============================================================================
	#include "xbase\private\x_integer_inline.h"

	#if defined TARGET_WII
		#include "xbase\private\x_integer_inline_wii.h"
	#elif defined TARGET_PSP
		#include "xbase\private\x_integer_inline_psp.h"
	#elif defined TARGET_360
		#include "xbase\private\x_integer_inline_x360.h"
	#elif defined TARGET_PC
		#include "xbase\private\x_integer_inline_win32.h"
	#elif defined TARGET_PS3
		#include "xbase\private\x_integer_inline_ps3.h"
	#elif defined TARGET_3DS
		#include "xbase\private\x_integer_inline_3ds.h"
	#else
		#error "Current platform is not supported!"
	#endif

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif    ///< __XBASE_INTEGER_UTILS_H__
