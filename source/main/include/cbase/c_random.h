#ifndef __CBASE_RANDOM_H__
#define __CBASE_RANDOM_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace ncore
{
	class random_t
	{
	public:
		virtual				~random_t() {}
		virtual void		reset(s64 inSeed = 0) = 0;
		virtual u32			generate() = 0;
	};

};

#endif // __CBASE_RANDOM_H__
