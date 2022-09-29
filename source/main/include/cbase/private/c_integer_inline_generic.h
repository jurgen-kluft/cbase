namespace ncore
{
    namespace math
    {
        // find the number of trailing zeros in 16-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u16 integer)
        {
            s8 count = 0;
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
                integer = integer >> 1;
                ;
            }
            if ((integer & 0x00000001) == 1)
            {
                return count;
            }
            return 0;
        }

        // find the number of trailing zeros in 32-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u32 integer)
        {
            s8 count = 0;
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
                integer = integer >> 1;
                ;
            }
            if ((integer & 0x00000001) == 1)
            {
                return count;
            }
            return 0;
        }
        // find the number of trailing zeros in 64-bit value
        // if 'v==0' this function returns 0
        inline s8 countTrailingZeros(u64 integer)
        {
            s8 count = 0;
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
                integer = integer >> 1;
            }
            if ((integer & 0x00000001) == 1)
            {
                return count;
            }
            return 0;
        }

        // find the number of leading zeros in 16-bit v
        // if 'v==0' this function returns 16
        inline s8 countLeadingZeros(u16 integer)
        {
            if (integer == 0)
                return 16;

            s8 count = 0;
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
            if ((integer & 0x8000) == 0)
            {
                count += 1;
                integer = integer << 1;
            }
            if ((integer & 0x8000) == 0)
            {
                count += 1;
            }
            return count;
        }
        // find the number of leading zeros in 32-bit v
        // if 'v==0' this function returns 32
        inline s8 countLeadingZeros(u32 integer)
        {
            if (integer == 0)
                return 32;

            s8 count = 0;
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
            if ((integer & 0x80000000) == 0)
            {
                count += 1;
                integer = integer << 1;
            }
            if ((integer & 0x80000000) == 0)
            {
                count += 1;
            }
            return count;
        }
        // find the number of leading zeros in 64-bit v
        // if 'v==0' this function returns 64
        inline s8 countLeadingZeros(u64 integer)
        {
            if (integer == 0)
                return 64;

            s8 count = 0;
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
            if ((integer & 0x8000000000000000UL) == 0)
            {
                count += 1;
                integer = integer << 1;
            }
            if ((integer & 0x8000000000000000UL) == 0)
            {
                count += 1;
            }
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
        // If 'integer == 0' this function will return 0
        inline s8 findFirstBit(u16 integer) { return countTrailingZeros(integer); }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return -1
        inline s8 findLastBit(u16 integer) { return 15 - countLeadingZeros(integer); }

        // find the bit position/index of the first bit from low to high
        // If 'integer == 0' this function will return 0
        inline s8 findFirstBit(u32 integer) { return countTrailingZeros(integer); }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return -1
        inline s8 findLastBit(u32 integer) { return 31 - countLeadingZeros(integer); }

        // find the bit position/index of the first bit from low to high
        // If 'integer == 0' this function will return 0
        inline s8 findFirstBit(u64 integer) { return countTrailingZeros(integer); }

        // find the bit position/index of the first bit from high to low
        // If 'integer == 0' this function will return -1
        inline s8 findLastBit(u64 integer) { return 63 - countLeadingZeros(integer); }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u8 inInteger)
        {
            inInteger -= ((inInteger >> 1) & 0x55555555);
            inInteger = (((inInteger >> 2) & 0x33333333) + (inInteger & 0x33333333));
            inInteger = (((inInteger >> 4) + inInteger) & 0x0f0f0f0f);
            return (s8)(inInteger & 0x0000003f);
        }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u16 inInteger)
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
        inline s8 countBits(u32 inInteger)
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
        inline s8 countBits(u64 inInteger)
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