/**
 * IEEE 754 description
 */
namespace math
{
    struct EDoubleIEEE
    {
        EDoubleIEEE()
            :DD_FRACTION_MASK        (D_CONSTANT_U64(0x000FFFFFFFFFFFFF))
            ,DD_EXPONENT_MASK        (D_CONSTANT_U64(0x7FF0000000000000))
            ,DD_SIGN_MASK            (D_CONSTANT_U64(0x8000000000000000))
            ,DD_EXPONENT_BIAS        ((1<<(11-1)) - 1)
            ,DD_MAX_EXPONENT_BIAS    ((1<<11) - 1)
            ,DD_GET_FRACTION_SHIFT    (0)
            ,DD_GET_EXPONENT_SHIFT    (52)
            ,DD_GET_SIGN_SHIFT        (63)
            ,DD_FRACTION_BITS        (52)
            ,DD_EXPONENT_BITS        (11)
            ,DD_SIGN_BITS            (1) {}
        const u64 DD_FRACTION_MASK;
        const u64 DD_EXPONENT_MASK;
        const u64 DD_SIGN_MASK;
        const u64 DD_EXPONENT_BIAS;
        const u64 DD_MAX_EXPONENT_BIAS;
        const u64 DD_GET_FRACTION_SHIFT;
        const u64 DD_GET_EXPONENT_SHIFT;
        const u64 DD_GET_SIGN_SHIFT;
        const u64 DD_FRACTION_BITS;
        const u64 DD_EXPONENT_BITS;
        const u64 DD_SIGN_BITS;
    private:
        EDoubleIEEE& operator = (const EDoubleIEEE&)        {return *this;}

    };

    static const EDoubleIEEE sDoubleIEEE;

    /**
     * @return  This funtion returns a negated number
     */
    inline f64			abs(f64 inNumber)								{ return inNumber<0.0 ? -inNumber : inNumber; }

    /**
     * @return  This funtion returns a negated number
     */
    inline f64			neg(f64 inNumber)								{ return -inNumber; }

    
    // double power function
    inline f64 powd(double x, s32 p)
    {
        if (p == 0)
            return 1.0;
        if (x == 0.0)
            return 0.0;

        if (p < 0)
        {
            p = -p;
            x = 1.0 / x;
        }

        f64 r = 1.0;
        while (true)
        {
            if (p & 1)
                r *= x;

            if ((p >>= 1) == 0)
                return r;

            x *= x;
        }

    }

    /**
     * Limits and const values
     */
    inline f64 positiveZero64()
    {
        u64 f = (u64)0;
        return *(f64*)&f;
    }
    inline f64 negativeZero64()
    {
        u64 f = (u64)sDoubleIEEE.DD_SIGN_MASK;
        return *(f64*)&f;
    }
#ifdef IEEE_FLOATS
    inline f64 nan64()
    {
        u64 f = (u64)(sDoubleIEEE.DD_EXPONENT_MASK | 1);
        return *(f64*)&f;
    }
    inline f64 positiveInfinity64()
    {
        u64 f = (u64)(sDoubleIEEE.DD_EXPONENT_MASK);
        return *(f64*)&f;
    }
    inline f64 negativeInfinity64()
    {
        u64 f = (u64)(sDoubleIEEE.DD_SIGN_MASK | sDoubleIEEE.DD_EXPONENT_MASK);
        return *(f64*)&f;
    }
#endif
    inline f64			positiveMaximum64()								{ u64 f = (u64)(((sDoubleIEEE.DD_MAX_EXPONENT_BIAS-1) << sDoubleIEEE.DD_GET_EXPONENT_SHIFT) | sDoubleIEEE.DD_FRACTION_MASK); return toF64(f); }
    inline f64			negativeMaximum64()								{ u64 f = (u64)((sDoubleIEEE.DD_SIGN_MASK | ((sDoubleIEEE.DD_MAX_EXPONENT_BIAS-1) << sDoubleIEEE.DD_GET_EXPONENT_SHIFT)) | sDoubleIEEE.DD_FRACTION_MASK);  return toF64(f); }
    inline f64			positiveMinimum64()								{ u64 f = (u64)((u64)(1) << sDoubleIEEE.DD_GET_EXPONENT_SHIFT); return toF64(f); }
    inline f64			negativeMinimum64()								{ u64 f = (u64)(sDoubleIEEE.DD_SIGN_MASK | ((u64)1 << sDoubleIEEE.DD_GET_EXPONENT_SHIFT)); return toF64(f); }

    /**
     * De-normalized (DEN) positive and negative minimums
     */
    inline f64 positiveMinimumDEN64()
    {
        u64 f = (u64)((u64)1 << (sDoubleIEEE.DD_FRACTION_BITS - 1));
        return toF64(f);
    }
    inline f64 negativeMinimumDEN64()
    {
        u64 f = (u64)(sDoubleIEEE.DD_SIGN_MASK | ((u64)1 << (sDoubleIEEE.DD_FRACTION_BITS - 1)));
        return toF64(f);
    }

    /**
     * Rational Determination.
     */
#ifdef IEEE_FLOATS
    inline bool		isInfinite(f64 inNumber)						{ return bool((bin(inNumber) & (sDoubleIEEE.DD_EXPONENT_MASK | sDoubleIEEE.DD_FRACTION_MASK)) == sDoubleIEEE.DD_EXPONENT_MASK); } 
    inline bool		isNAN(f64 inNumber)     						{ return bool(((bin(inNumber) & sDoubleIEEE.DD_EXPONENT_MASK) == sDoubleIEEE.DD_EXPONENT_MASK) && (bin(inNumber) & sDoubleIEEE.DD_FRACTION_MASK)); }
    inline bool		isFinite(f64 inNumber)  						{ return bool(!isInfinite(inNumber) && !isNAN(inNumber)); }
    inline bool		isRational(f64 inNumber)						{ return bool((bin(inNumber) & sDoubleIEEE.DD_EXPONENT_MASK) != sDoubleIEEE.DD_EXPONENT_MASK); }
#else
    inline bool		isInfinite(f64 inNumber)						{ return False; }
    inline bool		isNAN(f64 inNumber)     						{ return False; }
    inline bool		isFinite(f64 inNumber)  						{ return True; }
    inline bool		isRational(f64 inNumber)						{ return True; }
#endif
    /**
     * bin. Returns the f64 as an u64. It's not casted, but the address is casted, so you get the binary representation
     */
    inline u64        bin(f64 inNumber)                                 { return *(u64*)&inNumber; }

    /**
     * sbin. Returns the f64 as an s64. Used for signed operations like shifting down
     */
    inline s64        sbin(f64 inNumber)                                { return *(s64*)&inNumber; }

    /**
     * Float. Returns the u64 as a f64. It's not casted, but the address is, so you get the binary representation back.
     */
    inline f64        toF64(u64 inInt)                           { return *(f64*)&inInt; }

    inline s64        toS64(f64 inB)                                    { return (s64)inB;  }
    inline u64        toU64(f64 inB)                                    { return (u64)(s64)inB; }

    /**
     * IsEqual, returns True if two doubles are the same
     */
    inline bool      isEqual(f64 inNumber, f64 inG)                    { return bool(bin(inNumber)==bin(inG)); }

    /**
     * IsNotEqual, returns True if the two doubles are not the same
     */
    inline bool      isNotEqual(f64 inNumber, f64 inG)                 { return bool(bin(inNumber)!=bin(inG)); }

    /**
     * And. To 'And' a f64 with a binary value. Used for abs for example
     */
    inline f64        binaryAnd(f64 inNumber, s32 inB)                  { return toF64(bin(inNumber) & inB); }

    /**
     * Or. To 'or' a f64 with a binary value
     */
    inline f64        binaryOr(f64 inNumber, s32 inB)                   { return toF64(bin(inNumber) | inB); }

    /**
     * Fraction. Returns the Fractional part of the f64 point value
     */
    inline s64        fraction(f64 inNumber)                            { return bin(inNumber) & sDoubleIEEE.DD_FRACTION_MASK; }

    /**
     * ExponentBinary. Returns the Exponent part of the f64 point value. This value is biased
     */
    inline s32        exponentBinary(f64 inNumber)                      { return (s32)((bin(inNumber) & sDoubleIEEE.DD_EXPONENT_MASK) >> sDoubleIEEE.DD_GET_EXPONENT_SHIFT); }

    /**
     * Exponent. Returns the Exponent part of the f64 point value, without the bias
     */
    inline s32        exponent(f64 f)                                   { return exponentBinary(f) - (s32)sDoubleIEEE.DD_EXPONENT_BIAS; }

    /**
     * SignMask. Returns the f64 point value masked with the sign mask. 
     */
    inline u64        signMask(f64 inNumber)                            { return bin(inNumber) & sDoubleIEEE.DD_SIGN_MASK; }

    /**
     * SignBit. Returns the Sign bit (either 0 or 1)
     */
    inline s32        signBit(f64 inNumber)                             { return (s32)(bin(inNumber) >> sDoubleIEEE.DD_GET_SIGN_SHIFT); }

    /**
     * SignBitSigned. Returns the Sign bit expanded in the whole s32: 0 = positive -1 = negative
     */
    inline s32        signBitSigned(f64 inNumber)                       { return (s32)(sbin(inNumber) >> sDoubleIEEE.DD_GET_SIGN_SHIFT);  }

    /**
     * Sign returns -1 if the value is negative, 0 if the value is zero (0 or -0) and +1 if it's positive
     */
    inline s32        sign(f64 inNumber)                                { if (bin(inNumber) & ~sDoubleIEEE.DD_SIGN_MASK) return 1 | signBitSigned(inNumber); else return 0; }

    /**
     * Sign with a little delta. If the f64 value is greater than the delta, it returns +1.
     * @retval -1   If it's less than -delta it returns -1. 
     * @retval 0    If the value is between -delta and +delta it returns 0.
     */
    inline s32        sign(f64 inNumber, f64 inDelta)                   { ASSERT(inDelta >= 0.0); return abs(inNumber) <= inDelta ? 0 : sign(inNumber); }

    /**
     * Check if a number is close to zero
     * @param inExp Binary exponent for "nearness", default is -80 (so check is accurate to 2^-80 closeness)
     */
    inline bool      isNearZero(f64 inNumber, s32 inExp)                 { ASSERTS(inExp<0, "IsNearZero needs an exponent, this typically should be a negative s32"); return bool(exponent(inNumber) <= inExp); }

    /**
     * Check if number is close to another number
     * @param inExp Binary exponent for "nearness", default is -80 (so check is accurate to 2^-80 closeness)
     */
    inline bool      isNear(f64 inNumber, f64 inRef, s32 inExp)          { return isNearZero(inNumber-inRef, inExp); }

    /**
     * Round number to closes integer value
     */
    inline s32        roundToInt(f64 inNumber)                          { return (s32)(inNumber < 0.0 ? (inNumber - 0.5) : (inNumber + 0.5)); }

    /**
     *    Squared function. 
     *    @return This function returns squared number
     */
    inline f64        squared(f64 inNumber)                             { return inNumber*inNumber; }

    /**
     *    @return  This function returns one divided by a number
     */
    inline f64        oneOver(f64 inNumber)                             { return 1.0 / inNumber; }

    /**
     *    Trick routines. Optimized for doubles and sometimes for doubles
     *    Check if a number is zero
     */
    inline bool      isZero(f64 inNumber)
    {
        //ASSERT((-0.0 == 0.0) && (+0.0 == 0.0));
        return bool(inNumber == 0.0);
    }

    /**
     *    Compare routines. Like is equal etc.  
     *    Check if a number is positive
     */
    inline bool      isPositive(f64 inNumber) 
    {
        return bool(inNumber > 0.0);
    }

    /**
     * Check if a number is negative
     */
    inline bool      isNegative(f64 inNumber)
    {
        return bool(inNumber < 0.0);
    }
};
