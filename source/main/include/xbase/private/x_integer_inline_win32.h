#include <intrin.h>
#pragma intrinsic(_BitScanForward,_BitScanForward64,_BitScanReverse,_BitScanReverse64)

namespace xcore
{
	 // find the number of trailing zeros in 16-bit value
	 // if 'v==0' this function returns 0
	inline s32 xcountTrailingZeros(u16 v)
	{
		unsigned long result;
		::_BitScanForward(&result, v);
		return (s32)result;
	}
	 // find the number of trailing zeros in 32-bit value
	 // if 'v==0' this function returns 0
	inline s32 xcountTrailingZeros(u32 v)
	{
		unsigned long result;
		::_BitScanForward(&result, v);
		return (s32)result;
	}
	 // find the number of trailing zeros in 64-bit value
	 // if 'v==0' this function returns 0
	inline s32 xcountTrailingZeros(u64 v)
	{
		unsigned long result;
		::_BitScanForward64(&result, v);
		return (s32)result;
	}

	// find the number of leading zeros in 16-bit v
	// if 'v==0' this function returns 16
	inline s32 xcountLeadingZeros(u16 v)
	{
		return (s32)::__lzcnt16(v);
	}
	// find the number of leading zeros in 32-bit v
	// if 'v==0' this function returns 32
	inline s32 xcountLeadingZeros(u32 v)
	{
		return (s32)::__lzcnt(v);
	}
	// find the number of leading zeros in 64-bit v
	// if 'v==0' this function returns 64
	inline s32 xcountLeadingZeros(u64 v)
	{
		return (s32)::__lzcnt64(v);;
	}

	// Return v but with only the Least Significant Bit "1"
	inline s32 xleastSignificantOneBit(u32 integer) { return (integer ^ (integer & (integer - 1))); }

	// Return v but with only the Most Significant Bit "1"
	// If 'integer == 0' this function will return 0
	inline u32 xmostSignificantOneBit(u32 integer)
	{
		s32 const msob = xcountLeadingZeros(integer);
		return (u32)(0x80000000 >> msob) & integer;
	}

	// Return the bit index of the Least Significant Bit "1"
	// If 'integer == 0' this function will return 0
	inline s32 xleastSignificantBit(u32 integer)
	{
		s32 const lsob = xcountTrailingZeros(integer);
		return lsob;
	}

	// Return the bit index of the Most Significant Bit "1"
	// If 'integer == 0' this function will return -1
	inline s32 xmostSignificantBit(u32 integer)
	{
		s32 c = 31 - xcountLeadingZeros(integer);
		return c;
	}

	// find the bit position/index of the first bit from low to high
	// If 'integer == 0' this function will return 0
	inline s32 xfindFirstBit(u16 integer)
	{
		if (integer == 0)
			return -1;
		return xcountTrailingZeros(integer);
	}

	// find the bit position/index of the first bit from high to low
	// If 'integer == 0' this function will return -1
	inline s32 xfindLastBit(u16 integer)
	{
		return 15 - xcountLeadingZeros(integer);
	}

	// find the bit position/index of the first bit from low to high
	// If 'integer == 0' this function will return -1
	inline s32 xfindFirstBit(u32 integer)
	{
		if (integer == 0)
			return -1;
		return xcountTrailingZeros(integer);
	}

	// find the bit position/index of the first bit from high to low
	// If 'integer == 0' this function will return -1
	inline s32 xfindLastBit(u32 integer)
	{
		return 31 - xcountLeadingZeros(integer);
	}

	// find the bit position/index of the first bit from low to high
	// If 'integer == 0' this function will return 0
	inline s32 xfindFirstBit(u64 integer)
	{
		if (integer == 0)
			return -1;
		return xcountTrailingZeros(integer);
	}

	// find the bit position/index of the first bit from high to low
	// If 'integer == 0' this function will return -1
	inline s32 xfindLastBit(u64 integer)
	{
		return 63 - xcountLeadingZeros(integer);
	}

}