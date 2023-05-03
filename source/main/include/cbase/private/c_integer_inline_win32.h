#include <intrin.h>
#pragma intrinsic(_BitScanForward,_BitScanForward64,_BitScanReverse,_BitScanReverse64)

namespace ncore
{
    namespace math
    {
        // find the number of trailing zeros in 16-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u8 v)
        {
            unsigned long result;
            ::_BitScanForward(&result, v);
            return (s8)result;
        }
        // find the number of trailing zeros in 16-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u16 v)
        {
            unsigned long result;
            ::_BitScanForward(&result, v);
            return (s8)result;
        }
        // find the number of trailing zeros in 32-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u32 v)
        {
            unsigned long result;
            ::_BitScanForward(&result, v);
            return (s8)result;
        }
        // find the number of trailing zeros in 64-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u64 v)
        {
            unsigned long result;
            ::_BitScanForward64(&result, v);
            return (s8)result;
        }

        // find the number of leading zeros in 8-bit v
        // if 'v==0' this function returns 8
        inline s8 countLeadingZeros(u8 v) { return (s8)::__lzcnt(v); }
        // find the number of leading zeros in 16-bit v
        // if 'v==0' this function returns 16
        inline s8 countLeadingZeros(u16 v) { return (s8)::__lzcnt16(v); }
        // find the number of leading zeros in 32-bit v
        // if 'v==0' this function returns 32
        inline s8 countLeadingZeros(u32 v) { return (s8)::__lzcnt(v); }
        // find the number of leading zeros in 64-bit v
        // if 'v==0' this function returns 64
        inline s8 countLeadingZeros(u64 v) { return (s8)::__lzcnt64(v); }

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
            s32 const lsob = countTrailingZeros(integer);
            return lsob;
        }

        // Return the bit index of the Most Significant Bit "1"
        // If 'integer == 0' this function will return -1
        inline s8 mostSignificantBit(u32 integer)
        {
            s32 c = 31 - countLeadingZeros(integer);
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
        // If 'integer == 0' this function will return -1
        inline s8 findLastBit(u8 integer) { return countLeadingZeros(integer); }

        // find the bit position/index of the first bit from low to high
        // If 'integer == 0' this function will return
        inline s8 findFirstBit(u16 integer)
        {
            if (integer == 0)
                return -1;
            return countTrailingZeros(integer);
        }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return -1
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
        // If 'integer == 0' this function will return -1
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
        // If 'integer == 0' this function will return -1
        inline s8 findLastBit(u64 integer) { return countLeadingZeros(integer); }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u8 inInteger)
        {
            u16 i16 = inInteger;
            return (s8)__popcnt16(i16);
        }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u16 inInteger) { return (s8)__popcnt16(inInteger); }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u32 inInteger) { return (s8)__popcnt(inInteger); }

        /**
         * count one bits in 64 bit word
         */
        inline s8 countBits(u64 inInteger) { return (s8)__popcnt64(inInteger); }
    } // namespace math
}