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
        virtual u32  rand32()                            = 0;
        virtual u64  rand64()                            = 0;
        virtual void generate(u8* outData, u32 numBytes) = 0;
    };

    inline bool g_random_bool(random_t* rng)
    {
        u8 val;
        rng->generate(&val, 1);
        return val < 128;
    }
    inline u32 g_random_u32(random_t* rng) { return rng->rand32(); }
    inline u32 g_random_u32(random_t* rng, s8 inBits)
    {
        ASSERT(inBits <= 32);
        u32 val = rng->rand32();
        return (val >> (32 - inBits));
    }
    inline s32 g_random_s32(random_t* rng) { return (s32)(g_random_u32(rng) & 0x7fffffff); }
    inline s32 g_random_s32(random_t* rng, s8 inBits)
    {
        ASSERT(inBits <= 31);
        return ((s32)g_random_u32(rng, inBits + 1) - (s32)(1 << inBits));
    }
    inline u64 g_random_u64(random_t* rng) { return rng->rand64(); }
    inline u64 g_random_u64(random_t* rng, s8 inBits)
    {
        ASSERT(inBits <= 64);
        u64 val = rng->rand64();
        return (val >> (64 - inBits));
    }
    inline s64 g_random_s64(random_t* rng) { return ((s64)g_random_u64(rng) & D_CONSTANT_U64(0x7fffffffffffffff)); }
    inline s64 g_random_s64(random_t* rng, s8 inBits)
    {
        ASSERT(inBits <= 63);
        return ((s64)g_random_u64(rng, inBits + 1) - ((s64)1 << inBits));
    }
    inline f32 g_random_f32(random_t* rng) { return (f32)g_random_u32(rng) / (f32)cU32Max; }
    inline f32 g_random_f32_min_max(random_t* rng, f32 _min, f32 _max) { return _min + (g_random_f32(rng) * (_max - _min)); }
    inline f32 g_random_f32S(random_t* rng) { return ((g_random_f32(rng) - 0.5f) * 2.0f); }
    inline s32 g_random_s32_0_max(random_t* rng, s32 max) { return (s32)g_random_u32(rng, 31) % max; }
    inline s32 g_random_s32_0_1(random_t* rng) { return g_random_s32_0_max(rng, 2); }
    inline s32 g_random_s32_neg1_pos1(random_t* rng)
    {
        u32 const val = g_random_u32(rng, 2);
        if (val < (cU32Max / 3))
            return -1;
        if (val < (cU32Max * 2 / 3))
            return 0;
        return 1;
    }
    inline f32 g_random_f32_0_1(random_t* rng) { return g_random_f32(rng); }
    inline f64 g_random_f64_0_1(random_t* rng) { return ((f64)g_random_u64(rng) / (f64)cU64Max); }

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
            , s1(6364136223846793005)
        {
            next();
            next();
        }

        void reset(s64 seed) override
        {
            s0 = seed + 6364136223846793;
            s1 = 6364136223846793005;
            next();
            next();
        }

        u32 rand32() override { return (u32)next(); }
        u64 rand64() override { return next(); }

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

    class wyrand_t : public random_t
    {
    public:
        wyrand_t()
            : m_seed(0x7F2F0A584B0A4F3B)
        {
        }
        ~wyrand_t() {}

        virtual void reset(s64 inSeed = 0x7F2F0A584B0A4F3B);
        virtual u32  rand32();
        virtual u64  rand64();
        virtual void generate(u8* outData, u32 numBytes);

        DCORE_CLASS_PLACEMENT_NEW_DELETE
    private:
        u64 m_seed;
    };

};  // namespace ncore

#endif  // __CBASE_RANDOM_H__
