namespace x_f32u
{
    inline s32        toS32(f32 inNumber)
    {
#ifdef TARGET_32BIT
        s32 retv;
        __asm {
            fld inNumber
            fistp retv
        }
        return retv;
#else
		return (s32)inNumber;
#endif
    }

    inline u32        toU32(f32 inNumber)
    { 
#ifdef TARGET_32BIT
        u32 retv;
        __asm {
            fld inNumber
            fistp retv
        }
        return retv;
#else
		return (s32)inNumber;
#endif
	}

	/**
     * Function IsLessPositive. 
	 * @return xTRUE  If the first value is less than the second. Both values MUST be positive.
     */
    inline bool    isLessPositive(f32 inLHS, f32 inRHS) 
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));
        return bool(bin(inLHS) < bin(inRHS));
    }

    /**
     * Function IsGreater. 
     * @return xTRUE   If the first value is more than the second. Both values MUST be positive.
	 */
    inline bool    isGreater(f32 inLHS, f32 inRHS) 
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));

        return bool(bin(inLHS) > bin(inRHS));
    }
};