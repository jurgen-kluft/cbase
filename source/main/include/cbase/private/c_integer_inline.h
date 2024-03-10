namespace ncore
{
    namespace math
    {
        template <class T>
        inline T min(T integerA, T integerB)
        {
            return integerA <= integerB ? integerA : integerB;
        }
        template <class T>
        inline T min(T integerA, T integerB, T integerC)
        {
            return min(integerC, min(integerA, integerB));
        }
        template <>
        inline s32 min(s32 integerA, s32 integerB)
        {
            return integerA <= integerB ? integerA : integerB;
        }
        template <>
        inline s32 min(s32 integerA, s32 integerB, s32 integerC)
        {
            return min(integerC, min(integerA, integerB));
        }

        template <class T>
        inline T max(T integerA, T integerB)
        {
            return integerA >= integerB ? integerA : integerB;
        }
        template <>
        inline s32 max(s32 integerA, s32 integerB)
        {
            s32 r = integerA ^ ((integerA ^ integerB) & -(integerA < integerB));
            return r;
        }

        // Function template, integerA = smallest value, integerB = biggest value
        template <class T>
        inline void sort(T& integerA, T& integerB)
        {
            if (integerA > integerB)
            {
                T t      = integerA;
                integerA = integerB;
                integerA = t;
            }
        }

        // specialized template for s32
        template <>
        inline void sort(s32& X, s32& Y)
        {
            if (X > Y)
            {
                X = X ^ Y;
                Y = Y ^ X;
                X = X ^ Y;
            }
        }

        // Function template, returns the clamp value
        template <class T>
        inline T clamp(T value, T low, T high)
        {
            ASSERTS(low <= high, "Error: low > high");
            return value < low ? low : (value > high ? high : value);
        }

        // Return the aligned value of value
        // Same as alignUp
        inline u32 align(u32 value, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == true, "Error: alignment value should be a power of 2");
            return (value) & (~(alignment - 1));
        }

        // Return the floor based aligned value of value
        inline u32 alignDown(u32 value, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == true, "Error: alignment value should be a power of 2");
            return (value) & (~(alignment - 1));
        }

        // Return the ceiling based aligned value of value
        inline s32 alignUp(s32 value, s32 alignment)
        {
            ASSERTS(ispo2(alignment) == true, "Error: alignment value should be a power of 2");
            return ((value + (alignment - 1)) & (~(alignment - 1)));
        }

        // Return the ceiling based aligned value of value
        inline u32 alignUp(u32 value, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == true, "Error: alignment value should be a power of 2");
            return ((value + (alignment - 1)) & (~(alignment - 1)));
        }

        // Return the ceiling based aligned value of value
        inline u64 alignUp(u64 value, u64 alignment)
        {
            ASSERTS(ispo2(alignment) == true, "Error: alignment value should be a power of 2");
            return ((value + (alignment - 1)) & (~(alignment - 1)));
        }

        // Check if value is aligned
        inline bool isAligned(s32 value, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == true, "Error: alignment value should be a power of 2");
            return value == ((value) & (~(alignment - 1)));
        }

        inline bool isAligned(u32 value, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == true, "Error: alignment value should be a power of 2");
            return value == ((value) & (~(alignment - 1)));
        }

        inline bool isAligned(s64 value, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == true, "Error: alignment value should be a power of 2");
            return value == ((value) & (~((u64)alignment - 1)));
        }

        inline bool isAligned(u64 value, u32 alignment)
        {
            ASSERTS(ispo2(alignment) == true, "Error: alignment value should be a power of 2");
            return value == ((value) & (~((u64)alignment - 1)));
        }

        // Return the absolute value of value
        inline s32 abs(s32 value)
        {
            if (value > 0)
                return value;
            else
                return -value;
        }

        // Return the negated value of value
        // value:		Input value to negate if inNegate is false.
        // inDoNotNegate : Flag indicating we should not negate value.
        inline s32 neg(s32 value, bool inDoNotNegate)
        {
            s32 r = ((s32)inDoNotNegate ^ ((s32)inDoNotNegate - 1)) * value;
            return r;
        }

        // Return the square of value
        inline s32 sqr(s32 value) { return value * value; }

        // Return the unsigned square of value
        inline u32 sqr(u32 value) { return value * value; }

        // Return the modulo of value using inModuloValue
        inline s32 mod(s32 value, s32 inModuloValue) { return value % inModuloValue; }

        // Greatest common denominator (biggest modulo value of both integers)
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

        // Calculate average of A & B without overflow or s64 use
        inline s32 average(s32 inIntegerA, s32 inIntegerB) { return (inIntegerA & inIntegerB) + ((inIntegerA ^ inIntegerB) >> 1); }

        // Check if value is a power-of-two
        inline bool ispo2(s32 value) { return (value != 0) && (((value) & (value - 1)) == 0); }
        inline bool ispo2(u32 value) { return (value != 0) && (((value) & (value - 1)) == 0); }
        inline bool ispo2(u64 value) { return (value != 0) && (((value) & (value - 1)) == 0); }

        // Return the power-of-two larger than or equal to value
        inline u32 ceilpo2(u32 value) { return 1 << (32 - countLeadingZeros(value - 1)); }
        // Return the power-of-two smaller than or equal to value
        inline u32 floorpo2(u32 value) { return 1 << (31 - countLeadingZeros(value)); }

        // Reverse bits in 32 bit word
        inline u32 bitReverse(u32 value)
        {
            u32 n = value;
            n     = ((n >> 1) & 0x55555555) | ((n << 1) & 0xaaaaaaaa);
            n     = ((n >> 2) & 0x33333333) | ((n << 2) & 0xcccccccc);
            n     = ((n >> 4) & 0x0f0f0f0f) | ((n << 4) & 0xf0f0f0f0);
            n     = ((n >> 8) & 0x00ff00ff) | ((n << 8) & 0xff00ff00);
            n     = ((n >> 16) & 0x0000ffff) | ((n << 16) & 0xffff0000);
            return n;
        }

        inline s8 ilog2(u32 value)
        {
            value |= value >> 1;
            value |= value >> 2;    
            value |= value >> 4;
            value |= value >> 8;
            value |= value >> 16;
            return countBits(value) - 1;
        }

        inline s8 ilog2(u64 value)
        {
            value |= value >> 1;
            value |= value >> 2;
            value |= value >> 4;
            value |= value >> 8;
            value |= value >> 16;
            value |= value >> 32;
            return countBits(value) - 1;
        }

        // Roll all the bits in value to the left by shift number of bits
        inline u32 rol32(u32 value, u32 shift)
        {
            shift = shift & 31;
            return (value << shift) | (value >> (32 - shift));
        }

        // Roll all the bits in value to the right by shift number of bits
        inline u32 ror32(u32 value, u32 shift)
        {
            shift = shift & 31;
            return (value >> shift) | (value << (32 - shift));
        }

        template <class T>
        inline T align(T addr, u32 alignTo)
        {
            return T(((addr) & (~(alignTo - 1))));
        }
        template <class T>
        inline T align_floor(T addr, u32 alignTo)
        {
            return T(((((addr))) & (~(alignTo - 1))));
        }
        template <class T>
        inline bool aligned(T addr, u32 alignTo)
        {
            return bool(((addr) & (alignTo - 1)) == 0);
        }
    }  // namespace math
}  // namespace ncore
