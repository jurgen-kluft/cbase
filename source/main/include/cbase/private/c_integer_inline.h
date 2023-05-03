namespace ncore
{
    namespace math
    {
        template <class T> inline T min(T integerA, T integerB) { return integerA <= integerB ? integerA : integerB; }
        template <class T> inline T min(T integerA, T integerB, T integerC) { return min(integerC, min(integerA, integerB)); }
        template <> inline s32      min(s32 integerA, s32 integerB) { return integerA <= integerB ? integerA : integerB; }
        template <> inline s32      min(s32 integerA, s32 integerB, s32 integerC) { return min(integerC, min(integerA, integerB)); }

        template <class T> inline T max(T integerA, T integerB) { return integerA >= integerB ? integerA : integerB; }
        template <> inline s32      max(s32 integerA, s32 integerB)
        {
            s32 r = integerA ^ ((integerA ^ integerB) & -(integerA < integerB));
            return r;
        }

        /**
         * Function template, integerA = smallest value, integerB = biggest value
         */
        template <class T> inline void sort(T& integerA, T& integerB)
        {
            if (integerA > integerB)
            {
                T t      = integerA;
                integerA = integerB;
                integerA = t;
            }
        }

        /**
         * specialized template for s32
         */
        template <> inline void sort(s32& X, s32& Y)
        {
            if (X > Y)
            {
                X = X ^ Y;
                Y = Y ^ X;
                X = X ^ Y;
            }
        }

        /**
         * Function template, returns the clamp value
         */
        template <class T> inline T clamp(T integer, T low, T high)
        {
            ASSERTS(low <= high, "Error: low > high");
            return integer < low ? low : (integer > high ? high : integer);
        }

        /**
         * Return the aligned value of integer
         * Same as alignUp
         */
        inline u32 align(u32 integer, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == True, "Error: alignment value should be a power of 2");
            return (integer) & (~(alignment - 1));
        }

        /**
         * Return the floor based aligned value of integer
         */
        inline u32 alignDown(u32 integer, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == True, "Error: alignment value should be a power of 2");
            return (integer) & (~(alignment - 1));
        }

        /**
         * Return the ceiling based aligned value of integer
         */
        inline s32 alignUp(s32 integer, s32 alignment)
        {
            ASSERTS(ispo2(alignment) == True, "Error: alignment value should be a power of 2");
            return ((integer + (alignment - 1)) & (~(alignment - 1)));
        }

        /**
         * Return the ceiling based aligned value of integer
         */
        inline u32 alignUp(u32 integer, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == True, "Error: alignment value should be a power of 2");
            return ((integer + (alignment - 1)) & (~(alignment - 1)));
        }

        /**
         * Return the ceiling based aligned value of integer
         */
        inline u64 alignUp(u64 integer, u64 alignment)
        {
            ASSERTS(ispo2(alignment) == True, "Error: alignment value should be a power of 2");
            return ((integer + (alignment - 1)) & (~(alignment - 1)));
        }

        /**
         * Check if integer is aligned
         */
        inline bool isAligned(u32 integer, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == True, "Error: alignment value should be a power of 2");
            return integer == ((integer) & (~(alignment - 1)));
        }

        inline bool isAligned(u64 integer, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == True, "Error: alignment value should be a power of 2");
            return integer == ((integer) & (~((u64)alignment - 1)));
        }

        /**
         * Return the absolute value of integer
         */
        inline s32 abs(s32 inInteger)
        {
            if (inInteger > 0)
                return inInteger;
            else
                return -inInteger;
        }

        /**
         * Return the negated value of integer
         * inInteger:		Input value to negate if inNegate is false.
         * inDoNotNegate:	Flag indicating we should not negate inInteger.
         */
        inline s32 neg(s32 inInteger, bool inDoNotNegate)
        {
            s32 r = ((s32)inDoNotNegate ^ ((s32)inDoNotNegate - 1)) * inInteger;
            return r;
        }

        /**
         * Return the square of integer
         */
        inline s32 sqr(s32 inInteger) { return inInteger * inInteger; }

        /**
         * Return the unsigned square of integer
         */
        inline u32 sqr(u32 inInteger) { return inInteger * inInteger; }

        /**
         * Return the modulo of integer using inModuloValue
         */
        inline s32 mod(s32 inInteger, s32 inModuloValue) { return inInteger % inModuloValue; }

        /**
         * Greatest common denominator (biggest modulo value of both integers)
         */
        inline s32 gcd(s32 inIntegerA, s32 inIntegerB)
        {
            while (inIntegerB != 0)
            {
                s32 t      = inIntegerA % inIntegerB;
                inIntegerA = inIntegerB;
                inIntegerB = t;
            }
            return inIntegerA;
        }

        /**
         * Calculate average of A & B without overflow or s64 use
         */
        inline s32 average(s32 inIntegerA, s32 inIntegerB) { return (inIntegerA & inIntegerB) + ((inIntegerA ^ inIntegerB) >> 1); }

        /**
         * Check if integer is a power-of-two
         */
        inline bool ispo2(s32 inInteger) { return (inInteger != 0) && (((inInteger) & (inInteger - 1)) == 0); }

        /**
         * Check if integer is a power-of-two
         */
        inline bool ispo2(u32 inInteger) { return (inInteger != 0) && (((inInteger) & (inInteger - 1)) == 0); }

        /**
         * Check if integer is a power-of-two
         */
        inline bool ispo2(u64 inInteger) { return (inInteger != 0) && (((inInteger) & (inInteger - 1)) == 0); }

        /**
         * Return the smallest power-of-two larger than integer
         */
        inline u32 ceilpo2(u32 inInteger)
        {
            ASSERT(inInteger < 0x80000000);
            u32 const w = countLeadingZeros(inInteger);
            u32 const l = 0x80000000 >> w;
            if (l == inInteger)
                return inInteger;
            return l << 1;
        }

        /**
         * Return the biggest power-of-two smaller than integer
         */
        inline u32 floorpo2(u32 inInteger)
        {
            u32 const w = countLeadingZeros(inInteger);
            u32 const l = 0x80000000 >> w;
            if (l == inInteger)
                return inInteger;
            return l;
        }

        /**
         * Reverse bits in 32 bit word
         */
        inline u32 bitReverse(u32 inInteger)
        {
            u32 n = inInteger;
            n     = ((n >> 1) & 0x55555555) | ((n << 1) & 0xaaaaaaaa);
            n     = ((n >> 2) & 0x33333333) | ((n << 2) & 0xcccccccc);
            n     = ((n >> 4) & 0x0f0f0f0f) | ((n << 4) & 0xf0f0f0f0);
            n     = ((n >> 8) & 0x00ff00ff) | ((n << 8) & 0xff00ff00);
            n     = ((n >> 16) & 0x0000ffff) | ((n << 16) & 0xffff0000);
            return n;
        }

        s8 ilog2(u32 inInteger)
        {
            s32 r = 0;
            s32 i = inInteger;
            if (i & 0xFFFF0000)
                i >>= 16, r |= 16;
            if (i & 0xFF00FF00)
                i >>= 8, r |= 8;
            if (i & 0xF0F0F0F0)
                i >>= 4, r |= 4;
            if (i & 0xCCCCCCCC)
                i >>= 2, r |= 2;
            if (i & 0xAAAAAAAA)
                i >>= 1, r |= 1;
            return r;
        }

        inline s8 ilog2(u64 value)
        {
            static constexpr s8 table[64] = {63, 0,  58, 1,  59, 47, 53, 2,  60, 39, 48, 27, 54, 33, 42, 3,  61, 51, 37, 40, 49, 18, 28, 20, 55, 30, 34, 11, 43, 14, 22, 4,
                                             62, 57, 46, 52, 38, 26, 32, 41, 50, 36, 17, 19, 29, 10, 13, 21, 56, 45, 25, 31, 35, 16, 9,  12, 44, 24, 15, 8,  23, 7,  6,  5};
            value |= value >> 1;
            value |= value >> 2;
            value |= value >> 4;
            value |= value >> 8;
            value |= value >> 16;
            value |= value >> 32;
            return table[((value - (value >> 1)) * 0x07EDD5E59A4E28C2) >> 58];
        }

        /**
         * Roll all the bits in integer to the left by shift number of bits
         */
        inline u32 rol32(u32 inInteger, u32 shift)
        {
            shift = shift & 31;
            return (inInteger << shift) | (inInteger >> (32 - shift));
        }

        /**
         * Roll all the bits in integer to the right by shift number of bits
         */
        inline u32 ror32(u32 inInteger, u32 shift)
        {
            shift = shift & 31;
            return (inInteger >> shift) | (inInteger << (32 - shift));
        }

        template <class T> inline T    align(T addr, u32 alignTo) { return T(((addr) & (~(alignTo - 1)))); }
        template <class T> inline T    align_floor(T addr, u32 alignTo) { return T(((((addr))) & (~(alignTo - 1)))); }
        template <class T> inline bool aligned(T addr, u32 alignTo) { return bool(((addr) & (alignTo - 1)) == 0); }
    } // namespace math
} // namespace ncore
