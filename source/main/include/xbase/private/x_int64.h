// x_int64.h - xCore custom 64 bit signed integer
#ifndef __XBASE_64_BIT_SIGNED_INTEGER_H__
#define __XBASE_64_BIT_SIGNED_INTEGER_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif
#ifndef X_NO_CUSTOM_INT64

#include "xbase\x_types.h"
#include "xbase\x_debug.h"


//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	// Forward declares
	class __xuint64;


	//---------------------------------------------------------------------------------------------------------------------
	// 64 bit signed integer
	//---------------------------------------------------------------------------------------------------------------------
	class __xint64
	{
		enum ESettings
		{
			NUM_BITS = 64,
			NUM_INT8 = NUM_BITS / 8,
			NUM_INT16 = NUM_BITS / 16,
			NUM_INT32 = NUM_BITS / 32,
			NUM_UINT8 = NUM_BITS / 8,
			NUM_UINT16 = NUM_BITS / 16,
			NUM_UINT32 = NUM_BITS / 32,
		};

	public:
								// Constructors
								__xint64();
		explicit				__xint64(s8 _value);
		explicit				__xint64(s16 _value);
		explicit				__xint64(s32 _value);
		explicit				__xint64(u8 _value);
		explicit				__xint64(u16 _value);
		explicit				__xint64(u32 _value);
								__xint64(const s64& _value);
								__xint64(const u64& _value);
								__xint64(u32 msb, u32 lsb);

		// Assignment operator
		__xint64&				operator=(char _value)									{ *this = __xint64(_value); return *this; }
		__xint64&				operator=(s16 _value)									{ *this = __xint64(_value); return *this; }
		__xint64&				operator=(s32 _value)									{ *this = __xint64(_value); return *this; }
		__xint64&				operator=(const s64& _value);
		__xint64&				operator=(unsigned char _value)							{ *this = __xint64(_value); return *this; }
		__xint64&				operator=(u16 _value)									{ *this = __xint64(_value); return *this; }
		__xint64&				operator=(u32 _value)									{ *this = __xint64((u32)_value); return *this; }
		__xint64&				operator=(const u64& _value);

		// Arithmetic operators
		__xint64& 				operator++();
		__xint64& 				operator--();
		__xint64	 			operator++(s32)											{ __xint64 r(*this); ++(*this); return r; }
		__xint64 				operator--(s32)											{ __xint64 r(*this); --(*this); return r; }
		__xint64& 				operator*=(const __xint64& _value);
		__xint64& 				operator/=(const __xint64& _value);
		__xint64& 				operator+=(const __xint64& _value);
		__xint64& 				operator-=(const __xint64& _value);
		__xint64& 				operator%=(const __xint64& _value);
		friend __xint64			operator-(const __xint64& _value);
		friend __xint64  		operator~(const __xint64& _value);

		__xint64& 				operator*=(s32 _value)									{ return *this *= __xint64(_value); }
		__xint64& 				operator/=(s32 _value)									{ return *this *= __xint64(_value); }
		__xint64& 				operator+=(s32 _value)									{ return *this *= __xint64(_value); }
		__xint64& 				operator-=(s32 _value)									{ return *this *= __xint64(_value); }
		__xint64& 				operator%=(s32 _value)									{ return *this *= __xint64(_value); }

		// Misc operators
		__xint64  				operator>>(s32 nShift) const;
		__xint64  				operator<<(s32 nShift) const;
		__xint64& 				operator>>=(s32 nShift);
		__xint64& 				operator<<=(s32 nShift);
		__xint64& 				operator^=(const __xint64& _value);
		__xint64& 				operator|=(const __xint64& _value);
		__xint64& 				operator&=(const __xint64& _value);

		// Operators to convert back to basic types
								operator s32() const;
								operator u32() const;
								operator s64() const;
								operator u64() const;

		u32						operator [] (s32 index) const							{ ASSERT(index>=0 && index<2); return mUInt32[index]; }

		//Misc. Functions
		void 					SetBit(s32 nIndex, xbool _value);
		xbool 					IsBitSet(s32 nIndex) const;

		void 					Negate();

		xbool 					IsNegative() const;
		xbool 					IsPositive() const;

		void 					Modulus(const __xint64& Divisor, __xint64& Quotient, __xint64& Remainder) const;
		s32						Compare(const __xint64& other) const;

		static const __xint64	Zero;
		static const __xint64	One;

	protected:
		void					TwosComplement();
		void					InverseTwosComplement();

		friend class __xuint64;

		// Actual member data variables
		union
		{
			struct
			{
				u32					m_MSB;
				u32					m_LSB;
			};
			s8					mInt8[NUM_INT8];
			s16					mInt16[NUM_INT16];
			s32					mInt32[NUM_INT32];
			u8					mUInt8[NUM_UINT8];
			u16					mUInt16[NUM_UINT16];
			u32					mUInt32[NUM_UINT32];
		};
	};

	inline __xint64		operator+(const __xint64& lhs, const __xint64& rhs) { __xint64 r(lhs); r+=rhs; return r; }
	inline __xint64		operator-(const __xint64& lhs, const __xint64& rhs) { __xint64 r(lhs); r-=rhs; return r; }
	inline __xint64  	operator*(const __xint64& lhs, const __xint64& rhs) { __xint64 r(lhs); r*=rhs; return r; }
	inline __xint64  	operator/(const __xint64& lhs, const __xint64& rhs) { __xint64 r(lhs); r/=rhs; return r; }
	inline __xint64  	operator%(const __xint64& lhs, const __xint64& rhs) { __xint64 r(lhs); r%=rhs; return r; }
	inline __xint64  	operator^(const __xint64& lhs, const __xint64& rhs) { __xint64 r(lhs); r^=rhs; return r; }
	inline __xint64  	operator|(const __xint64& lhs, const __xint64& rhs) { __xint64 r(lhs); r|=rhs; return r; }
	inline __xint64  	operator&(const __xint64& lhs, const __xint64& rhs) { __xint64 r(lhs); r&=rhs; return r; }

	inline __xint64		operator+(const __xint64& lhs, s32 rhs) { __xint64 r(lhs); r+=__xint64(rhs); return r; }
	inline __xint64		operator-(const __xint64& lhs, s32 rhs) { __xint64 r(lhs); r-=__xint64(rhs); return r; }
	inline __xint64  	operator*(const __xint64& lhs, s32 rhs) { __xint64 r(lhs); r*=__xint64(rhs); return r; }
	inline __xint64  	operator/(const __xint64& lhs, s32 rhs) { __xint64 r(lhs); r/=__xint64(rhs); return r; }
	inline __xint64  	operator%(const __xint64& lhs, s32 rhs) { __xint64 r(lhs); r%=__xint64(rhs); return r; }
	inline __xint64  	operator^(const __xint64& lhs, s32 rhs) { __xint64 r(lhs); r^=__xint64(rhs); return r; }
	inline __xint64  	operator|(const __xint64& lhs, s32 rhs) { __xint64 r(lhs); r|=__xint64(rhs); return r; }
	inline __xint64  	operator&(const __xint64& lhs, s32 rhs) { __xint64 r(lhs); r&=__xint64(rhs); return r; }

	inline __xint64		operator+(s32 lhs, const __xint64& rhs) { __xint64 r(lhs); r+=rhs; return r; }
	inline __xint64		operator-(s32 lhs, const __xint64& rhs) { __xint64 r(lhs); r-=rhs; return r; }
	inline __xint64  	operator*(s32 lhs, const __xint64& rhs) { __xint64 r(lhs); r*=rhs; return r; }
	inline __xint64  	operator/(s32 lhs, const __xint64& rhs) { __xint64 r(lhs); r/=rhs; return r; }
	inline __xint64  	operator%(s32 lhs, const __xint64& rhs) { __xint64 r(lhs); r%=rhs; return r; }
	inline __xint64  	operator^(s32 lhs, const __xint64& rhs) { __xint64 r(lhs); r^=rhs; return r; }
	inline __xint64  	operator|(s32 lhs, const __xint64& rhs) { __xint64 r(lhs); r|=rhs; return r; }
	inline __xint64  	operator&(s32 lhs, const __xint64& rhs) { __xint64 r(lhs); r&=rhs; return r; }

	inline xbool		operator<(const __xint64& lhs, const __xint64& rhs) { return xbool(lhs.Compare(rhs) == -1); }
	inline xbool		operator>(const __xint64& lhs, const __xint64& rhs) { return xbool(lhs.Compare(rhs) == 1); }
	inline xbool		operator<=(const __xint64& lhs, const __xint64& rhs) { return xbool(lhs.Compare(rhs) != 1); }
	inline xbool		operator>=(const __xint64& lhs, const __xint64& rhs) { return xbool(lhs.Compare(rhs) != -1); }
	inline xbool		operator==(const __xint64& lhs, const __xint64& rhs) { return xbool(lhs.Compare(rhs) == 0); }
	inline xbool		operator!=(const __xint64& lhs, const __xint64& rhs) { return xbool(lhs.Compare(rhs) != 0); }

	inline xbool		operator<(const __xint64& lhs, s32 rhs) { return xbool(lhs.Compare(__xint64(rhs)) == -1); }
	inline xbool		operator>(const __xint64& lhs, s32 rhs) { return xbool(lhs.Compare(__xint64(rhs)) == 1); }
	inline xbool		operator<=(const __xint64& lhs, s32 rhs) { return xbool(lhs.Compare(__xint64(rhs)) != 1); }
	inline xbool		operator>=(const __xint64& lhs, s32 rhs) { return xbool(lhs.Compare(__xint64(rhs)) != -1); }
	inline xbool		operator==(const __xint64& lhs, s32 rhs) { return xbool(lhs.Compare(__xint64(rhs)) == 0); }
	inline xbool		operator!=(const __xint64& lhs, s32 rhs) { return xbool(lhs.Compare(__xint64(rhs)) != 0); }

	inline xbool		operator< (s32 lhs, const __xint64& rhs) { return xbool(__xint64(lhs).Compare(rhs) == -1); }
	inline xbool		operator> (s32 lhs, const __xint64& rhs) { return xbool(__xint64(lhs).Compare(rhs) == 1); }
	inline xbool		operator<=(s32 lhs, const __xint64& rhs) { return xbool(__xint64(lhs).Compare(rhs) != 1); }
	inline xbool		operator>=(s32 lhs, const __xint64& rhs) { return xbool(__xint64(lhs).Compare(rhs) != -1); }
	inline xbool		operator==(s32 lhs, const __xint64& rhs) { return xbool(__xint64(lhs).Compare(rhs) == 0); }
	inline xbool		operator!=(s32 lhs, const __xint64& rhs) { return xbool(__xint64(lhs).Compare(rhs) != 0); }

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
#endif
#endif	///< __XBASE_64_BIT_SIGNED_INTEGER_H__
