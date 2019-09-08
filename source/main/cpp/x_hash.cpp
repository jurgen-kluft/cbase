#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_hash.h"

namespace xcore
{
	// Return the bits of the specified 'x' rotated to the left by the
	// specified 'b' number of bits.  Bits that are rotated off the end are
	// wrapped around to the beginning.
	inline static u64 rotl(u64 x, u64 b)
	{
		return (x << b) | (x >> (64 - b));
	}

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
	}

	inline static void data_copy(const xbyte* src, const xbyte* end, xbyte* dst)
	{
		while (src < end)
			*dst++ = *src++;
	}

	xhashing::xhashing()
		: d_v0(0)
		, d_v1(0)
		, d_v2(0)
		, d_v3(0)
		, d_bufSize(0)
		, d_totalLength(0)
	{
	}

	void xhashing::reset()
	{
		d_v0 = (0x736f6d6570736575ULL);
		d_v1 = (0x646f72616e646f6dULL);
		d_v2 = (0x6c7967656e657261ULL);
		d_v3 = (0x7465646279746573ULL);
		d_bufSize = (0);
		d_totalLength = (0);
	}

	void xhashing::reset(const xbyte* seed)
	{
		ASSERT(seed);
		d_bufSize = (0);
		d_totalLength = (0);
		u64 k0 = u8to64_le(reinterpret_cast<const u8*>(&seed[0]));
		u64 k1 = u8to64_le(reinterpret_cast<const u8*>(&seed[SEED_LENGTH / 2]));
		d_v0 = 0x736f6d6570736575ULL ^ k0;
		d_v1 = 0x646f72616e646f6dULL ^ k1;
		d_v2 = 0x6c7967656e657261ULL ^ k0;
		d_v3 = 0x7465646279746573ULL ^ k1;
	}

	void xhashing::hash(const void *data, s32 numBytes)
	{
		ASSERT(0 != data || 0 == numBytes);

		u8 const* in = static_cast<const u8*>(data);

		d_totalLength += numBytes;
		if (d_bufSize + numBytes < 8)
		{
			data_copy(in, in + numBytes, d_buf + d_bufSize);
			d_bufSize += numBytes;
			return;                                                       // RETURN
		}
		if (d_bufSize > 0)
		{
			s32 t = 8 - d_bufSize;
			data_copy(in, in+t, d_buf + d_bufSize);
			u64 m = u8to64_le( d_buf );
			d_v3 ^= m;
			sipround(d_v0, d_v1, d_v2, d_v3);
			sipround(d_v0, d_v1, d_v2, d_v3);
			d_v0 ^= m;
			in += t;
			numBytes -= t;
		}
		d_bufSize = numBytes & 7;
		u8 const* const end = in + (numBytes - d_bufSize);
		for ( ; in != end; in += 8 )
		{
			u64 m = u8to64_le( in );
			d_v3 ^= m;
			sipround(d_v0, d_v1, d_v2, d_v3);
			sipround(d_v0, d_v1, d_v2, d_v3);
			d_v0 ^= m;
		}
		data_copy(end, end + d_bufSize, d_buf);
	}

	u64 xhashing::finalize()
	{
		u64 b = static_cast<u64>(d_totalLength) << 56;
		switch(d_bufSize)
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
		b = d_v0 ^ d_v1 ^ d_v2  ^ d_v3;
		return b;
	}

	xhashing hasher;
};


