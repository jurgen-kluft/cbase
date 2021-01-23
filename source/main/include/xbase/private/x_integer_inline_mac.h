namespace xcore
{
	// TODO: Optimize this using intrinsics

	// find the number of trailing zeros in 16-bit value
	// if 'v==0' this function returns 0
	inline s32 xcountTrailingZeros(u16 integer)
	{
		if (integer == 0)
			return 0;

		s32 count = 0;
		if ((integer & 0x000000FF) == 0)
		{
			count += 8;
			integer = integer >> 8;
		}
		if ((integer & 0x0000000F) == 0)
		{
			count += 4;
			integer = integer >> 4;
		}
		if ((integer & 0x00000003) == 0)
		{
			count += 2;
			integer = integer >> 2;
		}
		if ((integer & 0x00000001) == 0)
		{
			count += 1;
		}

		return count;
	}
	 // find the number of trailing zeros in 32-bit value
	 // if 'v==0' this function returns 0
	inline s32 xcountTrailingZeros(u32 integer)
	{
        if (integer == 0)
            return 0;

		s32 count = 0;
		if ((integer & 0x0000FFFF) == 0)
		{
			count += 16;
			integer = integer >> 16;
		}
		if ((integer & 0x000000FF) == 0)
		{
			count += 8;
			integer = integer >> 8;
		}
		if ((integer & 0x0000000F) == 0)
		{
			count += 4;
			integer = integer >> 4;
		}
		if ((integer & 0x00000003) == 0)
		{
			count += 2;
			integer = integer >> 2;
		}
		if ((integer & 0x00000001) == 0)
		{
			count += 1;
		}

		return count;
	}
	// find the number of trailing zeros in 64-bit value
	// if 'v==0' this function returns 0
	inline s32 xcountTrailingZeros(u64 integer)
	{
        if (integer == 0)
            return 0;

		s32 count = 0;
		if ((integer & 0xFFFFFFFF) == 0)
		{
			count += 32;
			integer = integer >> 32;
		}
		if ((integer & 0x0000FFFF) == 0)
		{
			count += 16;
			integer = integer >> 16;
		}
		if ((integer & 0x000000FF) == 0)
		{
			count += 8;
			integer = integer >> 8;
		}
		if ((integer & 0x0000000F) == 0)
		{
			count += 4;
			integer = integer >> 4;
		}
		if ((integer & 0x00000003) == 0)
		{
			count += 2;
			integer = integer >> 2;
		}
		if ((integer & 0x00000001) == 0)
		{
			count += 1;
		}

		return count;
	}

	// find the number of leading zeros in 16-bit v
	// if 'v==0' this function returns 16
	inline s32 xcountLeadingZeros(u16 integer)
	{
		if (integer == 0)
			return 16;

		s32 count = 0;
		if ((integer & 0xFF00) == 0)
		{
			count += 8;
			integer = integer << 8;
		}
		if ((integer & 0xF000) == 0)
		{
			count += 4;
			integer = integer << 4;
		}
		if ((integer & 0xC000) == 0)
		{
			count += 2;
			integer = integer << 2;
		}
		if ((integer & 0x4000) == 0)
		{
			count += 1;
		}

		return count;
	}
	// find the number of leading zeros in 32-bit v
	// if 'v==0' this function returns 32
	inline s32 xcountLeadingZeros(u32 integer)
	{
		if (integer == 0)
			return 32;

		s32 count = 0;
		if ((integer & 0xFFFF0000) == 0)
		{
			count += 16;
			integer = integer << 16;
		}
		if ((integer & 0xFF000000) == 0)
		{
			count += 8;
			integer = integer << 8;
		}
		if ((integer & 0xF0000000) == 0)
		{
			count += 4;
			integer = integer << 4;
		}
		if ((integer & 0xC0000000) == 0)
		{
			count += 2;
			integer = integer << 2;
		}
		if ((integer & 0x40000000) == 0)
		{
			count += 1;
		}

		return count;
	}
	// find the number of leading zeros in 64-bit v
	// if 'v==0' this function returns 64
	inline s32 xcountLeadingZeros(u64 integer)
	{
		if (integer == 0)
			return 64;

		s32 count = 0;
		if ((integer & 0xFFFFFFFF00000000UL) == 0)
		{
			count += 32;
			integer = integer << 32;
		}
		if ((integer & 0xFFFF000000000000UL) == 0)
		{
			count += 16;
			integer = integer << 16;
		}
		if ((integer & 0xFF00000000000000UL) == 0)
		{
			count += 8;
			integer = integer << 8;
		}
		if ((integer & 0xF000000000000000UL) == 0)
		{
			count += 4;
			integer = integer << 4;
		}
		if ((integer & 0xC000000000000000UL) == 0)
		{
			count += 2;
			integer = integer << 2;
		}
		if ((integer & 0x4000000000000000UL) == 0)
		{
			count += 1;
		}

		return count;
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
	// If 'integer == 0' this function will return 0
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