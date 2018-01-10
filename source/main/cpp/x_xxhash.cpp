#include "xbase/x_target.h"

namespace xcore
{
	static const u64 PRIME64_1 = 11400714785074694791ULL;
	static const u64 PRIME64_2 = 14029467366897019727ULL;
	static const u64 PRIME64_3 = 1609587929392839161ULL;
	static const u64 PRIME64_4 = 9650029242287828579ULL;
	static const u64 PRIME64_5 = 2870177450012600261ULL;

	#  define XXH_rotl32(x,r) ((x << r) | (x >> (32 - r)))
	#  define XXH_rotl64(x,r) ((x << r) | (x >> (64 - r)))

	enum XXH_endianess {
		XXH_littleEndian = 0,
		XXH_bigEndian = 1
	};

	enum XXH_alignment {
		XXH_aligned = 0,
		XXH_unaligned = 1
	};

	static u32 XXH_read32_BE(const void* memPtr)
	{
		xbyte const* bptr = (xbyte const*)memPtr;
		u32 val = bptr[0];
		val = val << 8 | bptr[1];
		val = val << 8 | bptr[2];
		val = val << 8 | bptr[3];
		return val;
	}
	static u32 XXH_read32_LE(const void* memPtr)
	{
		xbyte const* bptr = (xbyte const*)memPtr;
		u32 val = bptr[3];
		val = val << 8 | bptr[2];
		val = val << 8 | bptr[1];
		val = val << 8 | bptr[0];
		return val;
	}

	static u64 XXH_read64_BE(const void* memPtr)
	{
		xbyte const* bptr = (xbyte const*)memPtr;
		u64 val = bptr[0];
		val = val << 8 | bptr[1];
		val = val << 8 | bptr[2];
		val = val << 8 | bptr[3];
		val = val << 8 | bptr[4];
		val = val << 8 | bptr[5];
		val = val << 8 | bptr[6];
		val = val << 8 | bptr[7];
		return val;
	}
	static u64 XXH_read64_LE(const void* memPtr)
	{
		xbyte const* bptr = (xbyte const*)memPtr;
		u64 val = bptr[7];
		val = val << 8 | bptr[6];
		val = val << 8 | bptr[5];
		val = val << 8 | bptr[4];
		val = val << 8 | bptr[3];
		val = val << 8 | bptr[2];
		val = val << 8 | bptr[1];
		val = val << 8 | bptr[0];
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

#define XXH_get32bits(p) XXH_read32_BE(p)
#define XXH_get64bits(p) XXH_read64_BE(p)

	u64 XXH64(const void* input, u32 len, u64 seed = PRIME64_4)
	{
		const xbyte* p = (const xbyte*)input;
		const xbyte* bEnd = p + len;
		u64 h64;

		if (len >= 32) {
			const xbyte* const limit = bEnd - 32;
			u64 v1 = seed + PRIME64_1 + PRIME64_2;
			u64 v2 = seed + PRIME64_2;
			u64 v3 = seed + 0;
			u64 v4 = seed - PRIME64_1;

			do {
				v1 = XXH64_round(v1, XXH_get64bits(p)); p += 8;
				v2 = XXH64_round(v2, XXH_get64bits(p)); p += 8;
				v3 = XXH64_round(v3, XXH_get64bits(p)); p += 8;
				v4 = XXH64_round(v4, XXH_get64bits(p)); p += 8;
			} while (p <= limit);

			h64 = XXH_rotl64(v1, 1) + XXH_rotl64(v2, 7) + XXH_rotl64(v3, 12) + XXH_rotl64(v4, 18);
			h64 = XXH64_mergeRound(h64, v1);
			h64 = XXH64_mergeRound(h64, v2);
			h64 = XXH64_mergeRound(h64, v3);
			h64 = XXH64_mergeRound(h64, v4);

		}
		else {
			h64 = seed + PRIME64_5;
		}

		h64 += (u64)len;

		while (p + 8 <= bEnd) {
			u64 const k1 = XXH64_round(0, XXH_get64bits(p));
			h64 ^= k1;
			h64 = XXH_rotl64(h64, 27) * PRIME64_1 + PRIME64_4;
			p += 8;
		}

		if (p + 4 <= bEnd) {
			h64 ^= (u64)(XXH_get32bits(p)) * PRIME64_1;
			h64 = XXH_rotl64(h64, 23) * PRIME64_2 + PRIME64_3;
			p += 4;
		}

		while (p<bEnd) {
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

	u64		map_key_hasher(xbyte const* data, u32 size)
	{
		return XXH64(data, size);
	}

}

