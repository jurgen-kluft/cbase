namespace nf32
{
    inline s32 toS32(f32 inB) { return (s32)inB; }
    inline u32 toU32(f32 inB) { return (u32)(s32)inB; }

    inline bool isLessPositive(f32 inLHS, f32 inRHS) { return inLHS < inRHS; }
    inline bool isGreater(f32 inLHS, f32 inRHS)
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));
        return inLHS > inRHS;
    }
}; // namespace nf32