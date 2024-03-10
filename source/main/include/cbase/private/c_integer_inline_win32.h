#include <intrin.h>
#pragma intrinsic(_BitScanForward, _BitScanForward64, _BitScanReverse, _BitScanReverse64)

namespace ncore
{
    namespace math
    {
        // find the number of trailing zeros in 16-bit value
        // if 'value==0' this function returns 0
        inline s8 countTrailingZeros(u8 value)
        {
            unsigned long result;
            ::_BitScanForward(&result, value);
            return (s8)result;
        }
        // find the number of trailing zeros in 16-bit value
        // if 'value==0' this function returns 0
        inline s8 countTrailingZeros(u16 value)
        {
            unsigned long result;
            ::_BitScanForward(&result, value);
            return (s8)result;
        }
        // find the number of trailing zeros in 32-bit value
        // if 'value==0' this function returns 0
        inline s8 countTrailingZeros(u32 value)
        {
            unsigned long result;
            ::_BitScanForward(&result, value);
            return (s8)result;
        }
        // find the number of trailing zeros in 64-bit value
        // if 'value==0' this function returns 0
        inline s8 countTrailingZeros(u64 value)
        {
            unsigned long result;
            ::_BitScanForward64(&result, value);
            return (s8)result;
        }

        // find the number of leading zeros in 8-bit value
        // if 'value==0' this function returns 8
        inline s8 countLeadingZeros(u8 value)
        {
            return (s8)::__lzcnt16(value) - 8;
        }
        // find the number of leading zeros in 16-bit value
        // if 'value==0' this function returns 16
        inline s8 countLeadingZeros(u16 value)
        {
            return (s8)::__lzcnt16(value);
        }
        // find the number of leading zeros in 32-bit value
        // if 'value==0' this function returns 32
        inline s8 countLeadingZeros(u32 value)
        {
            return (s8)::__lzcnt(value);
        }
        // find the number of leading zeros in 64-bit value
        // if 'value==0' this function returns 64
        inline s8 countLeadingZeros(u64 value)
        {
            return (s8)::__lzcnt64(value);
        }

        // Return value but with only the Least Significant Bit "1"
        inline u32 leastSignificantOneBit(u32 value) { return (value ^ (value & (value - 1))); }

        // Return value but with only the Most Significant Bit "1"
        // If 'value == 0' this function will return 0
        inline u32 mostSignificantOneBit(u32 value)
        {
            s32 const msob = countLeadingZeros(value);
            return (u32)(0x80000000 >> msob) & value;
        }

        // Return the bit index of the Least Significant Bit "1"
        // If 'value == 0' this function will return 0
        inline s8 leastSignificantBit(u32 value)
        {
            s32 const lsob = countTrailingZeros(value);
            return lsob;
        }

        // Return the bit index of the Most Significant Bit "1"
        // If 'value == 0' this function will return -1
        inline s8 mostSignificantBit(u32 value)
        {
            s32 c = 31 - countLeadingZeros(value);
            return c;
        }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return -1
        inline s8 findFirstBit(u8 value)
        {
            if (value == 0)
                return -1;
            return countTrailingZeros(value);
        }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0' this function will return -1
        inline s8 findLastBit(u8 value) { return countLeadingZeros(value); }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return
        inline s8 findFirstBit(u16 value)
        {
            if (value == 0)
                return -1;
            return countTrailingZeros(value);
        }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0' this function will return -1
        inline s8 findLastBit(u16 value) { return countLeadingZeros(value); }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return -1
        inline s8 findFirstBit(u32 value)
        {
            if (value == 0)
                return -1;
            return countTrailingZeros(value);
        }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0' this function will return -1
        inline s8 findLastBit(u32 value) { return countLeadingZeros(value); }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return 0
        inline s8 findFirstBit(u64 value)
        {
            if (value == 0)
                return -1;
            return countTrailingZeros(value);
        }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0' this function will return -1
        inline s8 findLastBit(u64 value) { return countLeadingZeros(value); }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u8 value)
        {
            u16 i16 = value;
            return (s8)__popcnt16(i16);
        }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u16 value) { return (s8)__popcnt16(value); }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u32 value) { return (s8)__popcnt(value); }

        /**
         * count one bits in 64 bit word
         */
        inline s8 countBits(u64 value) { return (s8)__popcnt64(value); }
    }  // namespace math
}  // namespace ncore