namespace ncore
{
    namespace math
    {
        // find the number of trailing zeros in 16-bit value
        // if 'v==0' this function returns 8
        inline s8 countTrailingZeros(u8 value)
        {
            s8 count = 0;
            if ((value & 0x0F) == 0)
            {
                count += 4;
                value = value >> 4;
            }
            if ((value & 0x03) == 0)
            {
                count += 2;
                value = value >> 2;
            }
            if ((value & 0x01) == 0)
            {
                count += 1;
                value = value >> 1;
            }
            if ((value & 0x01) == 1)
            {
                return count;
            }
            return 8;
        }

        // find the number of trailing zeros in 16-bit value
        // if 'v==0' this function returns 16
        inline s8 countTrailingZeros(u16 value)
        {
            s8 count = 0;
            if ((value & 0x000000FF) == 0)
            {
                count += 8;
                value = value >> 8;
            }
            if ((value & 0x0000000F) == 0)
            {
                count += 4;
                value = value >> 4;
            }
            if ((value & 0x00000003) == 0)
            {
                count += 2;
                value = value >> 2;
            }
            if ((value & 0x00000001) == 0)
            {
                count += 1;
                value = value >> 1;
            }
            if ((value & 0x00000001) == 1)
            {
                return count;
            }
            return 16;
        }

        // find the number of trailing zeros in 32-bit value
        // if 'v==0' this function returns 32
        inline s8 countTrailingZeros(u32 value)
        {
            s8 count = 0;
            if ((value & 0x0000FFFF) == 0)
            {
                count += 16;
                value = value >> 16;
            }
            if ((value & 0x000000FF) == 0)
            {
                count += 8;
                value = value >> 8;
            }
            if ((value & 0x0000000F) == 0)
            {
                count += 4;
                value = value >> 4;
            }
            if ((value & 0x00000003) == 0)
            {
                count += 2;
                value = value >> 2;
            }
            if ((value & 0x00000001) == 0)
            {
                count += 1;
                value = value >> 1;
                ;
            }
            if ((value & 0x00000001) == 1)
            {
                return count;
            }
            return 32;
        }
        // find the number of trailing zeros in 64-bit value
        // if 'v==0' this function returns 64
        inline s8 countTrailingZeros(u64 value)
        {
            s8 count = 0;
            if ((value & 0xFFFFFFFF) == 0)
            {
                count += 32;
                value = value >> 32;
            }
            if ((value & 0x0000FFFF) == 0)
            {
                count += 16;
                value = value >> 16;
            }
            if ((value & 0x000000FF) == 0)
            {
                count += 8;
                value = value >> 8;
            }
            if ((value & 0x0000000F) == 0)
            {
                count += 4;
                value = value >> 4;
            }
            if ((value & 0x00000003) == 0)
            {
                count += 2;
                value = value >> 2;
            }
            if ((value & 0x00000001) == 0)
            {
                count += 1;
                value = value >> 1;
            }
            if ((value & 0x00000001) == 1)
            {
                return count;
            }
            return 64;
        }

        // find the number of leading zeros in 8-bit
        // if 'v==0' this function returns 8
        inline s8 countLeadingZeros(u8 value)
        {
            if (value == 0)
                return 8;

            s8 count = 0;
            if ((value & 0xF0) == 0)
            {
                count += 4;
                value = value << 4;
            }
            if ((value & 0xC0) == 0)
            {
                count += 2;
                value = value << 2;
            }
            if ((value & 0x80) == 0)
            {
                count += 1;
                value = value << 1;
            }
            if ((value & 0x80) == 0)
            {
                count += 1;
            }
            return count;
        }
        // find the number of leading zeros in 16-bit v
        // if 'v==0' this function returns 16
        inline s8 countLeadingZeros(u16 value)
        {
            if (value == 0)
                return 16;

            s8 count = 0;
            if ((value & 0xFF00) == 0)
            {
                count += 8;
                value = value << 8;
            }
            if ((value & 0xF000) == 0)
            {
                count += 4;
                value = value << 4;
            }
            if ((value & 0xC000) == 0)
            {
                count += 2;
                value = value << 2;
            }
            if ((value & 0x8000) == 0)
            {
                count += 1;
                value = value << 1;
            }
            if ((value & 0x8000) == 0)
            {
                count += 1;
            }
            return count;
        }
        // find the number of leading zeros in 32-bit v
        // if 'v==0' this function returns 32
        inline s8 countLeadingZeros(u32 value)
        {
            if (value == 0)
                return 32;

            s8 count = 0;
            if ((value & 0xFFFF0000) == 0)
            {
                count += 16;
                value = value << 16;
            }
            if ((value & 0xFF000000) == 0)
            {
                count += 8;
                value = value << 8;
            }
            if ((value & 0xF0000000) == 0)
            {
                count += 4;
                value = value << 4;
            }
            if ((value & 0xC0000000) == 0)
            {
                count += 2;
                value = value << 2;
            }
            if ((value & 0x80000000) == 0)
            {
                count += 1;
                value = value << 1;
            }
            if ((value & 0x80000000) == 0)
            {
                count += 1;
            }
            return count;
        }
        // find the number of leading zeros in 64-bit v
        // if 'v==0' this function returns 64
        inline s8 countLeadingZeros(u64 value)
        {
            if (value == 0)
                return 64;

            s8 count = 0;
            if ((value & 0xFFFFFFFF00000000UL) == 0)
            {
                count += 32;
                value = value << 32;
            }
            if ((value & 0xFFFF000000000000UL) == 0)
            {
                count += 16;
                value = value << 16;
            }
            if ((value & 0xFF00000000000000UL) == 0)
            {
                count += 8;
                value = value << 8;
            }
            if ((value & 0xF000000000000000UL) == 0)
            {
                count += 4;
                value = value << 4;
            }
            if ((value & 0xC000000000000000UL) == 0)
            {
                count += 2;
                value = value << 2;
            }
            if ((value & 0x8000000000000000UL) == 0)
            {
                count += 1;
                value = value << 1;
            }
            if ((value & 0x8000000000000000UL) == 0)
            {
                count += 1;
            }
            return count;
        }

        // Return v but with only the Least Significant Bit "1"
        inline u32 leastSignificantOneBit(u32 value) { return (value ^ (value & (value - 1))); }

        // Return v but with only the Most Significant Bit "1"
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
            s8 const lsob = countTrailingZeros(value);
            return lsob;
        }

        // Return the bit index of the Most Significant Bit "1"
        // If 'value == 0' this function will return -1
        inline s8 mostSignificantBit(u32 value)
        {
            s8 c = 31 - countLeadingZeros(value);
            return c;
        }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return 0
        inline s8 findFirstBit(u8 value) { return countTrailingZeros(value); }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0' this function will return -1
        inline s8 findLastBit(u8 value) { return 7 - countLeadingZeros(value); }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return 0
        inline s8 findFirstBit(u16 value) { return countTrailingZeros(value); }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0' this function will return -1
        inline s8 findLastBit(u16 value) { return 15 - countLeadingZeros(value); }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return 0
        inline s8 findFirstBit(u32 value) { return countTrailingZeros(value); }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0' this function will return -1
        inline s8 findLastBit(u32 value) { return 31 - countLeadingZeros(value); }

        // find the bit position/index of the first bit from low to high
        // If 'value == 0' this function will return 0
        inline s8 findFirstBit(u64 value) { return countTrailingZeros(value); }

        // find the bit position/index of the first bit from high to low
        // If 'value == 0' this function will return -1
        inline s8 findLastBit(u64 value) { return 63 - countLeadingZeros(value); }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u8 value)
        {
            value -= ((value >> 1) & 0x55555555);
            value = (((value >> 2) & 0x33333333) + (value & 0x33333333));
            value = (((value >> 4) + value) & 0x0f0f0f0f);
            return (s8)(value & 0x0000003f);
        }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u16 value)
        {
            value -= ((value >> 1) & 0x55555555);
            value = (((value >> 2) & 0x33333333) + (value & 0x33333333));
            value = (((value >> 4) + value) & 0x0f0f0f0f);
            value += (value >> 8);
            return (s8)(value & 0x0000003f);
        }

        /**
         * count one bits in 32 bit word
         */
        inline s8 countBits(u32 value)
        {
            value -= ((value >> 1) & 0x55555555);
            value = (((value >> 2) & 0x33333333) + (value & 0x33333333));
            value = (((value >> 4) + value) & 0x0f0f0f0f);
            value += (value >> 8);
            value += (value >> 16);
            return (s8)(value & 0x0000003f);
        }

        /**
         * count one bits in 64 bit word
         */
        inline s8 countBits(u64 value)
        {
            u32 high = (u64(value) >> 32) & 0xffffffff;
            high -= ((high >> 1) & 0x55555555);
            high = (((high >> 2) & 0x33333333) + (high & 0x33333333));
            high = (((high >> 4) + high) & 0x0f0f0f0f);
            high += (high >> 8);
            high += (high >> 16);
            u32 low = value & 0xffffffff;
            low -= ((low >> 1) & 0x55555555);
            low = (((low >> 2) & 0x33333333) + (low & 0x33333333));
            low = (((low >> 4) + low) & 0x0f0f0f0f);
            low += (low >> 8);
            low += (low >> 16);
            return (s8)(low & 0x0000003f) + (s8)(high & 0x0000003f);
        }
    }  // namespace math
}  // namespace ncore