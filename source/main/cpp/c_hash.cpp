#include "cbase/c_target.h"
#include "cbase/c_debug.h"
#include "cbase/c_hash.h"
#include "cbase/c_runes.h"

namespace ncore
{
    static constexpr u64 PRIME64_1 = 11400714785074694791ULL;
    static constexpr u64 PRIME64_2 = 14029467366897019727ULL;
    static constexpr u64 PRIME64_3 = 1609587929392839161ULL;
    static constexpr u64 PRIME64_4 = 9650029242287828579ULL;
    static constexpr u64 PRIME64_5 = 2870177450012600261ULL;

#define XXH_rotl32(x, r) ((x << r) | (x >> (32 - r)))
#define XXH_rotl64(x, r) ((x << r) | (x >> (64 - r)))

    static u32 XXH_get32bits(const void* memPtr)
    {
        u8 const* bptr = (u8 const*)memPtr;
        u32          val  = bptr[0];
        val               = val << 8 | bptr[1];
        val               = val << 8 | bptr[2];
        val               = val << 8 | bptr[3];
        return val;
    }

    static u64 XXH_get64bits(const void* memPtr)
    {
        u8 const* bptr = (u8 const*)memPtr;
        u64          val  = bptr[0];
        val               = val << 8 | bptr[1];
        val               = val << 8 | bptr[2];
        val               = val << 8 | bptr[3];
        val               = val << 8 | bptr[4];
        val               = val << 8 | bptr[5];
        val               = val << 8 | bptr[6];
        val               = val << 8 | bptr[7];
        return val;
    }

    static u64 XXH64_round(u64 acc, u64 input)
    {
        acc += input * PRIME64_2;
        acc = XXH_rotl64(acc, 31);
        acc *= PRIME64_1;
        return acc;
    }

    static u64 XXH64_mergeRound(u64 acc, u64 val)
    {
        val = XXH64_round(0, val);
        acc ^= val;
        acc = acc * PRIME64_1 + PRIME64_4;
        return acc;
    }

    u64 XXH64(const void* input, u32 len, u64 seed = PRIME64_4)
    {
        const u8* p    = (const u8*)input;
        const u8* bEnd = p + len;
        u64          h64;

        if (len >= 32)
        {
            const u8* const limit = bEnd - 32;
            u64                v1    = seed + PRIME64_1 + PRIME64_2;
            u64                v2    = seed + PRIME64_2;
            u64                v3    = seed + 0;
            u64                v4    = seed - PRIME64_1;

            do
            {
                v1 = XXH64_round(v1, XXH_get64bits(p));
                p += 8;
                v2 = XXH64_round(v2, XXH_get64bits(p));
                p += 8;
                v3 = XXH64_round(v3, XXH_get64bits(p));
                p += 8;
                v4 = XXH64_round(v4, XXH_get64bits(p));
                p += 8;
            } while (p <= limit);

            h64 = XXH_rotl64(v1, 1) + XXH_rotl64(v2, 7) + XXH_rotl64(v3, 12) + XXH_rotl64(v4, 18);
            h64 = XXH64_mergeRound(h64, v1);
            h64 = XXH64_mergeRound(h64, v2);
            h64 = XXH64_mergeRound(h64, v3);
            h64 = XXH64_mergeRound(h64, v4);
        }
        else
        {
            h64 = seed + PRIME64_5;
        }

        h64 += (u64)len;

        while (p + 8 <= bEnd)
        {
            u64 const k1 = XXH64_round(0, XXH_get64bits(p));
            h64 ^= k1;
            h64 = XXH_rotl64(h64, 27) * PRIME64_1 + PRIME64_4;
            p += 8;
        }

        if (p + 4 <= bEnd)
        {
            h64 ^= (u64)(XXH_get32bits(p)) * PRIME64_1;
            h64 = XXH_rotl64(h64, 23) * PRIME64_2 + PRIME64_3;
            p += 4;
        }

        while (p < bEnd)
        {
            h64 ^= (*p) * PRIME64_5;
            h64 = XXH_rotl64(h64, 11) * PRIME64_1;
            p++;
        }

        h64 ^= h64 >> 33;
        h64 *= PRIME64_2;
        h64 ^= h64 >> 29;
        h64 *= PRIME64_3;
        h64 ^= h64 >> 32;

        return h64;
    }

    // ----------------------------------------------------------------------------
    u64 calchash(u8 const* data, u32 size) { return XXH64(data, size); }

} // namespace ncore

namespace ncore
{
    // Paper: https://www.131002.net/siphash/siphash.pdf

    // Return the bits of the specified 'x' rotated to the left by the
    // specified 'b' number of bits.  Bits that are rotated off the end are
    // wrapped around to the beginning.
    inline static u64 rotl(u64 x, u64 b) { return (x << b) | (x >> (64 - b)); }

    // Mix the specified 'v0', 'v1', 'v2', and 'v3' in a "Sip Round" as
    // described in the SipHash algorithm
    inline static void sipround(u64& v0, u64& v1, u64& v2, u64& v3)
    {
        v0 += v1;
        v1 = rotl(v1, 13);
        v1 ^= v0;
        v0 = rotl(v0, 32);
        v2 += v3;
        v3 = rotl(v3, 16);
        v3 ^= v2;
        v0 += v3;
        v3 = rotl(v3, 21);
        v3 ^= v0;
        v2 += v1;
        v1 = rotl(v1, 17);
        v1 ^= v2;
        v2 = rotl(v2, 32);
    }

    // Return the 64-bit integer representation of the specified 'p' taking
    // into account endianness.  Undefined unless 'p' points to at least eight
    // bytes of initialized memory.
    inline static u64 u8to64_le(const u8* p)
    {
        ASSERT(p != nullptr);
        u64 result;
        result = p[0];
        result = (result << 8) | p[1];
        result = (result << 8) | p[2];
        result = (result << 8) | p[3];
        result = (result << 8) | p[4];
        result = (result << 8) | p[5];
        result = (result << 8) | p[6];
        result = (result << 8) | p[7];
        return result;
    }

    inline static void data_copy(const u8* src, const u8* end, u8* dst)
    {
        while (src < end)
            *dst++ = *src++;
    }

    hashing_t::hashing_t() : d_v0(0), d_v1(0), d_v2(0), d_v3(0), d_bufSize(0), d_totalLength(0) {}

    void hashing_t::reset()
    {
        u8 const seed[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        reset(seed);
    }

    void hashing_t::reset(const u8* seed)
    {
        ASSERT(seed);
        d_bufSize     = (0);
        d_totalLength = (0);
        u64 const k0  = u8to64_le(reinterpret_cast<const u8*>(&seed[0]));
        u64 const k1  = u8to64_le(reinterpret_cast<const u8*>(&seed[SEED_LENGTH / 2]));
        d_v0          = 0x736f6d6570736575ULL ^ k0;
        d_v1          = 0x646f72616e646f6dULL ^ k1;
        d_v2          = 0x6c7967656e657261ULL ^ k0;
        d_v3          = 0x7465646279746573ULL ^ k1;
    }

    void hashing_t::hash(const void* data, s32 numBytes)
    {
        ASSERT(0 != data || 0 == numBytes);

        u8 const* in = static_cast<const u8*>(data);

        d_totalLength += numBytes;
        if (d_bufSize + numBytes < 8)
        {
            data_copy(in, in + numBytes, d_buf + d_bufSize);
            d_bufSize += numBytes;
            return; // RETURN
        }
        if (d_bufSize > 0)
        {
            s32 t = 8 - d_bufSize;
            data_copy(in, in + t, d_buf + d_bufSize);
            u64 m = u8to64_le(d_buf);
            d_v3 ^= m;
            sipround(d_v0, d_v1, d_v2, d_v3);
            sipround(d_v0, d_v1, d_v2, d_v3);
            d_v0 ^= m;
            in += t;
            numBytes -= t;
        }
        d_bufSize           = numBytes & 7;
        u8 const* const end = in + (numBytes - d_bufSize);
        for (; in != end; in += 8)
        {
            u64 m = u8to64_le(in);
            d_v3 ^= m;
            sipround(d_v0, d_v1, d_v2, d_v3);
            sipround(d_v0, d_v1, d_v2, d_v3);
            d_v0 ^= m;
        }
        data_copy(end, end + d_bufSize, d_buf);
    }

    u64 hashing_t::finalize()
    {
        u64 b = static_cast<u64>(d_totalLength) << 56;
        switch (d_bufSize)
        {
            case 7: b |= static_cast<u64>(d_buf[6]) << 48; /* fall through */
            case 6: b |= static_cast<u64>(d_buf[5]) << 40; /* fall through */
            case 5: b |= static_cast<u64>(d_buf[4]) << 32; /* fall through */
            case 4: b |= static_cast<u64>(d_buf[3]) << 24; /* fall through */
            case 3: b |= static_cast<u64>(d_buf[2]) << 16; /* fall through */
            case 2: b |= static_cast<u64>(d_buf[1]) << 8;  /* fall through */
            case 1: b |= static_cast<u64>(d_buf[0]);       /* fall through */
            case 0: break;
        }
        d_v3 ^= b;
        sipround(d_v0, d_v1, d_v2, d_v3);
        sipround(d_v0, d_v1, d_v2, d_v3);
        d_v0 ^= b;
        d_v2 ^= 0xff;
        sipround(d_v0, d_v1, d_v2, d_v3);
        sipround(d_v0, d_v1, d_v2, d_v3);
        sipround(d_v0, d_v1, d_v2, d_v3);
        sipround(d_v0, d_v1, d_v2, d_v3);
        b = d_v0 ^ d_v1 ^ d_v2 ^ d_v3;
        return b;
    }

    // calculate the hash of a string, this string can be ascii, utf-8, utf-16 or utf-32.
    // so use an intermediate buffer to convert the string into utf-32 and feed that to
    // the hash algorithm.
    u64 calchash(crunes_t const& strdata)
    {
        hashing_t hash;
        hash.reset();

        runes_reader_t reader(strdata);
        uchar32 buffer[32];
        s32 i = 0;
        while (!reader.at_end())
        {
            buffer[i++] = reader.read();
            if (i == 32)
            {
                hash.hash(buffer, i * sizeof(uchar32));
                i = 0;
            }
        }
        if (i > 0)
        {
            hash.hash(buffer, i * sizeof(uchar32));
        }
        return hash.finalize();
    }

}; // namespace ncore
