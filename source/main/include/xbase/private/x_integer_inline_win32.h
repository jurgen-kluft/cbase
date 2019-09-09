/**
 * find the number of trailing zeros in 32-bit v
 */
inline s32 xcountTrailingZeros(u32 integer)
{
    if (integer == 0)
        return 32;

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
inline s32 xcountTrailingZeros(u64 integer)
{
    if (integer == 0)
        return 64;

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

/**
 * find the number of leading zeros in 32-bit v
 */
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
    if ((integer & 0x80000000) == 0)
    {
        count += 1;
    }

    return count;
}
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
    if ((integer & 0x8000000000000000UL) == 0)
    {
        count += 1;
    }

    return count;
}

/**
 * Return v but with only the Least Significant Bit "1"
 */
inline s32 xleastSignificantOneBit(u32 integer) { return (integer ^ (integer & (integer - 1))); }

/**
 * Return v but with only the Most Significant Bit "1"
 */
inline s32 xmostSignificantOneBit(u32 integer)
{
    integer |= (integer >> 1);
    integer |= (integer >> 2);
    integer |= (integer >> 4);
    integer |= (integer >> 8);
    integer |= (integer >> 16);
    return (integer & ~(integer >> 1));
}

/**
 * Return the bit index of the Least Significant Bit "1"
 */
inline s32 xleastSignificantBit(u32 integer)
{
    if (integer == 0)
        return 32;
    s32 c = xcountTrailingZeros(integer);
    return c;
}

/**
 * Return the bit index of the Most Significant Bit "1"
 */
inline s32 xmostSignificantBit(u32 integer)
{
    if (integer == 0)
        return 32;
    int c = 31 - xcountLeadingZeros(integer);
    return c;
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
