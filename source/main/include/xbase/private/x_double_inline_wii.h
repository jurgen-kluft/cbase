namespace x_f64u
{   
	/**
     * Function IsLessPositive. 
	 * @return xTRUE  If the first value is less than the second. Both values MUST be positive.
     */
    inline xbool      isLessPositive(f64 inLHS, f64 inRHS) 
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));

        return xbool(inLHS < inRHS);

    }

    /**
     * Function IsGreater. 
     * @return xTRUE   If the first value is more than the second. Both values MUST be positive.
	 */
    inline xbool      isGreater(f64 inLHS, f64 inRHS) 
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));

        return xbool(inLHS > inRHS);

    }
};