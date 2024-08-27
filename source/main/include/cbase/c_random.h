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

    class xor_random_t : public random_t
    {
        u64 s0, s1;

        inline u64 next(void)
        {
            u64 ss1    = s0;
            u64 ss0    = s1;
            u64 result = ss0 + ss1;
            s0         = ss0;
            ss1 ^= ss1 << 23;
            s1 = ss1 ^ ss0 ^ (ss1 >> 18) ^ (ss0 >> 5);
            return result;
        }

    public:
        inline xor_random_t(u64 seed = 0x1234567890abcdef)
            : s0(seed)
            , s1(0)
        {
            next();
            next();
        }

        void reset(s64 seed) override
        {
            s0 = seed;
            s1 = 0;
            next();
            next();
        }

        void generate(u8* outData, u32 numBytes) override
        {
            if (numBytes == 0)
                return;

            // Head
            u32 n = (8 - ((u64)outData & 0x07)) & 0x07;
            if (n != 0)
            {
                u64 const val = next();
                u8 const* p   = (u8 const*)&val;
                if (n > numBytes)
                    n = numBytes;
                numBytes -= n;
                switch (n)
                {
                    case 7: *outData++ = *p++;
                    case 6: *outData++ = *p++;
                    case 5: *outData++ = *p++;
                    case 4: *outData++ = *p++;
                    case 3: *outData++ = *p++;
                    case 2: *outData++ = *p++;
                    case 1: *outData++ = *p++;
                }
            }

            // Words
            u32 const numWords = numBytes >> 3;
            u64*      words    = (u64*)outData;
            for (u32 i = 0; i < numWords; ++i)
                *words++ = next();
            outData = (u8*)words;

            // Tail
            numBytes &= 0x07;
            if (numBytes != 0)
            {
                u64 const val = next();
                u8 const* p   = (u8 const*)&val;
                switch (numBytes)
                {
                    case 7: *outData++ = *p++;
                    case 6: *outData++ = *p++;
                    case 5: *outData++ = *p++;
                    case 4: *outData++ = *p++;
                    case 3: *outData++ = *p++;
                    case 2: *outData++ = *p++;
                    case 1: *outData++ = *p++;
                }
            }
        }
    };

};  // namespace ncore

#endif  // __CBASE_RANDOM_H__
