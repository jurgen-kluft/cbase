#ifndef __XBASE_TYPES_H__
#define __XBASE_TYPES_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xcore
{
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

	#define X_HANDLE_TYPE(TYPE_NAME)																						\
	struct TYPE_NAME : public xcore::xhandle																				\
	{																														\
		inline                  TYPE_NAME               (void)						{ }										\
		inline explicit         TYPE_NAME               (xcore::u32 value)			{ mHandle = value; }					\
		inline xcore::xbool     operator  ==			(xcore::xhandle h) const	{ return h.mHandle == mHandle; }		\
		inline xcore::xbool     operator  !=			(xcore::xhandle h) const	{ return h.mHandle != mHandle; }		\
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

//==============================================================================
// END X_TYPES_H
//==============================================================================
#endif
