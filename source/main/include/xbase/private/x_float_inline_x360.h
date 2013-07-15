namespace x_f32u
{
    inline s32        toS32(f32 inB)                            { return (s32)inB;  }
    inline u32        toU32(f32 inB)                            { return (u32)(s32)inB; }


    //
    // IsLessPositive. Returns xTRUE if the first value is less than the second. Both values MUST be positive
    //
    inline xbool    isLessPositive(f32 inLHS, f32 inRHS) 
    {
        return inLHS < inRHS;
    }

    //
    // IsGreater. Returns xTRUE if the first value is more than the second. Both values MUST be positive
    //
    inline xbool    isGreater(f32 inLHS, f32 inRHS) 
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));

        return inLHS > inRHS;
    }
};
