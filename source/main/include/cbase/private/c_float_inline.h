/**
 * IEEE 754 description
 */
enum EFloatIEEE
{
    FF_FRACTION_MASK      = 0x007FFFFF,
    FF_GET_FRACTION_SHIFT = 0,
    FF_FRACTION_BITS      = 23,
    FF_EXPONENT_MASK      = 0x7F800000,
    FF_GET_EXPONENT_SHIFT = 23,
    FF_EXPONENT_BITS      = 8,
    FF_SIGN_MASK          = 0x80000000,
    FF_GET_SIGN_SHIFT     = 31,
    FF_SIGN_BITS          = 1,
    FF_EXPONENT_BIAS      = 127
};

namespace math
{
    inline f32 abs(f32 inValue) { return (inValue < 0.0f) ? -inValue : inValue; }
    inline f32 neg(f32 inNumber) { return -inNumber; }
    inline f32 fmin(f32 numberA, f32 numberB) { return numberA <= numberB ? numberA : numberB; }
    inline f32 fmax(f32 numberA, f32 numberB) { return numberA >= numberB ? numberA : numberB; }
    inline f32 clamp(f32 number, f32 low, f32 high)
    {
        ASSERTS(low <= high, "Error: low > high");
        return number < low ? low : (number > high ? high : number);
    }

    // float power function
    inline f32 powf(f32 x, s32 p)
    {
        if (p == 0)
            return 1.0f;
        if (x == 0.0f)
            return 0.0f;

        if (p < 0)
        {
            p = -p;
            x = 1.0f / x;
        }

        f32 r = 1.0f;
        while (true)
        {
            if (p & 1)
                r *= x;

            if ((p >>= 1) == 0)
                return r;

            x *= x;
        }
        return r;
    }

    inline f32 positiveZero()
    {
        u32 f = (u32)0;
        return toF32(f);
    }
    inline f32 negativeZero()
    {
        u32 f = (u32)FF_SIGN_MASK;
        return toF32(f);
    }
#ifdef D_IEEE_FLOATS
    inline f32 nan()
    {
        u32 f = (u32)(FF_EXPONENT_MASK | 1);
        return toF32(f);
    }
    inline f32 positiveInfinity()
    {
        u32 f = (u32)(FF_EXPONENT_MASK);
        return toF32(f);
    }
    inline f32 negativeInfinity()
    {
        u32 f = (u32)(FF_SIGN_MASK | FF_EXPONENT_MASK);
        return toF32(f);
    }
#endif
    inline f32 positiveMaximum()
    {
        u32 f = (u32)((((1 << FF_EXPONENT_BITS) - 2) << FF_GET_EXPONENT_SHIFT) | FF_FRACTION_MASK);
        return toF32(f);
    }
    inline f32 negativeMaximum()
    {
        u32 f = (u32)((FF_SIGN_MASK | (((1 << FF_EXPONENT_BITS) - 2) << FF_GET_EXPONENT_SHIFT)) | FF_FRACTION_MASK);
        return toF32(f);
    }

    /**
     * De-normalized (DEN) positive and negative minimums.
     */
    inline f32 positiveMinimum()
    {
        u32 f = (u32)(1 << (FF_FRACTION_BITS - 1));
        return toF32(f);
    }
    inline f32 negativeMinimum()
    {
        u32 f = (u32)(FF_SIGN_MASK | (1 << (FF_FRACTION_BITS - 1)));
        return toF32(f);
    }

    /**
     * Rational Determination.
     */
    inline bool isPositiveZero(f32 inNumber) { return bool((bin(inNumber) & (FF_EXPONENT_MASK | FF_FRACTION_MASK | FF_SIGN_MASK)) == 0); }
    inline bool isNegativeZero(f32 inNumber) { return bool((bin(inNumber) & (FF_EXPONENT_MASK | FF_FRACTION_MASK | FF_SIGN_MASK)) == FF_SIGN_MASK); }
#ifdef D_IEEE_FLOATS
    inline bool isPositiveInfinite(f32 inNumber) { return bool((bin(inNumber) & (FF_EXPONENT_MASK | FF_FRACTION_MASK | FF_SIGN_MASK)) == FF_EXPONENT_MASK); }
    inline bool isNegativeInfinite(f32 inNumber) { return bool((bin(inNumber) & (FF_EXPONENT_MASK | FF_FRACTION_MASK | FF_SIGN_MASK)) == (FF_EXPONENT_MASK | FF_SIGN_MASK)); }

    inline bool isInfinite(f32 inNumber) { return bool((bin(inNumber) & (FF_EXPONENT_MASK | FF_FRACTION_MASK)) == FF_EXPONENT_MASK); }
    inline bool isNAN(f32 inNumber) { return bool(((bin(inNumber) & FF_EXPONENT_MASK) == FF_EXPONENT_MASK) && (bin(inNumber) & FF_FRACTION_MASK)); }
    inline bool isFinite(f32 inNumber) { return bool(!isInfinite(inNumber) && !isNAN(inNumber)); }
    inline bool isRational(f32 inNumber) { return bool((bin(inNumber) & FF_EXPONENT_MASK) != FF_EXPONENT_MASK); }
#else
    inline bool isPositiveInfinite(f32 inNumber) { return False; }
    inline bool isNegativeInfinite(f32 inNumber) { return False; }

    inline bool isInfinite(f32 inNumber) { return False; }
    inline bool isNAN(f32 inNumber) { return False; }
    inline bool isFinite(f32 inNumber) { return True; }
    inline bool isRational(f32 inNumber) { return True; }
#endif
    /**
     * bin. Returns the f32 as an u32. It's not casted, but the address is casted, so you get the binary representation.
     */
    inline u32 bin(f32 inNumber) { return *(u32*)&inNumber; }

    /**
     * sbin. Returns the f32 as an s32. Used for signed operations like shifting down.
     */
    inline s32 sbin(f32 inNumber) { return *(s32*)&inNumber; }

    /**
     * toF32. Returns the u32 as as f32. It's not casted, but the address is, so you get the binary representation back.
     */
    inline f32 toF32(u32 inInt) { return *(f32*)&inInt; }

    /**
     * IsEqual, returns True if two floats are the same
     */
    inline bool isEqual(f32 inNumber, f32 inG) { return bool(bin(inNumber) == bin(inG)); }

    /**
     * IsNotEqual, returns True if the two floats are not the same
     */
    inline bool isNotEqual(f32 inNumber, f32 inG) { return bool(bin(inNumber) != bin(inG)); }

    /**
     * And. To 'And' a f32 with a binary value. Used for abs for example
     */
    inline f32 binaryAnd(f32 inNumber, u32 inB) { return toF32(bin(inNumber) & inB); }

    /**
     * Or. To 'or' a f32 with a binary value
     */
    inline f32 binaryOr(f32 inNumber, u32 inB) { return toF32(bin(inNumber) | inB); }

    /**
     * Fraction. Returns the Fractional part of the floating point value
     */
    inline s32 fraction(f32 inNumber) { return bin(inNumber) & FF_FRACTION_MASK; }

    /**
     * ExponentBinary. Returns the Exponent part of the floating point value. This value is biased
     */
    inline s32 exponentBinary(f32 inNumber) { return (bin(inNumber) & FF_EXPONENT_MASK) >> FF_GET_EXPONENT_SHIFT; }

    /**
     * Exponent. Returns the Exponent part of the floating point value, without the bias
     */
    inline s32 exponent(f32 f) { return exponentBinary(f) - FF_EXPONENT_BIAS; }

    /**
     * SignMask. Returns the floating point value masked with the sign mask.
     */
    inline u32 signMask(f32 inNumber) { return bin(inNumber) & FF_SIGN_MASK; }

    /**
     * SignBit. Returns the Sign bit (either 0=positive or 1=negative)
     */
    inline s32 signBit(f32 inNumber) { return bin(inNumber) >> FF_GET_SIGN_SHIFT; }

    /**
     * SignBitSigned. Returns the Sign bit expanded in the whole s32: 0 = positive -1 = negative
     */
    inline s32 signBitSigned(f32 inNumber) { return sbin(inNumber) >> FF_GET_SIGN_SHIFT; }

    /**
     * Sign returns -1 if the value is negative, 0 if the value is zero (0 or -0) and +1 if it's positive
     */
    inline s32 sign(f32 inNumber)
    {
        if (bin(inNumber) & ~FF_SIGN_MASK)
            return 1 | signBitSigned(inNumber);
        else
            return 0;
    }

    /**
     * Sign with a little delta. If the f32 value is greater than the delta, it returns +1.
     * @retval -1   If it's less than -delta it returns -1.
     * @retval 0    If the value is between -delta and +delta it returns 0.
     */
    inline s32 sign(f32 inNumber, f32 inDelta)
    {
        ASSERT(inDelta >= 0.0f);
        return abs(inNumber) <= inDelta ? 0 : sign(inNumber);
    }

    /**
    Check if a number is close to zero
    @param    inExp Binary exponent for "nearness", default is -80 (so check is accurate to 2^-80 closeness)
    **/
    inline bool isNearZero(f32 inNumber, s32 inExp)
    {
        ASSERTS(inExp < 0, "IsNearZero needs an exponent, this typically should be a negative s32");
        return bool(exponent(inNumber) <= inExp);
    }

    /**
     * Check if number is close to another number
     * @param inExp Binary exponent for "nearness", default is -80 (so check is accurate to 2^-80 closeness)
     */
    inline bool isNear(f32 inNumber, f32 inRef, s32 inExp) { return isNearZero(inNumber - inRef, inExp); }

    /**
     * Round number to closes integer value
     */
    inline s32 roundToInt(f32 inNumber) { return (s32)(inNumber < 0.0f ? (inNumber - 0.5f) : (inNumber + 0.5f)); }

    /**
     *    Squared function.
     *    @return This function returns squared number
     */
    inline f32 squared(f32 inNumber) { return inNumber * inNumber; }

    /**
     *    @return  This function returns one divided by a number
     */
    inline f32 oneOver(f32 inNumber) { return 1.0f / inNumber; }

    /**
     *    Trick routines. Optimized for floats and sometimes for doubles
     */

    /**
     * Fast conversion from a IEEE 32-bit floating point number F in [0,1] to a
     * a 32-bit integer I in [0,2^L-1].
     *
     *   inFloat = F
     *   inLog = L
     *   localInt = I
     */

    inline s32 scaledFloatToInt(f32 inFloat, s32 inLog)
    {
        ASSERT((inFloat >= 0) && (inFloat <= 1));
        s32 localInt;
        s32 localShift = 150 - inLog - ((*(s32*)(&inFloat) >> 23) & 0xFF);
        if (localShift < 24)
        {
            localInt = ((*(s32*)(&inFloat) & 0x007FFFFF) | 0x00800000) >> localShift;
            if (localInt == (1 << inLog))
            {
                localInt--;
            }
        }
        else
        {
            localInt = 0;
        }
        return localInt;
    }

    /**
     * get smallest f32 for which value > <inFloat>
     */
    inline f32 minimumIncrement(f32 inFloat)
    {
        u32 exponent = exponentBinary(inFloat) - FF_FRACTION_BITS;

        if (exponent > FF_EXPONENT_MASK >> FF_GET_EXPONENT_SHIFT)
            exponent = FF_EXPONENT_MASK >> FF_GET_EXPONENT_SHIFT;

        f32 delta = toF32(exponent << FF_GET_EXPONENT_SHIFT);
        ASSERT(delta > 0);
        ASSERT(inFloat + delta > inFloat);
        return inFloat + delta;
    }

    inline bool isZero(f32 inNumber) { return bool(inNumber == 0.0f); }
    inline bool isPositive(f32 inNumber) { return bool(inNumber > 0.0f); }
    inline bool isNegative(f32 inNumber) { return bool(inNumber < 0.0f); }
}; // namespace nf32
