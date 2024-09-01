#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "cbase/c_hash.h"
#include "cbase/c_memory.h"
#include "cbase/c_runes.h"

namespace ncore
{
    namespace nhash
    {
        namespace xxhash
        {
            static constexpr u64 PRIME64_1 = 11400714785074694791ULL;
            static constexpr u64 PRIME64_2 = 14029467366897019727ULL;
            static constexpr u64 PRIME64_3 = 1609587929392839161ULL;
            static constexpr u64 PRIME64_4 = 9650029242287828579ULL;
            static constexpr u64 PRIME64_5 = 2870177450012600261ULL;

            static u32 get32bits(const void* memPtr)
            {
                u8 const* bptr = (u8 const*)memPtr;

#if defined(D_BIG_ENDIAN)
                u32 val = bptr[0];
                val     = val << 8 | bptr[1];
                val     = val << 8 | bptr[2];
                val     = val << 8 | bptr[3];
#else
                u32 val = bptr[3];
                val     = val << 8 | bptr[2];
                val     = val << 8 | bptr[1];
                val     = val << 8 | bptr[0];
#endif
                return val;
            }

            static u64 get64bits(const void* memPtr)
            {
                u8 const* bptr = (u8 const*)memPtr;

#if defined(D_BIG_ENDIAN)
                u64 val = bptr[0];
                val     = val << 8 | bptr[1];
                val     = val << 8 | bptr[2];
                val     = val << 8 | bptr[3];
                val     = val << 8 | bptr[4];
                val     = val << 8 | bptr[5];
                val     = val << 8 | bptr[6];
                val     = val << 8 | bptr[7];
#else
                u64 val = bptr[7];
                val     = val << 8 | bptr[6];
                val     = val << 8 | bptr[5];
                val     = val << 8 | bptr[4];
                val     = val << 8 | bptr[3];
                val     = val << 8 | bptr[2];
                val     = val << 8 | bptr[1];
                val     = val << 8 | bptr[0];
#endif

                return val;
            }

            static inline u64 round(u64 acc, u64 input)
            {
                acc += input * PRIME64_2;
                acc = (acc << 31) | (acc >> (64 - 31));
                acc *= PRIME64_1;
                return acc;
            }

            static inline u64 mergeRound(u64 acc, u64 val)
            {
                val = round(0, val);
                acc ^= val;
                acc = acc * PRIME64_1 + PRIME64_4;
                return acc;
            }

            static inline u64 rotl64(u64 x, s8 r) { return (x << r) | (x >> (64 - r)); }

            static u64 Hash(const void* input, u32 len, u64 seed = PRIME64_4)
            {
                const u8* p    = (const u8*)input;
                const u8* bEnd = p + len;
                u64       h64;

                if (len >= 32)
                {
                    const u8* const limit = bEnd - 32;
                    u64             v1    = seed + PRIME64_1 + PRIME64_2;
                    u64             v2    = seed + PRIME64_2;
                    u64             v3    = seed + 0;
                    u64             v4    = seed - PRIME64_1;

                    do
                    {
                        v1 = round(v1, get64bits(p));
                        p += 8;
                        v2 = round(v2, get64bits(p));
                        p += 8;
                        v3 = round(v3, get64bits(p));
                        p += 8;
                        v4 = round(v4, get64bits(p));
                        p += 8;
                    } while (p <= limit);

                    h64 = rotl64(v1, 1) + rotl64(v2, 7) + rotl64(v3, 12) + rotl64(v4, 18);
                    h64 = mergeRound(h64, v1);
                    h64 = mergeRound(h64, v2);
                    h64 = mergeRound(h64, v3);
                    h64 = mergeRound(h64, v4);
                }
                else
                {
                    h64 = seed + PRIME64_5;
                }

                h64 += (u64)len;

                while (p + 8 <= bEnd)
                {
                    u64 const k1 = round(0, get64bits(p));
                    h64 ^= k1;
                    h64 = rotl64(h64, 27) * PRIME64_1 + PRIME64_4;
                    p += 8;
                }

                if (p + 4 <= bEnd)
                {
                    h64 ^= (u64)(get32bits(p)) * PRIME64_1;
                    h64 = rotl64(h64, 23) * PRIME64_2 + PRIME64_3;
                    p += 4;
                }

                while (p < bEnd)
                {
                    h64 ^= (*p) * PRIME64_5;
                    h64 = rotl64(h64, 11) * PRIME64_1;
                    p++;
                }

                h64 ^= h64 >> 33;
                h64 *= PRIME64_2;
                h64 ^= h64 >> 29;
                h64 *= PRIME64_3;
                h64 ^= h64 >> 32;

                return h64;
            }
        } // namespace xxhash

        // TODO: Implement a xxhash version that can 'lowercase' the 'string' as it hashes.
        namespace xxhash_lowercase
        {
            static inline u8 lower_case(u8 c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }

            static u32 get32bits(const void* memPtr)
            {
                u8 const* bptr = (u8 const*)memPtr;
                u32       val  = lower_case(bptr[0]);
                val            = val << 8 | lower_case(bptr[1]);
                val            = val << 8 | lower_case(bptr[2]);
                val            = val << 8 | lower_case(bptr[3]);
                return val;
            }

            static u64 get64bits(const void* memPtr)
            {
                u8 const* bptr = (u8 const*)memPtr;
                u64       val  = lower_case(bptr[0]);
                val            = val << 8 | lower_case(bptr[1]);
                val            = val << 8 | lower_case(bptr[2]);
                val            = val << 8 | lower_case(bptr[3]);
                val            = val << 8 | lower_case(bptr[4]);
                val            = val << 8 | lower_case(bptr[5]);
                val            = val << 8 | lower_case(bptr[6]);
                val            = val << 8 | lower_case(bptr[7]);
                return val;
            }

            static u64 Hash(const void* input, u32 len, u64 seed = xxhash::PRIME64_4)
            {
                const u8* p    = (const u8*)input;
                const u8* bEnd = p + len;
                u64       h64;

                if (len >= 32)
                {
                    const u8* const limit = bEnd - 32;
                    u64             v1    = seed + xxhash::PRIME64_1 + xxhash::PRIME64_2;
                    u64             v2    = seed + xxhash::PRIME64_2;
                    u64             v3    = seed + 0;
                    u64             v4    = seed - xxhash::PRIME64_1;

                    do
                    {
                        v1 = xxhash::round(v1, get64bits(p));
                        p += 8;
                        v2 = xxhash::round(v2, get64bits(p));
                        p += 8;
                        v3 = xxhash::round(v3, get64bits(p));
                        p += 8;
                        v4 = xxhash::round(v4, get64bits(p));
                        p += 8;
                    } while (p <= limit);

                    h64 = xxhash::rotl64(v1, 1) + xxhash::rotl64(v2, 7) + xxhash::rotl64(v3, 12) + xxhash::rotl64(v4, 18);
                    h64 = xxhash::mergeRound(h64, v1);
                    h64 = xxhash::mergeRound(h64, v2);
                    h64 = xxhash::mergeRound(h64, v3);
                    h64 = xxhash::mergeRound(h64, v4);
                }
                else
                {
                    h64 = seed + xxhash::PRIME64_5;
                }

                h64 += (u64)len;

                while (p + 8 <= bEnd)
                {
                    u64 const k1 = xxhash::round(0, get64bits(p));
                    h64 ^= k1;
                    h64 = xxhash::rotl64(h64, 27) * xxhash::PRIME64_1 + xxhash::PRIME64_4;
                    p += 8;
                }

                if (p + 4 <= bEnd)
                {
                    h64 ^= (u64)(get32bits(p)) * xxhash::PRIME64_1;
                    h64 = xxhash::rotl64(h64, 23) * xxhash::PRIME64_2 + xxhash::PRIME64_3;
                    p += 4;
                }

                while (p < bEnd)
                {
                    h64 ^= nrunes::to_lower(*p) * xxhash::PRIME64_5;
                    h64 = xxhash::rotl64(h64, 11) * xxhash::PRIME64_1;
                    p++;
                }

                h64 ^= h64 >> 33;
                h64 *= xxhash::PRIME64_2;
                h64 ^= h64 >> 29;
                h64 *= xxhash::PRIME64_3;
                h64 ^= h64 >> 32;

                return h64;
            }
        } // namespace xxhash_lowercase

        // ----------------------------------------------------------------------------
        u64 datahash(u8 const* data, u32 size, u64 seed) { return xxhash::Hash(data, size, seed); }

        u64 strhash(const char* str, u64 seed)
        {
            const char* end = str;
            while (*end++ != '\0') {}
            return xxhash::Hash(str, (u32)(end - str), seed);
        }

        u64 strhash(const char* str, const char* end, u64 seed) { return xxhash::Hash(str, (u32)(end - str), seed); }
        u64 strhash_lowercase(const char* str, u64 seed)
        {
            const char* end = str;
            while (*end++ != '\0') {}
            return xxhash_lowercase::Hash(str, (u32)(end - str), seed);
        }
        u64 strhash_lowercase(const char* str, const char* end, u64 seed) { return xxhash_lowercase::Hash(str, (u32)(end - str), seed); }

        static u32 hash64_to_hash32(u64 key)
        {
            key = (~key) + (key << 18); // key = (key << 18) - key - 1;
            key = key ^ (key >> 31);
            key = key * 21; // key = (key + (key << 2)) + (key << 4);
            key = key ^ (key >> 11);
            key = key + (key << 6);
            key = key ^ (key >> 22);
            return (u32)key;
        }

        u32 strhash32(const char* str, u64 seed)
        {
            u64 const hash64 = strhash(str, seed);
            return hash64_to_hash32(hash64);
        }

        u32 strhash32(const char* str, const char* end, u64 seed)
        {
            u64 const hash64 = strhash(str, end, seed);
            return hash64_to_hash32(hash64);
        }

        u32 strhash32_lowercase(const char* str, u64 seed)
        {
            u64 const hash64 = strhash_lowercase(str, seed);
            return hash64_to_hash32(hash64);
        }

        u32 strhash32_lowercase(const char* str, const char* end, u64 seed)
        {
            u64 const hash64 = strhash_lowercase(str, end, seed);
            return hash64_to_hash32(hash64);
        }

        // calculate the hash of a string, this string can be ascii, utf-8, utf-16 or utf-32.
        // so use an intermediate buffer to convert the string into utf-32 and feed that to
        // the hash algorithm.
        u64 strhash(crunes_t const& strdata, u64 seed)
        {
            if ((strdata.m_type) == ascii::TYPE)
            {
                const char* begin = &strdata.m_ascii[strdata.m_str];
                const char* end   = &strdata.m_ascii[strdata.m_str];
                return strhash(begin, end, seed);
            }

            // We could extend it to support utf-32 and utf-16, but we don't need it for now.

            ASSERT(false);
            return 0;
        }

        u64 strhash_lowercase(crunes_t const& strdata, u64 seed)
        {
            if ((strdata.m_type) == ascii::TYPE)
            {
                const char* begin = &strdata.m_ascii[strdata.m_str];
                const char* end   = &strdata.m_ascii[strdata.m_str];
                return strhash_lowercase(begin, end, seed);
            }

            // We could extend it to support utf-32 and utf-16, but we don't need it for now.

            ASSERT(false);
            return 0;
        }

        u32 strhash32(crunes_t const& str, u64 seed)
        {
            if ((str.m_type) == ascii::TYPE)
            {
                const char* begin = &str.m_ascii[str.m_str];
                const char* end   = &str.m_ascii[str.m_str];
                return strhash32(begin, end, seed);
            }

            // We could extend it to support utf-32 and utf-16, but we don't need it for now.

            ASSERT(false);
            return 0;
        }

        u32 strhash32_lowercase(crunes_t const& str, u64 seed)
        {
            if ((str.m_type) == ascii::TYPE)
            {
                const char* begin = &str.m_ascii[str.m_str];
                const char* end   = &str.m_ascii[str.m_str];
                return strhash32_lowercase(begin, end, seed);
            }

            // We could extend it to support utf-32 and utf-16, but we don't need it for now.

            ASSERT(false);
            return 0;
        }

    } // namespace nhash
};    // namespace ncore
