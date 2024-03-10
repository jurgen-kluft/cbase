

namespace ncore
{
    namespace math
    {
        // find the number of trailing zeros in 8-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u8 integer)
        {
            s8 const count = __builtin_ctz(integer);
            return count & 0x7;
        }
        // find the number of trailing zeros in 16-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u16 integer)
        {
            s8 const count = __builtin_ctz(integer);
            return count & 0xF;
        }
        // find the number of trailing zeros in 32-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u32 integer)
        {
            s8 const count = __builtin_ctz(integer);
            return count & 0x1F;
        }
        // find the number of trailing zeros in 64-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u64 integer)
        {
            s8 const count = __builtin_ctzll(integer); // when integer is 0, this function returns 64
            return count & 0x3F;
        }

        // find the number of leading zeros in 8-bit v
        // if 'v==0' this function returns 8
        inline s8 countLeadingZeros(u8 integer)
        {
            s8 const count = __builtin_clz(integer);
            return count-24;
        }
        // find the number of leading zeros in 16-bit v
        // if 'v==0' this function returns 16
        inline s8 countLeadingZeros(u16 integer)
        {
            s8 const count = __builtin_clz(integer);
            return count-16;
        }
        // find the number of leading zeros in 32-bit v
        // if 'v==0' this function returns 32
        inline s8 countLeadingZeros(u32 integer)
        {
            s8 const count = __builtin_clz(integer); // when integer is 0, this function returns 32
            return count;
        }
        // find the number of leading zeros in 64-bit v
        // if 'v==0' this function returns 64
        inline s8 countLeadingZeros(u64 integer)
        {
            s8 const count = __builtin_clzll(integer); // when integer is 0, this function returns 64
            return count;
        }

        // Return v but with only the Least Significant Bit "1"
        inline u32 leastSignificantOneBit(u32 integer) { return (integer ^ (integer & (integer - 1))); }

        // Return v but with only the Most Significant Bit "1"
        // If 'integer == 0' this function will return 0
        inline u32 mostSignificantOneBit(u32 integer)
        {
            s32 const msob = countLeadingZeros(integer);
            return (u32)(0x80000000 >> msob) & integer;
        }

        // Return the bit index of the Least Significant Bit "1"
        // If 'integer == 0' this function will return 0
        inline s8 leastSignificantBit(u32 integer)
        {
            s8 const lsob = countTrailingZeros(integer);
            return lsob;
        }

        // Return the bit index of the Most Significant Bit "1"
        // If 'integer == 0' this function will return -1
        inline s8 mostSignificantBit(u32 integer)
        {
            s8 c = 31 - countLeadingZeros(integer);
            return c;
        }

        // find the bit position/index of the first bit from low to high
        // If 'integer == 0' this function will return -1
        inline s8 findFirstBit(u8 integer)
        {
            if (integer == 0)
                return -1;
            return countTrailingZeros(integer);
        }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return 8
        // If 'integer == 0x01' this function will return 0
        // If 'integer == 0x80' this function will return 7
        inline s8 findLastBit(u8 integer) { return countLeadingZeros(integer); }

        // find the bit position/index of the first bit from low to high
        // If 'integer == 0' this function will return -1
        inline s8 findFirstBit(u16 integer)
        {
            if (integer == 0)
                return -1;
            return countTrailingZeros(integer);
        }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return 16
        // If 'integer == 0x01' this function will return 15
        // If 'integer == 0x8000' this function will return 0
        inline s8 findLastBit(u16 integer) { return countLeadingZeros(integer); }

        // find the bit position/index of the first bit from low to high
        // If 'integer == 0' this function will return -1
        inline s8 findFirstBit(u32 integer)
        {
            if (integer == 0)
                return -1;
            return countTrailingZeros(integer);
        }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return 32
        // If 'integer == 0x00000001' this function will return 31
        // If 'integer == 0x80000000' this function will return 0
        inline s8 findLastBit(u32 integer) { return countLeadingZeros(integer); }

        // find the bit position/index of the first bit from low to high
        // If 'integer == 0' this function will return 0
        inline s8 findFirstBit(u64 integer)
        {
            if (integer == 0)
                return -1;
            return countTrailingZeros(integer);
        }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return 64
        // If 'integer == 0x0000000000000001' this function will return 63
        // If 'integer == 0x8000000000000000' this function will return 0
        inline s8 findLastBit(u64 integer) { return countLeadingZeros(integer); }

        inline s8 countBits(u8 inInteger)
        {
            u32 i = inInteger;
            return __builtin_popcount(i);
        }

        inline s8 countBits(u16 inInteger)
        {
            u32 i = inInteger;
            return __builtin_popcount(i);
        }

        inline s8 countBits(u32 inInteger)
        {
            u32 i = inInteger;
            return __builtin_popcount(i);
        }

        inline s8 countBits(u64 inInteger)
        {
            u64 i = inInteger;
            return __builtin_popcountll(i);
        }
    } // namespace math
} // namespace ncore