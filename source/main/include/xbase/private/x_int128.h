// x_int128.h - xCore custom 128 bit signed integer
#ifndef __XBASE_128_BIT_SIGNED_INTEGER_H__
#define __XBASE_128_BIT_SIGNED_INTEGER_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifndef X_NO_CUSTOM_INT128

#include "xbase\x_types.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{

	// Forward declares
	class __xint64;
	class __xuint64;
	class __xuint128;


	class __xint128
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
								// Constructors
								__xint128();
		explicit				__xint128(s16 value);
		explicit				__xint128(u16 value);
		explicit				__xint128(s32 value);
		explicit				__xint128(u32 value);
		explicit				__xint128(const s64& value);
								__xint128(const s64& high, const s64& low);
		explicit				__xint128(const u64& value);
								__xint128(const u64& high, const u64& low);
								__xint128(const __xint128& value);
								__xint128(const __xuint128& value);
								__xint128(u32 b127_96, u32 b95_64, u32 b63_32, u32 b31_0);

		__xint128&				operator=(const __xint128& value);

		// Arithmetic operators
		__xint128& 				operator++();
		__xint128& 				operator--();
		__xint128	 			operator++(s32)											{ __xint128 r(*this); (*this)++; return r; }
		__xint128 				operator--(s32)											{ __xint128 r(*this); (*this)--; return r; }
		__xint128& 				operator*=(const __xint128& value);
		__xint128& 				operator/=(const __xint128& value);
		__xint128& 				operator+=(const __xint128& value);
		__xint128& 				operator-=(const __xint128& value);
		__xint128& 				operator%=(const __xint128& value);
		friend __xint128  		operator-(const __xint128& value);
		friend __xint128  		operator~(const __xint128& value);

		// Misc operators
		__xint128  				operator>>(s32 nShift) const;
		__xint128  				operator<<(s32 nShift) const;
		__xint128& 				operator>>=(s32 nShift);
		__xint128& 				operator<<=(s32 nShift);
		__xint128& 				operator^=(const __xint128& value);
		__xint128& 				operator|=(const __xint128& value);
		__xint128& 				operator&=(const __xint128& value);

								// Operators to convert back to basic types
								operator s32() const;
								operator u32() const;
								operator s64() const;
								operator u64() const;

		inline u32&				operator [] (s32 index)									{ return mUInt32[index]; }
		inline u32				operator [] (s32 index) const							{ return mUInt32[index]; }

		//Misc. Functions
		void 					SetBit(s32 nIndex, xbool value);
		xbool 					IsBitSet(s32 nIndex) const;

		void 					Abs();
		void 					Negate();

		xbool 					IsNegative() const;
		xbool 					IsPositive() const;

		void 					Modulus(const __xint128& Divisor, __xint128& Quotient, __xint128& Remainder) const;
		s32						Compare(const __xint128& other) const;

		static const __xint128	Zero;
		static const __xint128	One;

	protected:
		void					TwosComplement();
		void					InverseTwosComplement();

		friend class __xuint128;

		// Actual member data variables
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

	inline __xint128		operator+(const __xint128& lhs, const __xint128& rhs)			{ __xint128 r(lhs); r+=rhs; return r; }
	inline __xint128		operator-(const __xint128& lhs, const __xint128& rhs)			{ __xint128 r(lhs); r-=rhs; return r; }
	inline __xint128  		operator*(const __xint128& lhs, const __xint128& rhs)			{ __xint128 r(lhs); r*=rhs; return r; }
	inline __xint128  		operator/(const __xint128& lhs, const __xint128& rhs)			{ __xint128 r(lhs); r/=rhs; return r; }
	inline __xint128  		operator%(const __xint128& lhs, const __xint128& rhs)			{ __xint128 r(lhs); r%=rhs; return r; }
	inline __xint128  		operator^(const __xint128& lhs, const __xint128& rhs)			{ __xint128 r(lhs); r^=rhs; return r; }
	inline __xint128  		operator|(const __xint128& lhs, const __xint128& rhs)			{ __xint128 r(lhs); r|=rhs; return r; }
	inline __xint128  		operator&(const __xint128& lhs, const __xint128& rhs)			{ __xint128 r(lhs); r&=rhs; return r; }

	inline __xint128		operator+(const __xint128& lhs, s32 rhs)						{ __xint128 r(lhs); r+=__xint128(rhs); return r; }
	inline __xint128		operator-(const __xint128& lhs, s32 rhs)						{ __xint128 r(lhs); r-=__xint128(rhs); return r; }
	inline __xint128  		operator*(const __xint128& lhs, s32 rhs)						{ __xint128 r(lhs); r*=__xint128(rhs); return r; }
	inline __xint128  		operator/(const __xint128& lhs, s32 rhs)						{ __xint128 r(lhs); r/=__xint128(rhs); return r; }
	inline __xint128  		operator%(const __xint128& lhs, s32 rhs)						{ __xint128 r(lhs); r%=__xint128(rhs); return r; }
	inline __xint128  		operator^(const __xint128& lhs, s32 rhs)						{ __xint128 r(lhs); r^=__xint128(rhs); return r; }
	inline __xint128  		operator|(const __xint128& lhs, s32 rhs)						{ __xint128 r(lhs); r|=__xint128(rhs); return r; }
	inline __xint128  		operator&(const __xint128& lhs, s32 rhs)						{ __xint128 r(lhs); r&=__xint128(rhs); return r; }

	inline __xint128		operator+(const __xint128& lhs, u32 rhs)						{ __xint128 r(lhs); r+=__xint128(rhs); return r; }
	inline __xint128		operator-(const __xint128& lhs, u32 rhs)						{ __xint128 r(lhs); r-=__xint128(rhs); return r; }
	inline __xint128  		operator*(const __xint128& lhs, u32 rhs)						{ __xint128 r(lhs); r*=__xint128(rhs); return r; }
	inline __xint128  		operator/(const __xint128& lhs, u32 rhs)						{ __xint128 r(lhs); r/=__xint128(rhs); return r; }
	inline __xint128  		operator%(const __xint128& lhs, u32 rhs)						{ __xint128 r(lhs); r%=__xint128(rhs); return r; }
	inline __xint128  		operator^(const __xint128& lhs, u32 rhs)						{ __xint128 r(lhs); r^=__xint128(rhs); return r; }
	inline __xint128  		operator|(const __xint128& lhs, u32 rhs)						{ __xint128 r(lhs); r|=__xint128(rhs); return r; }
	inline __xint128  		operator&(const __xint128& lhs, u32 rhs)						{ __xint128 r(lhs); r&=__xint128(rhs); return r; }

	inline __xint128		operator+(const __xint128& lhs, s64 rhs)						{ __xint128 r(lhs); r+=__xint128(rhs); return r; }
	inline __xint128		operator-(const __xint128& lhs, s64 rhs)						{ __xint128 r(lhs); r-=__xint128(rhs); return r; }
	inline __xint128  		operator*(const __xint128& lhs, s64 rhs)						{ __xint128 r(lhs); r*=__xint128(rhs); return r; }
	inline __xint128  		operator/(const __xint128& lhs, s64 rhs)						{ __xint128 r(lhs); r/=__xint128(rhs); return r; }
	inline __xint128  		operator%(const __xint128& lhs, s64 rhs)						{ __xint128 r(lhs); r%=__xint128(rhs); return r; }
	inline __xint128  		operator^(const __xint128& lhs, s64 rhs)						{ __xint128 r(lhs); r^=__xint128(rhs); return r; }
	inline __xint128  		operator|(const __xint128& lhs, s64 rhs)						{ __xint128 r(lhs); r|=__xint128(rhs); return r; }
	inline __xint128  		operator&(const __xint128& lhs, s64 rhs)						{ __xint128 r(lhs); r&=__xint128(rhs); return r; }

	inline __xint128		operator+(const __xint128& lhs, u64 rhs)						{ __xint128 r(lhs); r+=__xint128(rhs); return r; }
	inline __xint128		operator-(const __xint128& lhs, u64 rhs)						{ __xint128 r(lhs); r-=__xint128(rhs); return r; }
	inline __xint128  		operator*(const __xint128& lhs, u64 rhs)						{ __xint128 r(lhs); r*=__xint128(rhs); return r; }
	inline __xint128  		operator/(const __xint128& lhs, u64 rhs)						{ __xint128 r(lhs); r/=__xint128(rhs); return r; }
	inline __xint128  		operator%(const __xint128& lhs, u64 rhs)						{ __xint128 r(lhs); r%=__xint128(rhs); return r; }
	inline __xint128  		operator^(const __xint128& lhs, u64 rhs)						{ __xint128 r(lhs); r^=__xint128(rhs); return r; }
	inline __xint128  		operator|(const __xint128& lhs, u64 rhs)						{ __xint128 r(lhs); r|=__xint128(rhs); return r; }
	inline __xint128  		operator&(const __xint128& lhs, u64 rhs)						{ __xint128 r(lhs); r&=__xint128(rhs); return r; }

	inline __xint128		operator+(s32 lhs, const __xint128& rhs)						{ __xint128 r(lhs); r+=rhs; return r; }
	inline __xint128		operator-(s32 lhs, const __xint128& rhs)						{ __xint128 r(lhs); r-=rhs; return r; }
	inline __xint128  		operator*(s32 lhs, const __xint128& rhs)						{ __xint128 r(lhs); r*=rhs; return r; }
	inline __xint128  		operator/(s32 lhs, const __xint128& rhs)						{ __xint128 r(lhs); r/=rhs; return r; }
	inline __xint128  		operator%(s32 lhs, const __xint128& rhs)						{ __xint128 r(lhs); r%=rhs; return r; }
	inline __xint128  		operator^(s32 lhs, const __xint128& rhs)						{ __xint128 r(lhs); r^=rhs; return r; }
	inline __xint128  		operator|(s32 lhs, const __xint128& rhs)						{ __xint128 r(lhs); r|=rhs; return r; }
	inline __xint128  		operator&(s32 lhs, const __xint128& rhs)						{ __xint128 r(lhs); r&=rhs; return r; }

	inline xbool			operator< (const __xint128& lhs, const __xint128& rhs)			{ return xbool(lhs.Compare(rhs) == -1); }
	inline xbool			operator> (const __xint128& lhs, const __xint128& rhs)			{ return xbool(lhs.Compare(rhs) == 1); }
	inline xbool			operator<=(const __xint128& lhs, const __xint128& rhs)			{ return xbool(lhs.Compare(rhs) != 1); }
	inline xbool			operator>=(const __xint128& lhs, const __xint128& rhs)			{ return xbool(lhs.Compare(rhs) != -1); }
	inline xbool			operator==(const __xint128& lhs, const __xint128& rhs)			{ return xbool(lhs.Compare(rhs) == 0); }
	inline xbool			operator!=(const __xint128& lhs, const __xint128& rhs)			{ return xbool(lhs.Compare(rhs) != 0); }

	inline xbool			operator< (const __xint128& lhs, s32 rhs)						{ return xbool(lhs.Compare(__xint128(rhs)) == -1); }
	inline xbool			operator> (const __xint128& lhs, s32 rhs)						{ return xbool(lhs.Compare(__xint128(rhs)) == 1); }
	inline xbool			operator<=(const __xint128& lhs, s32 rhs)						{ return xbool(lhs.Compare(__xint128(rhs)) != 1); }
	inline xbool			operator>=(const __xint128& lhs, s32 rhs)						{ return xbool(lhs.Compare(__xint128(rhs)) != -1); }
	inline xbool			operator==(const __xint128& lhs, s32 rhs)						{ return xbool(lhs.Compare(__xint128(rhs)) == 0); }
	inline xbool			operator!=(const __xint128& lhs, s32 rhs)						{ return xbool(lhs.Compare(__xint128(rhs)) != 0); }

	inline xbool			operator< (s32 lhs, const __xint128& rhs)						{ return xbool(__xint128(lhs).Compare(rhs) == -1); }
	inline xbool			operator> (s32 lhs, const __xint128& rhs)						{ return xbool(__xint128(lhs).Compare(rhs) == 1); }
	inline xbool			operator<=(s32 lhs, const __xint128& rhs)						{ return xbool(__xint128(lhs).Compare(rhs) != 1); }
	inline xbool			operator>=(s32 lhs, const __xint128& rhs)						{ return xbool(__xint128(lhs).Compare(rhs) != -1); }
	inline xbool			operator==(s32 lhs, const __xint128& rhs)						{ return xbool(__xint128(lhs).Compare(rhs) == 0); }
	inline xbool			operator!=(s32 lhs, const __xint128& rhs)						{ return xbool(__xint128(lhs).Compare(rhs) != 0); }

	inline __xint128		abs(const __xint128& value)										{ __xint128 r(value); r.Abs(); return r; }

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif
#endif	///< __XBASE_128_BIT_SIGNED_INTEGER_H__
