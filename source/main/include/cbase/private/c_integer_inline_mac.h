

namespace ncore
{
    namespace math
    {
        // find the number of trailing zeros in 8-bit value
        // if 'v==0' this function returns 8
        inline s8 countTrailingZeros(u8 value) { return (value == 0) ? (s8)8 : (s8)__builtin_ctz(value); }

        // find the number of trailing zeros in 16-bit value
        // if 'v==0' this function returns 16
        inline s8 countTrailingZeros(u16 value) { return (value == 0) ? (s8)16 : (s8)__builtin_ctz(value); }

        // find the number of trailing zeros in 32-bit value
        // if 'v==0' this function returns 32
        inline s8 countTrailingZeros(u32 value) { return (value == 0) ? (s8)32 : (s8)__builtin_ctz(value); }

        // find the number of trailing zeros in 64-bit value
        // if 'v==0' this function returns 64
        inline s8 countTrailingZeros(u64 value) { return (value == 0) ? (s8)64 : (s8)__builtin_ctzll(value); }

        // find the number of leading zeros in 8-bit v
        // if 'v==0' this function returns 8
        inline s8 countLeadingZeros(u8 value) { return (value == 0) ? (s8)8 : ((s8)__builtin_clz(value) - 24); }

        // find the number of leading zeros in 16-bit v
        // if 'v==0' this function returns 16
        inline s8 countLeadingZeros(u16 value) { return (value == 0) ? (s8)16 : ((s8)__builtin_clz(value) - 16); }

        // find the number of leading zeros in 32-bit v
        // if 'v==0' this function returns 32
        inline s8 countLeadingZeros(u32 value) { return (value == 0) ? (s8)32 : (s8)__builtin_clz(value); }

        // find the number of leading zeros in 64-bit v
        // if 'v==0' this function returns 64
        inline s8 countLeadingZeros(u64 value) { return (value == 0) ? (s8)64 : (s8)__builtin_clzll(value); }

        // Return v but with only the Least Significant Bit "1"
        inline u32 leastSignificantOneBit(u32 value) { return (value ^ (value & (value - 1))); }

        // Return v but with only the Most Significant Bit "1"
        // If 'value == 0' this function will return 0
        inline u32 mostSignificantOneBit(u32 value) { return (u32)((u64)0x80000000 >> countLeadingZeros(value)); }

        // Return the bit index of the Least Significant Bit "1"
        // If 'value == 0' this function will return 32
        inline s8 leastSignificantBit(u32 value) { return countTrailingZeros(value); }

        // Return the bit index of the Most Significant Bit "1"
        // If 'value == 0' this function will return -1
        inline s8 mostSignificantBit(u32 value) { return (value == 0) ? (s8)32 : ((s8)31 - (s8)__builtin_clz(value)); }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return -1
        inline s8 findFirstBit(u8 value) { return countTrailingZeros(value); }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0x00' this function will return 8
        // If 'value == 0x01' this function will return 0
        // If 'value == 0x80' this function will return 7
        inline s8 findLastBit(u8 value) { return countLeadingZeros(value); }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return 16
        inline s8 findFirstBit(u16 value) { return countTrailingZeros(value); }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0x0000' this function will return 16
        // If 'value == 0x0001' this function will return 15
        // If 'value == 0x8000' this function will return 0
        inline s8 findLastBit(u16 value) { return countLeadingZeros(value); }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return 32
        inline s8 findFirstBit(u32 value) { return countTrailingZeros(value); }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0' this function will return 32
        // If 'value == 0x00000001' this function will return 31
        // If 'value == 0x80000000' this function will return 0
        inline s8 findLastBit(u32 value) { return countLeadingZeros(value); }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return 64
        inline s8 findFirstBit(u64 value) { return countTrailingZeros(value); }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0' this function will return 64
        // If 'value == 0x0000000000000001' this function will return 63
        // If 'value == 0x8000000000000000' this function will return 0
        inline s8 findLastBit(u64 value) { return countLeadingZeros(value); }

        inline s8 countBits(u8 value)
        {
            u32 i = value;
            return (s8)__builtin_popcount(i);
        }

        inline s8 countBits(u16 value)
        {
            u32 i = value;
            return (s8)__builtin_popcount(i);
        }

        inline s8 countBits(u32 value) { return (s8)__builtin_popcount(value); }
        inline s8 countBits(u64 value) { return (s8)__builtin_popcountll(value); }
    }  // namespace math
}  // namespace ncore
