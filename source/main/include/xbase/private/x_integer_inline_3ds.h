/**
 * find the number of trailing zeros in 32-bit v
 */
inline s32        x_intu::countTrailingZeros(s32 inInteger)
{
    if (inInteger == 0)
        return 32;

#ifdef X_INTU_OPTIMIZED
	u8 c = __clz(inInteger);
	return 31-c;
#else
    s32 count = 0;
    if ((inInteger & 0x0000FFFF) == 0) {count += 16; inInteger = inInteger >>16;}
    if ((inInteger & 0x000000FF) == 0) {count +=  8; inInteger = inInteger >> 8;}
    if ((inInteger & 0x0000000F) == 0) {count +=  4; inInteger = inInteger >> 4;}
    if ((inInteger & 0x00000003) == 0) {count +=  2; inInteger = inInteger >> 2;}
    if ((inInteger & 0x00000001) == 0) {count +=  1;}
    return count;
#endif
}

/**
 * find the number of leading zeros in 32-bit v
 */
inline s32        x_intu::countLeadingZeros(s32 inInteger)
{
    if (inInteger == 0)
        return 32;

#ifdef X_INTU_OPTIMIZED
	u8 c = __clz(inInteger);
	return c;
#else
    s32 count = 0;
    if ((inInteger & 0xFFFF0000) == 0) {count += 16; inInteger = inInteger <<16;}
    if ((inInteger & 0xFF000000) == 0) {count +=  8; inInteger = inInteger << 8;}
    if ((inInteger & 0xF0000000) == 0) {count +=  4; inInteger = inInteger << 4;}
    if ((inInteger & 0xC0000000) == 0) {count +=  2; inInteger = inInteger << 2;}
    if ((inInteger & 0x80000000) == 0) {count +=  1;}
    return count;
#endif
}

/**
 * Return v but with only the Least Significant Bit "1"
 */
inline s32        x_intu::leastSignificantOneBit(u32 inInteger)
{
    return (inInteger^(inInteger&(inInteger-1)));
}

/**
 * Return v but with only the Most Significant Bit "1"
 */
inline s32        x_intu::mostSignificantOneBit(u32 inInteger)
{
    if (inInteger == 0)
        return 0;

#ifdef X_INTU_OPTIMIZED
	u8 c = __clz(inInteger);
	return 1<<(31-c);
#else
    inInteger |= (inInteger >> 1);
    inInteger |= (inInteger >> 2);
    inInteger |= (inInteger >> 4);
    inInteger |= (inInteger >> 8);
    inInteger |= (inInteger >> 16);
    return(inInteger & ~(inInteger >> 1));
#endif
}

/**
 * Return the bit index of the Least Significant Bit "1"
 */
inline s32        x_intu::leastSignificantBit(u32 inInteger)
{
    if (inInteger==0)
        return -1;
    s32 c = x_intu::countTrailingZeros(inInteger);
    return c;
}

/**
 * Return the bit index of the Most Significant Bit "1"
 */
inline s32        x_intu::mostSignificantBit(u32 inInteger)
{
    if (inInteger==0)
        return -1;
    return 31-x_intu::countLeadingZeros(inInteger);
}

/**
 * find the bit position/index of the first bit from low to high
 */
inline s32        x_intu::findFirstBit(u32 inInteger)
{
    if (inInteger==0)
        return -1;
    return x_intu::countTrailingZeros(inInteger);
}

/**
 * find the bit position/index of the first bit from high to low
 */
inline s32        x_intu::findLastBit(u32 inInteger)
{
	if (inInteger==0)
        return -1;
    return 31-x_intu::countLeadingZeros(inInteger);
}
