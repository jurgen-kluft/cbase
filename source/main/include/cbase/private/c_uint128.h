#ifndef __CBASE_128_BIT_UNSIGNED_INTEGER_H__
#define __CBASE_128_BIT_UNSIGNED_INTEGER_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifndef D_NO_CUSTOM_UINT128

namespace ncore
{
    // Forward declares
    class s128;

    class u128
    {
        enum ESettings
        {
            NUM_BITS = 128,
            NUM_INT8 = NUM_BITS / 8,
            NUM_INT16 = NUM_BITS / 16,
            NUM_INT32 = NUM_BITS / 32,
            NUM_UINT8 = NUM_BITS / 8,
            NUM_UINT16 = NUM_BITS / 16,
            NUM_UINT32 = NUM_BITS / 32,
        };

        enum EConstants
        {
            MIN,
            MAX,
        };
                    u128(EConstants c);

    public:
                    u128();
        explicit    u128(s16 value);
        explicit    u128(u16 value);
        explicit    u128(s32 value);
        explicit    u128(u32 value);
        explicit    u128(const s64& value);
        explicit    u128(const u64& value);
        explicit    u128(const u64& high, const u64& low);
        explicit    u128(const s128& value);
        explicit    u128(u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0);
                    u128(const u128& value);

        u128&    operator=(s8 value);
        u128&    operator=(u8 value);
        u128&    operator=(s16 value);
        u128&    operator=(u16 value);
        u128&    operator=(s32 value);
        u128&    operator=(u32 value);
        u128&    operator=(const u128& value);

        u128&    operator++();
        u128&    operator--();
        u128     operator++(s32)           { u128 r(*this); ++r; return r; }
        u128     operator--(s32)           { u128 r(*this); --r; return r; }
        u128&    operator*=(const u128& value);
        u128&    operator/=(const u128& value);
        u128&    operator+=(const u128& value);
        u128&    operator-=(const u128& value);
        u128&    operator%=(const u128& value);
        u128     operator~() const;

        u128     operator>>(s32 nShift) const;
        u128     operator<<(s32 nShift) const;
        u128&    operator>>=(s32 nShift);
        u128&    operator<<=(s32 nShift);
        u128&    operator^=(const u128& value);
        u128&    operator|=(const u128& value);
        u128&    operator&=(const u128& value);

        operator s32() const;
        operator u32() const;
        operator s64() const;
        operator u64() const;

        inline u32&   operator [] (s32 index)         { return mUInt32[index]; }
        inline u32    operator [] (s32 index) const   { return mUInt32[index]; }

        void      SetBit(s32 nIndex, bool value);
        bool      IsBitSet(s32 nIndex) const;
        void      Modulus(const u128& Divisor, u128& Quotient, u128& Remainder) const;
        s32       Compare(const u128& other) const;

        static const u128 Zero;
        static const u128 One;
        static const u128 Max;

    protected:
        friend class s128;

        union
        {
            struct
            {
                u32    m_MSB;
                u32    m_DSB;
                u32    m_CSB;
                u32    m_LSB;
            };

            s8     mInt8[NUM_INT8];
            s16    mInt16[NUM_INT16];
            s32    mInt32[NUM_INT32];

            u8     mUInt8[NUM_UINT8];
            u16    mUInt16[NUM_UINT16];
            u32    mUInt32[NUM_UINT32];
        };
    };

    inline u128  operator+(const u128& lhs, const u128& rhs) { u128 r(lhs); r+=rhs; return r; }
    inline u128  operator-(const u128& lhs, const u128& rhs) { u128 r(lhs); r-=rhs; return r; }
    inline u128  operator*(const u128& lhs, const u128& rhs) { u128 r(lhs); r*=rhs; return r; }
    inline u128  operator/(const u128& lhs, const u128& rhs) { u128 r(lhs); r/=rhs; return r; }
    inline u128  operator%(const u128& lhs, const u128& rhs) { u128 r(lhs); r%=rhs; return r; }
    inline u128  operator^(const u128& lhs, const u128& rhs) { u128 r(lhs); r^=rhs; return r; }
    inline u128  operator|(const u128& lhs, const u128& rhs) { u128 r(lhs); r|=rhs; return r; }
    inline u128  operator&(const u128& lhs, const u128& rhs) { u128 r(lhs); r&=rhs; return r; }

    inline u128  operator+(const u128& lhs, const s32& rhs) { u128 r(lhs); r+=u128(rhs); return r; }
    inline u128  operator-(const u128& lhs, const s32& rhs) { u128 r(lhs); r-=u128(rhs); return r; }
    inline u128  operator*(const u128& lhs, const s32& rhs) { u128 r(lhs); r*=u128(rhs); return r; }
    inline u128  operator/(const u128& lhs, const s32& rhs) { u128 r(lhs); r/=u128(rhs); return r; }
    inline u128  operator%(const u128& lhs, const s32& rhs) { u128 r(lhs); r%=u128(rhs); return r; }
    inline u128  operator^(const u128& lhs, const s32& rhs) { u128 r(lhs); r^=u128(rhs); return r; }
    inline u128  operator|(const u128& lhs, const s32& rhs) { u128 r(lhs); r|=u128(rhs); return r; }
    inline u128  operator&(const u128& lhs, const s32& rhs) { u128 r(lhs); r&=u128(rhs); return r; }
    inline u128  operator+(const u128& lhs, const u32& rhs) { u128 r(lhs); r+=u128(rhs); return r; }
    inline u128  operator-(const u128& lhs, const u32& rhs) { u128 r(lhs); r-=u128(rhs); return r; }
    inline u128  operator*(const u128& lhs, const u32& rhs) { u128 r(lhs); r*=u128(rhs); return r; }
    inline u128  operator/(const u128& lhs, const u32& rhs) { u128 r(lhs); r/=u128(rhs); return r; }
    inline u128  operator%(const u128& lhs, const u32& rhs) { u128 r(lhs); r%=u128(rhs); return r; }
    inline u128  operator^(const u128& lhs, const u32& rhs) { u128 r(lhs); r^=u128(rhs); return r; }
    inline u128  operator|(const u128& lhs, const u32& rhs) { u128 r(lhs); r|=u128(rhs); return r; }
    inline u128  operator&(const u128& lhs, const u32& rhs) { u128 r(lhs); r&=u128(rhs); return r; }
    inline u128  operator+(const u128& lhs, const s64& rhs) { u128 r(lhs); r+=u128(rhs); return r; }
    inline u128  operator-(const u128& lhs, const s64& rhs) { u128 r(lhs); r-=u128(rhs); return r; }
    inline u128  operator*(const u128& lhs, const s64& rhs) { u128 r(lhs); r*=u128(rhs); return r; }
    inline u128  operator/(const u128& lhs, const s64& rhs) { u128 r(lhs); r/=u128(rhs); return r; }
    inline u128  operator%(const u128& lhs, const s64& rhs) { u128 r(lhs); r%=u128(rhs); return r; }
    inline u128  operator^(const u128& lhs, const s64& rhs) { u128 r(lhs); r^=u128(rhs); return r; }
    inline u128  operator|(const u128& lhs, const s64& rhs) { u128 r(lhs); r|=u128(rhs); return r; }
    inline u128  operator&(const u128& lhs, const s64& rhs) { u128 r(lhs); r&=u128(rhs); return r; }
    inline u128  operator+(const u128& lhs, const u64& rhs) { u128 r(lhs); r+=u128(rhs); return r; }
    inline u128  operator-(const u128& lhs, const u64& rhs) { u128 r(lhs); r-=u128(rhs); return r; }
    inline u128  operator*(const u128& lhs, const u64& rhs) { u128 r(lhs); r*=u128(rhs); return r; }
    inline u128  operator/(const u128& lhs, const u64& rhs) { u128 r(lhs); r/=u128(rhs); return r; }
    inline u128  operator%(const u128& lhs, const u64& rhs) { u128 r(lhs); r%=u128(rhs); return r; }
    inline u128  operator^(const u128& lhs, const u64& rhs) { u128 r(lhs); r^=u128(rhs); return r; }
    inline u128  operator|(const u128& lhs, const u64& rhs) { u128 r(lhs); r|=u128(rhs); return r; }
    inline u128  operator&(const u128& lhs, const u64& rhs) { u128 r(lhs); r&=u128(rhs); return r; }
    inline u128  operator&(const u128& lhs, u8 rhs) { u32 r = (u32)lhs; return u128(r&rhs); }
    inline u128  operator&(const u128& lhs, u16 rhs) { u32 r = (u32)lhs; return u128(r&rhs); }
    inline u128  operator&(const u128& lhs, u32 rhs) { u32 r = (u32)lhs; return u128(r&rhs); }
    inline u128  operator&(const u128& lhs, u64 rhs) { u64 r = (u64)lhs; return u128(r&rhs); }
    inline u128  operator&(const u128& lhs, s8 rhs) { s32 r = (s32)lhs; return u128(r&rhs); }
    inline u128  operator&(const u128& lhs, s16 rhs) { s32 r = (s32)lhs; return u128(r&rhs); }
    inline u128  operator&(const u128& lhs, s32 rhs) { s32 r = (s32)lhs; return u128(r&rhs); }
    inline u128  operator&(const u128& lhs, s64 rhs) { s64 r = (s64)lhs; return u128(r&rhs); }
    inline u8    operator&(u8  lhs, const u128& rhs) { u32 r = rhs; return r&lhs; }
    inline u16   operator&(u16 lhs, const u128& rhs) { u32 r = rhs; return r&lhs; }
    inline u32   operator&(u32 lhs, const u128& rhs) { u32 r = rhs; return r&lhs; }
    inline u64   operator&(u64 lhs, const u128& rhs) { u64 r = rhs; return r&lhs; }
    inline s8    operator&(s8  lhs, const u128& rhs) { s32 r = rhs; return r&lhs; }
    inline s16   operator&(s16 lhs, const u128& rhs) { s32 r = rhs; return r&lhs; }
    inline s32   operator&(s32 lhs, const u128& rhs) { s32 r = rhs; return r&lhs; }
    inline s64   operator&(s64 lhs, const u128& rhs) { s64 r = rhs; return r&lhs; }
    inline bool  operator<(const u128& lhs, const u128& rhs) { return lhs.Compare(rhs) == -1; }
    inline bool  operator>(const u128& lhs, const u128& rhs) { return lhs.Compare(rhs) == 1; }
    inline bool  operator<=(const u128& lhs, const u128& rhs) { return lhs.Compare(rhs) != 1; }
    inline bool  operator>=(const u128& lhs, const u128& rhs) { return lhs.Compare(rhs) != -1; }
    inline bool  operator==(const u128& lhs, const u128& rhs) { return lhs.Compare(rhs) == 0; }
    inline bool  operator!=(const u128& lhs, const u128& rhs) { return lhs.Compare(rhs) != 0; }
    inline bool  operator==(const u128& lhs, s32 rhs) { return lhs.Compare(u128(rhs)) == 0; }
    inline bool  operator!=(const u128& lhs, s32 rhs) { return lhs.Compare(u128(rhs)) != 0; }

};

#endif
#endif ///< __CBASE_128_BIT_UNSIGNED_INTEGER_H__
