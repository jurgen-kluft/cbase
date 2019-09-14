#ifndef __XBASE_RANDOM_H__
#define __XBASE_RANDOM_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xcore
{
	class xrandom
	{
	public:
		virtual				~xrandom() {}
		virtual void		reset(s32 inSeed = 0) = 0;
		virtual u32			generate() = 0;
	};

};

#endif // __XBASE_RANDOM_H__
