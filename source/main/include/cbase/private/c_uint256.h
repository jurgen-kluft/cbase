#ifndef __CBASE_256_BIT_UNSIGNED_INTEGER_H__
#define __CBASE_256_BIT_UNSIGNED_INTEGER_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifndef D_NO_CUSTOM_UINT256

namespace ncore
{
    // Forward declares
    class s128;
    class u128;
    class s256;


    class u256
    {
        enum ESettings
        {
            NUM_BITS = 256,
            NUM_INT8 = NUM_BITS / 8,
            NUM_INT16 = NUM_BITS / 16,
            NUM_INT32 = NUM_BITS / 32,
            NUM_UINT8 = NUM_BITS / 8,
            NUM_UINT16 = NUM_BITS / 16,
            NUM_UINT32 = NUM_BITS / 32,
        };

    public:
                                /** 
                                 *  Constructors
                                 */
                                u256();
        explicit        u256(s8 value);
        explicit        u256(u8 value);
        explicit        u256(s16 value);
        explicit        u256(u16 value);
        explicit        u256(s32 value);
        explicit        u256(u32 value);
        explicit        u256(const s64& value);
        explicit        u256(const u64& value);
        explicit        u256(const s128& value);
        explicit        u256(const u128& value);
                        u256(const s256& value);
                        u256(const u256& value);
                        u256(u32 b255_224, u32 b223_192, u32 b191_160, u32 b159_128, u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0);

        u256&        operator=(const s256& value);
        u256&        operator=(const u256& value);

        u256&       operator++();
        u256&       operator--();
        u256        operator++(s32)                      { u256 r(*this); r++; return r; }
        u256        operator--(s32)                      { u256 r(*this); r--; return r; }
        u256&       operator*=(const u256& value);
        u256&       operator/=(const u256& value);
        u256&       operator+=(const u256& value);
        u256&       operator-=(const u256& value);
        u256&       operator%=(const u256& value);
        u256        operator~() const;

        u256        operator>>(s32 nShift) const;
        u256        operator<<(s32 nShift) const;
        u256&       operator>>=(s32 nShift);
        u256&       operator<<=(s32 nShift);
        u256&       operator^=(const u256& value);
        u256&       operator|=(const u256& value);
        u256&       operator&=(const u256& value);

                                operator s32() const;
                                operator u32() const;
                                operator s64() const;
                                operator u64() const;

        inline u32&    operator [] (s32 index)                    { return mUInt32[index]; }
        inline u32     operator [] (s32 index) const              { return mUInt32[index]; }

        void           SetBit(s32 nIndex, bool value);
        bool           IsBitSet(s32 nIndex) const;

        void           Modulus(const u256& Divisor, u256& Quotient, u256& Remainder) const;
        s32            Compare(const u256& other) const;

        static const u256  Zero;
        static const u256  One;

    protected:
        friend class s256;

        union
        {
            struct
            {
                u32        m_MSB;
                u32        m_FSB;
                u32        m_ESB;
                u32        m_DSB;
                u32        m_CSB;
                u32        m_BSB;
                u32        m_ASB;
                u32        m_LSB;
            };
            s8         mInt8[NUM_INT8];
            s16        mInt16[NUM_INT16];
            s32        mInt32[NUM_INT32];

            u8         mUInt8[NUM_UINT8];
            u16        mUInt16[NUM_UINT16];
            u32        mUInt32[NUM_UINT32];
        };
    };


    inline u256      operator+(const u256& lhs, const u256& rhs) { u256 r(lhs); r+=rhs; return r; }
    inline u256      operator-(const u256& lhs, const u256& rhs) { u256 r(lhs); r-=rhs; return r; }
    inline u256      operator*(const u256& lhs, const u256& rhs) { u256 r(lhs); r*=rhs; return r; }
    inline u256      operator/(const u256& lhs, const u256& rhs) { u256 r(lhs); r/=rhs; return r; }
    inline u256      operator%(const u256& lhs, const u256& rhs) { u256 r(lhs); r%=rhs; return r; }
    inline u256      operator^(const u256& lhs, const u256& rhs) { u256 r(lhs); r^=rhs; return r; }
    inline u256      operator|(const u256& lhs, const u256& rhs) { u256 r(lhs); r|=rhs; return r; }
    inline u256      operator&(const u256& lhs, const u256& rhs) { u256 r(lhs); r&=rhs; return r; }

    inline u256      operator+(const u256& lhs, s32 rhs) { u256 r(lhs); r+=u256(rhs); return r; }
    inline u256      operator-(const u256& lhs, s32 rhs) { u256 r(lhs); r-=u256(rhs); return r; }
    inline u256      operator*(const u256& lhs, s32 rhs) { u256 r(lhs); r*=u256(rhs); return r; }
    inline u256      operator/(const u256& lhs, s32 rhs) { u256 r(lhs); r/=u256(rhs); return r; }
    inline u256      operator%(const u256& lhs, s32 rhs) { u256 r(lhs); r%=u256(rhs); return r; }
    inline u256      operator^(const u256& lhs, s32 rhs) { u256 r(lhs); r^=u256(rhs); return r; }
    inline u256      operator|(const u256& lhs, s32 rhs) { u256 r(lhs); r|=u256(rhs); return r; }
    inline u256      operator&(const u256& lhs, s32 rhs) { u256 r(lhs); r&=u256(rhs); return r; }

    inline bool      operator<(const u256& lhs, const u256& rhs) { return bool(lhs.Compare(rhs) == -1); }
    inline bool      operator>(const u256& lhs, const u256& rhs) { return bool(lhs.Compare(rhs) == 1); }
    inline bool      operator<=(const u256& lhs, const u256& rhs) { return bool(lhs.Compare(rhs) != 1); }
    inline bool      operator>=(const u256& lhs, const u256& rhs) { return bool(lhs.Compare(rhs) != -1); }
    inline bool      operator==(const u256& lhs, const u256& rhs) { return bool(lhs.Compare(rhs) == 0); }
    inline bool      operator!=(const u256& lhs, const u256& rhs) { return bool(lhs.Compare(rhs) != 0); }

};

#endif
#endif  ///< __CBASE_256_BIT_UNSIGNED_INTEGER_H__
