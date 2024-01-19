#ifndef __CBASE_RANDOM_H__
#define __CBASE_RANDOM_H__
#include "ccore/c_target.h"
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
		virtual void		generate(u8* outData, u32 numBytes) = 0;
	};

	bool random_bool(random_t* inRandom);
	u32 random_u32(random_t* inRandom);
	s32 random_s32_zero_limit(random_t* inRandom, s32 maxt);
	s32 random_s32_zero_pos1(random_t* inRandom);
	s32 random_s32_neg1_pos1(random_t* inRandom);
	f32 random_f32_zero_pos1(random_t* inRandom);
	f64 random_f64_zero_pos1(random_t* inRandom);

};

#endif // __CBASE_RANDOM_H__
