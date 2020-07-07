#include <intrin.h>
#pragma intrinsic(_BitScanForward,_BitScanForward64,_BitScanReverse,_BitScanReverse64)

namespace xcore
{
	/**
	 * find the number of trailing zeros in 16-bit value
	 */
	inline s32 xcountTrailingZeros(u16 v)
	{
		if (v == 0)
			return 16;

		unsigned long result;
		::_BitScanForward(&result, v);
		return (s32)result;
	}
	inline s32 xcountTrailingZeros(u32 v)
	{
		if (v == 0)
			return 32;
		unsigned long result;
		::_BitScanForward(&result, v);
		return (s32)result;
	}
	inline s32 xcountTrailingZeros(u64 v)
	{
		if (v == 0)
			return 64;
		unsigned long result;
		::_BitScanForward64(&result, v);
		return (s32)result;
	}

	/**
	 * find the number of leading zeros in 32-bit v
	 */
	inline s32 xcountLeadingZeros(u16 v)
	{
		if (v == 0)
			return 16;
		return (s32)::__lzcnt16(v);
	}
	inline s32 xcountLeadingZeros(u32 v)
	{
		if (v == 0)
			return 32;
		return (s32)::__lzcnt(v);;
	}
	inline s32 xcountLeadingZeros(u64 v)
	{
		if (v == 0)
			return 64;
		return (s32)::__lzcnt64(v);;
	}

	/**
	 * Return v but with only the Least Significant Bit "1"
	 */
	inline s32 xleastSignificantOneBit(u32 integer) { return (integer ^ (integer & (integer - 1))); }

	/**
	 * Return v but with only the Most Significant Bit "1"
	 */
	inline u32 xmostSignificantOneBit(u32 integer)
	{
		if (integer == 0)
			return 32;
		s32 const msob = xcountLeadingZeros(integer);
		return (u32)(0x80000000 >> msob);
	}

	/**
	 * Return the bit index of the Least Significant Bit "1"
	 */
	inline s32 xleastSignificantBit(u32 integer)
	{
		if (integer == 0)
			return 32;
		s32 const lsob = xcountTrailingZeros(integer);
		return lsob;
	}

	/**
	 * Return the bit index of the Most Significant Bit "1"
	 */
	inline s32 xmostSignificantBit(u32 integer)
	{
		if (integer == 0)
			return 32;
		s32 c = 31 - xcountLeadingZeros(integer);
		return c;
	}

	/**
	 * find the bit position/index of the first bit from low to high
	 */
	inline s32 xfindFirstBit(u16 integer)
	{
		if (integer == 0)
			return 16;
		return xcountTrailingZeros(integer);
	}

	/**
	 * find the bit position/index of the first bit from high to low
	 */
	inline s32 xfindLastBit(u16 integer)
	{
		if (integer == 0)
			return 16;
		return 15 - xcountLeadingZeros(integer);
	}

	/**
	 * find the bit position/index of the first bit from low to high
	 */
	inline s32 xfindFirstBit(u32 integer)
	{
		if (integer == 0)
			return 32;
		return xcountTrailingZeros(integer);
	}

	/**
	 * find the bit position/index of the first bit from high to low
	 */
	inline s32 xfindLastBit(u32 integer)
	{
		if (integer == 0)
			return 32;
		return 31 - xcountLeadingZeros(integer);
	}

	/**
	 * find the bit position/index of the first bit from low to high
	 */
	inline s32 xfindFirstBit(u64 integer)
	{
		if (integer == 0)
			return 64;
		return xcountTrailingZeros(integer);
	}

	/**
	 * find the bit position/index of the first bit from high to low
	 */
	inline s32 xfindLastBit(u64 integer)
	{
		if (integer == 0)
			return 64;
		return 63 - xcountLeadingZeros(integer);
	}

}