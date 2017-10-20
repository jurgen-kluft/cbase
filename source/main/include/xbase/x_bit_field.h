// x_bit_field.h - xCore bitfield template - 
#ifndef __XBASE_BITFIELD_H__
#define __XBASE_BITFIELD_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//------------------------------------------------------------------------------
	// Author:
	//		Jurgen Kluft
	// Description:
	//		Bitfield template helper functions are used to allow easy access to flags 
	//		and settings combined in a bitfield.
	//		 
	//		<taTYPE> must be the type of the enumerated value that is used to define
	//		the bits.
	//		 
	//		<taFIELD> (which defaults to u32) is the storage type which will hold
	//		the bitfield. This allows you to use the bitfield for types
	//		of 8, 16, 32 or 64 bits.
	//
	//<CODE [lang= "c++"]>
	// 		/// Position/orientation inheritance flags
	// 		enum EFlags
	// 		{
	// 			// Inheritance flags, determines which parts of the
	// 			// object's world position it inherits from it's parent
	// 			LINK_NONE       = 0x00000000,       ///< Don't link anything
	// 			LINK_POSITION   = 0x00000001,       ///< Link position
	// 			LINK_ROTATION   = 0x00000002,       ///< Link rotation
	// 			LINK_SCALE      = 0x00000004,       ///< Link scaling
	// 			LINK_ALL        = 0x00000007,       ///< Link everything
	// 
	// 			// General flags
	// 			CACHED_MATRIX   = 0x40000000,       ///< When set, mWorldMatrix is valid
	// 			VISIBLE         = 0x80000000        ///< Should this WorldNode be drawn?
	// 		};
	// 
	// 		u32		   mFlags;
	//</CODE>
	//     Now, mFlags allows you to modify the objects flags more easily than with
	//     the conventional AND, OR, XOR and similar operations. You can use commands
	//     like:
	//
	//<CODE [lang= "c++"]>
	// 		xbfSet(mFlags, LINK_ALL);
	// 		xbfToggle(mFlags, LINK_ROTATION);
	// 		xbfClear(mFlags, VISIBLE);
	//</CODE>
	//
	//------------------------------------------------------------------------------
	enum EBitState
	{
		BIT_SET,		///< All bits in the specified field are set
		BIT_CLEARED,	///< All bits in the specified field are cleared
		BIT_MIXED		///< Some bits in the specified field are set, others are cleared
	};

	inline bool     	xbfIsSet(u16 bitfield, u16 inField)   								{ return (bitfield & ((u16)inField)) == ((u16)inField); } ///< Check if all bits in <inField> are set
	inline bool     	xbfIsSet(u32 bitfield, u32 inField)   								{ return (bitfield & ((u32)inField)) == ((u32)inField); } ///< Check if all bits in <inField> are set
	inline bool     	xbfIsSet(u64 bitfield, u64 inField)   								{ return (bitfield & ((u64)inField)) == ((u64)inField); } ///< Check if all bits in <inField> are set

	inline bool     	xbfIsAnySet(u16 bitfield, u16 inField)      						{ return (bitfield & ((u16)inField)) != 0; }    ///< Check if any bits of <inField> have been set in this bitfield
	inline bool     	xbfIsAnySet(u32 bitfield, u32 inField)      						{ return (bitfield & ((u32)inField)) != 0; }    ///< Check if any bits of <inField> have been set in this bitfield
	inline bool     	xbfIsAnySet(u64 bitfield, u64 inField)      						{ return (bitfield & ((u64)inField)) != 0; }    ///< Check if any bits of <inField> have been set in this bitfield

	inline bool     	xbfIsOnlySet(u16 bitfield, u16 inField)     						{ return (bitfield == ((u16)inField)); }        ///< Check if <inField> is the only bit set in this bitfield
	inline bool     	xbfIsOnlySet(u32 bitfield, u32 inField)     						{ return (bitfield == ((u32)inField)); }        ///< Check if <inField> is the only bit set in this bitfield
	inline bool     	xbfIsOnlySet(u64 bitfield, u64 inField)     						{ return (bitfield == ((u64)inField)); }        ///< Check if <inField> is the only bit set in this bitfield

	inline bool     	xbfIsCleared(u16 bitfield, u16 inField)     						{ return (bitfield & ((u16)inField)) == 0; }    ///< Check if all bits in <inField> are cleared
	inline bool     	xbfIsCleared(u32 bitfield, u32 inField)     						{ return (bitfield & ((u32)inField)) == 0; }    ///< Check if all bits in <inField> are cleared
	inline bool     	xbfIsCleared(u64 bitfield, u64 inField)     						{ return (bitfield & ((u64)inField)) == 0; }    ///< Check if all bits in <inField> are cleared

	inline EBitState   	xbfGetState(u16 bitfield, u16 inField)     							///< get state of a number of bits
	{
		u16 result = bitfield & inField;
		if (result==0)				return BIT_CLEARED;
		else if (result==inField)	return BIT_SET;
		else						return BIT_MIXED;
	}
	inline EBitState   	xbfGetState(u32 bitfield, u32 inField)     							///< get state of a number of bits
	{
		u32 result = bitfield & inField;
		if (result==0)				return BIT_CLEARED;
		else if (result==inField)	return BIT_SET;
		else						return BIT_MIXED;
	}
	inline EBitState   	xbfGetState(u64 bitfield, u64 inField)     							///< get state of a number of bits
	{
		u64 result = bitfield & inField;
		if (result==0)				return BIT_CLEARED;
		else if (result==inField)	return BIT_SET;
		else						return BIT_MIXED;
	}

	inline void			xbfSet(u16& bitfield, u16 inField)                 					{ bitfield |= inField; } ///< Set <inField>
	inline void			xbfSet(u32& bitfield, u32 inField)                 					{ bitfield |= inField; } ///< Set <inField>
	inline void			xbfSet(u64& bitfield, u64 inField)                 					{ bitfield |= inField; } ///< Set <inField>

	inline void			xbfSet(u16& bitfield, u16 inField, u16 inMask)  					{ ASSERTS(inField==(inField&inMask), "<inField> contains bits not within the specified mask."); bitfield = (bitfield & (~inMask)) | inField; } ///< Mask off <inMask> and then set <inField>
	inline void			xbfSet(u32& bitfield, u32 inField, u32 inMask)  					{ ASSERTS(inField==(inField&inMask), "<inField> contains bits not within the specified mask."); bitfield = (bitfield & (~inMask)) | inField; } ///< Mask off <inMask> and then set <inField>
	inline void			xbfSet(u64& bitfield, u64 inField, u64 inMask)  					{ ASSERTS(inField==(inField&inMask), "<inField> contains bits not within the specified mask."); bitfield = (bitfield & (~inMask)) | inField; } ///< Mask off <inMask> and then set <inField>

	inline void			xbfTestSet(u16& bitfield, u16 inField, bool inValue)   				{ bitfield &= ~inField; bitfield |= (inField & (0-(u16)inValue)); } ///< Set or clear <inField> depending on <inValue>
	inline void			xbfTestSet(u32& bitfield, u32 inField, bool inValue)   				{ bitfield &= ~inField; bitfield |= (inField & (0-(u32)inValue)); } ///< Set or clear <inField> depending on <inValue>
	inline void			xbfTestSet(u64& bitfield, u64 inField, bool inValue)   				{ bitfield &= ~inField; bitfield |= (inField & (0-(u32)inValue)); } ///< Set or clear <inField> depending on <inValue>

	inline void			xbfClear(u16& bitfield, u16 inField)               					{ bitfield &= (~inField); } ///< clear <inField>
	inline void			xbfClear(u32& bitfield, u32 inField)               					{ bitfield &= (~inField); } ///< clear <inField>
	inline void			xbfClear(u64& bitfield, u64 inField)               					{ bitfield &= (~inField); } ///< clear <inField>

	inline void			xbfClearAll(u16& bitfield)                          				{ bitfield = (u16)0; } ///< clear all bits
	inline void			xbfClearAll(u32& bitfield)                          				{ bitfield = (u32)0; } ///< clear all bits
	inline void			xbfClearAll(u64& bitfield)                          				{ bitfield = (u64)0; } ///< clear all bits

	inline void			xbfToggle(u16& bitfield, u16 inField)              					{ bitfield ^= inField; } ///< Toggle <inField>
	inline void			xbfToggle(u32& bitfield, u32 inField)              					{ bitfield ^= inField; } ///< Toggle <inField>
	inline void			xbfToggle(u64& bitfield, u64 inField)              					{ bitfield ^= inField; } ///< Toggle <inField>


	template<class ENUM>
	struct xenum
	{
		typedef u32 xfield;
		inline				xenum() : bitfield(0)								{ ASSERTCT(sizeof(ENUM) <= sizeof(xfield)); }
		inline				xenum(ENUM inField) : bitfield(inField)				{ ASSERTCT(sizeof(ENUM) <= sizeof(xfield)); }

		inline bool     	IsSet(ENUM inField) const							{ return (bitfield & ((xfield)inField)) == ((xfield)inField); }		///< Check if all bits in <inField> are set
		inline bool     	IsAnySet(ENUM inField) const						{ return (bitfield & ((xfield)inField)) != 0; }						///< Check if any bits of <inField> have been set in this bitfield
		inline bool     	IsOnlySet(ENUM inField) const						{ return (bitfield == ((xfield)inField)); }							///< Check if <inField> is the only bit set in this bitfield
		inline bool     	IsCleared(ENUM inField) const						{ return (bitfield & ((xfield)inField)) == 0; }						///< Check if all bits in <inField> are cleared

		inline EBitState   	GetState(ENUM inField) const   						///< get state of a number of bits
		{
			xfield result = bitfield & (xfield)inField;
			if (result==0)						return BIT_CLEARED;
			else if (result==(xfield)inField)	return BIT_SET;
			else								return BIT_MIXED;
		}

		inline void			Set(ENUM inField)                 					{ bitfield |= (xfield)inField; } ///< Set <inField>
		inline void			SetMasked(ENUM inField, ENUM inMask)  				{ ASSERTS((xfield)inField==((xfield)inField&(xfield)inMask), "<inField> contains bits not within the specified mask."); bitfield = (bitfield & (~inMask)) | inField; } ///< Mask off <inMask> and then set <inField>
		inline void			TestSet(ENUM inField, bool inValue)   				{ bitfield &= ~(xfield)inField; bitfield |= ((xfield)inField & (0-(xfield)inValue)); } ///< Set or clear <inField> depending on <inValue>
		inline void			Clear(ENUM inField)               					{ bitfield &= (~(xfield)inField); } ///< clear <inField>
		inline void			ClearAll()                          				{ bitfield = (xfield)0; } ///< clear all bits
		inline void			Toggle(ENUM inField)              					{ bitfield ^= (xfield)inField; } ///< Toggle <inField>

	private:
		xfield				bitfield;
	};

	#define XENUM_BEGIN(enum_name)				\
			class enum_name						\
			{									\
			public:								\
				enum EEnum
												

	#define XENUM_END(enum_name)				\
				;								\
				inline enum_name(EEnum _e) : e(_e){} \
				EEnum e;						\
			};

	#define XENUM_DECLARE(enum_name)			\
			class enum_name;

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

//==============================================================================
// END
//==============================================================================
#endif    /// __XBASE_BITFIELD_H__
