namespace x_f32u
{
    inline s32        toS32(f32 inNumber)
    {
        s32 retv;
        __asm {
            fld inNumber
                fistp retv
        }
        return retv;
    }

    inline u32        toU32(f32 inNumber)
    { 
        u32 retv;
        __asm {
            fld inNumber
                fistp retv
        }
        return retv;
    }

    //
    // IsLessPositive. Returns xTRUE if the first value is less than the second. Both values MUST be positive
    //
    inline xbool    isLessPositive(f32 inLHS, f32 inRHS) 
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));
        return xbool(bin(inLHS) < bin(inRHS));
    }

    //
    // IsGreater. Returns xTRUE if the first value is more than the second. Both values MUST be positive
    //
    inline xbool    isGreater(f32 inLHS, f32 inRHS) 
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));

        return xbool(bin(inLHS) > bin(inRHS));
    }
};