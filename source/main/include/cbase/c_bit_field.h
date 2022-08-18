// x_bit_field.h - xCore bitfield template - 
#ifndef __CBASE_BITFIELD_H__
#define __CBASE_BITFIELD_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "cbase/c_debug.h"

namespace ncore
{
	//------------------------------------------------------------------------------
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
	class bits_t
	{
	public:
		inline 				bits_t(T inField) : mField(inField)						{ }
		inline 				bits_t(const bits_t<T>& inField) : mField(inField.mField)	{ }
		
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

		bits_t<T>&			operator = (const bits_t<T>& other) 					{ mField = other.mField; return *this; }

		bool				operator == (const bits_t<T>& other) const			{ return mField == other.mField; }
		bool				operator != (const bits_t<T>& other) const			{ return mField != other.mField; }

		T					mField;
	};


	template<class ENUM>
	struct enum_t
	{
		typedef u32 field_t;
		inline				enum_t() : bitfield(0)								{ ASSERTCT(sizeof(ENUM) <= sizeof(field_t)); }
		inline				enum_t(field_t inField) : bitfield(inField)			{ ASSERTCT(sizeof(ENUM) <= sizeof(field_t)); }
		inline				enum_t(ENUM inField) : bitfield(inField)			{ ASSERTCT(sizeof(ENUM) <= sizeof(field_t)); }

		inline bool     	is_set(ENUM inField) const							{ return (bitfield & ((field_t)inField)) == ((field_t)inField); }		///< Check if all bits in <inField> are set
		inline bool     	is_any_set(ENUM inField) const						{ return (bitfield & ((field_t)inField)) != 0; }						///< Check if any bits of <inField> have been set in this bitfield
		inline bool     	is_only_set(ENUM inField) const						{ return (bitfield == ((field_t)inField)); }							///< Check if <inField> is the only bit set in this bitfield
		inline bool     	is_cleared(ENUM inField) const						{ return (bitfield & ((field_t)inField)) == 0; }						///< Check if all bits in <inField> are cleared

		inline EBits   		get_state(ENUM inField) const   					///< get state of a number of bits
		{
			field_t result = bitfield & (field_t)inField;
			if (result==0)						return BITS_CLEARED;
			else if (result==(field_t)inField)	return BITS_SET;
			else								return BITS_MIXED;
		}

		inline void			set(ENUM inField)                 					{ bitfield |= (field_t)inField; } ///< Set <inField>
		inline void			set_masked(ENUM inField, ENUM inMask)  				{ ASSERTS((field_t)inField==((field_t)inField&(field_t)inMask), "<inField> contains bits not within the specified mask."); bitfield = (bitfield & (~inMask)) | inField; } ///< Mask off <inMask> and then set <inField>
		inline void			test_set(ENUM inField, bool inValue)   				{ bitfield &= ~(field_t)inField; bitfield |= ((field_t)inField & (0-(field_t)inValue)); } ///< Set or clear <inField> depending on <inValue>
		inline void			clear(ENUM inField)               					{ bitfield &= (~(field_t)inField); } ///< clear <inField>
		inline void			clear_all()                          				{ bitfield = (field_t)0; } ///< clear all bits
		inline void			toggle(ENUM inField)              					{ bitfield ^= (field_t)inField; } ///< Toggle <inField>

		enum_t<ENUM>&		operator = (const enum_t<ENUM>& other) 				{ bitfield = other.bitfield; return *this; }

		bool				operator == (const enum_t<ENUM>& other) const		{ return bitfield == other.bitfield; }
		bool				operator != (const enum_t<ENUM>& other) const		{ return bitfield != other.bitfield; }

	private:
		field_t				bitfield;
	};

	#define DENUM_BEGIN(enum_name)				\
			class enum_name						\
			{									\
			public:								\
				enum EEnum
												

	#define DENUM_END(enum_name)				\
				;								\
				inline enum_name(EEnum _e) : e(_e){} \
				EEnum e;						\
			};

	#define DENUM_DECLARE(enum_name)			\
			class enum_name;

};

#endif    /// __CBASE_BITFIELD_H__
