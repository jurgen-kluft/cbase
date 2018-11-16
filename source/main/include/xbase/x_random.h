#ifndef __XBASE_RANDOM_H__
#define __XBASE_RANDOM_H__
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
	class xbuffer;

	//----------------------------------------------------------------------------------------------------------------
	// Random Number Generator
	class xrandom
	{
	public:
		virtual				~xrandom() {}
		virtual void		reset(u32 inSeed = 0) = 0;
		virtual u32			generate() = 0;
	};

};

#endif // __XBASE_RANDOM_H__
