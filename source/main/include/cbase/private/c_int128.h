#ifndef __CBASE_128_BIT_SIGNED_INTEGER_H__
#define __CBASE_128_BIT_SIGNED_INTEGER_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//#ifndef D_NO_CUSTOM_INT128

namespace ncore
{
    class s128
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

    public:

                            s128();
        explicit			s128(s16 value);
        explicit			s128(u16 value);
        explicit			s128(s32 value);
        explicit			s128(u32 value);
        explicit			s128(const s64& value);
                            s128(const s64& high, const s64& low);
        explicit			s128(const u64& value);
                            s128(const u64& high, const u64& low);
                            s128(const s128& value);
                            s128(const u128& value);
                            s128(u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0);

        s128&				operator=(const s128& value);

        s128& 				operator++();
        s128& 				operator--();
        s128	 			operator++(s32)											{ s128 r(*this); (*this)++; return r; }
        s128 				operator--(s32)											{ s128 r(*this); (*this)--; return r; }
        s128& 				operator*=(const s128& value);
        s128& 				operator/=(const s128& value);
        s128& 				operator+=(const s128& value);
        s128& 				operator-=(const s128& value);
        s128& 				operator%=(const s128& value);
        friend s128  		operator-(const s128& value);
        friend s128  		operator~(const s128& value);

        s128  				operator>>(s32 nShift) const;
        s128  				operator<<(s32 nShift) const;
        s128& 				operator>>=(s32 nShift);
        s128& 				operator<<=(s32 nShift);
        s128& 				operator^=(const s128& value);
        s128& 				operator|=(const s128& value);
        s128& 				operator&=(const s128& value);

                                operator s32() const;
                                operator u32() const;
                                operator s64() const;
                                operator u64() const;

        inline u32&				operator [] (s32 index)									{ return mUInt32[index]; }
        inline u32				operator [] (s32 index) const							{ return mUInt32[index]; }


        void 					SetBit(s32 nIndex, bool value);
        bool 					IsBitSet(s32 nIndex) const;

        void 					Abs();
        void 					Negate();

        bool 					IsNegative() const;
        bool 					IsPositive() const;

        void 					Modulus(const s128& Divisor, s128& Quotient, s128& Remainder) const;
        s32						Compare(const s128& other) const;

        static const s128	    Zero;
        static const s128	    One;

    protected:
        void					TwosComplement();
        void					InverseTwosComplement();

        friend class u128;

        union
        {
            struct
            {
                u32				m_MSB;
                u32				m_DSB;
                u32				m_CSB;
                u32				m_LSB;
            };
            s8				mInt8[NUM_INT8];
            s16				mInt16[NUM_INT16];
            s32				mInt32[NUM_INT32];

            u8				mUInt8[NUM_UINT8];
            u16				mUInt16[NUM_UINT16];
            u32				mUInt32[NUM_UINT32];
        };
    };

    inline s128		operator+(const s128& lhs, const s128& rhs)			{ s128 r(lhs); r+=rhs; return r; }
    inline s128		operator-(const s128& lhs, const s128& rhs)			{ s128 r(lhs); r-=rhs; return r; }
    inline s128  	operator*(const s128& lhs, const s128& rhs)			{ s128 r(lhs); r*=rhs; return r; }
    inline s128  	operator/(const s128& lhs, const s128& rhs)			{ s128 r(lhs); r/=rhs; return r; }
    inline s128  	operator%(const s128& lhs, const s128& rhs)			{ s128 r(lhs); r%=rhs; return r; }
    inline s128  	operator^(const s128& lhs, const s128& rhs)			{ s128 r(lhs); r^=rhs; return r; }
    inline s128  	operator|(const s128& lhs, const s128& rhs)			{ s128 r(lhs); r|=rhs; return r; }
    inline s128  	operator&(const s128& lhs, const s128& rhs)			{ s128 r(lhs); r&=rhs; return r; }

    inline s128		operator+(const s128& lhs, s32 rhs)					{ s128 r(lhs); r+=s128(rhs); return r; }
    inline s128		operator-(const s128& lhs, s32 rhs)					{ s128 r(lhs); r-=s128(rhs); return r; }
    inline s128  	operator*(const s128& lhs, s32 rhs)					{ s128 r(lhs); r*=s128(rhs); return r; }
    inline s128  	operator/(const s128& lhs, s32 rhs)					{ s128 r(lhs); r/=s128(rhs); return r; }
    inline s128  	operator%(const s128& lhs, s32 rhs)					{ s128 r(lhs); r%=s128(rhs); return r; }
    inline s128  	operator^(const s128& lhs, s32 rhs)					{ s128 r(lhs); r^=s128(rhs); return r; }
    inline s128  	operator|(const s128& lhs, s32 rhs)					{ s128 r(lhs); r|=s128(rhs); return r; }
    inline s128  	operator&(const s128& lhs, s32 rhs)					{ s128 r(lhs); r&=s128(rhs); return r; }

    inline s128		operator+(const s128& lhs, u32 rhs)					{ s128 r(lhs); r+=s128(rhs); return r; }
    inline s128		operator-(const s128& lhs, u32 rhs)					{ s128 r(lhs); r-=s128(rhs); return r; }
    inline s128  	operator*(const s128& lhs, u32 rhs)					{ s128 r(lhs); r*=s128(rhs); return r; }
    inline s128  	operator/(const s128& lhs, u32 rhs)					{ s128 r(lhs); r/=s128(rhs); return r; }
    inline s128  	operator%(const s128& lhs, u32 rhs)					{ s128 r(lhs); r%=s128(rhs); return r; }
    inline s128  	operator^(const s128& lhs, u32 rhs)					{ s128 r(lhs); r^=s128(rhs); return r; }
    inline s128  	operator|(const s128& lhs, u32 rhs)					{ s128 r(lhs); r|=s128(rhs); return r; }
    inline s128  	operator&(const s128& lhs, u32 rhs)					{ s128 r(lhs); r&=s128(rhs); return r; }

    inline s128		operator+(const s128& lhs, s64 rhs)					{ s128 r(lhs); r+=s128(rhs); return r; }
    inline s128		operator-(const s128& lhs, s64 rhs)					{ s128 r(lhs); r-=s128(rhs); return r; }
    inline s128  	operator*(const s128& lhs, s64 rhs)					{ s128 r(lhs); r*=s128(rhs); return r; }
    inline s128  	operator/(const s128& lhs, s64 rhs)					{ s128 r(lhs); r/=s128(rhs); return r; }
    inline s128  	operator%(const s128& lhs, s64 rhs)					{ s128 r(lhs); r%=s128(rhs); return r; }
    inline s128  	operator^(const s128& lhs, s64 rhs)					{ s128 r(lhs); r^=s128(rhs); return r; }
    inline s128  	operator|(const s128& lhs, s64 rhs)					{ s128 r(lhs); r|=s128(rhs); return r; }
    inline s128  	operator&(const s128& lhs, s64 rhs)					{ s128 r(lhs); r&=s128(rhs); return r; }

    inline s128		operator+(const s128& lhs, u64 rhs)					{ s128 r(lhs); r+=s128(rhs); return r; }
    inline s128		operator-(const s128& lhs, u64 rhs)					{ s128 r(lhs); r-=s128(rhs); return r; }
    inline s128  	operator*(const s128& lhs, u64 rhs)					{ s128 r(lhs); r*=s128(rhs); return r; }
    inline s128  	operator/(const s128& lhs, u64 rhs)					{ s128 r(lhs); r/=s128(rhs); return r; }
    inline s128  	operator%(const s128& lhs, u64 rhs)					{ s128 r(lhs); r%=s128(rhs); return r; }
    inline s128  	operator^(const s128& lhs, u64 rhs)					{ s128 r(lhs); r^=s128(rhs); return r; }
    inline s128  	operator|(const s128& lhs, u64 rhs)					{ s128 r(lhs); r|=s128(rhs); return r; }
    inline s128  	operator&(const s128& lhs, u64 rhs)					{ s128 r(lhs); r&=s128(rhs); return r; }

    inline s128		operator+(s32 lhs, const s128& rhs)					{ s128 r(lhs); r+=rhs; return r; }
    inline s128		operator-(s32 lhs, const s128& rhs)					{ s128 r(lhs); r-=rhs; return r; }
    inline s128  	operator*(s32 lhs, const s128& rhs)					{ s128 r(lhs); r*=rhs; return r; }
    inline s128  	operator/(s32 lhs, const s128& rhs)					{ s128 r(lhs); r/=rhs; return r; }
    inline s128  	operator%(s32 lhs, const s128& rhs)					{ s128 r(lhs); r%=rhs; return r; }
    inline s128  	operator^(s32 lhs, const s128& rhs)					{ s128 r(lhs); r^=rhs; return r; }
    inline s128  	operator|(s32 lhs, const s128& rhs)					{ s128 r(lhs); r|=rhs; return r; }
    inline s128  	operator&(s32 lhs, const s128& rhs)					{ s128 r(lhs); r&=rhs; return r; }

    inline bool		operator< (const s128& lhs, const s128& rhs)		{ return bool(lhs.Compare(rhs) == -1); }
    inline bool		operator> (const s128& lhs, const s128& rhs)		{ return bool(lhs.Compare(rhs) == 1); }
    inline bool		operator<=(const s128& lhs, const s128& rhs)		{ return bool(lhs.Compare(rhs) != 1); }
    inline bool		operator>=(const s128& lhs, const s128& rhs)		{ return bool(lhs.Compare(rhs) != -1); }
    inline bool		operator==(const s128& lhs, const s128& rhs)		{ return bool(lhs.Compare(rhs) == 0); }
    inline bool		operator!=(const s128& lhs, const s128& rhs)		{ return bool(lhs.Compare(rhs) != 0); }

    inline bool		operator< (const s128& lhs, s32 rhs)				{ return bool(lhs.Compare(s128(rhs)) == -1); }
    inline bool		operator> (const s128& lhs, s32 rhs)				{ return bool(lhs.Compare(s128(rhs)) == 1); }
    inline bool		operator<=(const s128& lhs, s32 rhs)				{ return bool(lhs.Compare(s128(rhs)) != 1); }
    inline bool		operator>=(const s128& lhs, s32 rhs)				{ return bool(lhs.Compare(s128(rhs)) != -1); }
    inline bool		operator==(const s128& lhs, s32 rhs)				{ return bool(lhs.Compare(s128(rhs)) == 0); }
    inline bool		operator!=(const s128& lhs, s32 rhs)				{ return bool(lhs.Compare(s128(rhs)) != 0); }

    inline bool		operator< (s32 lhs, const s128& rhs)				{ return bool(s128(lhs).Compare(rhs) == -1); }
    inline bool		operator> (s32 lhs, const s128& rhs)				{ return bool(s128(lhs).Compare(rhs) == 1); }
    inline bool		operator<=(s32 lhs, const s128& rhs)				{ return bool(s128(lhs).Compare(rhs) != 1); }
    inline bool		operator>=(s32 lhs, const s128& rhs)				{ return bool(s128(lhs).Compare(rhs) != -1); }
    inline bool		operator==(s32 lhs, const s128& rhs)				{ return bool(s128(lhs).Compare(rhs) == 0); }
    inline bool		operator!=(s32 lhs, const s128& rhs)				{ return bool(s128(lhs).Compare(rhs) != 0); }

    inline s128		abs(const s128& value)								{ s128 r(value); r.Abs(); return r; }
};

//#endif
#endif	///< __CBASE_128_BIT_SIGNED_INTEGER_H__
