namespace nf32
{
    inline s32 toS32(f32 inNumber)
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

    inline u32 toU32(f32 inNumber)
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

    inline bool isLessPositive(f32 inLHS, f32 inRHS)
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));
        return bool(bin(inLHS) < bin(inRHS));
    }

    inline bool isGreater(f32 inLHS, f32 inRHS)
    {
        ASSERT(isPositive(inLHS) && isPositive(inRHS));
        return bool(bin(inLHS) > bin(inRHS));
    }
}; // namespace nf32