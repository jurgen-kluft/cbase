/**
 * Function template, returns the smallest value
 */
template<class T>
inline T        xmin(T integerA, T integerB)
{
    return integerA<=integerB ? integerA : integerB;
}

/**
 * specialized template for s32
 */
template<>
inline s32		xmin(s32 integerA, s32 integerB)
{
    return integerA<=integerB ? integerA : integerB;
}

/**
 * Function template, returns the smallest value
 */
template<class T>
inline T        xmax(T integerA, T integerB)
{
    return integerA>=integerB ? integerA : integerB;
}

/**
 * specialized template for s32
 */
template<>
inline s32		xmax(s32 integerA, s32 integerB)
{
	s32 r = integerA ^ ((integerA ^ integerB) & -(integerA < integerB));
	return r;
}

/**
 * Function template, integerA = smallest value, integerB = biggest value
 */
template<class T>
inline void        xsort(T& integerA, T& integerB)
{
    if (integerA>integerB)
	{
		T t = integerA;	
		integerA = integerB;
		integerA = t;
	}
}

/**
 * specialized template for s32
 */
template<>
inline void		xsort(s32& X, s32& Y)
{
    if (X>Y)
	{
		X = X ^ Y;
		Y = Y ^ X;
		X = X ^ Y;
	}
}

/**
 * Function template, returns the clamp value
 */
template<class T>
inline T        xclamp(T integer, T low, T high)
{
    ASSERTS(low <= high, "Error: low > high");
    return integer<low ? low : (integer>high ? high : integer);
}

/**
 * Return the aligned value of integer
 * Same as alignUp
 */
inline s32       xalign(s32 integer, s32 alignment)
{
	ASSERTS(xispo2(alignment) == xTRUE, "Error: alignment value should be a power of 2");
	return (integer) & (~(alignment-1));
}

/**
 * Return the floor based aligned value of integer
 */
inline s32        xalignDown(s32 integer, s32 alignment)
{
	ASSERTS(xispo2(alignment) == xTRUE, "Error: alignment value should be a power of 2");
	return (integer) & (~(alignment-1));
}

/**
 * Return the ceiling based aligned value of integer
 */
inline s32        xalignUp(s32 integer, s32 alignment)
{
	ASSERTS(xispo2(alignment) == xTRUE, "Error: alignment value should be a power of 2");
	return ((integer + (alignment-1)) & (~(alignment-1)));
}

/**
 * Check if integer is aligned
 */
inline xbool       xisAligned(s32 integer, s32 alignment)
{
	ASSERTS(xispo2(alignment) == xTRUE, "Error: alignment value should be a power of 2");
	return integer == ((integer) & (~(alignment-1)));
}

/**
 * Return the absolute value of integer
 */
inline s32        xabs(s32 inInteger)
{
    if (inInteger > 0)
        return inInteger;
    else
        return -inInteger;
}

/**
 * Return the negated value of integer
 * inInteger:		Input value to negate if inNegate is false.
 * inDoNotNegate:	Flag indicating we should not negate inInteger.
 */
inline s32        xneg(s32 inInteger, bool inDoNotNegate)
{
	s32 r = ((s32)inDoNotNegate ^ ((s32)inDoNotNegate - 1)) * inInteger;
	return r;
}

/**
 * Return the square of integer
 */
inline s32        xsqr(s32 inInteger)
{
    return inInteger*inInteger;
}

/**
 * Return the unsigned square of integer
 */
inline u32        xsqr(u32 inInteger)
{
    return inInteger*inInteger;
}

/**
 * Return the modulo of integer using inModuloValue
 */
inline s32        xmod(s32 inInteger, s32 inModuloValue)
{
    return inInteger % inModuloValue;
}

/**
 * Greatest common denominator (biggest modulo value of both integers)
 */
inline s32        xgcd(s32 inIntegerA, s32 inIntegerB)
{
    while (inIntegerB!=0)
    {
        s32 t = inIntegerA % inIntegerB;
        inIntegerA = inIntegerB;
        inIntegerB = t;
    }
    return inIntegerA;
}

/**
 * Calculate average of A & B without overflow or s64 use
 */
inline s32        xaverage(s32 inIntegerA, s32 inIntegerB)
{
    return (inIntegerA&inIntegerB)+((inIntegerA^inIntegerB)>>1);
}

/**
 * Check if integer is a power-of-two
 */
inline xbool       xispo2(s32 inInteger)
{
    return (inInteger!=0) && (((inInteger) & (inInteger-1)) == 0);
}

/**
 * Return the smallest power-of-two larger than integer
 */
inline u32        xceilpo2(u32 inInteger)
{
    u32 t = inInteger-1;
    t |= t >> 1;
    t |= t >> 2;
    t |= t >> 4;
    t |= t >> 8;
    t |= t >> 16;
    t++;
    return (inInteger==t) ? (inInteger) : (t);
}

/**
 * Return the biggest power-of-two smaller than integer
 */
inline u32        xfloorpo2(u32 inInteger)
{
    u32 t = inInteger-1;
    t |= t >> 1;
    t |= t >> 2;
    t |= t >> 4;
    t |= t >> 8;
    t |= t >> 16;
    t++;
    return (inInteger==t) ? (inInteger) : (t>>1);
}

/**
 * Reverse bits in 32 bit word
 */
inline u32        xbitReverse(u32 inInteger)
{
    u32 n = inInteger;
    n = ((n >>  1) & 0x55555555) | ((n <<  1) & 0xaaaaaaaa) ;
    n = ((n >>  2) & 0x33333333) | ((n <<  2) & 0xcccccccc) ;
    n = ((n >>  4) & 0x0f0f0f0f) | ((n <<  4) & 0xf0f0f0f0) ;
    n = ((n >>  8) & 0x00ff00ff) | ((n <<  8) & 0xff00ff00) ;
    n = ((n >> 16) & 0x0000ffff) | ((n << 16) & 0xffff0000) ;
    return n;
}

/**
 * count one bits in 32 bit word
 */
inline s32        xcountBits(u32 inInteger)
{
    inInteger -= ((inInteger >> 1) & 0x55555555);
    inInteger = (((inInteger >> 2) & 0x33333333) + (inInteger & 0x33333333));
    inInteger = (((inInteger >> 4) + inInteger) & 0x0f0f0f0f);
    inInteger += (inInteger >> 8);
    inInteger += (inInteger >> 16);
    return(inInteger & 0x0000003f);
}

/**
 * count one bits in 64 bit word
 */
inline s32        xcountBits(u64 i)
{
    u32 high = (u64(i) >> 32) & 0xffffffff;
    high -= ((high >> 1) & 0x55555555);
    high = (((high >> 2) & 0x33333333) + (high & 0x33333333));
    high = (((high >> 4) + high) & 0x0f0f0f0f);
    high += (high >> 8);
    high += (high >> 16);
    u32 low = i & 0xffffffff;
    low -= ((low >> 1) & 0x55555555);
    low = (((low >> 2) & 0x33333333) + (low & 0x33333333));
    low = (((low >> 4) + low) & 0x0f0f0f0f);
    low += (low >> 8);
    low += (low >> 16);
    return (low & 0x0000003f) + (high & 0x0000003f);
}

/**
 * Log2 of an integer
 */
s32               xilog2(s32 inInteger)
{
    s32 r = 0;
    s32 i = inInteger;
    if (i&0xFFFF0000) i>>=16, r|=16;
    if (i&0xFF00FF00) i>>=8,  r|=8;
    if (i&0xF0F0F0F0) i>>=4,  r|=4;
    if (i&0xCCCCCCCC) i>>=2,  r|=2;
    if (i&0xAAAAAAAA) i>>=1,  r|=1;
    return r;
}

/**
 * Roll all the bits in integer to the left by shift number of bits
 */
inline u32        xrol32(u32 inInteger, u32 shift)
{
	shift = shift & 31;
    return (inInteger << shift) | (inInteger >> (32 - shift));
}

/**
 * Roll all the bits in integer to the right by shift number of bits
 */
inline u32        xror32(u32 inInteger, u32 shift)
{
	shift = shift & 31;
    return (inInteger >> shift) | (inInteger << (32 - shift));
}

/**
 *  MACROS
 *  Summary:
 *     convert from constant binary to u32 Example: X_Bin8( 111 ) == 7
 *  Description:
 *     Is used to convert from a constant binary number to a regular
 *     decimal number. Example: X_Bin8( 111 ) == 7.
 *     x_Bin8 can do a a max of 8bits worth of binary number ( 11111111 ).
 * Sample usage:
 *      x_Bin8(01010101) = 85
 *      x_Bin16(10101010,01010101) = 43605
 *      x_Bin32(10000000,11111111,10101010,01010101) = 2164238933
 */

#define x_Bin8(d)
#define x_Bin16(d)
#define x_Bin32(d)


/**
 * Summary:
 *     This function is design to convert a number to an align power of 2.
 * Arguments:
 *     Addr       - This is the address/number/offset to align
 *     AlignTo    - This is a power of 2 number that the user wants it to be align to
 * Description:
 *     The result in the next number greater than or equal to "n" 
 *     which is a multiple of "a".  For example, x_Align(57,16) is 64.
 * See Also:
 *     x_IsAlign x_AlignLower
 */

template< class T > T xalign( T addr, s32 alignTo );

/**
 * Summary:
 *     This function is design to convert a number to an align power of 2.
 * Arguments:
 *     Addr       - This is the address/number/offset to align
 *     AlignTo    - This is a power of 2 number that the user wants it to be align to
 * Description:
 *     The result in the next number less than or equal to "n" 
 *     which is a multiple of "a".  For example, x_AlignLower(57,16) is 48.
 * See Also:
 *     x_Align x_IsAlign
 */

template< class T > T xalign_floor(T addr, s32 alignTo);


/**
 * Summary:
 *     This macro checks whether a number is align to a power of 2.
 * Arguments:
 *     Addr       - This is the address/number/offset to align
 *     AlignTo    - This is a power of 2 number that the user wants it to be align to
 * Description:
 *     This macro result in a xTRUE/xFALSE which indicates whether a number/address/offset
 *     is align to a certain power of two provided by the user.
 * See Also:
 *     x_Align x_AlignLower
 */
template< class T > xbool xaligned( T addr, s32 alignTo );


/**
 * Summary:
 *     This macro is used to turn a bit on.
 * Arguments:
 *     A - Is the bit number that you want to turn on.
 * Description:
 *     This macro is used to turn a bit on. careful not to pass unfriendly
 *     bit manipulation types such f32
 * See Also:
 *     x_FlagOn x_FlagOff x_FlagIsOn
 */
#define x_Bit(A) ((1)<<(A))

 /** Summary:
 *     It is a function used to test whether an integer number is a power of two.
 * Arguments:
 *     N    - Test if a number is a power of two.
 * Returns:
 *     xTRUE if it is a power of two, xFALSE if it is not.
 * Description:
 *     It is a function used to test whether an integer number is a power of two.
 */
template< class T > 
inline xbool xpowof2(T n) 
{ 
	return xbool( n==1 || ((n&(n-1))==0) ); 
}

/**
 * x_Align functions
 */

template< class T > inline 
T xalign( T addr, s32 alignTo ) 
{
    return T( ( (((u64)(addr)) + ((alignTo)-1)) & (-(alignTo)) )); 
}

//------------------------------------------------------------------------------

template< class T > inline 
T xalign_floor( T addr, s32 alignTo ) 
{ 
    return T( ( (((u64)(addr))) & (-(alignTo)) )); 
}

//------------------------------------------------------------------------------

template< class T > inline 
xbool xaligned( T addr, s32 alignTo ) 
{ 
    return xbool((((u64)addr) & (alignTo-1)) == 0);
}


/**
 * MACRO BIN
 *
 * DOM-IGNORE-BEGIN

 * All macro's evaluate to compile-time constants
 *
 * *** helper macros ***
 *
 * turn a numeric literal into a hex constant
 * (avoids problems with leading zeros)
 * 8-bit constants max value 0x11111111, always fits in unsigned long
 */
#define XHEX__(n) 0x##n##LU

/// 8-bit conversion function
#define XB8__(x) ((x&0x0000000FLU)?1:0)      \
             +((x&0x000000F0LU)?2:0)      \
             +((x&0x00000F00LU)?4:0)      \
             +((x&0x0000F000LU)?8:0)      \
             +((x&0x000F0000LU)?16:0)     \
             +((x&0x00F00000LU)?32:0)     \
             +((x&0x0F000000LU)?64:0)     \
             +((x&0xF0000000LU)?128:0)

/// *** user macros ***

/// for up to 8-bit binary constants
#undef xbin8
#define xbin8(d) ((u8)XB8__(XHEX__(d)))

/// for up to 16-bit binary constants, MSB first
#undef xbin16
#define xbin16(dmsb,dlsb) (((u16)xbin8(dmsb)<<8) + xbin8(dlsb))

/// for up to 32-bit binary constants, MSB first
 #undef xbin32
 #define xbin32(dmsb,db2,db3,dlsb) (((u32)xbin8(dmsb)<<24)		\
                                + ((u32)xbin8(db2)<<16)		\
								+ ((u32)xbin8(db3)<<8)			\
                                + xbin8(dlsb))

///  DOM-IGNORE-END


