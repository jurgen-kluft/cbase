// bitfield.h - xCore bitfield template - 
#ifndef __XBASE_BITFIELD_H__
#define __XBASE_BITFIELD_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_types.h"
#include "xbase\x_debug.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//------------------------------------------------------------------------------
	// Author:
	//		Jurgen Kluft
	// Description:
	//		The bitfield template is used to allow easy access to flags and settings
	//		combined in a bitfield.
	//		 
	//		<taTYPE> must be the type of the enumerated value that is used to 
	//		 
	//		<taFIELD> (which defaults to uint32) is the storage type which will hold
	//		the bitfield. This allows you to use the bitfield template for bitsfields
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
	// 		x_bitfield<EFlags>    mFlags;
	//</CODE>
	//     Now, mFlags allows you to modify the objects flags more easily than with
	//     the conventional AND, OR, XOR and similar operations. You can use commands
	//     like:
	//
	//<CODE [lang= "c++"]>
	// 		mFlags.set(LINK_ALL);
	// 		mFlags.toggle(LINK_ROTATION);
	// 		mFlags.clear(VISIBLE);
	//</CODE>
	//
	// See Also:
	//     x_Flag
	//------------------------------------------------------------------------------
	template <typename taTYPE, typename taFIELD = s32>
	class x_bitfield
	{
		typedef				x_bitfield<taTYPE, taFIELD>								__self;
	public:
		enum EState
		{
			SET,                                                                    ///< All bits in the specified field are set
			CLEARED,                                                                ///< All bits in the specified field are cleared
			MIXED                                                                    ///< Some bits in the specified field are set, others are cleared
		};

		///@name Construction/Destruction
		inline				x_bitfield()                           					: mField(taFIELD()) { } ///< Construct bitfield
		inline				x_bitfield(const __self& other)        					: mField(other.mField) { } ///< Construct bitfield from other bitfield
		inline				x_bitfield(taTYPE inDefault)           					: mField(inDefault) { }

		///@name Properties
		inline bool     	isSet(taTYPE inField) const         					{ return (mField & ((taFIELD)inField)) == ((taFIELD)inField); } ///< Check if all bits in <inField> are set
		inline bool     	isAnySet(taTYPE inField) const      					{ return (mField & ((taFIELD)inField)) != 0; }    ///< Check if any bits of <inField> have been set in this bitfield
		inline bool     	isOnlySet(taTYPE inField) const     					{ return (mField == ((taFIELD)inField)); }        ///< Check if <inField> is the only bit set in this bitfield
		inline bool     	isCleared(taTYPE inField) const     					{ return (mField & ((taFIELD)inField)) == 0; }    ///< Check if all bits in <inField> are cleared
		inline EState   	getState(taTYPE inField) const;     					///< get state of a number of bits
		inline taFIELD  	getBinary(taTYPE inField) const     					{ return (mField & inField); } ///< Checks a field, and returns the field value
		inline void     	setBinary(taFIELD inValue)          					{ mField=inValue; }

		///@name Actions		
		inline void			set(taTYPE inField)                 					{ mField |= inField; } ///< Set <inField>
		inline void			set(taTYPE inField, taTYPE inMask)  					{ ASSERTS(inField==(inField&inMask), "<inField> contains bits not within the specified mask."); mField = (mField & (~inMask)) | inField; } ///< Mask off <inMask> and then set <inField>
		inline void			set(taTYPE inField, bool inValue)   					{ mField &= ~inField; if (inValue) mField |= inField; } ///< Set or clear <inField> depending on <inValue>
		inline void			clear(taTYPE inField)               					{ mField &= (~inField); } ///< clear <inField>
		inline void			clearAll()                          					{ mField = (taFIELD)0; } ///< clear all bits
		inline void			toggle(taTYPE inField)              					{ mField ^= inField; } ///< Toggle <inField>

		///@name Binary operations
		inline __self&		operator |=(const __self &inRHS)     					{ mField |= inRHS.mField; return *this; } ///< Unite this bitfield with another
		inline __self&		operator &=(const __self &inRHS)     					{ mField &= inRHS.mField; return *this; } ///< Difference this bitfield with another
		inline __self 		operator |(const __self& inRHS) const					{ __self temp = *this; temp |= inRHS; return temp; } 
		inline __self 		operator &(const __self& inRHS) const					{ __self temp = *this; temp &= inRHS; return temp; } 

		///@name Comparison
		inline bool			operator==(const __self& inRHS) const					{ return mField == inRHS.mField; } ///< Check if two bitfields are equal
		inline bool			operator!=(const __self& inRHS) const					{ return mField != inRHS.mField; } ///< Check if two bitfields are inequal

		///@name Conversion
		inline				operator taTYPE() const              					{ return (taTYPE)mField; } ///< Convert BitField<taTYPE, taFIELD> to taTYPE

	protected:
		///@name Binary data
		taFIELD				mField;													///< The bitfield data
	};

	//------------------------------------------------------------------------------
	// Author:
	//     Jurgen Kluft
	// Summary:
	//     Check the bitfield against the bits in <taField>, returns an EState for these bits.
	// Description:
	//     Returns:
	// 		- SET if all bits in <inField> are set in this bitfield
	// 		- MIXED if at least one - but not all - bits in <inField> are set in this bitfield
	// 		- CLEARED if all bits in <inField> are cleared in this bitfield
	// See Also:
	//     BREAK
	//------------------------------------------------------------------------------
	template <typename taTYPE, typename taFIELD>
	inline typename x_bitfield<taTYPE, taFIELD>::EState x_bitfield<taTYPE, taFIELD>::getState(taTYPE inField) const
	{
		taFIELD result=mField & inField;
		if (result==0)
			return CLEARED;
		else if (result==inField)
			return SET;
		else
			return MIXED;
	}


	//==============================================================================
	// x_FlagToggle -   Toggles a bit flag on or off depending of its previous status
	// x_FlagOn     -   Turns a bit on in an atomic integer type. 
	// x_FlagOff    -   Turns a bit off in an atomic integer type. 
	// x_FlagIsOn   -   Test whether a bit is on or off.
	//==============================================================================

	template< class ta > void    x_FlagToggle(       ta& N, const u32 F );
	template< class ta > void    x_FlagOn    (       ta& N, const u32 F );
	template< class ta > void    x_FlagOff   (       ta& N, const u32 F );
	template< class ta > xbool   x_FlagIsOn  ( const ta  N, const u32 F );
	template< class ta > xbool   x_FlagsAreOn( const ta  N, const u32 F );

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Toggles a bit flag on or off depending of its previous status. 
	// Arguments:
	//     N - Is the atomic type to activate the flag. Note that the variable passed will changed its value.
	//     F - Is the bit flags which identifies which which bits to toggle
	// Returns:
	//     void
	// Description:
	//     Toggles a bit flag on or off depending of its previous status. 
	// See Also:
	//     x_FlagOn x_FlagOff x_FlagsAreOn X_BIN x_Bit x_FlagIsOn
	//------------------------------------------------------------------------------
	template< class ta > inline 
	void x_FlagToggle( ta& N, const u32 F )
	{ 
		N = (ta)(N^F);
	}

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Turns a bit on in an atomic integer type. 
	// Arguments:
	//     N - Is the atomic type to activate the flag. Note that the variable passed will changed its value.
	//     F - Is the flag
	// Returns:
	//     void
	// Description:
	//     Turns a bit on in an atomic integer type. 
	// See Also:
	//     x_FlagOff x_FlagsAreOn X_BIN x_Bit x_FlagIsOn x_FlagToggle
	//------------------------------------------------------------------------------
	template< class ta > inline 
	void x_FlagOn( ta& N, const u32 F )
	{ 
		N = (ta)(N|F); 
	}

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Turns a bit off in an atomic integer type. 
	// Arguments:
	//     N - Is the atomic type to deactivate the flag. Note that the variable passed will changed its value.
	//     F - Is the flag
	// Returns:
	//     void
	// Description:
	//     Turns a bit off in an atomic integer type. 
	// See Also:
	//     x_FlagOn x_FlagIsOn X_BIN x_Bit x_FlagsAreOn x_FlagToggle
	//------------------------------------------------------------------------------
	template< class ta > inline 
	void x_FlagOff( ta& N, const u32 F )
	{ 
		N = (ta)(N & (~F)); 
	}

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Test whether a set of flags is on at all or not.
	// Arguments:
	//     N - Is the atomic type that contains the current value.  
	//     F - Is the set of flags to test.
	// Returns:
	//     xTRUE if any of the flags in F turns to be on, xFALSE other wise.
	// Description:
	//     Test whether in a set of flags one IS on or not.
	// See Also:
	//     x_FlagOff X_BIN x_Bit x_FlagsAreOn x_FlagOn x_FlagToggle
	//------------------------------------------------------------------------------
	template< class ta > inline 
	xbool x_FlagIsOn( const ta  N, const u32 F )
	{ 
		return xbool((N&F)!=0); 
	}

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     Test whether a set of flags is on at all or not.
	// Arguments:
	//     N - Is the atomic type that contains the current value.  
	//     F - Is the set of flags to test.
	// Returns:
	//     xTRUE if any of the flags in F turns to be on, xFALSE other wise.
	// Description:
	//     Test whether in a set of flags all ARE on or not.
	// See Also:
	//     x_FlagOff X_BIN x_Bit x_FlagIsOn x_FlagOn x_FlagToggle
	//------------------------------------------------------------------------------
	template< class ta > inline 
	xbool x_FlagsAreOn( const ta  N, const u32 F )
	{ 
		return (N&F)==F; 
	}

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

//==============================================================================
// END
//==============================================================================
#endif    /// __XBASE_BITFIELD_H__
