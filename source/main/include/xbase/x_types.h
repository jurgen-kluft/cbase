//------------------------------------------------------------------------------
// Author:
//     Virtuos Games
// Summary:
//     This file assists with cross platform development by providing a standard
//     set of type definitions.
// Description:
//     This file contains all basic types needed to create a homogeneous cross 
//     platform environment. Only key base types and definitions are included here. 
//------------------------------------------------------------------------------
#ifndef __XBASE_TYPES_H__
#define __XBASE_TYPES_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif        // Include this file only once

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//==============================================================================
	// INCLUDES
	//==============================================================================

	//DOM-IGNORE-BEGIN
	//==============================================================================

	#ifndef NULL
		#define NULL        0
	#endif

	typedef		unsigned	X_INT8   			u8;
	typedef		unsigned	X_INT16  			u16;
	typedef		unsigned	X_INT32  			u32;
	typedef					X_UINT64 			u64;
	typedef					X_UINT128			u128;
	typedef					X_UINT256			u256;
	typedef		signed      X_INT8   			s8;
	typedef		signed      X_INT16  			s16;
	typedef		signed      X_INT32  			s32;
	typedef					X_INT64  			s64;
	typedef					X_INT128 			s128;
	typedef					X_INT256 			s256;
	typedef					X_FLOAT  			f32;
	typedef					X_DOUBLE 			f64;
	typedef					X_BYTE   			xbyte;
	typedef					X_SIZE   			xalloc_size;
	typedef					X_BOOL				xbool32;
	typedef					X_BOOL				xbool;
	typedef					X_SIZE				xsize_t;
	typedef		unsigned	X_INT8   			xtchar;
	typedef					X_WCHAR  			xwchar;

	#define					X_INT32_4BYTE(a,b,c,d)	( (a)<<24 | (b)<<16 | (c)<<8 | (d) )

	//DOM-IGNORE-END
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Description:
	//     This class is used to define a handle which is 32bits wide. Handles have the property
	//     that can be pass to function as a unique type. This makes function type checking much 
	//     safer. The only thing which is standard in a handle is its NULL value which is xhandle::H_NULL
	//     while it has a member variable and it is public it is not recommended to access it if
	//     you don't have to.
	// See Also:
	//     X_HANDLE_TYPE
	//------------------------------------------------------------------------------
#ifdef H_NULL
	#pragma error
#endif

	struct xhandle
	{
		u32 mHandle;

		enum
		{
			H_NULL = 0xffffffff
		};

		inline				xhandle		(void) : mHandle(H_NULL)	{}
		inline explicit		xhandle		(u32 value)					{ mHandle = value;            }
		inline xbool		isValid		(void) const				{ return xbool(mHandle != H_NULL);    }    
		inline xbool		isNull 		(void) const				{ return xbool(mHandle == H_NULL);    }    
		inline void 		setNull		(void)						{ mHandle = (u32)H_NULL;            }  
		inline u32  		get			(void) const				{ return mHandle;             } 
		inline void			set			(u32 v)						{ mHandle = v;                } 
		inline bool			operator  ==(xhandle h) const			{ return h.mHandle == mHandle; }
		inline bool			operator  !=(xhandle h) const			{ return h.mHandle != mHandle; }
		inline bool			operator  <	(xhandle h) const			{ return mHandle < h.mHandle; }
		inline bool			operator  <=(xhandle h) const			{ return mHandle <= h.mHandle; }
		inline bool			operator  >	(xhandle h) const			{ return mHandle > h.mHandle; }
		inline bool			operator  >=(xhandle h) const			{ return mHandle <= h.mHandle; }
	};

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos Games
	// Summary:
	//     This macro it is used to create a unique handle type.
	// Arguments:
	//     TYPE_NAME  - Name of the type
	// Description:
	//     When creating classes that uses handles it is convenient to create unique types for
	//     user safety reasons. This macro allows to create a unique handle type with the same 
	//     functionality as the xhandle. 
	// See Also:
	//     xhandle
	//------------------------------------------------------------------------------

	#define X_HANDLE_TYPE(TYPE_NAME)																						\
	struct TYPE_NAME : public xcore::xhandle																				\
	{																														\
		inline                  TYPE_NAME               (void)						{ }										\
		inline explicit         TYPE_NAME               (xcore::u32 value)			{ mHandle = value; }					\
		inline xcore::xbool     operator  ==			(xcore::xhandle h) const	{ return h.mHandle == mHandle; }		\
		inline xcore::xbool     operator  !=			(xcore::xhandle h) const	{ return h.mHandle != mHandle; }		\
	};

	//==============================================================================
	//  HANDLE BOOL 
	//==============================================================================

	#ifdef xTRUE
	#undef xTRUE
	#endif
	#define                xTRUE		(X_BOOL)true

	#ifdef xFALSE
	#undef xFALSE
	#endif
	#define                xFALSE		(X_BOOL)false

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

//==============================================================================
// END X_TYPES_H
//==============================================================================
#endif
