#ifndef __CBASE_RANDOM_H__
#define __CBASE_RANDOM_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_debug.h"

namespace ncore
{
    class random_t
    {
    public:
        virtual ~random_t() {}
        virtual void reset(s64 inSeed = 0)               = 0;
        virtual void generate(u8* outData, u32 numBytes) = 0;
    };

    inline bool random_bool(random_t* inRandom)
    {
        u8 val;
        inRandom->generate(&val, 1);
        return val == 0;
    }
    inline u32 random_u32(random_t* inRandom, u32 inBits = 32)
    {
        ASSERT(inBits <= 32);
        u32 val;
        u8* pval = (u8*)&val;
        inRandom->generate(pval, 4);
        return (val >> (32 - inBits));
    }
    inline s32 random_s32(random_t* inRandom, u32 inBits = 31)
    {
        ASSERT(inBits <= 31);
        return ((s32)random_u32(inRandom, inBits + 1) - (s32)(1 << inBits));
    }
    inline f32 random_f32(random_t* inRandom)
    {
        u32 r          = random_u32(inRandom);
        u32 fake_float = (r >> (32 - 23)) | 0x3f800000;
        return ((*(f32*)&fake_float) - 1.0f);
    }
    inline f32 random_f32S(random_t* inRandom) { return ((random_f32(inRandom) - 0.5f) * 2.0f); }

    s32 random_s32_0_max(random_t* inRandom, s32 maxt);
    s32 random_s32_0_1(random_t* inRandom);
    s32 random_s32_neg1_pos1(random_t* inRandom);
    f32 random_f32_0_1(random_t* inRandom);
    f64 random_f64_0_1(random_t* inRandom);

};  // namespace ncore

#endif  // __CBASE_RANDOM_H__
