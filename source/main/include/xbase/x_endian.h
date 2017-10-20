#ifndef __XBASE_ENDIAN_H__
#define __XBASE_ENDIAN_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"

/**
 * xCore namespace
 */
namespace xcore
{
	/**
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     Function swaps the endian of a given atomic type. There is one function per type.
	 * Arguments:
	 *     A - Where A is a number to be endian swapped. 
	 * Returns:
	 *     Is the same as the source except with the endian swapped.
	 * Description:
	 *     The LITTLE_ENDIAN_xx and BIG_ENDIAN_xx macros convert the Endian of 16 or 32
	 *     bit values between the system "native" Endian to the specified Endian.  Note
	 *     that these macros work for both reading and writing.  Or, said another way, 
	 *     these macros toggle when the system native Endian does NOT match the macro 
	 *     name, and do nothing when the system native does match the macro name.
	 * See Also:
	 *     X_LITTLE_ENDIAN X_BIG_ENDIAN X_INTEL_ENDIAN
	 */
	namespace x_endian_swap
	{
		inline char		swap(const char  A)				{ return A; }
		inline u8		swap(const u8  A)    			{ return A; }
		inline s8		swap(const s8  A)    			{ return A; }
		inline u16		swap(const u16 A)    			{ return (A >>  8) | (A <<  8); }
		inline u32		swap(const u32 A)    			{ return (A >> 24) | (A << 24) | ((A & 0x00FF0000) >> 8) | ((A & 0x0000FF00) << 8); }
		inline u64		swap(const u64 A)    			{ return (A >> 56) | (A << 56) | ((A & X_CONSTANT_64(0x00FF000000000000)) >> 40) | ((A & X_CONSTANT_64(0x000000000000FF00)) << 40) | ((A & X_CONSTANT_64(0x0000FF0000000000)) >> 24) | ((A & X_CONSTANT_64(0x0000000000FF0000)) << 24) | ((A & X_CONSTANT_64(0x000000FF00000000)) >> 8) | ((A & X_CONSTANT_64(0x00000000FF000000)) << 8); }
		inline s16		swap(const s16 A)    			{ return (s16)swap( (u16)A ); }
		inline s32		swap(const s32 A)    			{ return (s32)swap( (u32)A ); }
		inline s64		swap(const s64 A)    			{ return (s64)swap( (u64)A ); }
		inline f32		swap(const f32 A)    			{ u32 a = swap( *((u32*)&A) ); return *(f32*)&a; }
		inline f64		swap(const f64 A)    			{ u64 a = swap( *((u64*)&A) ); return *(f64*)&a; }
	};

	namespace x_endian_nop
	{
		inline char		swap(const char A)				{ return A; }
		inline u8		swap(const u8  A)    			{ return A; }
		inline s8		swap(const s8  A)    			{ return A; }
		inline u16		swap(const u16 A)    			{ return A; }
		inline u32		swap(const u32 A)    			{ return A; }
		inline u64		swap(const u64 A)    			{ return A; }
		inline s16		swap(const s16 A)    			{ return A; }
		inline s32		swap(const s32 A)    			{ return A; }
		inline s64		swap(const s64 A)    			{ return A; }
		inline f32		swap(const f32 A)    			{ return A; }
		inline f64		swap(const f64 A)    			{ return A; }
	};

	//==============================================================================
	namespace x_endian = x_endian_swap;

	/**
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     This macro returns the number/variable converted into intels endian.
	 * Description:
	 *     This macro uses the swap to convert when needed a number to 
	 *     the intel's native endian.
	 * See Also:
	 *     x_NetworkEndian swap X_LITTLE_ENDIAN X_BIG_ENDIAN
	 */
	#ifdef X_LITTLE_ENDIAN
		namespace x_IntelEndian = x_endian_nop;
	#else
		namespace x_IntelEndian = x_endian_swap;
	#endif

	/**
	 * Author:
	 *     Virtuos Games
	 * Summary:
	 *     This macro returns the number/variable converted into network endian.
	 * Description:
	 *     This macro uses the swap to convert when needed a number to 
	 *     the network endianness.
	 * See Also:
	 *     x_IntelEndian swap X_LITTLE_ENDIAN X_BIG_ENDIAN
	 */
	#ifdef X_LITTLE_ENDIAN
		namespace x_NetworkEndian = x_endian_swap;
	#else
		namespace x_NetworkEndian = x_endian_nop;
	#endif


//==============================================================================
// END xCore namespace
//==============================================================================
};
/**
 *  END xCore namespace
 */

#endif ///< END __XBASE_ENDIAN_H__
