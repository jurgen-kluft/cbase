#ifndef __CBASE_256_BIT_SIGNED_INTEGER_H__
#define __CBASE_256_BIT_SIGNED_INTEGER_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifndef D_NO_CUSTOM_INT256

namespace ncore
{
	// Forward declares
	class s128;
	class u128;
	class u256;

	class s256
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
								s256();
		explicit				s256(s8 value);
		explicit				s256(u8 value);
		explicit				s256(s16 value);
		explicit				s256(u16 value);
		explicit				s256(s32 value);
		explicit				s256(u32 value);
		explicit				s256(const s64& value);
		explicit				s256(const u64& value);
		explicit				s256(const s128& value);
		explicit				s256(const u128& value);
								s256(const s256& value);
								s256(const u256& value);
								s256(u32 b255_224, u32 b223_192, u32 b191_160, u32 b159_128, u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0);

		s256&				operator=(const s256& value);
		s256&				operator=(const u256& value);
		s256& 				operator++();
		s256& 				operator--();
		s256	 			operator++(s32)											{ s256 r(*this); (*this)++; return r; }
		s256 				operator--(s32)											{ s256 r(*this); (*this)--; return r; }
		s256& 				operator*=(const s256& value);
		s256& 				operator/=(const s256& value);
		s256& 				operator+=(const s256& value);
		s256& 				operator-=(const s256& value);
		s256& 				operator%=(const s256& value);
		friend s256  		operator-(const s256& value);
		s256  				operator~() const;

		s256  				operator>>(s32 nShift) const;
		s256  				operator<<(s32 nShift) const;
		s256& 				operator>>=(s32 nShift);
		s256& 				operator<<=(s32 nShift);
		s256& 				operator^=(const s256& value);
		s256& 				operator|=(const s256& value);
		s256& 				operator&=(const s256& value);

								operator s32() const;
								operator u32() const;
								operator s64() const;
								operator u64() const;

		inline u32&				operator [] (s32 index)									{ return mUInt32[index]; }
		inline u32				operator [] (s32 index) const							{ return mUInt32[index]; }

		void 					SetBit(s32 nIndex, bool value);
		bool 					IsBitSet(s32 nIndex) const;

		void 					Negate();

		bool 					IsNegative() const;
		bool 					IsPositive() const;

		void 					Modulus(const s256& Divisor, s256& Quotient, s256& Remainder) const;
		s32						Compare(const s256& other) const;

		static const s256	Zero;
		static const s256	One;

	protected:
		void					TwosComplement();
		void					InverseTwosComplement();

		friend class u256;

		union
		{
			struct
			{
				u32				m_MSB;
				u32				m_FSB;
				u32				m_ESB;
				u32				m_DSB;
				u32				m_CSB;
				u32				m_BSB;
				u32				m_ASB;
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

	inline s256	    operator+(const s256& lhs, const s256& rhs) { s256 r(lhs); r+=rhs; return r; }
	inline s256	    operator-(const s256& lhs, const s256& rhs) { s256 r(lhs); r-=rhs; return r; }
	inline s256  	operator*(const s256& lhs, const s256& rhs) { s256 r(lhs); r*=rhs; return r; }
	inline s256  	operator/(const s256& lhs, const s256& rhs) { s256 r(lhs); r/=rhs; return r; }
	inline s256  	operator%(const s256& lhs, const s256& rhs) { s256 r(lhs); r%=rhs; return r; }
	inline s256  	operator^(const s256& lhs, const s256& rhs) { s256 r(lhs); r^=rhs; return r; }
	inline s256  	operator|(const s256& lhs, const s256& rhs) { s256 r(lhs); r|=rhs; return r; }
	inline s256  	operator&(const s256& lhs, const s256& rhs) { s256 r(lhs); r&=rhs; return r; }

	inline s256	    operator+(const s256& lhs, s32 rhs) { s256 r(lhs); r+=s256(rhs); return r; }
	inline s256	    operator-(const s256& lhs, s32 rhs) { s256 r(lhs); r-=s256(rhs); return r; }
	inline s256  	operator*(const s256& lhs, s32 rhs) { s256 r(lhs); r*=s256(rhs); return r; }
	inline s256  	operator/(const s256& lhs, s32 rhs) { s256 r(lhs); r/=s256(rhs); return r; }
	inline s256  	operator%(const s256& lhs, s32 rhs) { s256 r(lhs); r%=s256(rhs); return r; }
	inline s256  	operator^(const s256& lhs, s32 rhs) { s256 r(lhs); r^=s256(rhs); return r; }
	inline s256  	operator|(const s256& lhs, s32 rhs) { s256 r(lhs); r|=s256(rhs); return r; }
	inline s256  	operator&(const s256& lhs, s32 rhs) { s256 r(lhs); r&=s256(rhs); return r; }

	inline bool		operator<(const s256& lhs, const s256& rhs) { return bool(lhs.Compare(rhs) == -1); }
	inline bool		operator>(const s256& lhs, const s256& rhs) { return bool(lhs.Compare(rhs) == 1); }
	inline bool		operator<=(const s256& lhs, const s256& rhs) { return bool(lhs.Compare(rhs) != 1); }
	inline bool		operator>=(const s256& lhs, const s256& rhs) { return bool(lhs.Compare(rhs) != -1); }
	inline bool		operator==(const s256& lhs, const s256& rhs) { return bool(lhs.Compare(rhs) == 0); }
	inline bool		operator!=(const s256& lhs, const s256& rhs) { return bool(lhs.Compare(rhs) != 0); }
};

#endif
#endif	///< __CBASE_256_BIT_SIGNED_INTEGER_H__
