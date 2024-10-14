/**
 * IEEE 754 description
 */
namespace math
{
    namespace DoubleIEEE
    {
#define DD_FRACTION_MASK      (0x000FFFFFFFFFFFFFULL)
#define DD_EXPONENT_MASK      (0x7FF0000000000000ULL)
#define DD_SIGN_MASK          (0x8000000000000000ULL)
#define DD_EXPONENT_BIAS      ((1ULL << (11 - 1)) - 1)
#define DD_MAX_EXPONENT_BIAS  ((1ULL << 11) - 1)
#define DD_GET_FRACTION_SHIFT (0ULL)
#define DD_GET_EXPONENT_SHIFT (52ULL)
#define DD_GET_SIGN_SHIFT     (63ULL)
#define DD_FRACTION_BITS      (52ULL)
#define DD_EXPONENT_BITS      (11ULL)
#define DD_SIGN_BITS          (1ULL)
    };  // namespace DoubleIEEE

    /**
     * @return  This funtion returns a negated number
     */
    inline f64 abs(f64 inNumber) { return inNumber < 0.0 ? -inNumber : inNumber; }

    /**
     * @return  This funtion returns a negated number
     */
    inline f64 neg(f64 inNumber) { return -inNumber; }

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
        return r;
    }

    /**
     * Rational Determination.
     */
#ifdef D_IEEE_FLOATS
    inline bool isInfinite(f64 inNumber) { return bool((bin(inNumber) & (DD_EXPONENT_MASK | DD_FRACTION_MASK)) == DD_EXPONENT_MASK); }
    inline bool isNAN(f64 inNumber) { return bool(((bin(inNumber) & DD_EXPONENT_MASK) == DD_EXPONENT_MASK) && (bin(inNumber) & DD_FRACTION_MASK)); }
    inline bool isFinite(f64 inNumber) { return bool(!isInfinite(inNumber) && !isNAN(inNumber)); }
    inline bool isRational(f64 inNumber) { return bool((bin(inNumber) & DD_EXPONENT_MASK) != DD_EXPONENT_MASK); }
#else
    inline bool isInfinite(f64 inNumber) { return false; }
    inline bool isNAN(f64 inNumber) { return false; }
    inline bool isFinite(f64 inNumber) { return true; }
    inline bool isRational(f64 inNumber) { return true; }
#endif
    /**
     * bin. Returns the f64 as an u64. It's not casted, but the address is casted, so you get the binary representation
     */
    inline u64 bin(f64 inNumber) { return *(u64*)&inNumber; }

    /**
     * sbin. Returns the f64 as an s64. Used for signed operations like shifting down
     */
    inline s64 sbin(f64 inNumber) { return *(s64*)&inNumber; }

    /**
     * Float. Returns the u64 as a f64. It's not casted, but the address is, so you get the binary representation back.
     */
    inline f64 toF64(u64 inInt) { return *(f64*)&inInt; }

    inline s64 toS64(f64 inB) { return (s64)inB; }
    inline u64 toU64(f64 inB) { return (u64)(s64)inB; }

    /**
     * IsEqual, returns true if two doubles are the same
     */
    inline bool isEqual(f64 inNumber, f64 inG) { return bool(bin(inNumber) == bin(inG)); }

    /**
     * IsNotEqual, returns true if the two doubles are not the same
     */
    inline bool isNotEqual(f64 inNumber, f64 inG) { return bool(bin(inNumber) != bin(inG)); }

    /**
     * And. To 'And' a f64 with a binary value. Used for abs for example
     */
    inline f64 binaryAnd(f64 inNumber, u64 inB) { return toF64(bin(inNumber) & inB); }

    /**
     * Or. To 'or' a f64 with a binary value
     */
    inline f64 binaryOr(f64 inNumber, u64 inB) { return toF64(bin(inNumber) | inB); }

    /**
     * Fraction. Returns the Fractional part of the f64 point value
     */
    inline s64 fraction(f64 inNumber) { return bin(inNumber) & DD_FRACTION_MASK; }

    /**
     * ExponentBinary. Returns the Exponent part of the f64 point value. This value is biased
     */
    inline s32 exponentBinary(f64 inNumber) { return (s32)((bin(inNumber) & DD_EXPONENT_MASK) >> DD_GET_EXPONENT_SHIFT); }

    /**
     * Exponent. Returns the Exponent part of the f64 point value, without the bias
     */
    inline s32 exponent(f64 f) { return exponentBinary(f) - (s32)DD_EXPONENT_BIAS; }

    /**
     * SignMask. Returns the f64 point value masked with the sign mask.
     */
    inline u64 signMask(f64 inNumber) { return bin(inNumber) & DD_SIGN_MASK; }

    /**
     * SignBit. Returns the Sign bit (either 0 or 1)
     */
    inline s32 signBit(f64 inNumber) { return (s32)(bin(inNumber) >> DD_GET_SIGN_SHIFT); }

    /**
     * SignBitSigned. Returns the Sign bit expanded in the whole s32: 0 = positive -1 = negative
     */
    inline s32 signBitSigned(f64 inNumber) { return (s32)(sbin(inNumber) >> DD_GET_SIGN_SHIFT); }

    /**
     * Sign returns -1 if the value is negative, 0 if the value is zero (0 or -0) and +1 if it's positive
     */
    inline s32 sign(f64 inNumber)
    {
        if (bin(inNumber) & ~DD_SIGN_MASK)
            return 1 | signBitSigned(inNumber);
        else
            return 0;
    }

    /**
     * Sign with a little delta. If the f64 value is greater than the delta, it returns +1.
     * @retval -1   If it's less than -delta it returns -1.
     * @retval 0    If the value is between -delta and +delta it returns 0.
     */
    inline s32 sign(f64 inNumber, f64 inDelta)
    {
        ASSERT(inDelta >= 0.0);
        return abs(inNumber) <= inDelta ? 0 : sign(inNumber);
    }

    /**
     * Check if a number is close to zero
     * @param inExp Binary exponent for "nearness", default is -80 (so check is accurate to 2^-80 closeness)
     */
    inline bool isNearZero(f64 inNumber, s32 inExp)
    {
        ASSERTS(inExp < 0, "IsNearZero needs an exponent, this typically should be a negative s32");
        return bool(exponent(inNumber) <= inExp);
    }

    /**
     * Check if number is close to another number
     * @param inExp Binary exponent for "nearness", default is -80 (so check is accurate to 2^-80 closeness)
     */
    inline bool isNear(f64 inNumber, f64 inRef, s32 inExp) { return isNearZero(inNumber - inRef, inExp); }

    /**
     * Round number to closes integer value
     */
    inline s32 roundToInt(f64 inNumber) { return (s32)(inNumber < 0.0 ? (inNumber - 0.5) : (inNumber + 0.5)); }

    /**
     *    Squared function.
     *    @return This function returns squared number
     */
    inline f64 squared(f64 inNumber) { return inNumber * inNumber; }

    /**
     *    @return  This function returns one divided by a number
     */
    inline f64 oneOver(f64 inNumber) { return 1.0 / inNumber; }

    /**
     *    Trick routines. Optimized for doubles and sometimes for doubles
     *    Check if a number is zero
     */
    inline bool isZero(f64 inNumber)
    {
        // ASSERT((-0.0 == 0.0) && (+0.0 == 0.0));
        return bool(inNumber == 0.0);
    }

    /**
     *    Compare routines. Like is equal etc.
     *    Check if a number is positive
     */
    inline bool isPositive(f64 inNumber) { return bool(inNumber > 0.0); }

    /**
     * Check if a number is negative
     */
    inline bool isNegative(f64 inNumber) { return bool(inNumber < 0.0); }
};  // namespace math
