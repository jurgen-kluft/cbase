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
	enum EBits
	{
		BITS_SET,		///< All bits in the specified field are set
		BITS_CLEARED,	///< All bits in the specified field are cleared
		BITS_MIXED		///< Some bits in the specified field are set, others are cleared
	};

	template<typename T>
	class xbits
	{
	public:
		inline 				xbits(T inField) : mField(inField)						{ }
		inline 				xbits(const xbits<T>& inField) : mField(inField.mField)	{ }
		
		inline bool     	is_set(T inField) const  							{ return (mField & ((T)inField)) == ((T)inField); } ///< Check if all bits in <inField> are set
		inline bool     	is_any_set(T inField) const     					{ return (mField & ((T)inField)) != 0; }    ///< Check if any bits of <inField> have been set in this bitfield
		inline bool     	is_only_set(T inField) const    					{ return (mField == ((T)inField)); }        ///< Check if <inField> is the only bit set in this bitfield
		inline bool     	is_cleared(T inField) const     					{ return (mField & ((T)inField)) == 0; }    ///< Check if all bits in <inField> are cleared

		inline void			set(T inField)                 						{ mField |= inField; } ///< Set <inField>
		inline void			set(T inField, T inMask) 	 						{ ASSERTS(inField==(inField&inMask), "<inField> contains bits not within the specified mask."); mField = (mField & (~inMask)) | inField; } ///< Mask off <inMask> and then set <inField>
		inline void			test_set(T inField, bool inValue)   				{ mField &= ~inField; mField |= (inField & (0-(T)inValue)); } ///< Set or clear <inField> depending on <inValue>
		inline void			clear(T inField)               						{ mField &= (~inField); } ///< clear <inField>
		inline void			clear_all()   	                       				{ mField = (T)0; } ///< clear all bits
		inline void			toggle(T inField)              						{ mField ^= inField; } ///< Toggle <inField>

		inline EBits   		get_state(T inField)     							///< get state of a number of bits
		{
			T result = mField & inField;
			if (result==0)				return BITS_CLEARED;
			else if (result==inField)	return BITS_SET;
			else						return BITS_MIXED;
		}

		xbits<T>&			operator = (const xbits<T>& other) 					{ mField = other.mField; return *this; }

		bool				operator == (const xbits<T>& other) const			{ return mField == other.mField; }
		bool				operator != (const xbits<T>& other) const			{ return mField != other.mField; }

		T					mField;
	};


	template<class ENUM>
	struct xenum
	{
		typedef u32 xfield;
		inline				xenum() : bitfield(0)								{ ASSERTCT(sizeof(ENUM) <= sizeof(xfield)); }
		inline				xenum(ENUM inField) : bitfield(inField)				{ ASSERTCT(sizeof(ENUM) <= sizeof(xfield)); }

		inline bool     	is_set(ENUM inField) const							{ return (bitfield & ((xfield)inField)) == ((xfield)inField); }		///< Check if all bits in <inField> are set
		inline bool     	is_any_set(ENUM inField) const						{ return (bitfield & ((xfield)inField)) != 0; }						///< Check if any bits of <inField> have been set in this bitfield
		inline bool     	is_only_set(ENUM inField) const						{ return (bitfield == ((xfield)inField)); }							///< Check if <inField> is the only bit set in this bitfield
		inline bool     	is_cleared(ENUM inField) const						{ return (bitfield & ((xfield)inField)) == 0; }						///< Check if all bits in <inField> are cleared

		inline EBits   		get_state(ENUM inField) const   					///< get state of a number of bits
		{
			xfield result = bitfield & (xfield)inField;
			if (result==0)						return BITS_CLEARED;
			else if (result==(xfield)inField)	return BITS_SET;
			else								return BITS_MIXED;
		}

		inline void			set(ENUM inField)                 					{ bitfield |= (xfield)inField; } ///< Set <inField>
		inline void			set_masked(ENUM inField, ENUM inMask)  				{ ASSERTS((xfield)inField==((xfield)inField&(xfield)inMask), "<inField> contains bits not within the specified mask."); bitfield = (bitfield & (~inMask)) | inField; } ///< Mask off <inMask> and then set <inField>
		inline void			test_set(ENUM inField, bool inValue)   				{ bitfield &= ~(xfield)inField; bitfield |= ((xfield)inField & (0-(xfield)inValue)); } ///< Set or clear <inField> depending on <inValue>
		inline void			clear(ENUM inField)               					{ bitfield &= (~(xfield)inField); } ///< clear <inField>
		inline void			clear_all()                          				{ bitfield = (xfield)0; } ///< clear all bits
		inline void			toggle(ENUM inField)              					{ bitfield ^= (xfield)inField; } ///< Toggle <inField>

		xenum<ENUM>&		operator = (const xenum<ENUM>& other) 				{ bitfield = other.bitfield; return *this; }

		bool				operator == (const xenum<ENUM>& other) const		{ return bitfield == other.bitfield; }
		bool				operator != (const xenum<ENUM>& other) const		{ return bitfield != other.bitfield; }

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

};

#endif    /// __XBASE_BITFIELD_H__
