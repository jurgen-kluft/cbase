

namespace ncore
{
    // TODO: Optimize this using intrinsics
    namespace math
    {

        // find the number of trailing zeros in 16-bit value
        // if 'v==0' this function returns 0
        inline s8 xcountTrailingZeros(u16 integer)
        {
            if (integer == 0)
                return 0;
            s8 count = __builtin_ctzll(integer);
            return count;
        }
        // find the number of trailing zeros in 32-bit value
        // if 'v==0' this function returns 0
        inline s8 xcountTrailingZeros(u32 integer)
        {
            if (integer == 0)
                return 0;
            s8 count = __builtin_ctzll(integer);
            return count;
        }
        // find the number of trailing zeros in 64-bit value
        // if 'v==0' this function returns 0
        inline s8 xcountTrailingZeros(u64 integer)
        {
            if (integer == 0)
                return 0;
            s8 count = __builtin_ctzll(integer);
            return count;
        }

        // find the number of leading zeros in 16-bit v
        // if 'v==0' this function returns 16
        inline s8 xcountLeadingZeros(u16 integer)
        {
            if (integer == 0)
                return 16;
            s8 count = __builtin_clz(integer);
            return count - 16;
        }
        // find the number of leading zeros in 32-bit v
        // if 'v==0' this function returns 32
        inline s8 xcountLeadingZeros(u32 integer)
        {
            if (integer == 0)
                return 32;
            s8 count = __builtin_clz(integer);
            return count;
        }
        // find the number of leading zeros in 64-bit v
        // if 'v==0' this function returns 64
        inline s8 xcountLeadingZeros(u64 integer)
        {
            if (integer == 0)
                return 64;
            s8 count = __builtin_clzll(integer);
            return count;
        }

        // Return v but with only the Least Significant Bit "1"
        inline u32 xleastSignificantOneBit(u32 integer) { return (integer ^ (integer & (integer - 1))); }

        // Return v but with only the Most Significant Bit "1"
        // If 'integer == 0' this function will return 0
        inline u32 xmostSignificantOneBit(u32 integer)
        {
            s32 const msob = xcountLeadingZeros(integer);
            return (u32)(0x80000000 >> msob) & integer;
        }

        // Return the bit index of the Least Significant Bit "1"
        // If 'integer == 0' this function will return 0
        inline s8 xleastSignificantBit(u32 integer)
        {
            s8 const lsob = xcountTrailingZeros(integer);
            return lsob;
        }

        // Return the bit index of the Most Significant Bit "1"
        // If 'integer == 0' this function will return -1
        inline s8 xmostSignificantBit(u32 integer)
        {
            s8 c = 31 - xcountLeadingZeros(integer);
            return c;
        }

        // find the bit position/index of the first bit from low to high
        // If 'integer == 0' this function will return 0
        inline s8 xfindFirstBit(u16 integer)
        {
            if (integer == 0)
                return -1;
            return xcountTrailingZeros(integer);
        }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return -1
        inline s8 xfindLastBit(u16 integer) { return 15 - xcountLeadingZeros(integer); }

        // find the bit position/index of the first bit from low to high
        // If 'integer == 0' this function will return 0
        inline s8 xfindFirstBit(u32 integer)
        {
            if (integer == 0)
                return -1;
            return xcountTrailingZeros(integer);
        }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return -1
        inline s8 xfindLastBit(u32 integer) { return 31 - xcountLeadingZeros(integer); }

        // find the bit position/index of the first bit from low to high
        // If 'integer == 0' this function will return 0
        inline s8 xfindFirstBit(u64 integer)
        {
            if (integer == 0)
                return -1;
            return xcountTrailingZeros(integer);
        }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return -1
        inline s8 xfindLastBit(u64 integer) { return 63 - xcountLeadingZeros(integer); }

        /**
         * count one bits in 32 bit word
         */
        inline s8 xcountBits(u8 inInteger)
        {
            inInteger -= ((inInteger >> 1) & 0x55555555);
            inInteger = (((inInteger >> 2) & 0x33333333) + (inInteger & 0x33333333));
            inInteger = (((inInteger >> 4) + inInteger) & 0x0f0f0f0f);
            return (s8)(inInteger & 0x0000003f);
        }

        /**
         * count one bits in 32 bit word
         */
        inline s8 xcountBits(u16 inInteger)
        {
            inInteger -= ((inInteger >> 1) & 0x55555555);
            inInteger = (((inInteger >> 2) & 0x33333333) + (inInteger & 0x33333333));
            inInteger = (((inInteger >> 4) + inInteger) & 0x0f0f0f0f);
            inInteger += (inInteger >> 8);
            return (s8)(inInteger & 0x0000003f);
        }

        /**
         * count one bits in 32 bit word
         */
        inline s8 xcountBits(u32 inInteger)
        {
            inInteger -= ((inInteger >> 1) & 0x55555555);
            inInteger = (((inInteger >> 2) & 0x33333333) + (inInteger & 0x33333333));
            inInteger = (((inInteger >> 4) + inInteger) & 0x0f0f0f0f);
            inInteger += (inInteger >> 8);
            inInteger += (inInteger >> 16);
            return (s8)(inInteger & 0x0000003f);
        }

        /**
         * count one bits in 64 bit word
         */
        inline s8 xcountBits(u64 inInteger)
        {
            u32 high = (u64(inInteger) >> 32) & 0xffffffff;
            high -= ((high >> 1) & 0x55555555);
            high = (((high >> 2) & 0x33333333) + (high & 0x33333333));
            high = (((high >> 4) + high) & 0x0f0f0f0f);
            high += (high >> 8);
            high += (high >> 16);
            u32 low = inInteger & 0xffffffff;
            low -= ((low >> 1) & 0x55555555);
            low = (((low >> 2) & 0x33333333) + (low & 0x33333333));
            low = (((low >> 4) + low) & 0x0f0f0f0f);
            low += (low >> 8);
            low += (low >> 16);
            return (s8)(low & 0x0000003f) + (s8)(high & 0x0000003f);
        }
    } // namespace math
} // namespace ncore