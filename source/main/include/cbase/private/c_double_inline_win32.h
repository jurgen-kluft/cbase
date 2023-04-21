namespace math
{
	/**
     * Function IsLessPositive. 
	 * @return True  If the first value is less than the second. Both values MUST be positive.
     */
    inline bool      isLessPositive(f64 inLHS, f64 inRHS) 
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));

        return bool(bin(inLHS) < bin(inRHS));

    }

    /**
     * Function IsGreater. 
     * @return True   If the first value is more than the second. Both values MUST be positive.
	 */
    inline bool      isGreater(f64 inLHS, f64 inRHS) 
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));

        return bool(bin(inLHS) > bin(inRHS));

    }
};